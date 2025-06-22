/*!
 * \file   mfront/src/EuroplexusInterface.cxx
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
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/EuroplexusSymbolsGenerator.hxx"
#include "MFront/EuroplexusInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void checkFiniteStrainStrategy(const std::string& fs) {
    tfel::raise_if((fs != "FiniteRotationSmallStrain") &&
                       (fs != "MieheApelLambrechtLogarithmicStrain"),
                   "checkFiniteStrainStrategy: "
                   "unsupported strategy '" +
                       fs +
                       "'\n"
                       "The only supported strategies are "
                       "'FiniteRotationSmallStrain' and "
                       "'MieheApelLambrechtLogarithmicStrain'.");
  }  // end of checkFiniteStrainStrategy

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd, const std::string& fs) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "checkFiniteStrainStrategyDefinitionConsistency: " + msg);
    };
    checkFiniteStrainStrategy(fs);
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::LINEARISED) {
        throw_if(fs != "Native",
                 "incompatible finite strain strategy "
                 "'" +
                     fs + "' (only `Native` accepted)");
      } else if (ms == BehaviourDescription::GREENLAGRANGE) {
        throw_if(fs != "FiniteRotationSmallStrain",
                 "incompatible finite strain strategy "
                 "'" +
                     fs + "' (only `FiniteRotationSmallStrain` accepted)");
      } else if (ms == BehaviourDescription::HENCKY) {
        throw_if(fs != "MieheApelLambrechtLogarithmicStrain",
                 "incompatible finite strain strategy '" + fs +
                     "' "
                     "(only `MieheApelLambrechtLogarithmicStrain` accepted)");
      } else {
        throw_if(true, "unsupported finite strain strategy");
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c,
                     "checkFiniteStrainStrategyDefinitionConsistency: " + msg);
    };
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      throw_if(bd.hasAttribute(EuroplexusInterface::finiteStrainStrategy),
               "finite strain strategy is only supported for strain based "
               "behaviours");
    } else {
      if (bd.hasAttribute(EuroplexusInterface::finiteStrainStrategy)) {
        const auto fs = bd.getAttribute<std::string>(
            EuroplexusInterface::finiteStrainStrategy);
        checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  bool EuroplexusInterface::hasFiniteStrainStrategy(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      return bd.getStrainMeasure() != BehaviourDescription::LINEARISED;
    }
    return bd.hasAttribute(EuroplexusInterface::finiteStrainStrategy);
  }  // end of EuroplexusInterface::hasFiniteStrainStrategy

  static std::string getFiniteStrainStrategy(const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    auto throw_if = [](const bool c, const std::string& msg) {
      tfel::raise_if(c, "getFiniteStrainStrategy: " + msg);
    };
    if (bd.isStrainMeasureDefined()) {
      const auto ms = bd.getStrainMeasure();
      if (ms == BehaviourDescription::GREENLAGRANGE) {
        return "FiniteRotationSmallStrain";
      } else if (ms == BehaviourDescription::HENCKY) {
        return "MieheApelLambrechtLogarithmicStrain";
      } else {
        throw_if(true, "unsupported strain measure");
      }
    }
    throw_if(!bd.hasAttribute(EuroplexusInterface::finiteStrainStrategy),
             "no finite strain strategy defined");
    return bd.getAttribute<std::string>(
        EuroplexusInterface::finiteStrainStrategy);
  }  // end of getFiniteStrainStrategy

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

  const char* const EuroplexusInterface::finiteStrainStrategy =
      "epx::finiteStrainStrategy";

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

  std::string EuroplexusInterface::getFunctionNameBasis(
      const std::string& name) const {
    return name;
  }  // end of EuroplexusInterface::getLibraryName

  std::pair<bool, EuroplexusInterface::tokens_iterator>
  EuroplexusInterface::treatKeyword(BehaviourDescription& bd,
                                    const std::string& key,
                                    const std::vector<std::string>& i,
                                    tokens_iterator current,
                                    const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "EuroplexusInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        throw_if((key != "@EuroplexusFiniteStrainStrategy") &&
                     (key != "@EPXFiniteStrainStrategy") &&
                     (key != "@EuroplexusGenerateMTestFileOnFailure") &&
                     (key != "@EPXGenerateMTestFileOnFailure") &&
                     (key != "@GenerateMTestFileOnFailure"),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if ((key == "@EuroplexusFiniteStrainStrategy") ||
        (key == "@EPXFiniteStrainStrategy")) {
      throw_if(bd.hasAttribute(EuroplexusInterface::finiteStrainStrategy),
               "a finite strain strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      const auto fs = current->value;
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      if (fs == "Native") {
        throw_if(bd.getSymmetryType() == mfront::ORTHOTROPIC,
                 "orthotropic behaviours are not supported with the "
                 "`Native` finite strain strategy");
      }
      bd.setAttribute(EuroplexusInterface::finiteStrainStrategy, fs, false);
      return {true, current};
    } else if ((key == "@EuroplexusGenerateMTestFileOnFailure") ||
               (key == "@EPXGenerateMTestFileOnFailure") ||
               (key == "@GenerateMTestFileOnFailure")) {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(key, current, end));
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
    tfel::raise_if(mh.empty(),
                   "EuroplexusInterfaceModellingHypothesesToBeTreated: "
                   "no hypotheses selected, so it does not "
                   "make sense to use the Europlexus interface");
    return mh;
  }  // end of EuroplexusInterface::getModellingHypothesesToBeTreated

  void EuroplexusInterface::endTreatment(const BehaviourDescription& mb,
                                         const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "EuroplexusInterface::endTreatment: " + m);
    };
    throw_if((mb.getBehaviourType() !=
              BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) &&
                 ((mb.getBehaviourType() ==
                   BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
                  (!EuroplexusInterface::hasFiniteStrainStrategy(mb))),
             "the europlexus interface only supports "
             "finite strain behaviours");
    checkFiniteStrainStrategyDefinitionConsistency(mb);
    // get the modelling hypotheses to be treated
    const auto& mhs = this->getModellingHypothesesToBeTreated(mb);
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

    const auto header = this->getHeaderGuard(mb);
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

    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      this->writeEuroplexusBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
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
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
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
    if (mb.getBehaviourType() !=
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "#include\"MFront/Europlexus/EuroplexusFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"MFront/Europlexus/"
           "EuroplexusStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/Europlexus/EuroplexusInterface.hxx\"\n\n"
        << "#include\"MFront/Europlexus/europlexus" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    EuroplexusSymbolsGenerator sg;
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

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
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
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      const auto fs = getFiniteStrainStrategy(mb);
      if (fs == "FiniteRotationSmallStrain") {
        this->writeFiniteRotationSmallStrainCall(out, mb, name);
      } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
        this->writeLogarithmicStrainCall(out, mb, name);
      } else {
        throw_if(true, "unsupported finite strain strategy");
      }
    } else {
      // this->generateMTestFile1(out);
      out << "const epx::EPXData d = "
             "{STATUS,STRESS,STATEV,DDSDDE,PNEWDT,BROKEN,MSG,\n"
          << "                        *NSTATV,*DTIME,F0,F1,R,PROPS,*NPROPS,\n"
          << "                        TEMP,DTEMP,PREDEF,DPRED,*NPREDEF,\n"
          << "                        " << this->getFunctionNameBasis(name)
          << "_getOutOfBoundsPolicy(),\n"
          << "                        nullptr};\n"
          << "epx::EuroplexusInterface<" << mb.getClassName()
          << ">::exe(d,*HYPOTHESIS);\n";
    }

    // this->generateMTestFile2(out,mb.getBehaviourType(),
    // 			     name,"",mb);
    out << "}\n\n"
        << "} // end of extern \"C\"\n";
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
      out << "LAW '" << this->getFunctionNameBasis(name) << "'\n"
          << "!! material properties\n";
      for (const auto& mp : buildMaterialPropertiesList(mb, h).first) {
        throw_if(SupportedTypes::getTypeFlag(mp.type) != SupportedTypes::SCALAR,
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
          if (f == SupportedTypes::SCALAR) {
            out << " ???";
          } else if (f == SupportedTypes::STENSOR) {
            if (h == ModellingHypothesis::TRIDIMENSIONAL) {
              out << " ??? ??? ??? ??? ??? ???";
            } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                       (h == ModellingHypothesis::PLANESTRAIN) ||
                       (h == ModellingHypothesis::PLANESTRESS)) {
              out << " ??? ??? ??? ???";
            }
          } else if (f == SupportedTypes::TENSOR) {
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
          display(SupportedTypes::getTypeFlag(iv.type));
          out << '\n';
        } else {
          for (unsigned short i = 0; i != iv.arraySize; ++i) {
            out << "!! IVAR '" << d.getExternalName(iv.name) << '[' << i
                << "]' ";
            display(SupportedTypes::getTypeFlag(iv.type));
            out << '\n';
          }
        }
      }
      out << "!! external state variables\n";
      for (const auto& e : d.getExternalStateVariables()) {
        throw_if(SupportedTypes::getTypeFlag(e.type) != SupportedTypes::SCALAR,
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
    tfel::raise_if(mb.getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "EuroplexusInterface::writeFiniteRotationSmallStrainCall: "
                   "internal error, the 'finite rotation small strain' "
                   "strategy shall be used only with small "
                   "strain behaviour");
    const auto sfeh =
        "epx::EuroplexusStandardSmallStrainStressFreeExpansionHandler";
    out << "epx::EuroplexusReal eto[6];\n"
        << "epx::EuroplexusReal deto[6];\n"
        << "epx::EuroplexusReal sig[6];\n"
        << "epx::computeGreenLagrangeStrain(eto,deto,F0,F1,*HYPOTHESIS);\n"
        << "const bool b = std::abs(*DDSDDE)>0.5;\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(
          mb, ModellingHypothesis::PLANESTRESS);
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
        << "                        " << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy(),\n"
        << "                        " << sfeh << "};\n"
        << "epx::EuroplexusInterface<" << mb.getClassName()
        << ">::exe(d,*HYPOTHESIS);\n"
        << "if(*STATUS==0){\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(
          mb, ModellingHypothesis::PLANESTRESS);
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
    tfel::raise_if(mb.getBehaviourType() !=
                       BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
                   "EuroplexusInterface::writeFiniteRotationSmallStrainCall: "
                   "internal error, the 'finite rotation small strain' "
                   "strategy shall be used only with small "
                   "strain behaviour");
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
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(
          mb, ModellingHypothesis::PLANESTRESS);
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
        << "                        " << this->getFunctionNameBasis(name)
        << "_getOutOfBoundsPolicy(),\n"
        << "                        " << sfeh << "};\n"
        << "epx::EuroplexusInterface<" << mb.getClassName()
        << ">::exe(d,*HYPOTHESIS);\n"
        << "if(*STATUS==0){\n"
        << "if(*HYPOTHESIS==2){\n";
    if (mb.isModellingHypothesisSupported(ModellingHypothesis::PLANESTRESS)) {
      const auto v = this->checkIfAxialStrainIsDefinedAndGetItsOffset(
          mb, ModellingHypothesis::PLANESTRESS);
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
    os << "void set" << iprefix << "BehaviourDataGradients(const Type* const "
       << iprefix << "stran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataGradientSetter(os, v.first, ov);
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const Type* const " << iprefix
       << "stress_)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of EuroplexusInterface::writeBehaviourDataMainVariablesSetters

  void EuroplexusInterface::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!Gradient::isIncrementKnown(v)) {
      tfel::raise_if(
          SupportedTypes::getTypeFlag(v.type) != SupportedTypes::TENSOR,
          "EuroplexusInterface::writeBehaviourDataGradientSetter: "
          "unsupported driving variable type");
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix << "stran+" << o
           << ",this->" << v.name << "0.begin());\n";
      } else {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix
           << "stran,this->" << v.name << "0.begin());\n";
      }
    } else {
      tfel::raise_if(
          SupportedTypes::getTypeFlag(v.type) != SupportedTypes::STENSOR,
          "EuroplexusInterface::writeBehaviourDataGradientSetter: "
          "unsupported driving variable type");
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix << "stran+"
           << o << ",this->" << v.name << ".begin());\n";
      } else {
        os << "tfel::fsalgo::copy<StensorSize>::exe(" << iprefix
           << "stran,this->" << v.name << ".begin());\n";
      }
    }
  }

  void EuroplexusInterface::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!Gradient::isIncrementKnown(v)) {
      tfel::raise_if(
          SupportedTypes::getTypeFlag(v.type) != SupportedTypes::TENSOR,
          "EuroplexusInterface::writeIntegrationDataGradientSetter: "
          "unsupported driving variable type");
      if (!o.isNull()) {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix << "dstran+"
           << o << ",this->" << v.name << "1.begin());\n";
      } else {
        os << "tfel::fsalgo::copy<TensorSize>::exe(" << iprefix
           << "dstran,this->" << v.name << "1.begin());\n";
      }
    } else {
      tfel::raise_if(
          SupportedTypes::getTypeFlag(v.type) != SupportedTypes::STENSOR,
          "EuroplexusInterface::writeIntegrationDataGradientSetter: "
          "unsupported driving variable type");
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
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      os << "tfel::fsalgo::copy<StensorSize>::exe(";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o;
      } else {
        os << iprefix << "stress_";
      }
      os << ",this->" << f.name << ".begin());\n";
    } else {
      tfel::raise(
          "EuroplexusInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type");
    }
  }  // end of EuroplexusInterface::writeBehaviourDataThermodynamicForceSetter

  void EuroplexusInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if (flag == SupportedTypes::STENSOR) {
      out << "tfel::fsalgo::copy<StensorSize>::exe(this->" << f.name
          << ".begin()," << a;
      if (!o.isNull()) {
        out << "+" << o;
      }
      out << ");\n";
    } else {
      tfel::raise(
          "EuroplexusInterface::exportThermodynamicForce: "
          "unsupported forces type");
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
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "epx" + name + ".cxx");
    d.headers.push_back("MFront/Europlexus/europlexus" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries,
              tfel::getLibraryInstallName("EuroplexusInterface"));
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
    // insert_if(l.epts,name);
    insert_if(l.epts, this->getFunctionNameBasis(name));
  }  // end of EuroplexusInterface::getTargetsDescription

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
    out << ">\n"
        << "struct EuroplexusTraits<tfel::material::" << mb.getClassName()
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
    out << "> >\n{\n"
        << "//! behaviour type\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " EuroplexusBehaviourType btype = "
             "epx::STANDARDSTRAINBASEDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " EuroplexusBehaviourType btype = "
             "epx::STANDARDFINITESTRAINBEHAVIOUR;"
             "\n";
    } else {
      tfel::raise(
          "EuroplexusInterface::writeEuroplexusBehaviourTraits : "
          "unsupported behaviour type");
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
    out << "// tiny vector size\n"
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "// symmetric tensor size\n"
        << "static " << constexpr_c << " unsigned short StensorSize = "
        << "tfel::math::StensorDimeToSize<N>::value;\n"
        << "// tensor size\n"
        << "static " << constexpr_c << " unsigned short TensorSize  = "
        << "tfel::math::TensorDimeToSize<N>::value;\n"
        << "// size of the driving variable array\n"
        << "static " << constexpr_c
        << " unsigned short GradientSize = " << mvs.first << ";\n"
        << "// size of the thermodynamic force variable array (STRESS)\n"
        << "static " << constexpr_c
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
      tfel::raise(
          "EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
          "The europlexus interface only support isotropic or orthotropic "
          "behaviour at this time.");
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
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
      tfel::raise(
          "EuroplexusInterface::endTreatment: unsupported behaviour type.\n"
          "The europlexus interface only support isotropic or "
          "orthotropic behaviour at this time.");
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
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        res.insert({h, this->getModellingHypothesisTest(h)});
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
    tfel::raise(
        "EuroplexusInterface::getModellingHypothesisTest : "
        "unsupported modelling hypothesis");
  }  // end of EuroplexusInterface::gatherModellingHypothesesAndTests

}  // end of namespace mfront
