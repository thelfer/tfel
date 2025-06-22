/*!
 * \file   mfront/src/CyranoInterface.cxx
 * \brief
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/CyranoMaterialPropertyInterface.hxx"
#include "MFront/CyranoSymbolsGenerator.hxx"
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
    tfel::raise(msg.str());
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

  std::string CyranoInterface::getFunctionNameBasis(
      const std::string& n) const {
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
  CyranoInterface::treatKeyword(BehaviourDescription& bd,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "Cyrano::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) == i.end()) {
        return {false, current};
      }
    }
    if ((key == "@CyranoGenerateMTestFileOnFailure") ||
        (key == "@UMATGenerateMTestFileOnFailure") ||
        (key == "@GenerateMTestFileOnFailure")) {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(key, current, end));
      return {true, current};
    } else if ((key == "@CyranoUseTimeSubStepping") ||
               (key == "@UMATUseTimeSubStepping")) {
      this->useTimeSubStepping = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if ((key == "@CyranoMaximumSubStepping") ||
               (key == "@UMATMaximumSubStepping")) {
      throw_if(!this->useTimeSubStepping,
               "time sub stepping is not enabled at this stage.\n"
               "Use the @CyranoUseTimeSubStepping directive before "
               "@CyranoMaximumSubStepping");
      throw_if(current == end, "unexpected end of file");
      std::istringstream flux(current->value);
      flux >> this->maximumSubStepping;
      throw_if(flux.fail(), "failed to read maximum substepping value.");
      ++(current);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';',read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if ((key == "@CyranoDoSubSteppingOnInvalidResults") ||
               (key == "@UMATDoSubSteppingOnInvalidResults")) {
      throw_if(!this->useTimeSubStepping,
               "time sub stepping is not "
               "enabled at this stage.\n"
               "Use the @CyranoUseTimeSubStepping directive before "
               "@CyranoMaximumSubStepping");
      this->doSubSteppingOnInvalidResults =
          this->readBooleanValue(key, current, end);
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

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
    tfel::raise_if(
        h.empty(),
        "CyranoInterface::getModellingHypothesesToBeTreated : "
        "no hypotheses selected. This means that the given beahviour "
        "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
        "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
        "make sense to use the Cyrano interface");
    return h;
  }  // end of CyranoInterface::getModellingHypothesesToBeTreated

  void CyranoInterface::writeGetOutOfBoundsPolicyFunctionImplementation(
      std::ostream& out, const std::string& name) const {
    out << "static tfel::material::OutOfBoundsPolicy&\n"
        << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(){\n"
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
      tfel::raise_if(b, "Cyrano::endTreatment: " + m);
    };
    // get the modelling hypotheses to be treated
    const auto& mhs = this->getModellingHypothesesToBeTreated(mb);
    const auto name = mb.getLibrary() + mb.getClassName();
    // some checks
    throw_if(mb.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "the cyrano interface only supports "
             "small strain behaviours");
    if (mb.isStrainMeasureDefined()) {
      throw_if((mb.getStrainMeasure() != BehaviourDescription::LINEARISED) &&
                   (mb.getStrainMeasure() != BehaviourDescription::HENCKY),
               "the cyrano interface only supports:\n"
               "- small strain behaviours: the only strain measure "
               "supported is the HPP one (linearised)\n"
               "- finite strain behaviours based on the Hencky strain measure");
    }
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

    // write the material properties
    if (mb.areElasticMaterialPropertiesDefined()) {
      for (const auto& emp : mb.getElasticMaterialPropertiesDescriptions()) {
        CyranoMaterialPropertyInterface i;
        i.writeOutputFiles(emp, fd);
      }
    }

    // write the thermal expansions
    if (mb.areThermalExpansionCoefficientsDefined()) {
      for (const auto& themp :
           mb.getThermalExpansionCoefficientsDescriptions()) {
        CyranoMaterialPropertyInterface i;
        i.writeOutputFiles(themp, fd);
      }
    }

    // opening header file
    auto fileName = "cyrano" + name + ".hxx";
    std::ofstream out("include/MFront/Cyrano/" + fileName);
    throw_if(!out, "could not open file '" + fileName + "'");

    out << "/*!\n";
    out << "* \\file   " << fileName << endl;
    out << "* \\brief  This file declares the cyrano interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << endl;
    out << "* \\date   " << fd.date << endl;
    out << "*/\n\n";

    const auto header = this->getHeaderGuard(mb);
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

    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      this->writeCyranoBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeCyranoBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace cyrano\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetParametersFunctionsDeclarations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);

    this->writeCyranoFunctionDeclaration(out, name);

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName = "cyrano" + name + ".cxx";
    out.open("src/" + fileName);
    tfel::raise_if(!out,
                   "CyranoInterface::endTreatment: "
                   "could not open file '" +
                       fileName + "'");

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
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "#include\"MFront/Cyrano/CyranoGetModellingHypothesis.hxx\"\n";
    }
    out << "#include\"MFront/Cyrano/CyranoInterface.hxx\"\n\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#include\"MFront/Cyrano/CyranoOutOfBoundsPolicy.hxx\"\n";
    out << "#include\"MFront/Cyrano/CyranoStressFreeExpansionHandler.hxx\"\n";
    out << "#include\"MFront/Cyrano/cyrano" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    CyranoSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, mb, fd, mhs, name);
    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      sg.generateSymbols(out, *this, mb, fd, name, uh);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        sg.generateSymbols(out, *this, mb, fd, name, h);
      }
    }

    out << "MFRONT_SHAREDOBJ unsigned short cyrano" << makeLowerCase(name)
        << "_Interface = 1u;\n\n";

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    if (mb.isStrainMeasureDefined()) {
      if (mb.getStrainMeasure() == BehaviourDescription::HENCKY) {
        this->writeLogarithmicStrainCyranoFunction(out, name, mb);
      } else {
        this->writeStandardCyranoFunction(out, name, mb);
      }
    } else {
      this->writeStandardCyranoFunction(out, name, mb);
    }
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
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
#if CYRANO_ARCH == 64
    insert_if(l.cppflags, "-DCYRANO_ARCH=64");
#elif CYRANO_ARCH == 32
    insert_if(l.cppflags, "-DCYRANO_ARCH=32");
#else
#error "CyranoInterface::getGlobalIncludes : unsuported architecture"
#endif
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "cyrano" + name + ".cxx");
    insert_if(l.epts, this->getFunctionNameBasis(name));
    insert_if(d.headers, "MFront/Cyrano/cyrano" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries, tfel::getLibraryInstallName("CyranoInterface"));
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(l.link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
#if __cplusplus >= 201703L
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(l.link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    if (bd.areElasticMaterialPropertiesDefined()) {
      for (const auto& emp : bd.getElasticMaterialPropertiesDescriptions()) {
        CyranoMaterialPropertyInterface i;
        i.getLibraryDescription(d, l, emp);
      }
    }
    if (bd.areThermalExpansionCoefficientsDefined()) {
      for (const auto& themp :
           bd.getThermalExpansionCoefficientsDescriptions()) {
        CyranoMaterialPropertyInterface i;
        i.getLibraryDescription(d, l, themp);
      }
    }
  }  // end of CyranoInterface::getTargetsDescription(TargetsDescription&)

  void CyranoInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Cyrano/Cyrano.hxx\"\n\n";
  }

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

  static void writeSecondaryCyranoFunction(std::ostream& out,
                                           const std::string& fname,
                                           const std::string& n) {
    out << "MFRONT_SHAREDOBJ void\n"
        << fname
        << "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal "
           "*const DTIME,\n"
        << "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const "
           "DDSDDE,\n"
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
        << n << "(NTENS, DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,\n"
        << "PROPS,NPROPS,PREDEF,DPRED,STATEV,NSTATV,\n"
        << "STRESS,NDI,KINC);\n";
    out << "}\n\n";
  }  // end of writeSecondaryCyranoFunction

  void CyranoInterface::writeStandardCyranoFunction(
      std::ostream& out,
      const std::string& n,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ void\n"
        << n
        << "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal "
           "*const DTIME,\n"
        << "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const "
           "DDSDDE,\n"
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
    out << "const auto op = " << this->getFunctionNameBasis(n)
        << "_getOutOfBoundsPolicy();\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out, mb);
    out << "cyrano::CyranoInterface<tfel::material::" << mb.getClassName()
        << ">::exe(NTENS,DTIME,DROT,DDSDDE,STRAN,DSTRAN,TEMP,DTEMP,PROPS,"
           "NPROPS,"
        << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NDI,KINC,"
        << "cyrano::CyranoStandardSmallStrainStressFreeExpansionHandler,op);\n";
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "if(*KINC<0){\n";
      this->generateMTestFile2(
          out, mb, BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR, n, "");
      out << "}\n";
    }
    out << "}\n\n";
    writeSecondaryCyranoFunction(out, this->getFunctionNameBasis(n), n);
  }  // end of CyranoInterface::writeStandardCyranoFunction

  void CyranoInterface::writeLogarithmicStrainCyranoFunction(
      std::ostream& out,
      const std::string& n,
      const BehaviourDescription& mb) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        tfel::raise("CyranoInterface::writeLogarithmicStrainCyranoFunction: " +
                    m);
      }
    };
    out << "MFRONT_SHAREDOBJ void\n"
        << n
        << "(const cyrano::CyranoInt *const NTENS, const cyrano::CyranoReal "
           "*const DTIME,\n"
        << "const cyrano::CyranoReal *const DROT,  cyrano::CyranoReal *const "
           "DDSDDE,\n"
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
        << "{\n";
    out << "const auto op = " << this->getFunctionNameBasis(n)
        << "_getOutOfBoundsPolicy();\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out, mb);
    out << "const auto k = std::abs(*DDSDDE)>0.5;\n"
        << "// computing the logarithmic strain\n"
        << "cyrano::CyranoReal eto[3];\n"
        << "cyrano::CyranoReal deto[3];\n"
        << "cyrano::CyranoReal s[3];\n"
        << "cyrano::CyranoReal K[9];\n";
    // axisymmetrical generalised plane stress
    out << "if(*NDI!=1){\n"
        << "// axisymmetrical generalised plane stress\n";
    if (mb.isModellingHypothesisSupported(
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
      const auto& d = mb.getBehaviourData(
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
      // this must be added for gcc 4.7.2
      const auto astress =
          [this, &d, throw_if]() -> std::pair<bool, SupportedTypes::TypeSize> {
        SupportedTypes::TypeSize o;
        // skipping the temperature
        auto pev = std::next(d.getExternalStateVariables().begin());
        while (pev != d.getExternalStateVariables().end()) {
          if (d.getExternalName(pev->name) ==
              tfel::glossary::Glossary::AxialStress) {
            throw_if(
                SupportedTypes::getTypeFlag(pev->type) !=
                    SupportedTypes::SCALAR,
                "invalid type for the `AxialStress` external state variable");
            return {true, o};
          }
          o += SupportedTypes::getTypeSize(pev->type, pev->arraySize);
          ++pev;
        }
        return {false, o};
      }();
      const auto astrain = this->checkIfAxialStrainIsDefinedAndGetItsOffset(
          mb, ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
      tfel::raise_if(
          !astress.first,
          "no external state state variable standing for the axial stress");
      tfel::raise_if(!astrain.first,
                     "no state variable standing for the axial strain");
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "{\n"
            << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
      }
      out << "const auto Pzz0 = PREDEF["
          << astress.second.getValueForDimension(1) << "];\n"
          << "const auto Pzz1 = Pzz0+DPRED["
          << astress.second.getValueForDimension(1) << "];\n"
          << "const auto Tzz0 = "
          << "Pzz0*std::exp(STATEV[" << astrain.second.getValueForDimension(1)
          << "]);\n"
          << "eto[0]=std::log1p(*STRAN);\n"
          << "eto[1]=std::log1p(*(STRAN+1));\n"
          << "eto[2]=0;\n"
          << "deto[0]=std::log1p(*STRAN+*DSTRAN)-eto[0];\n"
          << "deto[1]=std::log1p(*(STRAN+1)+*(DSTRAN+1))-eto[1];\n"
          << "deto[2]=0;\n"
          << "s[0]=(*STRESS)*(1+*STRAN);\n"
          << "s[1]=(*(STRESS+1))*(1+*(STRAN+1));\n"
          << "s[2]=Tzz0;\n"
          << "K[0]=*DDSDDE;\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "}\n";
      }
      out << "cyrano::CyranoInterface<tfel::material::" << mb.getClassName()
          << ">::exe(NTENS,DTIME,DROT,K,eto,deto,TEMP,DTEMP,PROPS,NPROPS,"
          << "PREDEF,DPRED,STATEV,NSTATV,s,NDI,KINC,"
          << "cyrano::CyranoLogarithmicStrainStressFreeExpansionHandler,op);\n";
      out << "if(*KINC>=0){\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "{\n"
            << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
            << "Profiler::getProfiler(),\n"
            << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
      }
      // First Piola-Kirchhoff stress
      out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n"
          << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n"
          << "STRESS[2]=Pzz1;\n";
      // computation of the stiffness matrix
      out << "if(k){\n"
          << "*DDSDDE     = "
             "(-STRESS[0]+K[0]/(1+STRAN[0]+DSTRAN[0]))/"
             "(1+STRAN[0]+DSTRAN[0]);\n"
          << "*(DDSDDE+3) = "
             "K[3]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[0]+DSTRAN[0]));\n"
          << "*(DDSDDE+6) = 0;\n"
          << "*(DDSDDE+1) = "
             "K[1]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[1]+DSTRAN[1]));\n"
          << "*(DDSDDE+4) = "
             "(-STRESS[1]+K[4]/(1+STRAN[1]+DSTRAN[1]))/"
             "(1+STRAN[1]+DSTRAN[1]);\n"
          << "*(DDSDDE+7) = 0;\n"
          << "*(DDSDDE+2) = 0;\n"
          << "*(DDSDDE+5) = 0;\n"
          << "*(DDSDDE+8) = 0;\n"
          << "}\n";
      if (mb.getAttribute(BehaviourData::profiling, false)) {
        out << "}\n";
      }
      out << "}\n";
    } else {
      out << "*KINC=-7;\n"
          << "return;\n";
    }
    // axisymmetrical generalised plane strain
    out << "} else {\n"
        << "// axisymmetrical generalised plane strain\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "eto[0]=std::log1p(*STRAN);\n"
        << "eto[1]=std::log1p(*(STRAN+1));\n"
        << "eto[2]=std::log1p(*(STRAN+2));\n"
        << "deto[0]=std::log1p(*STRAN+*DSTRAN)-eto[0];\n"
        << "deto[1]=std::log1p(*(STRAN+1)+*(DSTRAN+1))-eto[1];\n"
        << "deto[2]=std::log1p(*(STRAN+2)+*(DSTRAN+2))-eto[2];\n"
        << "s[0]=(*STRESS)*(1+*STRAN);\n"
        << "s[1]=(*(STRESS+1))*(1+*(STRAN+1));\n"
        << "s[2]=(*(STRESS+2))*(1+*(STRAN+2));\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "K[0]=*DDSDDE;\n"
        << "cyrano::CyranoInterface<tfel::material::" << mb.getClassName()
        << ">::exe(NTENS,DTIME,DROT,K,eto,deto,TEMP,DTEMP,PROPS,NPROPS,"
        << "PREDEF,DPRED,STATEV,NSTATV,s,NDI,KINC,"
        << "cyrano::CyranoLogarithmicStrainStressFreeExpansionHandler,op);\n";
    out << "if(*KINC>=0){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    // First Piola-Kirchhoff stress
    out << "STRESS[0]=s[0]/(1+*STRAN+*DSTRAN);\n"
        << "STRESS[1]=s[1]/(1+*(STRAN+1)+*(DSTRAN+1));\n"
        << "STRESS[2]=s[2]/(1+*(STRAN+2)+*(DSTRAN+2));\n";
    // computation of the stiffness matrix
    out << "if(k){\n"
        << "*DDSDDE     = "
           "(-STRESS[0]+K[0]/(1+STRAN[0]+DSTRAN[0]))/(1+STRAN[0]+DSTRAN[0]);\n"
        << "*(DDSDDE+3) = "
           "K[3]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[0]+DSTRAN[0]));\n"
        << "*(DDSDDE+6) = "
           "K[6]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[0]+DSTRAN[0]));\n"
        << "*(DDSDDE+1) = "
           "K[1]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[1]+DSTRAN[1]));\n"
        << "*(DDSDDE+4) = "
           "(-STRESS[1]+K[4]/(1+STRAN[1]+DSTRAN[1]))/(1+STRAN[1]+DSTRAN[1]);\n"
        << "*(DDSDDE+7) = "
           "K[7]/((1+STRAN[2]+DSTRAN[2])*(1+STRAN[1]+DSTRAN[1]));\n"
        << "*(DDSDDE+2) = "
           "K[2]/((1+STRAN[0]+DSTRAN[0])*(1+STRAN[2]+DSTRAN[2]));\n"
        << "*(DDSDDE+5) = "
           "K[5]/((1+STRAN[1]+DSTRAN[1])*(1+STRAN[2]+DSTRAN[2]));\n"
        << "*(DDSDDE+8) = "
           "(-STRESS[2]+K[8]/(1+STRAN[2]+DSTRAN[2]))/(1+STRAN[2]+DSTRAN[2]);\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "} // end of if(*KINC>=0)\n";
    out << "} // end of if(*NDI!=1)\n";
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "if(*KINC<0){\n";
      this->generateMTestFile2(
          out, mb, BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR, n, "");
      out << "}\n";
    }
    out << "}\n\n";
    writeSecondaryCyranoFunction(out, this->getFunctionNameBasis(n), n);
  }  // end of CyranoInterface::writeLogarithmicStrainCyranoFunction

  void CyranoInterface::writeCyranoBehaviourTraits(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    using namespace tfel::material;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
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
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    out << "// space dimension\n";
    out << "static " << constexpr_c
        << " unsigned short N           = "
           "tfel::material::ModellingHypothesisToSpaceDimension<H>::"
           "value;\n";
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
        << " unsigned short GradientSize  = " << mvs.first << ";\n";
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
      msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
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
      tfel::raise(
          "CyranoInterface::writeCyranoBehaviourTraits: "
          "unsupported behaviour symmetry type.\n"
          "The cyrano interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " CyranoSymmetryType stype = cyrano::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " CyranoSymmetryType stype = cyrano::ORTHOTROPIC;\n";
    } else {
      tfel::raise(
          "CyranoInterface::writeCyranoBehaviourTraits: "
          "unsupported behaviour symmetry type.\n"
          "The cyrano interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    out << "}; // end of class CyranoTraits\n\n";
  }  // end of CyranoInterface::writeCyranoBehaviourTraits

  std::string CyranoInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    std::ostringstream test;
    test << "*NDI==" << this->getModellingHypothesisIdentifier(h);
    return test.str();
  }

}  // end of namespace mfront
