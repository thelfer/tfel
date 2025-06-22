/*!
 * \file   mfront/src/CyranoInterface.cxx
 * \brief
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <algorithm>

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/CyranoInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  std::string CyranoInterface::getName() { return "cyrano"; }

  int CyranoInterface::getModellingHypothesisIdentifier(const Hypothesis h) {
    switch (h) {
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN:
        return 1;
      case ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS:
        return 2;
      default:
        break;
    }
    std::ostringstream msg;
    msg << "CyranoInterface::getModellingHypothesisIdentifier : "
        << "unsupported hypothesis";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      msg << " (default)";
    } else {
      msg << " (" << ModellingHypothesis::toString(h) << "')";
    }
    throw(std::runtime_error(msg.str()));
  }

  std::string CyranoInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    auto lib = std::string{};
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = "Cyrano" + mb.getMaterialName();
      } else {
        lib = "CyranoBehaviour";
      }
    } else {
      lib = "Cyrano" + mb.getLibrary();
    }
    return lib;
  }  // end of CyranoInterface::getLibraryName

  std::string CyranoInterface::getInterfaceName() const {
    return "Cyrano";
  }  // end of CyranoInterface::getInterfaceName

  std::string CyranoInterface::getFunctionName(const std::string& n) const {
    return "cyrano" + makeLowerCase(n);
  }  // end of CyranoInterface::getLibraryName

  std::string CyranoInterface::getBehaviourName(
      const std::string& library, const std::string& className) const {
    return library + className;
  }  // end of CyranoInterface::getBehaviourName

  std::string CyranoInterface::getUmatFunctionName(
      const std::string& library, const std::string& className) const {
    return "cyrano" + makeLowerCase(this->getBehaviourName(library, className));
  }  // end of CyranoInterface::getUmatFunctionName

  CyranoInterface::CyranoInterface()
      : useTimeSubStepping(false),
        doSubSteppingOnInvalidResults(false),
        maximumSubStepping(0u) {}  // end of CyranoInterface::CyranoInterface

  std::pair<bool, CyranoInterface::tokens_iterator>
  CyranoInterface::treatKeyword(BehaviourDescription&,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    using namespace std;
    using namespace tfel::utilities;
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) == i.end()) {
        return {false, current};
      }
    }
    if ((key == "@CyranoGenerateMTestFileOnFailure") ||
        (key == "@UMATGenerateMTestFileOnFailure")) {
      this->generateMTestFile = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CyranoUseTimeSubStepping") ||
               (key == "@UMATUseTimeSubStepping")) {
      this->useTimeSubStepping = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CyranoMaximumSubStepping") ||
               (key == "@UMATMaximumSubStepping")) {
      if (!this->useTimeSubStepping) {
        throw(runtime_error(
            "CyranoInterface::treatKeyword (@CyranoMaximumSubStepping): "
            "time sub stepping is not enabled at this stage.\n"
            "Use the @CyranoUseTimeSubStepping directive before "
            "@CyranoMaximumSubStepping"));
      }
      if (current == end) {
        throw(runtime_error(
            "CyranoInterface::treatKeyword (@CyranoMaximumSubStepping): "
            "unexpected end of file"));
      }
      istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      if (flux.fail()) {
        throw(runtime_error(
            "CyranoInterface::treatKeyword (@CyranoMaximumSubStepping): "
            "failed to read maximum substepping value."));
      }
      ++(current);
      if (current == end) {
        throw(runtime_error(
            "CyranoInterface::treatKeyword (@CyranoMaximumSubStepping): "
            "unexpected end of file"));
      }
      if (current->value != ";") {
        throw(
            runtime_error("CyranoInterface::treatKeyword : expected ';',"
                          " read '" +
                          current->value + "'"));
      }
      ++(current);
      return {true, current};
    } else if ((key == "@CyranoDoSubSteppingOnInvalidResults") ||
               (key == "@UMATDoSubSteppingOnInvalidResults")) {
      if (!this->useTimeSubStepping) {
        throw(
            runtime_error("CyranoInterface::treatKeyword "
                          "(@CyranoDoSubSteppingOnInvalidResults): "
                          "time sub stepping is not enabled at this stage.\n"
                          "Use the @CyranoUseTimeSubStepping directive before "
                          "@CyranoMaximumSubStepping"));
      }
      this->doSubSteppingOnInvalidResults =
          this->readBooleanValue(key, current, end);
      return {true, current};
    }
    return make_pair(false, current);
  }  // end of treatKeyword

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
            SupportedTypes::TypeSize>
  CyranoInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
                                               const Hypothesis h) const {
    using namespace std;
    pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize> res;
    auto& mprops = res.first;
    if ((h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) &&
        (h != ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) &&
        (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS)) {
      throw(
          runtime_error("CyranoInterface::buildMaterialPropertiesList: "
                        "unsupported modelling hypothesis "));
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      if (mb.getSymmetryType() == mfront::ISOTROPIC) {
        this->appendToMaterialPropertiesList(mprops, "stress", "YoungModulus",
                                             "youn", false);
        this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio",
                                             "nu", false);
      } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
        this->appendToMaterialPropertiesList(mprops, "stress", "YoungModulus1",
                                             "yg1", false);
        this->appendToMaterialPropertiesList(mprops, "stress", "YoungModulus2",
                                             "yg2", false);
        this->appendToMaterialPropertiesList(mprops, "stress", "YoungModulus3",
                                             "yg3", false);
        this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio12",
                                             "nu12", false);
        this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio23",
                                             "nu23", false);
        this->appendToMaterialPropertiesList(mprops, "real", "PoissonRatio13",
                                             "nu13", false);
      } else {
        string msg(
            "CyranoInterface::buildMaterialPropertiesList : "
            "unsupported behaviour symmetry type");
        throw(runtime_error(msg));
      }
    }
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      if (mb.getSymmetryType() == mfront::ISOTROPIC) {
        this->appendToMaterialPropertiesList(mprops, "thermalexpansion",
                                             "ThermalExpansion", "alph", false);
      } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
        this->appendToMaterialPropertiesList(
            mprops, "thermalexpansion", "ThermalExpansion1", "alp1", false);
        this->appendToMaterialPropertiesList(
            mprops, "thermalexpansion", "ThermalExpansion2", "alp2", false);
        this->appendToMaterialPropertiesList(
            mprops, "thermalexpansion", "ThermalExpansion3", "alp3", false);
      } else {
        throw(
            runtime_error("CyranoInterface::buildMaterialPropertiesList: "
                          "unsupported behaviour symmetry type"));
      }
    }
    if (!mprops.empty()) {
      const auto& m = mprops.back();
      res.second = m.offset;
      res.second += this->getTypeSize(m.type, m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops, mb, h);
    return res;
  }  // end of CyranoInterface::buildMaterialPropertiesList

  std::set<CyranoInterface::Hypothesis>
  CyranoInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    // treatment
    std::set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    // cyrano only supports the AxisymmetricalGeneralisedPlaneStrain
    // and the AxisymmetricalGeneralisedPlaneStress hypotheses
    if (bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) !=
        bh.end()) {
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) !=
        bh.end()) {
      h.insert(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
    }
    if (h.empty()) {
      throw(std::runtime_error(
          "CyranoInterface::getModellingHypothesesToBeTreated : "
          "no hypotheses selected. This means that the given beahviour "
          "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
          "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
          "make sense to use the Cyrano interface"));
    }
    return h;
  }  // end of CyranoInterface::getModellingHypothesesToBeTreated

  void CyranoInterface::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << getFunctionName(name) << "_getOutOfBoundsPolicy(void){\n"
        << "using namespace cyrano;\n"
        << "using namespace tfel::material;\n"
        << "static OutOfBoundsPolicy policy = "
           "CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy()."
           "getOutOfBoundsPolicy();\n"
        << "return policy;\n"
        << "}\n\n";
  }  // end of
     // MFrontCyranoInterface::writeGetOutOfBoundsPolicyFunctionImplementation

  void CyranoInterface::endTreatment(const BehaviourDescription& mb,
                                     const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("Cyrano::endTreatment: " + m));
      }
    };
    // get the modelling hypotheses to be treated
    const auto& h = this->getModellingHypothesesToBeTreated(mb);
    const auto name = mb.getLibrary() + mb.getClassName();
    // some checks
    throw_if(mb.getBehaviourType() !=
                 BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
             "the cyrano interface only supports "
             "small strain behaviours");
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      throw_if(
          mb.getSymmetryType() != mb.getElasticSymmetryType(),
          "the type of the behaviour (isotropic or orthotropic) does not "
          "match the the type of its elastic behaviour.\n"
          "This is not allowed here :\n"
          "- an isotropic behaviour must have an isotropic elastic behaviour\n"
          "- an orthotropic behaviour must have an orthotropic elastic "
          "behaviour");
    }
    if (this->useTimeSubStepping) {
      throw_if(
          this->maximumSubStepping == 0u,
          "use of time sub stepping requested but MaximumSubStepping is zero.\n"
          "Please use the @CyranoMaximumSubStepping directive");
    }
    // create the output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Cyrano");

    // opening header file
    string fileName("cyrano");
    fileName += name;
    fileName += ".hxx";

    ofstream out("include/MFront/Cyrano/" + fileName);
    throw_if(!out, "could not open file '" + fileName + "'");

    out << "/*!\n";
    out << "* \\file   " << fileName << endl;
    out << "* \\brief  This file declares the cyrano interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << endl;
    out << "* \\date   " << fd.date << endl;
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Cyrano/CyranoTraits.hxx\"\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace cyrano{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(h)) {
      this->writeCyranoBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& elem : h) {
      if (mb.hasSpecialisedMechanicalData(elem)) {
        this->writeCyranoBehaviourTraits(out, mb, elem);
      }
    }

    out << "} // end of namespace cyrano\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);

    this->writeCyranoFunctionDeclaration(out, name);

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName = "cyrano";
    fileName += name;
    fileName += ".cxx";

    out.open("src/" + fileName);
    if (!out) {
      string msg("CyranoInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   " << fileName << endl;
    out << "* \\brief  This file implements the cyrano interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << endl;
    out << "* \\date   " << fd.date << endl;
    out << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (this->generateMTestFile) {
      out << "#include\"MFront/Cyrano/CyranoGetModellingHypothesis.hxx\"\n";
    }
    out << "#include\"MFront/Cyrano/CyranoInterface.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx\"\n";
    out << "#include\"MFront/Cyrano/cyrano" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    this->generateUMATxxGeneralSymbols(out, name, mb, fd);
    if (!mb.areAllMechanicalDataSpecialised(h)) {
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out, name, uh, mb, fd);
    }
    for (const auto& elem : h) {
      if (mb.hasSpecialisedMechanicalData(elem)) {
        this->generateUMATxxSymbols(out, name, elem, mb, fd);
      }
    }

    out << "MFRONT_SHAREDOBJ unsigned short cyrano" << makeLowerCase(name)
        << "_Interface = 1u;\n\n";

    this->writeSetParametersFunctionsImplementations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    this->writeStandardCyranoFunction(out, name, mb);

    out << "} // end of extern \"C\"\n";
    out.close();
  }  // end of CyranoInterface::endTreatment

  void CyranoInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(cyrano::getModellingHypothesis(*NDI));\n";
  }

  CyranoInterface::~CyranoInterface() = default;

  void CyranoInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    const auto lib = CyranoInterface::getLibraryName(bd);
    const auto name =
        ((!bd.getLibrary().empty()) ? bd.getLibrary() : "") + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags, "$(shell " + tfel_config + " --includes)");
#if CYRANO_ARCH == 64
    insert_if(d[lib].cppflags, "-DCYRANO_ARCH=64");
#elif CYRANO_ARCH == 32
    insert_if(d[lib].cppflags, "-DCYRANO_ARCH=32");
#else
#error "CyranoInterface::getGlobalIncludes : unsuported architecture"
#endif
    insert_if(d[lib].sources, "cyrano" + name + ".cxx");
    insert_if(d[lib].epts, name);
    insert_if(d[lib].epts, this->getFunctionName(name));
    insert_if(d.headers, "MFront/Cyrano/cyrano" + name + ".hxx");
    insert_if(d[lib].ldflags,
              "-l" + tfel::getLibraryInstallName("CyranoInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].ldflags,
                "-l" + tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
  }  // end of CyranoInterface::getTargetsDescription(TargetsDescription&)

  void CyranoInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"\n\n";
  }

  void CyranoInterface::writeUMATxxAdditionalSymbols(
      std::ostream&,
      const std::string&,
      const Hypothesis,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of CyranoInterface::writeUMATxxAdditionalSymbols

  void CyranoInterface::writeCyranoFunctionDeclaration(
      std::ostream& out, const std::string& name) const {
    using namespace std;
    out << "MFRONT_SHAREDOBJ void\n"
        << name
        << "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoInt *const);\n\n";
    out << "MFRONT_SHAREDOBJ void\n"
        << makeUpperCase(name) << "_F77"
        << "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoInt *const);\n\n";
    out << "MFRONT_SHAREDOBJ void\ncyrano" << makeLowerCase(name)
        << "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoInt *const);\n\n";
    out << "MFRONT_SHAREDOBJ void\n"
        << "cyrano" << makeUpperCase(name) << "_F77"
        << "(const cyrano::CyranoInt *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,      cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "const cyrano::CyranoReal *const,const cyrano::CyranoReal *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoReal *const,const cyrano::CyranoInt  *const,\n"
        << "      cyrano::CyranoInt *const);\n\n";
  }  // end of CyranoInterface::writeCyranoFunctionDeclaration

  static void writeSecondaryStandardCyranoFunction(std::ostream& out,
                                                   const std::string& fname,
                                                   const std::string& n) {
    out << "MFRONT_SHAREDOBJ void\n"
        << fname
        << "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal "
           "*const DTIME,\n"
        << "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const "
           "DDSOE,\n"
        << "const cyrano::CyranoReal *const STRAN, const cyrano::CyranoReal "
           "*const DSTRAN,\n"
        << "const cyrano::CyranoReal *const TEMP,  const cyrano::CyranoReal "
           "*const DTEMP,\n"
        << "const cyrano::CyranoReal *const PROPS, const cyrano::CyranoInt    "
           "*const NPROPS,\n"
        << "const cyrano::CyranoReal *const PREDEF,const cyrano::CyranoReal "
           "*const DPRED,\n"
        << "cyrano::CyranoReal *const STATEV,const cyrano::CyranoInt    *const "
           "NSTATV,\n"
        << "cyrano::CyranoReal *const STRESS,const cyrano::CyranoInt    *const "
           "NDI,\n"
        << "cyrano::CyranoInt    *const KINC)\n"
        << "{\n"
        << n << "(NTENS, DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "STRESS,NDI,KINC);\n";
    out << "}\n\n";
  }

  void CyranoInterface::writeStandardCyranoFunction(
      std::ostream& out,
      const std::string& n,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << n
        << "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal "
           "*const DTIME,\n"
        << "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const "
           "DDSOE,\n"
        << "const cyrano::CyranoReal *const STRAN, const cyrano::CyranoReal "
           "*const DSTRAN,\n"
        << "const cyrano::CyranoReal *const TEMP,  const cyrano::CyranoReal "
           "*const DTEMP,\n"
        << "const cyrano::CyranoReal *const PROPS, const cyrano::CyranoInt    "
           "*const NPROPS,\n"
        << "const cyrano::CyranoReal *const PREDEF,const cyrano::CyranoReal "
           "*const DPRED,\n"
        << "cyrano::CyranoReal *const STATEV,const cyrano::CyranoInt    *const "
           "NSTATV,\n"
        << "cyrano::CyranoReal *const STRESS,const cyrano::CyranoInt    *const "
           "NDI,\n"
        << "cyrano::CyranoInt    *const KINC)\n";
    out << "{\n";
    out << "tfel::material::OutOfBoundsPolicy op = " << getFunctionName(n)
        << "_getOutOfBoundsPolicy();\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    out << "cyrano::CyranoInterface<tfel::material::" << mb.getClassName()
        << ">::exe(NTENS,DTIME,DROT,DDSOE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,NPROPS,"
        << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC,op);\n";
    if (this->generateMTestFile) {
      out << "if(*KINC!=1){\n";
      this->generateMTestFile2(
          out, BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR, n, "", mb);
      out << "}\n";
    }
    out << "}\n\n";
    writeSecondaryStandardCyranoFunction(out, this->getFunctionName(n), n);
  }

  void CyranoInterface::writeCyranoBehaviourTraits(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    using namespace std;
    using namespace tfel::material;
    const pair<SupportedTypes::TypeSize, SupportedTypes::TypeSize> mvs =
        mb.getMainVariablesSize();
    pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize> mprops =
        this->buildMaterialPropertiesList(mb, h);
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "template<tfel::material::ModellingHypothesis::Hypothesis "
             "H,typename Type";
      if (mb.useQt()) {
        out << ",bool use_qt";
      }
    } else {
      out << "template<typename Type";
      if (mb.useQt()) {
        out << ",bool use_qt";
      }
    }
    out << ">\n";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "struct CyranoTraits<tfel::material::" << mb.getClassName()
          << "<H,Type,";
    } else {
      out << "struct CyranoTraits<tfel::material::" << mb.getClassName()
          << "<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ",Type,";
    }
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >{\n";
    if (h != ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "// modelling hypothesis\n";
      out << "static " << constexpr_c
          << " tfel::material::ModellingHypothesis::Hypothesis H = "
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";" << endl;
    }
    out << "// space dimension\n";
    out << "static " << constexpr_c
        << " unsigned short N           = "
           "tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n";
    out << "// size of the driving variable array (STRAN)\n";
    out << "static " << constexpr_c
        << " unsigned short DrivingVariableSize  = " << mvs.first << ";\n";
    out << "// size of the thermodynamic force variable array (STRAN)\n";
    out << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize  = " << mvs.second
        << ";\n";
    out << "static " << constexpr_c << " bool useTimeSubStepping = ";
    if (this->useTimeSubStepping) {
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " bool doSubSteppingOnInvalidResults = ";
    if (this->doSubSteppingOnInvalidResults) {
      out << "true;\n";
    } else {
      out << "false;\n";
    }
    out << "static " << constexpr_c << " unsigned short maximumSubStepping = ";
    if (this->useTimeSubStepping) {
      out << this->maximumSubStepping << ";\n";
    } else {
      out << "0u;\n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = true;\n";
      if (mb.getAttribute(
              BehaviourDescription::requiresUnAlteredStiffnessTensor, false)) {
        out << "static " << constexpr_c
            << " bool requiresUnAlteredStiffnessTensor = true;\n";
      } else {
        out << "static " << constexpr_c
            << " bool requiresUnAlteredStiffnessTensor = false;\n";
      }
    } else {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    // computing material properties size
    SupportedTypes::TypeSize msize;
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += this->getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        if (mb.getAttribute(
                BehaviourDescription::requiresThermalExpansionCoefficientTensor,
                false)) {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 3u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 2u;\n";
        } else {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 2u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 2u;\n";
        }
      } else {
        if (mb.getAttribute(
                BehaviourDescription::requiresThermalExpansionCoefficientTensor,
                false)) {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 1u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 0u;\n";
        } else {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 0u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 0u;\n";
        }
      }
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        if (mb.getAttribute(
                BehaviourDescription::requiresThermalExpansionCoefficientTensor,
                false)) {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 9u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 6u;\n";
        } else {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 6u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 6u;\n";
        }
      } else {
        if (mb.getAttribute(
                BehaviourDescription::requiresThermalExpansionCoefficientTensor,
                false)) {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset        = 3u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 0u;\n";
        } else {
          out << "static " << constexpr_c
              << " unsigned short propertiesOffset = 0u;\n";
          out << "static " << constexpr_c
              << " unsigned short elasticPropertiesOffset = 0u;\n";
        }
      }
    } else {
      string msg("CyranoInterface::writeCyranoBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg +=
          "The cyrano interface only support isotropic or orthotropic "
          "behaviour at this time.";
      throw(runtime_error(msg));
    }
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " CyranoSymmetryType stype = cyrano::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " CyranoSymmetryType stype = cyrano::ORTHOTROPIC;\n";
    } else {
      string msg("CyranoInterface::writeCyranoBehaviourTraits : ");
      msg += "unsupported behaviour symmetry type.\n";
      msg +=
          "The cyrano interface only support isotropic or orthotropic "
          "behaviour at this time.";
      throw(runtime_error(msg));
    }
    out << "}; // end of class CyranoTraits\n\n";
  }  // end of CyranoInterface::writeCyranoBehaviourTraits

  std::string CyranoInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    using namespace std;
    ostringstream test;
    test << "*NDI==" << this->getModellingHypothesisIdentifier(h);
    return test.str();
  }

}  // end of namespace mfront
