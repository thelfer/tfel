/*!
 * \file   mfront/src/CalculiXInterface.cxx
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
#include "MFront/MFrontLock.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/CalculiXSymbolsGenerator.hxx"
#include "MFront/CalculiXInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void checkFiniteStrainStrategy(const std::string& fs) {
    tfel::raise_if((fs != "FiniteRotationSmallStrain") &&
                       (fs != "MieheApelLambrechtLogarithmicStrain") &&
                       (fs != "MieheApelLambrechtLogarithmicStrainII"),
                   "checkFiniteStrainStrategy: "
                   "unsupported strategy '" +
                       fs +
                       "'\n"
                       "The only supported strategies are "
                       "'FiniteRotationSmallStrain', "
                       "'MieheApelLambrechtLogarithmicStrain' and "
                       "'MieheApelLambrechtLogarithmicStrainII'.");
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
      throw_if(bd.hasAttribute(CalculiXInterface::finiteStrainStrategy),
               "finite strain strategy is only supported for strain based "
               "behaviours");
    } else {
      if (bd.hasAttribute(CalculiXInterface::finiteStrainStrategy)) {
        const auto fs = bd.getAttribute<std::string>(
            CalculiXInterface::finiteStrainStrategy);
        checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      }
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  bool CalculiXInterface::hasFiniteStrainStrategy(
      const BehaviourDescription& bd) {
    checkFiniteStrainStrategyDefinitionConsistency(bd);
    if (bd.isStrainMeasureDefined()) {
      return bd.getStrainMeasure() != BehaviourDescription::LINEARISED;
    }
    return bd.hasAttribute(CalculiXInterface::finiteStrainStrategy);
  }  // end of CalculiXInterface::hasFiniteStrainStrategy

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
    throw_if(!bd.hasAttribute(CalculiXInterface::finiteStrainStrategy),
             "no finite strain strategy defined");
    return bd.getAttribute<std::string>(
        CalculiXInterface::finiteStrainStrategy);
  }  // end of getFiniteStrainStrategy

  static void writeArguments(std::ostream& out,
                             const BehaviourDescription& mb,
                             const bool base) {
    if (!base) {
      const auto requires_strain = [&mb] {
        if (mb.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (CalculiXInterface::hasFiniteStrainStrategy(mb)) {
            return getFiniteStrainStrategy(mb) == "FiniteRotationSmallStrain";
          }
        }
        return true;
      }();
      out << "(const char * const amat,\n"
          << " const calculix::CalculiXInt* const iel,\n"
          << " const calculix::CalculiXInt* const iint,\n"
          << " const calculix::CalculiXInt* const NPROPS,\n"
          << " const calculix::CalculiXReal* const MPROPS,\n";
      if (requires_strain) {
        out << " const calculix::CalculiXReal* const STRAN1,\n"
            << " const calculix::CalculiXReal* const STRAN0,\n";
      } else {
        out << " const calculix::CalculiXReal* const,\n"
            << " const calculix::CalculiXReal* const,\n";
      }
      out << " const calculix::CalculiXReal* const beta,\n"
          << " const calculix::CalculiXReal* const XOKL,\n"
          << " const calculix::CalculiXReal* const voj,\n"
          << " const calculix::CalculiXReal* const XKL,\n"
          << " const calculix::CalculiXReal* const vj,\n"
          << " const calculix::CalculiXInt* const ithermal,\n"
          << " const calculix::CalculiXReal* const TEMP1,\n"
          << " const calculix::CalculiXReal* const DTIME,\n"
          << " const calculix::CalculiXReal* const time,\n"
          << " const calculix::CalculiXReal* const ttime,\n"
          << " const calculix::CalculiXInt* const icmd,\n"
          << " const calculix::CalculiXInt* const ielas,\n"
          << " const calculix::CalculiXInt* const mi,\n"
          << " const calculix::CalculiXInt* const NSTATV,\n"
          << " const calculix::CalculiXReal* const STATEV0,\n"
          << "       calculix::CalculiXReal* const STATEV1,\n"
          << "       calculix::CalculiXReal* const STRESS,\n"
          << "       calculix::CalculiXReal* const DDSDDE,\n"
          << " const calculix::CalculiXInt* const iorien,\n"
          << " const calculix::CalculiXReal* const pgauss,\n"
          << " const calculix::CalculiXReal* const orab,\n"
          << "       calculix::CalculiXReal* const PNEWDT,\n"
          << " const calculix::CalculiXInt* const ipkon,\n"
          << " const int size)";
    } else {
      const auto requires_strain =
          (mb.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR);
      const auto requires_F =
          (mb.getBehaviourType() ==
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR);
      out << "(const char * const,\n"
          << " const calculix::CalculiXInt* const iel,\n"
          << " const calculix::CalculiXInt* const iint,\n"
          << " const calculix::CalculiXInt*  const,\n"  //  NPROPS
          << " const calculix::CalculiXReal* const MPROPS,\n";
      if (requires_strain) {
        out << " const calculix::CalculiXReal *const DSTRAN,\n"
            << " const calculix::CalculiXReal *const STRAN0,\n";
      } else {
        out << " const calculix::CalculiXReal *const,\n"
            << " const calculix::CalculiXReal *const,\n";
      }
      out << " const calculix::CalculiXReal* const,\n";
      if (requires_F) {
        out << " const calculix::CalculiXReal* const XOKL,\n"
            << " const calculix::CalculiXReal* const ,\n"
            << " const calculix::CalculiXReal* const XKL,\n"
            << " const calculix::CalculiXReal* const,\n";
      } else {
        out << " const calculix::CalculiXReal* const,\n"
            << " const calculix::CalculiXReal* const ,\n"
            << " const calculix::CalculiXReal* const,\n"
            << " const calculix::CalculiXReal* const,\n";
      }
      out << " const calculix::CalculiXInt* const,\n"  //  ithermal
          << " const calculix::CalculiXReal* const TEMP1,\n"
          << " const calculix::CalculiXReal* const DTIME,\n"
          << " const calculix::CalculiXReal* const,\n"
          << " const calculix::CalculiXReal* const,\n"
          << " const calculix::CalculiXInt* const,\n"  // icmd
          << " const calculix::CalculiXInt* const,\n"  // ielas
          << " const calculix::CalculiXInt* const mi,\n"
          << " const calculix::CalculiXInt* const NSTATV,\n"
          << " const calculix::CalculiXReal* const STATEV0,\n"
          << "       calculix::CalculiXReal* const STATEV1,\n"
          << "       calculix::CalculiXReal* const STRESS,\n"
          << "       calculix::CalculiXReal* const DDSDDE,\n"
          << " const calculix::CalculiXInt* const,\n"
          << " const calculix::CalculiXReal* const,\n"
          << " const calculix::CalculiXReal* const,\n"
          << "       calculix::CalculiXReal* const PNEWDT,\n"
          << " const calculix::CalculiXInt* const,\n"
          << " const int)";
    }
  }  // end of writeArguments

  static void writeArguments(std::ostream& out) {
    out << "(const char * const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << "       calculix::CalculiXReal* const,\n"
        << "       calculix::CalculiXReal* const,\n"
        << "       calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXReal* const,\n"
        << "       calculix::CalculiXReal* const,\n"
        << " const calculix::CalculiXInt* const,\n"
        << " const int)";
  }  // end of writeArguments

  const char* const CalculiXInterface::finiteStrainStrategy =
      "calculix::finiteStrainStrategy";

  std::string CalculiXInterface::getName() { return "calculix"; }

  std::string CalculiXInterface::getInterfaceName() const {
    return "CalculiX";
  }  // end of CalculiXInterface::getInterfaceName

  std::pair<bool, CalculiXInterface::tokens_iterator>
  CalculiXInterface::treatKeyword(BehaviourDescription& bd,
                                  const std::string& k,
                                  const std::vector<std::string>& i,
                                  tokens_iterator current,
                                  const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CalculiXInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        const auto keys =
            std::vector<std::string>{"@CalculiXFiniteStrainStrategy",
                                     "@CalculiXGenerateMTestFileOnFailure",
                                     "@CalculiXStrainPerturbationValue"};
        throw_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                 "unsupported key '" + k + "'");
      } else {
        return {false, current};
      }
    }
    if (k == "@CalculiXFiniteStrainStrategy") {
      throw_if(bd.hasAttribute(CalculiXInterface::finiteStrainStrategy),
               "a finite strain strategy has already been defined");
      throw_if(current == end, "unexpected end of file");
      const auto fs = current->value;
      throw_if(++current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + '\'');
      ++(current);
      checkFiniteStrainStrategyDefinitionConsistency(bd, fs);
      bd.setAttribute(CalculiXInterface::finiteStrainStrategy, fs, false);
      return {true, current};
    } else if (k == "@CalculiXGenerateMTestFileOnFailure") {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(k, current, end));
      return {true, current};
    }
    return {false, current};
  }  // end of CalculiXInterface::treatKeyword

  void CalculiXInterface::endTreatment(const BehaviourDescription& mb,
                                       const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CalculiXInterface::endTreatment: " + m);
    };
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "the calculix interface only supports small and "
             "finite strain behaviours");
    checkFiniteStrainStrategyDefinitionConsistency(mb);
    // the only supported modelling hypothesis
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto& d = mb.getBehaviourData(h);
    throw_if(d.getExternalStateVariables().size() != 1u,
             "external state variables are not supported "
             "by CalculiX's native interface");
    // get the modelling hypotheses to be treated
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/CalculiX");
    systemCall::mkdir("calculix");

    std::ofstream out;

    // header
    auto fname = "calculix" + name + ".hxx";
    out.open("include/MFront/CalculiX/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the calculix interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderGuard(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    if ((mb.getBehaviourType() ==
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
        (CalculiXInterface::hasFiniteStrainStrategy(mb))) {
      const auto fs = getFiniteStrainStrategy(mb);
      if ((fs == "MieheApelLambrechtLogarithmicStrain") ||
          (fs == "MieheApelLambrechtLogarithmicStrainII")) {
        out << "#include\"TFEL/Material/LogarithmicStrainHandler.hxx\"\n\n";
      }
    }
    out << "#include\"MFront/CalculiX/CalculiX.hxx\"\n"
        << "#include\"MFront/CalculiX/CalculiXData.hxx\"\n\n"
        << "#ifdef __cplusplus\n"
        << "#include\"MFront/CalculiX/CalculiXTraits.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace calculix{\n\n";

    this->writeCalculiXBehaviourTraits(out, mb);

    out << "} // end of namespace calculix\n\n"
        << "#endif /* __cplusplus */\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
    writeArguments(out);
    out << ";\n\n";

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "calculix" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the calculix interface for the "
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
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/CalculiX/CalculiXRotationMatrix.hxx\"\n";
    }
    out << "#include\"MFront/CalculiX/"
           "CalculiXStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/CalculiX/CalculiXInterface.hxx\"\n\n"
        << "#include\"MFront/CalculiX/calculix" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    CalculiXSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, mb, fd, {h}, name);
    sg.generateSymbols(out, *this, mb, fd, name, h);

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (CalculiXInterface::hasFiniteStrainStrategy(mb)) {
        const auto fs = getFiniteStrainStrategy(mb);
        if (fs == "FiniteRotationSmallStrain") {
          this->writeFiniteRotationSmallStrainFunction(out, mb, name);
        } else if ((fs == "MieheApelLambrechtLogarithmicStrain") ||
                   (fs == "MieheApelLambrechtLogarithmicStrainII")) {
          this->writeMieheApelLambrechtLogarithmicStrainFunction(out, mb, name);
        } else {
          throw_if(true, "unsupported finite strain strategy !");
        }
      } else {
        this->writeSmallStrainFunction(out, mb, name);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      this->writeFiniteStrainFunction(out, mb, name);
    } else {
      throw_if(true,
               "the calculix interface only supports small "
               "and finite strain behaviours");
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb, fd, true);
  }  // end of CalculiXInterface::endTreatment

  void CalculiXInterface::writeFunctionBase(std::ostream& out,
                                            const BehaviourDescription& mb,
                                            const std::string& name,
                                            const std::string& sfeh) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CalculiXInterface::writeFunctionBase: " + m);
    };
    std::string dv0, dv1, sig, statev, nstatev;
    const auto btype = mb.getBehaviourType();
    out << "static void\n" << name << "_base";
    writeArguments(out, mb, true);
    out << "{\n";
    if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      dv0 = "STRAN0";
      dv1 = "DSTRAN";
    } else if (btype == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      dv0 = "XOKL";
      dv1 = "XKL";
    } else {
      throw_if(true,
               "the calculix interface only supports small "
               "and finite strain behaviours");
    }
    out << "using calculix::CalculiXData;\n"
        << "const auto ivs0 =  STATEV0+(*NSTATV)*((*iint-1)+(*mi)*(*iel-1));\n"
        << "const auto ivs1 =  STATEV1+(*NSTATV)*((*iint-1)+(*mi)*(*iel-1));\n"
        << "CalculiXData d = {STRESS,PNEWDT,DDSDDE,ivs1,*DTIME,ivs0,\n"
        << "                  " << dv0 << "," << dv1 << ",TEMP1,MPROPS,\n"
        << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
        << sfeh << "};\n"
        << "if(calculix::CalculiXInterface<tfel::material::"
        << mb.getClassName() << ">::exe(d)!=0){\n"
        << "*PNEWDT = 0.2;\n"
        << "return;\n"
        << "}\n"
        << "}\n\n";
  }  // end of CalculiXInterface::writeFunctionBase

  void CalculiXInterface::writeFiniteStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CalculiXInterface::writeFiniteStrainFunction: " + m);
    };
    const std::string sfeh = "nullptr";
    this->writeFunctionBase(out, mb, name, sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
    writeArguments(out, mb, false);
    out << "{\n"
        << "using namespace tfel::math;\n"
        << "using real = calculix::CalculiXReal;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << "st2tost2<3u,real> D = {0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0};\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "if(*iorien==0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation defined for an orthotropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n"
          << "const auto r  = "
             "calculix::getRotationMatrix(orab+7*(*iorien-1),pgauss);\n"
          << "const auto rb = transpose(r);\n";
    } else {
      throw_if(mb.getSymmetryType() != mfront::ISOTROPIC,
               "unsupported symmetry type");
      out << "if(*iorien!=0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation shall be defined for an istropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n";
    }
    out << "const auto F0 = tensor<3u,real>::buildFromFortranMatrix(XOKL);\n"
        << "const auto F1 = tensor<3u,real>::buildFromFortranMatrix(XKL);\n"
        << "stensor<3u,real> pk2;\n"
        << "pk2.importTab(STRESS);\n"
        << "auto s = "
           "convertSecondPiolaKirchhoffStressToCauchyStress(pk2,F1);\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "const auto rF0 = change_basis(F0,r);\n"
          << "const auto rF1 = change_basis(F1,r);\n"
          << "s.changeBasis(r);\n"
          << name << "_base"
          << "(amat,iel,iint,NPROPS,MPROPS,STRAN1,STRAN0,beta,rF0.begin(),"
          << " voj,rF1.begin(),vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
          << " ielas,mi,NSTATV,STATEV0,STATEV1,s.begin(),D.begin(),"
          << "iorien,pgauss,orab,PNEWDT,ipkon,size);\n";
    } else {
      out << name << "_base"
          << "(amat,iel,iint,NPROPS,MPROPS,STRAN1,STRAN0,beta,F0.begin(),"
          << " voj,F1.begin(),vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
          << " ielas,mi,NSTATV,STATEV0,STATEV1,s.begin(),D.begin(),"
          << "iorien,pgauss,orab,PNEWDT,ipkon,size);\n";
    }
    out << "if(*PNEWDT>=1){\n"
        << "*PNEWDT=-1;\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "s.changeBasis(rb);\n"
          << "D = change_basis(st2tost2<3u,real>(D),rb);\n";
    }
    out << "// turning Cauchy stress to pk2\n"
        << "pk2 = convertCauchyStressToSecondPiolaKirchhoffStress(s,F1);\n"
        << "pk2.exportTab(STRESS);\n"
        << "// converting the consistent tangent operator\n"
        << "calculix::ConvertUnsymmetricTangentOperator::exe(DDSDDE,D.begin());"
           "\n";
    if (getDebugMode()) {
      out << "std::cout << \"Dt :\" << std::endl;\n"
          << "const calculix::CalculiXReal *p = DDSDDE;\n"
          << "for(calculix::CalculiXInt i=0;i!=6;++i){\n"
          << "for(calculix::CalculiXInt j=0;j!=i+1;++j,++p){\n"
          << "std::cout << *p << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n";
    }
    out << "}\n"
        << "} // end of " << this->getFunctionNameBasis(name) << "\n\n";
  }

  void CalculiXInterface::writeSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "CalculiXInterface::writeSmallStrainFunction: " + m);
    };
    const std::string sfeh =
        "calculix::CalculiXStandardSmallStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
    writeArguments(out, mb, false);
    out << "{\n"
        << "using namespace tfel::math;\n"
        << "using real = calculix::CalculiXReal;\n"
        << "TFEL_CONSTEXPR const real sqrt2  = Cste<real>::sqrt2;\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      this->generateMTestFile1(out, mb);
    }
    out << "st2tost2<3u,real> D = {0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0};\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "stensor<3u,real> eto  = {STRAN0[0],\n"
          << "                         STRAN0[1],\n"
          << "                         STRAN0[2],\n"
          << "                         STRAN0[3]*sqrt2,\n"
          << "                         STRAN0[4]*sqrt2,\n"
          << "                         STRAN0[5]*sqrt2};\n"
          << "stensor<3u,real> deto = {STRAN1[0]-STRAN0[0],\n"
          << "                         STRAN1[1]-STRAN0[1],\n"
          << "                         STRAN1[2]-STRAN0[2],\n"
          << "                         (STRAN1[3]-STRAN0[3])*sqrt2,\n"
          << "                         (STRAN1[4]-STRAN0[4])*sqrt2,\n"
          << "                         (STRAN1[5]-STRAN0[5])*sqrt2};\n"
          << "stensor<3u,real> s;\n"
          << "s.importTab(STRESS);\n"
          << "if(*iorien==0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation defined for an orthotropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n"
          << "const auto r  = "
             "calculix::getRotationMatrix(orab+7*(*iorien-1),pgauss);\n"
          << "eto.changeBasis(r);\n"
          << "deto.changeBasis(r);\n"
          << "s.changeBasis(r);\n";
    } else {
      throw_if(mb.getSymmetryType() != mfront::ISOTROPIC,
               "unsupported symmetry type");
      out << "const stensor<3u,real> eto  = {STRAN0[0],\n"
          << "                               STRAN0[1],\n"
          << "                               STRAN0[2],\n"
          << "                               STRAN0[3]*sqrt2,\n"
          << "                               STRAN0[4]*sqrt2,\n"
          << "                               STRAN0[5]*sqrt2};\n"
          << "const stensor<3u,real> deto = {STRAN1[0]-STRAN0[0],\n"
          << "                               STRAN1[1]-STRAN0[1],\n"
          << "                               STRAN1[2]-STRAN0[2],\n"
          << "                               (STRAN1[3]-STRAN0[3])*sqrt2,\n"
          << "                               (STRAN1[4]-STRAN0[4])*sqrt2,\n"
          << "                               (STRAN1[5]-STRAN0[5])*sqrt2};\n"
          << "stensor<3u,real> s;\n"
          << "s.importTab(STRESS);\n"
          << "if(*iorien!=0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation shall be defined for an istropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n";
    }
    out << name << "_base"
        << "(amat,iel,iint,NPROPS,MPROPS,deto.begin(),eto.begin(),beta,XOKL,"
        << " voj,XKL,vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
        << " ielas,mi,NSTATV,STATEV0,STATEV1,s.begin(),D.begin(),"
        << "iorien,pgauss,orab,PNEWDT,ipkon,size);\n"
        << "if(*PNEWDT>=1){\n"
        << "*PNEWDT=-1;\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "const auto rb = transpose(r);\n"
          << "s.changeBasis(rb);\n"
          << "D = change_basis(st2tost2<3u,real>(D),rb);\n";
    }
    out << "s.exportTab(STRESS);\n"
        << "// converting the consistent tangent operator\n"
        << "calculix::ConvertUnsymmetricTangentOperator::exe(DDSDDE,D.begin());"
           "\n";
    if (getDebugMode()) {
      out << "std::cout << \"Dt :\" << std::endl;\n"
          << "const calculix::CalculiXReal *p = DDSDDE;\n"
          << "for(calculix::CalculiXInt i=0;i!=6;++i){\n"
          << "for(calculix::CalculiXInt j=0;j!=i+1;++j,++p){\n"
          << "std::cout << *p << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "} else {\n";
      this->generateMTestFile2(out, mb, mb.getBehaviourType(), name, "");
    }
    out << "}\n";
    out << "} // end of " << this->getFunctionNameBasis(name) << "\n\n";
  }

  void CalculiXInterface::writeFiniteRotationSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    this->writeSmallStrainFunction(out, mb, name);
  }  // end of CalculiXInterface::writeFiniteRotationSmallStrainFunction

  void CalculiXInterface::writeMieheApelLambrechtLogarithmicStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "CalculiXInterface::writeMieheApelLambrecht"
                     "LogarithmicStrainFunction: " +
                         m);
    };
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto& ivs = mb.getBehaviourData(h).getPersistentVariables();
    const auto nivs = ivs.getTypeSize().getValueForModellingHypothesis(h);
    const std::string sfeh =
        "calculix::CalculiXLogarithmicStrainStressFreeExpansionHandler";
    const auto variant = [&mb, throw_if] {
      const auto fs = getFiniteStrainStrategy(mb);
      throw_if((fs != "MieheApelLambrechtLogarithmicStrain") &&
                   (fs != "MieheApelLambrechtLogarithmicStrainII"),
               "invalid finite strain strategy (internal error)");
      return fs == "MieheApelLambrechtLogarithmicStrain";
    }();
    this->writeFunctionBase(out, mb, name, sfeh);
    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
    writeArguments(out, mb, false);
    out << "{\n"
        << "using namespace tfel::math;\n"
        << "using namespace tfel::material;\n"
        << "using real = calculix::CalculiXReal;\n";
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      this->generateMTestFile1(out, mb);
    }
    out << "st2tost2<3u,real> D = {0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0,\n"
        << "                       0,0,0,0,0,0};\n";
    if (variant) {
      out << "LogarithmicStrainHandler<3u,real> "
          << "lsh0(LogarithmicStrainHandlerBase::LAGRANGIAN,\n"
          << "     tensor<3u,real>::buildFromFortranMatrix(XOKL));\n"
          << "LogarithmicStrainHandler<3u,real> "
          << "lsh1(LogarithmicStrainHandlerBase::LAGRANGIAN,\n"
          << "     tensor<3u,real>::buildFromFortranMatrix(XKL));\n"
          << "auto eto0 = lsh0.getHenckyLogarithmicStrain();\n"
          << "tfel::math::stensor<3u,real> pk2;\n"
          << "pk2.importTab(STRESS);\n"
          << "auto T = lsh0.convertFromSecondPiolaKirchhoffStress(pk2);\n";
    } else {
      out << "LogarithmicStrainHandler<3u,real> "
          << "lsh1(LogarithmicStrainHandlerBase::LAGRANGIAN,\n"
          << "     tensor<3u,real>::buildFromFortranMatrix(XKL));\n"
          << "const auto ivs0 =  "
             "STATEV0+(*NSTATV)*((*iint-1)+(*mi)*(*iel-1));\n"
          << "const auto ivs1 =  "
             "STATEV1+(*NSTATV)*((*iint-1)+(*mi)*(*iel-1));\n"
          << "stensor<3u,real> eto0(ivs0+" << nivs << ");\n"
          << "stensor<3u,real> T;\n"
          << "tfel::fsalgo::copy<6u>::exe(ivs1+" << nivs + 6
          << ",T.begin());\n";
    }
    out << "auto eto1 = lsh1.getHenckyLogarithmicStrain();\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "if(*iorien==0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation defined for an orthotropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n"
          << "const auto r  = "
             "calculix::getRotationMatrix(orab+7*(*iorien-1),pgauss);\n"
          << "const auto rb = transpose(r);\n";
      if (variant) {
        out << "eto0.changeBasis(r);\n";
      }
      out << "eto1.changeBasis(r);\n";
      if (variant) {
        out << "T.changeBasis(r);\n";
      }
    } else {
      throw_if(mb.getSymmetryType() != mfront::ISOTROPIC,
               "unsupported symmetry type");
      out << "if(*iorien!=0){\n"
          << "  std::cerr << \"" << this->getFunctionNameBasis(name) << ":\"\n"
          << "            << \"no orientation shall be defined for an istropic "
             "behaviour\\n\";\n"
          << "  std::exit(-1);\n"
          << "}\n";
    }
    out << "auto deto = eval(eto1-eto0);\n"
        << "// behaviour integration\n"
        << name << "_base"
        << "(amat,iel,iint,NPROPS,MPROPS,deto.begin(),eto0.begin(),beta,XOKL,"
        << " voj,XKL,vj,ithermal,TEMP1,DTIME,time,ttime,icmd,"
        << " ielas,mi,NSTATV,STATEV0,STATEV1,T.begin(),D.begin(),"
        << "iorien,pgauss,orab,PNEWDT,ipkon,size);\n"
        << "if(*PNEWDT>=1){\n"
        << "*PNEWDT=-1;\n";
    if (!variant) {
      // saving the stresses in the material frame
      out << "tfel::fsalgo::copy<6u>::exe(eto1.begin(),ivs1+" << nivs << ");\n"
          << "tfel::fsalgo::copy<6u>::exe(T.begin(),ivs1+" << nivs + 6
          << ");\n";
    }
    out << "// stress at the end of the time step\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "auto s = lsh1.convertToSecondPiolaKirchhoffStress(T);\n"
          << "s.changeBasis(rb);\n";
    } else {
      out << "const auto s = lsh1.convertToSecondPiolaKirchhoffStress(T);\n";
    }
    out << "D = lsh1.convertToMaterialTangentModuli(st2tost2<3u,real>(D),T);\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "D = change_basis(st2tost2<3u,real>(D),rb);\n";
    }
    out << "// converting the stress\n"
        << "s.exportTab(STRESS);\n"
        << "// converting the consistent tangent operator\n"
        << "calculix::ConvertUnsymmetricTangentOperator::exe(DDSDDE,D.begin());"
           "\n";
    if (getDebugMode()) {
      out << "std::cout << \"Dt :\" << std::endl;\n"
          << "const calculix::CalculiXReal *p = DDSDDE;\n"
          << "for(calculix::CalculiXInt i=0;i!=6;++i){\n"
          << "for(calculix::CalculiXInt j=0;j!=i+1;++j,++p){\n"
          << "std::cout << *p << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "} else {\n";
      this->generateMTestFile2(out, mb, mb.getBehaviourType(), name, "");
    }
    out << "}\n"
        << "} // end of " << this->getFunctionNameBasis(name) << "\n\n";
  }  // end of
     // CalculiXInterface::writeMieheApelLambrechtLogarithmicStrainFunction

  void CalculiXInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/CalculiX/CalculiX.hxx\"\n"
        << "#include\"MFront/CalculiX/CalculiXConvert.hxx\"\n\n";
  }  // end of CalculiXInterface::writeInterfaceSpecificIncludes

  void CalculiXInterface::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(!o.isNull(),
                   "CalculiXInterface::writeBehaviourDataMainVariablesSetter : "
                   "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "calculix::ImportGradients::exe(this->" << v.name << "," << iprefix
         << "stran);\n";
    } else {
      os << "calculix::ImportGradients::exe(this->" << v.name << "0," << iprefix
         << "stran);\n";
    }
  }  // end of CalculiXInterface::writeBehaviourDataGradientSetter

  void CalculiXInterface::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(
        !o.isNull(),
        "CalculiXInterface::writeIntegrationDataMainVariablesSetter : "
        "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "calculix::ImportGradients::exe(this->d" << v.name << "," << iprefix
         << "dstran);\n";
    } else {
      os << "calculix::ImportGradients::exe(this->" << v.name << "1," << iprefix
         << "dstran);\n";
    }
  }  // end of CalculiXInterface::writeIntegrationDataGradientSetter

  void CalculiXInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      os << "calculix::ImportThermodynamicForces::exe(this->" << f.name << ",";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o << ");\n";
      } else {
        os << iprefix << "stress_);\n";
      }
    } else {
      tfel::raise(
          "CalculiXInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type");
    }
  }  // end of CalculiXInterface::writeBehaviourDataThermodynamicForceSetter

  void CalculiXInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if (flag == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        out << "calculix::ExportThermodynamicForces::exe(" << a << "+" << o
            << ",this->sig);\n";
      } else {
        out << "calculix::ExportThermodynamicForces::exe(" << a
            << ",this->sig);\n";
      }
    } else {
      tfel::raise(
          "CalculiXInterface::exportThermodynamicForce: "
          "unsupported forces type");
    }
  }  // end of CalculiXInterface::exportThermodynamicForce

  void CalculiXInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "calculix" + name + ".cxx");
    d.headers.push_back("MFront/CalculiX/calculix" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries,
              tfel::getLibraryInstallName("CalculiXInterface"));
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
    insert_if(l.epts, this->getFunctionNameBasis(name));
  }  // end of CalculiXInterface::getTargetsDescription

  std::string CalculiXInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    auto lib = std::string{};
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = this->getInterfaceName() + mb.getMaterialName();
      } else {
        lib = this->getInterfaceName() + "Behaviour";
      }
    } else {
      lib = this->getInterfaceName() + mb.getLibrary();
    }
    return makeUpperCase(lib);
  }  // end of CalculiXInterface::getLibraryName

  std::string CalculiXInterface::getFunctionNameBasis(
      const std::string& name) const {
    return makeUpperCase(name);
  }  // end of CalculiXInterface::getFunctionName

  std::set<CalculiXInterface::Hypothesis>
  CalculiXInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    const auto& bh = mb.getModellingHypotheses();
    tfel::raise_if(bh.find(ModellingHypothesis::TRIDIMENSIONAL) == bh.end(),
                   "CalculiXInterface::getModellingHypothesesToBeTreated : "
                   "the 'Tridimensional' hypothesis is not supported, "
                   "which is required for the CalculiX interface");
    return {ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of CalculiXInterface::getModellingHypothesesToBeTreated

  void CalculiXInterface::writeCalculiXBehaviourTraits(
      std::ostream& out, const BehaviourDescription& mb) const {
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    out << "template<typename Type";
    if (mb.useQt()) {
      out << ",bool use_qt";
    }
    out << ">\n"
        << "struct CalculiXTraits<tfel::material::" << mb.getClassName()
        << "<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,";
    out << "Type,";
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << ">>\n{\n"
        << "//! behaviour type\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CalculiXBehaviourType btype = "
             "calculix::"
             "STANDARDSTRAINBASEDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " CalculiXBehaviourType btype = "
             "calculix::"
             "STANDARDFINITESTRAINBEHAVIOUR;\n";
    } else {
      tfel::raise(
          "CalculiXInterface::writeCalculiXBehaviourTraits : "
          "unsupported behaviour type");
    }
    out << "//! space dimension\n"
        << "static " << constexpr_c << " unsigned short N "
        << "= "
           "tfel::material::ModellingHypothesisToSpaceDimension<tfel::material:"
           ":ModellingHypothesis::TRIDIMENSIONAL>::value;\n"
        << "// tiny vector size\n"
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "// symmetric tensor size\n"
        << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::value;\n"
        << "// tensor size\n"
        << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n"
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
          << " CalculiXSymmetryType type = calculix::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " CalculiXSymmetryType type = calculix::ORTHOTROPIC;\n";
    } else {
      tfel::raise(
          "CalculiXInterface::writeCalculiXBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The calculix interface only support isotropic or orthotropic "
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
          << " CalculiXSymmetryType etype = calculix::ISOTROPIC;\n";
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
          << " CalculiXSymmetryType etype = calculix::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= 9u;\n";
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
          "CalculiXInterface::writeCalculiXBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The calculix interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    out << "}; // end of class CalculiXTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  CalculiXInterface::gatherModellingHypothesesAndTests(
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
  }  // end of CalculiXInterface::gatherModellingHypothesesAndTests

  std::string CalculiXInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "true";
    }
    tfel::raise(
        "CalculiXInterface::getModellingHypothesisTest : "
        "unsupported modelling hypothesis");
  }  // end of CalculiXInterface::gatherModellingHypothesesAndTests

  bool CalculiXInterface::areExternalStateVariablesSupported() const {
    return false;
  }  // end of CalculiXInterface::areExternalStateVariablesSupported()

  bool CalculiXInterface::isTemperatureIncrementSupported() const {
    return false;
  }  // end of CalculiXInterface::isTemperatureIncrementSupported()

  void CalculiXInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(ModellingHypothesis::TRIDIMENSIONAL);\n";
  }  // end of CalculiXInterface::writeMTestFileGeneratorSetModellingHypothesis

  void CalculiXInterface::writeInputFileExample(const BehaviourDescription& mb,
                                                const FileDescription& fd,
                                                const bool b) const {
    try {
      auto throw_if = [](const bool c, const std::string& m) {
        tfel::raise_if(c, "CalculiXInterface::writeInputFileExample: " + m);
      };
      const auto name = mb.getLibrary() + mb.getClassName();
      const auto mn = this->getLibraryName(mb) + "_" + mb.getClassName();
      const auto fn = (b ? "calculix/" : "calculix-explicit/") + name + ".inp";
      std::ofstream out{fn};
      throw_if(!out, "could not open file '" + fn + "'");
      // header
      out << "** \n"
          << "** File generated by MFront from the " << fd.fileName
          << " source\n"
          << "** Example of how to use the " << mb.getClassName()
          << " behaviour law\n"
          << "** Author " << fd.authorName << '\n'
          << "** Date   " << fd.date << '\n'
          << "**\n\n";
      // loop over hypothesis
      const auto h = ModellingHypothesis::TRIDIMENSIONAL;
      const auto& d = mb.getBehaviourData(h);
      const auto mps = this->buildMaterialPropertiesList(mb, h);
      auto msize = SupportedTypes::TypeSize{};
      if (!mps.first.empty()) {
        const auto& m = mps.first.back();
        msize = m.offset;
        msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
      }
      const auto& persistentVarsHolder = d.getPersistentVariables();
      auto vs = SupportedTypes::TypeSize{};
      for (const auto& v : persistentVarsHolder) {
        vs += SupportedTypes::getTypeSize(v.type, v.arraySize);
      }
      // this is included for gcc 4.7.2
      const auto vsize = [mb, &vs, h, this]() -> unsigned int {
        if ((mb.getBehaviourType() ==
             BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) &&
            (CalculiXInterface::hasFiniteStrainStrategy(mb)) &&
            (getFiniteStrainStrategy(mb) ==
             "MieheApelLambrechtLogarithmicStrainII")) {
          return vs.getValueForModellingHypothesis(h) + 12u;
        }
        return vs.getValueForModellingHypothesis(h);
      }();
      out << "*Material, name=@" << this->getFunctionNameBasis(mn) << '\n';
      if (!b) {
        out << "*DENSITY\n<density>\n";
      }
      if (vsize != 0) {
        out << "*Depvar\n" << vsize << "\n";
      }
      if (!mps.first.empty()) {
        out << "** The material properties are given as if we used parameters "
               "to "
               "explicitly\n"
            << "** display their names. Users shall replace those declaration "
               "by\n"
            << "** theirs values\n";
      }
      out << "*User Material, constants="
          << msize.getValueForModellingHypothesis(h);
      out << '\n';
      if (!mps.first.empty()) {
        int i = 1;
        auto write = [&i, &out](const std::string& n) {
          if (i % 9 == 0) {
            out << "\n";
            i = 1;
          }
          out << '<' << n << '>';
          ++i;
        };
        for (auto pm = mps.first.begin(); pm != mps.first.end();) {
          if (pm->arraySize == 1u) {
            write(pm->name);
          } else {
            for (unsigned short a = 0; a != pm->arraySize;) {
              write(pm->name + "_" + std::to_string(a));
              if (++a != pm->arraySize) {
                out << ", ";
              }
            }
          }
          if (++pm != mps.first.end()) {
            out << ", ";
          }
        }
      }
      out << "\n\n";
    } catch (std::exception& e) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << e.what() << std::endl;
      }
    } catch (...) {
      if (getVerboseMode() > VERBOSE_QUIET) {
        getLogStream() << "CalculiXInterface::generateInputFileExample: "
                       << "unknown exception thrown" << std::endl;
      }
    }
  }  // end of CalculiXInterface::writeInputFileExample

  CalculiXInterface::~CalculiXInterface() = default;

}  // end of namespace mfront
