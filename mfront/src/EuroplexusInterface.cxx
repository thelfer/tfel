/*!
 * \file   mfront/src/EuroplexusInterface.cxx
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
#include <stdexcept>

#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/EuroplexusInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void writeEPXArguments(std::ostream& out) {
    out << "(epx::EuroplexusInt  *const,\n"
        << " epx::EuroplexusReal *const,\n"
        << " epx::EuroplexusReal *const,\n"
        << " epx::EuroplexusReal *const,\n"
        << " epx::EuroplexusReal *const,\n"
        << " epx::EuroplexusInt *const,\n"
        << "                char *const,\n"
        << " const epx::EuroplexusInt  *const,\n"
        << " const epx::EuroplexusInt  *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusInt  *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusReal *const,\n"
        << " const epx::EuroplexusInt  *const)";
  }  // end of writeEuroplexusArguments

  static void writeEPXArgumentsII(std::ostream& out) {
    out << "(epx::EuroplexusInt  *const STATUS,\n"
        << " epx::EuroplexusReal *const STRESS,\n"
        << " epx::EuroplexusReal *const STATEV,\n"
        << " epx::EuroplexusReal *const DDSDDE,\n"
        << " epx::EuroplexusReal *const PNEWDT,\n"
        << " epx::EuroplexusInt  *const BROKEN,\n"
        << "                char *const MSG,\n"
        << " const epx::EuroplexusInt  *const NSTATV,\n"
        << " const epx::EuroplexusInt  *const HYPOTHESIS,\n"
        << " const epx::EuroplexusReal *const DTIME,\n"
        << " const epx::EuroplexusReal *const F0,\n"
        << " const epx::EuroplexusReal *const F1,\n"
        << " const epx::EuroplexusReal *const R,\n"
        << " const epx::EuroplexusReal *const PROPS,\n"
        << " const epx::EuroplexusInt  *const NPROPS,\n"
        << " const epx::EuroplexusReal *const TEMP,\n"
        << " const epx::EuroplexusReal *const DTEMP,\n"
        << " const epx::EuroplexusReal *const PREDEF,\n"
        << " const epx::EuroplexusReal *const DPRED,\n"
        << " const epx::EuroplexusInt  *const NPREDEF)";
  }  // end of writeEPXArguments

  std::string EuroplexusInterface::getName() { return "epx"; }

  std::string EuroplexusInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    auto lib = std::string{};
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = "Europlexus" + mb.getMaterialName();
      } else {
        lib = "EuroplexusBehaviour";
      }
    } else {
      lib = "Europlexus" + mb.getLibrary();
    }
    return lib;
  }  // end of EuroplexusInterface::getLibraryName

  std::string EuroplexusInterface::getInterfaceName() const {
    return "Europlexus";
  }  // end of EuroplexusInterface::getInterfaceName

  std::string EuroplexusInterface::getFunctionName(
      const std::string& name) const {
    return name;
  }  // end of EuroplexusInterface::getLibraryName

  std::pair<bool, EuroplexusInterface::tokens_iterator>
  EuroplexusInterface::treatKeyword(BehaviourDescription&,
                                    const std::string& key,
                                    const std::vector<std::string>& i,
                                    tokens_iterator current,
                                    const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("EuroplexusInterface::treatKeyword: " + m));
      }
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        throw_if((key != "@EuroplexusFiniteStrainStrategy") &&
                     (key != "@EPXFiniteStrainStrategy") &&
                     (key != "@EuroplexusGenerateMTestFileOnFailure") &&
                     (key != "@EPXGenerateMTestFileOnFailure"),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if ((key == "@EuroplexusFiniteStrainStrategy") ||
        (key == "@EPXFiniteStrainStrategy")) {
      auto read = [](const std::string& s) {
        if (s == "FiniteRotationSmallStrain") {
          return FINITEROTATIONSMALLSTRAIN;
        } else if (s == "MieheApelLambrechtLogarithmicStrain") {
          return MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN;
        } else {
          throw(
              std::runtime_error("EuroplexusInterface::treatKeyword: "
                                 "unsupported strategy '" +
                                 s +
                                 "'\n"
                                 "The only supported strategies are "
                                 "'FiniteRotationSmallStrain' and "
                                 "'MieheApelLambrechtLogarithmicStrain'"));
        }
      };
      throw_if(this->fss != UNDEFINEDSTRATEGY,
               "a finite strain strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      this->fss = read(current->value);
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      return {true, current};
    } else if ((key == "@EuroplexusGenerateMTestFileOnFailure") ||
               (key == "@EPXGenerateMTestFileOnFailure")) {
      this->generateMTestFile = this->readBooleanValue(key, current, end);
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  std::set<EuroplexusInterface::Hypothesis>
  EuroplexusInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    // treatment
    std::set<ModellingHypothesis::Hypothesis> mh;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    for (const auto h :
         {ModellingHypothesis::AXISYMMETRICAL, ModellingHypothesis::PLANESTRAIN,
          ModellingHypothesis::PLANESTRESS,
          ModellingHypothesis::TRIDIMENSIONAL}) {
      if (bh.find(h) != bh.end()) {
        mh.insert(h);
      }
    }
    if (mh.empty()) {
      throw(std::runtime_error(
          "EuroplexusInterfaceModellingHypothesesToBeTreated : "
          "no hypotheses selected, so it does not "
          "make sense to use the Europlexus interface"));
    }
    return mh;
  }  // end of EuroplexusInterface::getModellingHypothesesToBeTreated

  void EuroplexusInterface::endTreatment(const BehaviourDescription& mb,
                                         const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("EuroplexusInterface::endTreatment: " + m));
      }
    };
    throw_if((mb.getBehaviourType() !=
              BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) &&
                 (mb.getBehaviourType() !=
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR),
             "the europlexus interface only supports "
             "finite strain behaviours");
    if (this->fss == UNDEFINEDSTRATEGY) {
      throw_if(mb.getBehaviourType() ==
                   BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
               "behaviours written in the small strain framework "
               "must be embedded in a strain strategy");
    } else {
      throw_if(mb.getBehaviourType() !=
                   BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
               "finite strain strategies can only be defined "
               "for behaviours writtent in the "
               "small strain framework");
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Europlexus");
    systemCall::mkdir("src");
    systemCall::mkdir("europlexus");

    // header
    auto fname = "europlexus" + name + ".hxx";
    std::ofstream out("include/MFront/Europlexus/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the europlexus interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
        << "#include\"MFront/Europlexus/Europlexus.hxx\"\n\n"
        << "#ifdef __cplusplus\n"
        << "#include\"MFront/Europlexus/EuroplexusTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Europlexus/"
             "EuroplexusOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace epx{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      this->writeEuroplexusBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mh) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeEuroplexusBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace epx\n\n"
        << "#endif /* __cplusplus */\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, name, mb);

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
    writeEPXArguments(out);
    out << ";\n\n"
        << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "epx" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the europlexus interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);

    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (this->fss != UNDEFINEDSTRATEGY) {
      out << "#include\"MFront/Europlexus/EuroplexusFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/Europlexus/"
           "EuroplexusStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/Europlexus/EuroplexusInterface.hxx\"\n\n"
        << "#include\"MFront/Europlexus/europlexus" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    this->generateUMATxxGeneralSymbols(out, name, mb, fd);
    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out, name, uh, mb, fd);
    }
    for (const auto& h : mh) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->generateUMATxxSymbols(out, name, h, mb, fd);
      }
    }

    this->writeSetParametersFunctionsImplementations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
    writeEPXArgumentsII(out);
    out << "{\n";
    if (getDebugMode()) {
      out << "using namespace std;\n";
    }
    out << "using tfel::material::ModellingHypothesis;\n"
        << "using tfel::material::" << mb.getClassName() << ";\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    if (this->fss == FINITEROTATIONSMALLSTRAIN) {
      this->writeFiniteRotationSmallStrainCall(out, mb, name);
    } else if (this->fss == MIEHEAPELLAMBRECHTLOGARITHMICSTRAIN) {
      this->writeLogarithmicStrainCall(out, mb, name);
    } else {
      // this->generateMTestFile1(out);
      out << "const epx::EPXData d = "
             "{STATUS,STRESS,STATEV,DDSDDE,PNEWDT,BROKEN,MSG,\n"
          << "                        *NSTATV,*DTIME,F0,F1,R,PROPS,*NPROPS,\n"
          << "                        TEMP,DTEMP,PREDEF,DPRED,*NPREDEF,\n"
          << "                        " << getFunctionName(name)
          << "_getOutOfBoundsPolicy(),\n"
          << "                        nullptr};\n"
          << "epx::EuroplexusInterface<" << mb.getClassName()
          << ">::exe(d,*HYPOTHESIS);\n";
    }

    // this->generateMTestFile2(out,mb.getBehaviourType(),
    // 			     name,"",mb);
    out << "}\n\n";
    out << "} // end of extern \"C\"\n";
    out.close();
    out.open("europlexus/" + mb.getClassName() + ".epx");
    out << "!\n"
        << "! \\file   " << fd.fileName << '\n'
        << "! \\brief  example of how to use the " << mb.getClassName()
        << " behaviour law\n"
        << "! in the Cast3M finite element solver\n"
        << "! \\author " << fd.authorName << '\n'
        << "! \\date   " << fd.date << '\n'
        << "!\n";
    for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
      const auto& d = mb.getBehaviourData(h);
      out << "\n!! " << ModellingHypothesis::toString(h) << " example\n"
          << "MFRONT\n";
      out << "LIB 'lib" << this->getLibraryName(mb) << ".so'\n";
      out << "LAW '" << this->getFunctionName(name) << "'\n"
          << "!! material properties\n";
      for (const auto& mp : this->buildMaterialPropertiesList(mb, h).first) {
        throw_if(this->getTypeFlag(mp.type) != SupportedTypes::Scalar,
                 "material property '" + mp.name +
                     "' is not a scalar. "
                     "This is not supported yet");
        if (mp.arraySize == 1u) {
          out << "MATP '" << mp.name << "' ???\n";
        } else {
          for (unsigned short i = 0; i != mp.arraySize; ++i) {
            out << "MATP '" << mp.name << '[' << i << "]' ???\n";
          }
        }
      }
      out << "!! internal state variables\n";
      out << "!! by default, internal state variables are set to zero\n";
      for (const auto& iv : d.getPersistentVariables()) {
        auto display = [&h, &out, &throw_if](const SupportedTypes::TypeFlag f) {
          if (f == SupportedTypes::Scalar) {
            out << " ???";
          } else if (f == SupportedTypes::Stensor) {
            if (h == ModellingHypothesis::TRIDIMENSIONAL) {
              out << " ??? ??? ??? ??? ??? ???";
            } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                       (h == ModellingHypothesis::PLANESTRAIN) ||
                       (h == ModellingHypothesis::PLANESTRESS)) {
              out << " ??? ??? ??? ???";
            }
          } else if (f == SupportedTypes::Tensor) {
            if (h == ModellingHypothesis::TRIDIMENSIONAL) {
              out << " ??? ??? ??? ??? ??? ??? ??? ??? ???";
            } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                       (h == ModellingHypothesis::PLANESTRAIN) ||
                       (h == ModellingHypothesis::PLANESTRESS)) {
              out << " ??? ??? ??? ??? ???";
            }
          } else {
            throw_if(true, "unsupported state variable type");
          }
        };
        if (iv.arraySize == 1u) {
          out << "!! IVAR '" << d.getExternalName(iv.name) << "' ";
          display(this->getTypeFlag(iv.type));
          out << '\n';
        } else {
          for (unsigned short i = 0; i != iv.arraySize; ++i) {
            out << "!! IVAR '" << d.getExternalName(iv.name) << '[' << i
                << "]' ";
            display(this->getTypeFlag(iv.type));
            out << '\n';
          }
        }
      }
      out << "!! external state variables\n"
          << "EVAR 'Temperature' 293.15\n";
      for (const auto& e : d.getExternalStateVariables()) {
        throw_if(this->getTypeFlag(e.type) != SupportedTypes::Scalar,
                 "external state variable '" + e.name +
                     "' is not a scalar. This is not supported yet");
        if (e.arraySize == 1u) {
          out << "EVAR '" << d.getExternalName(e.name) << "' ???\n";
        } else {
          for (unsigned short i = 0; i != e.arraySize; ++i) {
            out << "EVAR '" << d.getExternalName(e.name) << '[' << i
                << "]' ???\n";
          }
        }
      }
      out << "LECT MESH TERM\n";
    }
    out.close();
  }  // end of EuroplexusInterface::endTreatment

  void EuroplexusInterface::writeFiniteRotationSmallStrainCall(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    if (mb.getBehaviourType() !=
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      throw(std::runtime_error(
          "EuroplexusInterface::writeLogarithmicStrainCall: "
          "internal error, the 'finite rotation small strain' "
          "strategy shall be used only with small "
          "strain behaviour"));
    }
    const auto sfeh =
        "epx::EuroplexusStandardSmallStrainStressFreeExpansionHandler";
    out << "epx::EuroplexusReal eto[6];\n"
        << "epx::EuroplexusReal deto[6];\n"
        << "epx::EuroplexusReal sig[6];\n"
        << "epx::computeGreenLagrangeStrain(eto,deto,F0,F1,*HYPOTHESIS);\n"
        << "const bool b = std::abs(*DDSDDE)>0.5;\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if (v.first) {
        out << "const epx::EuroplexusReal ezz = "
            << "STATEV[" << v.second.getValueForDimension(2) << "];\n"
            << "epx::EuroplexusReal F_0[5] = "
               "{F0[0],F0[1],std::sqrt(1+2*ezz),F0[3],F0[4]};\n"
            << "epx::computeSecondPiolaKirchhoffStressFromCauchyStress(sig,"
               "STRESS,"
            << "F_0,*HYPOTHESIS);\n";
      } else {
        // no axial strain
        out << "std::cerr << \"no state variable standing for the axial strain "
               "(variable with the "
            << "glossary name 'AxialStrain')\" << std::endl;\n";
        out << "*STATUS=-2;\n";
        out << "return;\n";
      }
    } else {
      out << "*STATUS=-2;\n"
          << "return;\n";
    }
    out << "} else {\n"
        << "epx::computeSecondPiolaKirchhoffStressFromCauchyStress(sig,STRESS,"
           "F0,*HYPOTHESIS);\n"
        << "}\n"
        << "const epx::EPXData d = "
           "{STATUS,sig,STATEV,DDSDDE,PNEWDT,BROKEN,MSG,\n"
        << "                        *NSTATV,*DTIME,eto,deto,R,PROPS,*NPROPS,\n"
        << "                        TEMP,DTEMP,PREDEF,DPRED,*NPREDEF,\n"
        << "                        " << this->getFunctionName(name)
        << "_getOutOfBoundsPolicy(),\n"
        << "                        " << sfeh << "};\n"
        << "epx::EuroplexusInterface<" << mb.getClassName()
        << ">::exe(d,*HYPOTHESIS);\n"
        << "if(*STATUS==0){\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if (v.first) {
        out << "const epx::EuroplexusReal ezz = "
            << "STATEV[" << v.second.getValueForDimension(2) << "];\n"
            << "epx::EuroplexusReal F_1[5] = "
               "{F1[1],F1[1],std::sqrt(1+2*ezz),F1[3],F1[4]};\n"
            << "epx::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,"
               "sig,F_1,*HYPOTHESIS);\n";
      } else {
        // no axial strain
        out << "std::cerr << \"no state variable standing for the axial strain "
               "(variable with the "
            << "glossary name 'AxialStrain')\" << std::endl;\n";
        out << "*STATUS=-2;\n";
        out << "return;\n";
      }
    } else {
      out << "*STATUS=-2;\n"
          << "return;\n";
    }
    out << "} else {\n"
        << "epx::computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,sig,"
           "F1,*HYPOTHESIS);\n"
        << "}\n"
        << "if(b){\n"
        << "epx::computeElasticModuli(DDSDDE,*HYPOTHESIS);\n"
        << "}\n"
        << "}\n";
  }

  void EuroplexusInterface::writeLogarithmicStrainCall(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    if (mb.getBehaviourType() !=
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      throw(std::runtime_error(
          "EuroplexusInterface::writeFiniteRotationSmallStrainCall: "
          "internal error, the 'finite rotation small strain' "
          "strategy shall be used only with small "
          "strain behaviour"));
    }
    const auto sfeh =
        "epx::EuroplexusLogarithmicStrainStressFreeExpansionHandler";
    out << "epx::EuroplexusReal eto[6];\n"
        << "epx::EuroplexusReal deto[6];\n"
        << "epx::EuroplexusReal sig[6];\n"
        << "epx::EuroplexusReal P0[36];\n"
        << "epx::EuroplexusReal P1[36];\n"
        << "const bool b = std::abs(*DDSDDE)>0.5;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "epx::computeLogarithmicStrainAndDerivative(P0,P1,eto,deto,F0,F1,*"
           "HYPOTHESIS);\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if (v.first) {
        out << "const epx::EuroplexusReal ezz = "
            << "STATEV[" << v.second.getValueForDimension(2) << "];\n"
            << "epx::EuroplexusReal F_0[5] = "
               "{F0[0],F0[1],std::exp(ezz),F0[3],F0[4]};\n"
            << "epx::computeDualStressOfLogarithmicStrainFromCauchyStress(sig,"
               "STRESS,P0,F_0,*HYPOTHESIS);\n";
      } else {
        // no axial strain
        out << "std::cerr << \"no state variable standing for the axial strain "
               "(variable with the "
            << "glossary name 'AxialStrain')\" << std::endl;\n";
        out << "*STATUS=-2;\n";
        out << "return;\n";
      }
    } else {
      out << "*STATUS=-2;\n"
          << "return;\n";
    }
    out << "} else {\n"
        << "epx::computeDualStressOfLogarithmicStrainFromCauchyStress(sig,"
           "STRESS,P0,F0,*HYPOTHESIS);\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "const epx::EPXData d = "
        << "                       "
           "{STATUS,sig,STATEV,DDSDDE,PNEWDT,BROKEN,MSG,\n"
        << "                        *NSTATV,*DTIME,eto,deto,R,PROPS,*NPROPS,\n"
        << "                        TEMP,DTEMP,PREDEF,DPRED,*NPREDEF,\n"
        << "                        " << this->getFunctionName(name)
        << "_getOutOfBoundsPolicy(),\n"
        << "                        " << sfeh << "};\n"
        << "epx::EuroplexusInterface<" << mb.getClassName()
        << ">::exe(d,*HYPOTHESIS);\n"
        << "if(*STATUS==0){\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(mb);
      if (v.first) {
        out << "const epx::EuroplexusReal ezz = "
            << "STATEV[" << v.second.getValueForDimension(2) << "];\n"
            << "epx::EuroplexusReal F_1[5] = "
               "{F1[1],F1[1],std::exp(ezz),F1[3],F1[4]};\n"
            << "epx::computeCauchyStressFromDualStressOfLogarithmicStrain("
               "STRESS,sig,P1,F_1,*HYPOTHESIS);\n";
      } else {
        // no axial strain
        out << "std::cerr << \"no state variable standing for the axial strain "
               "(variable with the "
            << "glossary name 'AxialStrain')\" << std::endl;\n";
        out << "*STATUS=-2;\n";
        out << "return;\n";
      }
    } else {
      out << "*STATUS=-2;\n"
          << "return;\n";
    }
    out << "} else {\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "epx::computeCauchyStressFromDualStressOfLogarithmicStrain(STRESS,"
           "sig,P1,F1,*HYPOTHESIS);\n"
        << "}\n"
        << "if(b){\n"
        << "epx::computeElasticModuli(DDSDDE,*HYPOTHESIS);\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "}\n";
  }

  void EuroplexusInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Europlexus/Europlexus.hxx\"\n\n";
  }  // end of EuroplexusInterface::writeInterfaceSpecificIncludes

  void EuroplexusInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov, of;
    os << "void set" << iprefix
       << "BehaviourDataDrivingVariables(const Type* const " << iprefix
       << "stran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataDrivingVariableSetter(os, v.first, ov);
      ov += this->getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const Type* const " << iprefix
       << "stress_)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += this->getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of EuroplexusInterface::writeBehaviourDataMainVariablesSetters

  void EuroplexusInterface::writeBehaviourDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (this->getTypeFlag(v.type) != SupportedTypes::Tensor) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeBehaviourDataDrivingVariableSetter: "
            "unsupported driving variable type"));
      }
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix << "stran+" << o
           << ",this->" << v.name << "0.begin());\n";
      } else {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix
           << "stran,this->" << v.name << "0.begin());\n";
      }
    } else {
      if (this->getTypeFlag(v.type) != SupportedTypes::Stensor) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeBehaviourDataDrivingVariableSetter: "
            "unsupported driving variable type"));
      }
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix << "stran+"
           << o << ",this->" << v.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix
           << "stran,this->" << v.name << ".begin());\n";
      }
    }
  }

  void EuroplexusInterface::writeIntegrationDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!v.increment_known) {
      if (this->getTypeFlag(v.type) != SupportedTypes::Tensor) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeIntegrationDataDrivingVariableSetter: "
            "unsupported driving variable type"));
      }
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix << "dstran+"
           << o << ",this->" << v.name << "1.begin());\n";
      } else {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix
           << "dstran,this->" << v.name << "1.begin());\n";
      }
    } else {
      if (this->getTypeFlag(v.type) != SupportedTypes::Stensor) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeIntegrationDataDrivingVariableSetter: "
            "unsupported driving variable type"));
      }
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix << "dstran+"
           << o << ",this->d" << v.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix
           << "dstran,this->d" << v.name << ".begin());\n";
      }
    }
  }

  void EuroplexusInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (this->getTypeFlag(f.type) == SupportedTypes::Stensor) {
      os << "tfel::fsalgo::copy<StensorSize>::exe(";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o;
      } else {
        os << iprefix << "stress_";
      }
      os << ",this->" << f.name << ".begin());\n";
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type"));
    }
  }  // end of EuroplexusInterface::writeBehaviourDataThermodynamicForceSetter

  void EuroplexusInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = this->getTypeFlag(f.type);
    if (flag == SupportedTypes::Stensor) {
      out << "tfel::fsalgo::copy<StensorSize>::exe(this->" << f.name
          << ".begin()," << a;
      if (!o.isNull()) {
        out << "+" << o;
      }
      out << ");\n";
    } else {
      throw(
          std::runtime_error("EuroplexusInterface::exportThermodynamicForce: "
                             "unsupported forces type"));
    }
  }  // end of EuroplexusInterface::exportThermodynamicForce

  void EuroplexusInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(h);\n";
  }  // end of
     // EuroplexusInterface::writeMTestFileGeneratorSetModellingHypothesis

  EuroplexusInterface::~EuroplexusInterface() {}

  void EuroplexusInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = EuroplexusInterface::getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags, "$(shell " + tfel_config + " --includes)");
    insert_if(d[lib].sources, "epx" + name + ".cxx");
    d.headers.push_back("MFront/Europlexus/europlexus" + name + ".hxx");
    insert_if(d[lib].ldflags,
              "-l" + tfel::getLibraryInstallName("EuroplexusInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].ldflags,
                "-l" + tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
    // insert_if(d[lib].epts,name);
    insert_if(d[lib].epts, this->getFunctionName(name));
  }  // end of EuroplexusInterface::getTargetsDescription

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
            SupportedTypes::TypeSize>
  EuroplexusInterface::buildMaterialPropertiesList(
      const BehaviourDescription& mb, const Hypothesis h) const {
    using namespace std;
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      set<Hypothesis> uh;
      for (const auto& elem : ah) {
        if (!mb.hasSpecialisedMechanicalData(elem)) {
          uh.insert(elem);
        }
      }
      if (uh.empty()) {
        throw(
            std::runtime_error("UMATInterface::endTreatment: "
                               "internal error, the mechanical behaviour "
                               "says that not all handled mechanical data "
                               "are specialised, but we found none."));
      }
      // material properties for all the selected hypothesis
      auto mpositions = vector<
          pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize>>{};
      for (const auto& elem : uh) {
        mpositions.push_back(this->buildMaterialPropertiesList(mb, elem));
      }
      auto ph = uh.begin();
      auto pum = mpositions.begin();
      const auto& mfirst = *pum;
      ++ph;
      ++pum;
      for (; ph != uh.end(); ++ph, ++pum) {
        const auto& d =
            mb.getBehaviourData(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
        const auto& mps = d.getMaterialProperties();
        for (const auto& mp : mps) {
          const auto& mp1 = findUMATMaterialProperty(
              mfirst.first, mb.getExternalName(h, mp.name));
          const auto& mp2 = findUMATMaterialProperty(
              pum->first, mb.getExternalName(h, mp.name));
          auto o1 = mp1.offset;
          o1 += pum->second;
          auto o2 = mp2.offset;
          o2 += mfirst.second;
          if (o1 != o2) {
            throw(runtime_error(
                "UMATInterface::buildMaterialPropertiesList : "
                "incompatible offset for material property '" +
                mp.name + "' (aka '" + mp1.name +
                "'). This is one pitfall of the "
                "exp interface. To by-pass this limitation, you may "
                "want to explicitely specialise some modelling hypotheses"));
          }
        }
      }
      return mfirst;
    }
    auto res = pair<vector<UMATMaterialProperty>, SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    if ((h != ModellingHypothesis::GENERALISEDPLANESTRAIN) &&
        (h != ModellingHypothesis::AXISYMMETRICAL) &&
        (h != ModellingHypothesis::PLANESTRAIN) &&
        (h != ModellingHypothesis::PLANESTRESS) &&
        (h != ModellingHypothesis::TRIDIMENSIONAL)) {
      string msg(
          "EuroplexusInterface::buildMaterialPropertiesList : "
          "unsupported modelling hypothesis");
      if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        msg += " (default)";
      } else {
        msg += " '" + ModellingHypothesis::toString(h) + "'";
      }
      throw(runtime_error(msg));
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
        this->appendToMaterialPropertiesList(mprops, "stress", "ShearModulus12",
                                             "g12", false);
        if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus23", "g23", false);
          this->appendToMaterialPropertiesList(mprops, "stress",
                                               "ShearModulus13", "g13", false);
        } else if ((h != ModellingHypothesis::GENERALISEDPLANESTRAIN) &&
                   (h != ModellingHypothesis::AXISYMMETRICAL) &&
                   (h != ModellingHypothesis::PLANESTRAIN) &&
                   (h != ModellingHypothesis::PLANESTRESS)) {
          throw(runtime_error(
              "EuroplexusInterface::buildMaterialPropertiesList : "
              "unsupported modelling hypothesis"));
        }
      } else {
        throw(
            runtime_error("EuroplexusInterface::buildMaterialPropertiesList : "
                          "unsupported behaviour symmetry type"));
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
            runtime_error("EuroplexusInterface::buildMaterialPropertiesList : "
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
  }  // end of EuroplexusInterface::buildMaterialPropertiesList

  void EuroplexusInterface::writeEuroplexusBehaviourTraits(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    using namespace std;
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
    out << "struct EuroplexusTraits<tfel::material::" << mb.getClassName()
        << "<";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "H";
    } else {
      out << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h);
    }
    out << ",Type,";
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << "> >\n{\n";
    out << "//! behaviour type\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " EuroplexusBehaviourType btype = "
             "epx::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " EuroplexusBehaviourType btype = "
             "epx::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::writeEuroplexusBehaviourTraits : "
          "unsupported behaviour type"));
    }
    out << "//! space dimension\n";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "static " << constexpr_c
          << " unsigned short N           = "
             "tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c
          << " unsigned short N           = "
             "tfel::material::ModellingHypothesisToSpaceDimension<"
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
    }
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
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c
        << " unsigned short DrivingVariableSize = " << mvs.first << ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n";
    if (mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,
                        false)) {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = true;\n";
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
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " EuroplexusSymmetryType type = epx::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " EuroplexusSymmetryType type = epx::ORTHOTROPIC;\n";
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
          "The europlexus interface only support isotropic or orthotropic "
          "behaviour at this time."));
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += this->getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " EuroplexusSymmetryType etype = epx::ISOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 1u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " EuroplexusSymmetryType etype = epx::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= EuroplexusOrthotropicElasticPropertiesOffset<N>::value;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 3u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
          "The europlexus interface only support isotropic or "
          "orthotropic behaviour at this time."));
    }
    out << "}; // end of class EuroplexusTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  EuroplexusInterface::gatherModellingHypothesesAndTests(
      const BehaviourDescription& mb) const {
    auto res = std::map<Hypothesis, std::string>{};
    if ((mb.getSymmetryType() == mfront::ORTHOTROPIC) &&
        ((mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) ||
         (mb.getAttribute(
             BehaviourDescription::requiresThermalExpansionCoefficientTensor,
             false)))) {
      auto h = this->getModellingHypothesesToBeTreated(mb);
      for (const auto& mh : h) {
        res.insert({mh, this->getModellingHypothesisTest(mh)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  }  // end of EuroplexusInterface::gatherModellingHypothesesAndTests

  std::string EuroplexusInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
      return "*HYPOTHESIS==3";
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      return "*HYPOTHESIS==2";
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      return "*HYPOTHESIS==1";
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "*HYPOTHESIS==0";
    }
    throw(
        std::runtime_error("EuroplexusInterface::getModellingHypothesisTest : "
                           "unsupported modelling hypothesis"));
  }  // end of EuroplexusInterface::gatherModellingHypothesesAndTests

  void EuroplexusInterface::writeUMATxxBehaviourTypeSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->fss == UNDEFINEDSTRATEGY) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeUMATxxBehaviourTypeSymbols: "
            "behaviours written in the small strain framework "
            "must be embedded in a strain strategy"));
      }
      out << "2u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "2u;\n\n";
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::writeUMATxxBehaviourTypeSymbols: "
          "unsupported behaviour type"));
    }
  }  // end of EuroplexusInterface::writeUMATxxBehaviourTypeSymbols

  void EuroplexusInterface::writeUMATxxBehaviourKinematicSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->fss == UNDEFINEDSTRATEGY) {
        throw(std::runtime_error(
            "EuroplexusInterface::writeUMATxxBehaviourKinematicSymbols: "
            "behaviours written in the small strain framework "
            "must be embedded in a strain strategy"));
      }
      out << "3u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "3u;\n\n";
    } else {
      throw(std::runtime_error(
          "EuroplexusInterface::writeUMATxxBehaviourKinematicSymbols: "
          "unsupported behaviour type"));
    }
  }  // end of EuroplexusInterface::writeUMATxxBehaviourKinematicSymbols

  void EuroplexusInterface::writeUMATxxAdditionalSymbols(
      std::ostream&,
      const std::string&,
      const Hypothesis,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of EuroplexusInterface::writeUMATxxAdditionalSymbols

}  // end of namespace mfront
