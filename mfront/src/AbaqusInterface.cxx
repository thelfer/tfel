/*!
 * \file   mfront/src/AbaqusInterface.cxx
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

#include <iostream>
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
#include "MFront/AbaqusSymbolsGenerator.hxx"
#include "MFront/AbaqusInterface.hxx"

namespace mfront {

  static void checkCompareToNumericalTangentOperatorConsistency(
      const BehaviourDescription& bd) {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "checkCompareToNumericalTangentOperatorConsistency "
                     "(AbaqusInterface): " +
                         m);
    };
    throw_if(bd.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "unsupported feature @AbaqusSaveTangentOperator "
             "and @AbaqusCompareToNumericalTangentOperator : "
             "those are only valid for small strain beahviours");
    if (AbaqusInterfaceBase::hasFiniteStrainStrategy(bd)) {
      const auto fs = AbaqusInterfaceBase::getFiniteStrainStrategy(bd);
      throw_if(fs != "Native",
               "unsupported feature "
               "@AbaqusCompareToNumericalTangentOperator: "
               "those are only valid for small strain beahviours or "
               "finite strain behaviours based on the `Native` "
               "finite strain strategy");
    }
  }

  static bool usesMFrontOrthotropyManagementPolicy(
      const BehaviourDescription& mb) {
    if (AbaqusInterfaceBase::hasOrthotropyManagementPolicy(mb)) {
      return AbaqusInterfaceBase::getOrthotropyManagementPolicy(mb) == "MFront";
    }
    return false;
  }  // end of usesMFrontOrthotropyManagementPolicy

  static void writeArguments(std::ostream& out,
                             const BehaviourDescription& bd,
                             const bool f) {
    if (f) {
      const auto requires_stran = [&bd] {
        if (bd.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (AbaqusInterfaceBase::hasFiniteStrainStrategy(bd)) {
            const auto fs = AbaqusInterfaceBase::getFiniteStrainStrategy(bd);
            return fs == "Native";
          }
        }
        return true;
      }();
      out << "(abaqus::AbaqusReal *const STRESS,\n"
          << " abaqus::AbaqusReal *const STATEV,\n"
          << " abaqus::AbaqusReal *const DDSDDE,\n"
          << " abaqus::AbaqusReal *const SSE,\n"
          << " abaqus::AbaqusReal *const SPD,\n"
          << " abaqus::AbaqusReal *const SCD,\n"
          << " abaqus::AbaqusReal *const RPL,\n"
          << " abaqus::AbaqusReal *const DDSDDT,\n"
          << " abaqus::AbaqusReal *const DRPLDE,\n"
          << " abaqus::AbaqusReal *const DRPLDT,\n";
      if (requires_stran) {
        out << " const abaqus::AbaqusReal *const STRAN,\n"
            << " const abaqus::AbaqusReal *const DSTRAN,\n";
      } else {
        out << " const abaqus::AbaqusReal *const,\n"
            << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusReal *const TIME,\n"
          << " const abaqus::AbaqusReal *const DTIME,\n"
          << " const abaqus::AbaqusReal *const TEMP,\n"
          << " const abaqus::AbaqusReal *const DTEMP,\n"
          << " const abaqus::AbaqusReal *const PREDEF,\n"
          << " const abaqus::AbaqusReal *const DPRED,\n"
          << " const char           *const CMNAME,\n"
          << " const abaqus::AbaqusInt  *const NDI,\n"
          << " const abaqus::AbaqusInt  *const NSHR,\n"
          << " const abaqus::AbaqusInt  *const NTENS,\n"
          << " const abaqus::AbaqusInt  *const NSTATV,\n"
          << " const abaqus::AbaqusReal *const PROPS,\n"
          << " const abaqus::AbaqusInt  *const NPROPS,\n"
          << " const abaqus::AbaqusReal *const COORDS,\n"
          << " const abaqus::AbaqusReal *const DROT,\n"
          << "       abaqus::AbaqusReal *const PNEWDT,\n"
          << " const abaqus::AbaqusReal *const CELENT,\n"
          << " const abaqus::AbaqusReal *const DFGRD0,\n"
          << " const abaqus::AbaqusReal *const DFGRD1,\n"
          << " const abaqus::AbaqusInt  *const NOEL,\n"
          << " const abaqus::AbaqusInt  *const NPT,\n"
          << " const abaqus::AbaqusInt  *const LAYER,\n"
          << " const abaqus::AbaqusInt  *const KSPT,\n"
          << " const abaqus::AbaqusInt  *const KSTEP,\n"
          << "       abaqus::AbaqusInt  *const KINC,\n"
          << "const int size)";
    } else {
      const auto btype = bd.getBehaviourType();
      const auto sb =
          (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR);
      const auto requires_kstep = [&bd, &btype] {
        if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (AbaqusInterfaceBase::hasFiniteStrainStrategy(bd)) {
            return AbaqusInterfaceBase::getFiniteStrainStrategy(bd) != "Native";
          }
          return false;
        }
        return true;
      }();
      out << "(abaqus::AbaqusReal *const STRESS,\n"
          << " abaqus::AbaqusReal *const STATEV,\n"
          << " abaqus::AbaqusReal *const DDSDDE,\n"
          << " abaqus::AbaqusReal *const SSE,\n"
          << " abaqus::AbaqusReal *const SPD,\n"
          << " abaqus::AbaqusReal *const SCD,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n";
      if (sb) {
        out << " const abaqus::AbaqusReal *const STRAN,\n"
            << " const abaqus::AbaqusReal *const DSTRAN,\n";
      } else {
        out << " const abaqus::AbaqusReal *const,\n"
            << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusReal *const,\n"
          << " const abaqus::AbaqusReal *const DTIME,\n"
          << " const abaqus::AbaqusReal *const TEMP,\n"
          << " const abaqus::AbaqusReal *const DTEMP,\n"
          << " const abaqus::AbaqusReal *const PREDEF,\n"
          << " const abaqus::AbaqusReal *const DPRED,\n"
          << " const char           *const,\n"
          << " const abaqus::AbaqusInt  *const,\n"
          << " const abaqus::AbaqusInt  *const,\n"
          << " const abaqus::AbaqusInt  *const NTENS,\n"
          << " const abaqus::AbaqusInt  *const NSTATV,\n"
          << " const abaqus::AbaqusReal *const PROPS,\n"
          << " const abaqus::AbaqusInt  *const NPROPS,\n"
          << " const abaqus::AbaqusReal *const,\n"
          << " const abaqus::AbaqusReal *const DROT,\n"
          << "       abaqus::AbaqusReal *const PNEWDT,\n"
          << " const abaqus::AbaqusReal *const,\n";
      if (!sb) {
        out << " const abaqus::AbaqusReal *const F0,\n"
            << " const abaqus::AbaqusReal *const F1,\n";
      } else {
        out << " const abaqus::AbaqusReal *const,\n"
            << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusInt  *const,\n"
          << " const abaqus::AbaqusInt  *const,\n"
          << " const abaqus::AbaqusInt  *const,\n"
          << " const abaqus::AbaqusInt  *const,\n";
      if (requires_kstep) {
        out << " const abaqus::AbaqusInt  *const KSTEP,\n";
      } else {
        out << " const abaqus::AbaqusInt  *const,\n";
      }
      out << "       abaqus::AbaqusInt  *const,\n"
          << "const int)";
    }
  }  // end of writeArguments

  static void writeArguments(std::ostream& out) {
    out << "(abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const char           *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << "       abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << " const abaqus::AbaqusInt  *const,\n"
        << "       abaqus::AbaqusInt  *const,\n"
        << "const int)";
  }  // end of writeArguments

  std::string AbaqusInterface::getName() { return "abaqus"; }

  std::string AbaqusInterface::getInterfaceName() const {
    return "Abaqus";
  }  // end of AbaqusInterface::getInterfaceName

  std::pair<bool, AbaqusInterface::tokens_iterator>
  AbaqusInterface::treatKeyword(BehaviourDescription& bd,
                                const std::string& k,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AbaqusInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        auto keys = AbaqusInterfaceBase::getCommonKeywords();
        keys.insert(keys.end(), {"@AbaqusGenerateMTestFileOnFailure",
                                 "@GenerateMTestFileOnFailure",
                                 "@AbaqusCompareToNumericalTangentOperator",
                                 "@AbaqusTangentOperatorComparisonCriterium",
                                 "@AbaqusTangentOperatorComparisonCriterion",
                                 "@AbaqusStrainPerturbationValue"});
        throw_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                 "unsupported key '" + k + "'");
      } else {
        return {false, current};
      }
    }
    const auto r =
        AbaqusInterfaceBase::treatCommonKeywords(bd, k, current, end);
    if (r.first) {
      return r;
    }
    if ((k == "@AbaqusGenerateMTestFileOnFailure") ||
        (k == "@GenerateMTestFileOnFailure")) {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(k, current, end));
      return {true, current};
    } else if (k == "@AbaqusCompareToNumericalTangentOperator") {
      checkCompareToNumericalTangentOperatorConsistency(bd);
      this->compareToNumericalTangentOperator =
          this->readBooleanValue(k, current, end);
      return make_pair(true, current);
    } else if ((k == "@AbaqusTangentOperatorComparisonCriterium") ||
               (k == "@AbaqusTangentOperatorComparisonCriterion")) {
      throw_if(
          !this->compareToNumericalTangentOperator,
          "comparison to tangent operator is not enabled at this stage.\n"
          "Use the @AbaqusCompareToNumericalTangentOperator directive before "
          "@AbaqusTangentOperatorComparisonCriterion");
      checkCompareToNumericalTangentOperatorConsistency(bd);
      throw_if(current == end, "unexpected end of file");
      this->tangentOperatorComparisonCriterion =
          CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if (k == "@AbaqusStrainPerturbationValue") {
      throw_if(!this->compareToNumericalTangentOperator,
               "time stepping is not enabled at this stage.\n"
               "Use the @AbaqusUseTimeSubStepping directive before "
               "@AbaqusStrainPerturbationValue");
      throw_if(current == end, "unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  void AbaqusInterface::endTreatment(const BehaviourDescription& mb,
                                     const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AbaqusInterface::endTreatment: " + m);
    };
    AbaqusInterfaceBase::checkFiniteStrainStrategyDefinitionConsistency(mb);
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "the abaqus interface only supports small and "
             "finite strain behaviours");
    if (this->compareToNumericalTangentOperator) {
      checkCompareToNumericalTangentOperatorConsistency(mb);
    }
    AbaqusInterfaceBase::checkOrthotropyManagementPolicyConsistency(mb);
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      // this is required for gcc 4.7.2
      const auto requires_mfront_omp = [&mb] {
        if (mb.getBehaviourType() ==
            BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
          if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
            const auto fs = AbaqusInterfaceBase::getFiniteStrainStrategy(mb);
            return fs != "Native";
          } else {
            return false;
          }
        }
        return true;
      }();
      const auto mfront_omp = usesMFrontOrthotropyManagementPolicy(mb);
      throw_if(((requires_mfront_omp) && !(mfront_omp)),
               "orthotropic finite strain behaviours "
               "are only supported with the "
               "'MFront' othotropy management policy. See the "
               "'@AbaqusOrthotropyManagementPolicy' for details");
    }
    // get the modelling hypotheses to be treated
    const auto& mhs = this->getModellingHypothesesToBeTreated(mb);
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Abaqus");
    systemCall::mkdir("abaqus");

    std::ofstream out;
    {
      // copy umat.cpp locally
      MFrontLockGuard lock;
      out.open("abaqus/umat.cpp");
      if (out) {
        const auto root = tfel::getInstallPath();
#ifdef TFEL_APPEND_SUFFIX
        const auto fn = root + "/share/doc/mfront-" VERSION "/abaqus/umat.cpp";
#else  /* TFEL_APPEND_SUFFIX */
        const auto fn = root + "/share/doc/mfront/abaqus/umat.cpp";
#endif /* TFEL_APPEND_SUFFIX */
        std::ifstream in{fn};
        if (in) {
          out << in.rdbuf();
          in.close();
        } else {
          std::cerr << "AbaqusInterface::endTreatment: "
                    << "could not open file '" << fn << "'" << std::endl;
        }
      } else {
        std::cerr << "AbaqusInterface::endTreatment: "
                  << "could not open file 'abaqus/umat.cpp'" << std::endl;
      }
      out.close();
    }

    // header
    auto fname = "abaqus" + name + ".hxx";
    out.open("include/MFront/Abaqus/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the abaqus interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderGuard(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
        << "#include\"MFront/Abaqus/Abaqus.hxx\"\n"
        << "#include\"MFront/Abaqus/AbaqusData.hxx\"\n\n";
    out << "#ifdef __cplusplus\n"
        << "#include\"MFront/Abaqus/AbaqusTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Abaqus/AbaqusOrthotropicBehaviour.hxx\"\n";
    }
    if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
      const auto fs = AbaqusInterfaceBase::getFiniteStrainStrategy(mb);
      if (fs == "FiniteRotationSmallStrain") {
        out << "#include\"MFront/Abaqus/AbaqusFiniteStrain.hxx\"\n\n";
      } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
        out << "#include\"TFEL/Material/LogarithmicStrainHandler.hxx\"\n\n";
      }
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace abaqus{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      this->writeAbaqusBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeAbaqusBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace abaqus\n\n"
        << "#endif /* __cplusplus */\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    for (const auto h : mhs) {
      out << "MFRONT_SHAREDOBJ void\n"
          << this->getFunctionNameForHypothesis(name, h);
      writeArguments(out);
      out << ";\n\n";
    }

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "abaqus" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the abaqus interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);

    if (this->compareToNumericalTangentOperator) {
      out << "#include<cmath>\n"
          << "#include<vector>\n"
          << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (mb.hasAttribute(AbaqusInterfaceBase::orthotropyManagementPolicy)) {
      if (AbaqusInterfaceBase::getOrthotropyManagementPolicy(mb) != "Native") {
        out << "#include\"MFront/Abaqus/AbaqusRotation.hxx\"\n\n";
      }
    }
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/Abaqus/AbaqusInterface.hxx\"\n\n"
        << "#include\"MFront/Abaqus/abaqus" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";
    AbaqusSymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, mb, fd, mhs, name);
    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      sg.generateSymbols(out, *this, mb, fd, name, uh);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        sg.generateSymbols(out, *this, mb, fd, name, h);
      }
    }

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    for (const auto h : mhs) {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
          const auto fs = AbaqusInterfaceBase::getFiniteStrainStrategy(mb);
          if (fs == "Native") {
            this->writeSmallStrainFunction(out, mb, name, h);
          } else if (fs == "FiniteRotationSmallStrain") {
            this->writeFiniteRotationSmallStrainFunction(out, mb, name, h);
          } else if (fs == "MieheApelLambrechtLogarithmicStrain") {
            this->writeMieheApelLambrechtLogarithmicStrainFunction(out, mb,
                                                                   name, h);
          } else {
            throw_if(true, "unsupported finite strain strategy '" + fs + "'");
          }
        } else {
          this->writeSmallStrainFunction(out, mb, name, h);
        }
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        this->writeFiniteStrainFunction(out, mb, name, h);
      } else {
        throw_if(true,
                 "the abaqus interface only supports small "
                 "and finite strain behaviours");
      }
    }
    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb, fd, true);
  }  // end of AbaqusInterface::endTreatment

  void AbaqusInterface::writeFunctionBase(std::ostream& out,
                                          const BehaviourDescription& mb,
                                          const std::string& name,
                                          const std::string& sfeh,
                                          const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AbaqusInterface::writeFunctionBase: " + m);
    };
    std::string dv0, dv1, sig, statev, nstatev;
    const auto btype = mb.getBehaviourType();
    out << "static void\n"
        << name << "_base" << this->getFunctionNameForHypothesis("", h);
    writeArguments(out, mb, false);
    out << "{\n";
    // this is required for gcc 4.7.2
    const auto is_fs = [&mb, &btype] {
      if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
          return AbaqusInterfaceBase::getFiniteStrainStrategy(mb) != "Native";
        }
        return false;
      }
      return true;
    }();
    if (is_fs) {
      out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
          << "if(KSTEP[2]!=1){\n"
          << "std::cerr << \"the " << name << " behaviour is only "
          << "valid in finite strain analysis\\n\";\n"
          << "*PNEWDT=-1;\n"
          << "return;\n"
          << "}\n"
          << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n";
    }
    const auto mfront_omp = usesMFrontOrthotropyManagementPolicy(mb);
    if (mfront_omp) {
      if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        // turning the deformation and the deformation gradient
        // increment to the material frame
        if (h == ModellingHypothesis::PLANESTRESS) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation2D<abaqus::AbaqusReal> R(STATEV);\n"
              << "const abaqus::AbaqusReal eto[4u]  = {*STRAN ,*(STRAN+1) "
                 ",0,*(STRAN+2)};\n"
              << "const abaqus::AbaqusReal deto[4u] = "
                 "{*DSTRAN,*(DSTRAN+1),0,*(DSTRAN+2)};\n"
              << "abaqus::AbaqusReal sg[4u] = {*STRESS ,*(STRESS+1) "
                 ",0,*(STRESS+2)};\n"
              << "abaqus::AbaqusReal e[4u];\n"
              << "abaqus::AbaqusReal de[4u];\n"
              << "abaqus::AbaqusReal sm[4u];\n"
              << "R.rotateStrainsForward(eto,e);\n"
              << "R.rotateStrainsForward(deto,de);\n"
              << "R.rotateStressesForward(sg,sm);\n"
              << "e[2]=e[3];\n"
              << "e[3]=e[0];\n"
              << "de[2]=de[3];\n"
              << "de[3]=de[0];\n"
              << "sm[2]=sm[3];\n"
              << "sm[3]=0;\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-2;\n";
          statev = "STATEV+2";
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRAIN)) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation2D<abaqus::AbaqusReal> R(STATEV);\n"
              << "abaqus::AbaqusReal e[4u];\n"
              << "abaqus::AbaqusReal de[4u];\n"
              << "abaqus::AbaqusReal sm[4u];\n"
              << "R.rotateStrainsForward(STRAN,e);\n"
              << "R.rotateStrainsForward(DSTRAN,de);\n"
              << "R.rotateStressesForward(STRESS,sm);\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-2;\n";
          statev = "STATEV+2";
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<6){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation3D<abaqus::AbaqusReal> R(STATEV);\n"
              << "abaqus::AbaqusReal e[6u];\n"
              << "abaqus::AbaqusReal de[6u];\n"
              << "abaqus::AbaqusReal sm[6u];\n"
              << "R.rotateStrainsForward(STRAN,e);\n"
              << "R.rotateStrainsForward(DSTRAN,de);\n"
              << "R.rotateStressesForward(STRESS,sm);\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-6;\n";
          statev = "STATEV+6";
        } else {
          throw_if(true, "unsupported hypothesis");
        }
        dv0 = "e";
        dv1 = "de";
        sig = "sm";
        nstatev = "nstatev";
      } else if (btype == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        // turning the deformation gradients to the material frame
        if (h == ModellingHypothesis::PLANESTRESS) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation2D<abaqus::AbaqusReal> R(STATEV);\n"
              << "abaqus::AbaqusReal Fm0[9u];\n"
              << "abaqus::AbaqusReal Fm1[9u];\n"
              << "abaqus::AbaqusReal sg[4u] = {*STRESS ,*(STRESS+1) "
                 ",0,*(STRESS+2)};\n"
              << "abaqus::AbaqusReal sm[4u];\n"
              << "R.rotateDeformationGradientForward(F0,Fm0);\n"
              << "R.rotateDeformationGradientForward(F1,Fm1);\n"
              << "R.rotateStressesForward(sg,sm);\n"
              << "sm[2]=sm[3];\n"
              << "sm[3]=0;\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-2;\n";
          statev = "STATEV+2";
        } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                   (h == ModellingHypothesis::PLANESTRAIN)) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation2D<abaqus::AbaqusReal> R(STATEV);\n"
              << "abaqus::AbaqusReal Fm0[9u];\n"
              << "abaqus::AbaqusReal Fm1[9u];\n"
              << "abaqus::AbaqusReal sm[4u];\n"
              << "R.rotateDeformationGradientForward(F0,Fm0);\n"
              << "R.rotateDeformationGradientForward(F1,Fm1);\n"
              << "R.rotateStressesForward(STRESS,sm);\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-2;\n";
          statev = "STATEV+2";
        } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
          out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
              << "if(*NSTATV<6){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
              << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n"
              << "abaqus::AbaqusRotation3D<abaqus::AbaqusReal> R(STATEV);\n"
              << "abaqus::AbaqusReal Fm0[9u];\n"
              << "abaqus::AbaqusReal Fm1[9u];\n"
              << "abaqus::AbaqusReal sm[6u];\n"
              << "R.rotateDeformationGradientForward(F0,Fm0);\n"
              << "R.rotateDeformationGradientForward(F1,Fm1);\n"
              << "R.rotateStressesForward(STRESS,sm);\n"
              << "const abaqus::AbaqusInt nstatev = *NSTATV-6;\n";
          statev = "STATEV+6";
        } else {
          throw_if(true, "unsupported hypothesis");
        }
        dv0 = "Fm0";
        dv1 = "Fm1";
        sig = "sm";
        nstatev = "nstatev";
      } else {
        throw_if(true,
                 "the abaqus interface only supports small "
                 "and finite strain behaviours");
      }
    } else {
      if (btype == BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        dv0 = "STRAN";
        dv1 = "DSTRAN";
      } else if (btype == BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        dv0 = "F0";
        dv1 = "F1";
      } else {
        throw_if(true,
                 "the abaqus interface only supports small "
                 "and finite strain behaviours");
      }
      sig = "STRESS";
      statev = "STATEV";
      nstatev = "*NSTATV";
    }
    out << "abaqus::AbaqusData d = {" << sig << ",PNEWDT,DDSDDE," << statev
        << ",\n"
        << "                        SSE,SPD,SCD,\n"
        << "                        *NTENS,*NPROPS," << nstatev << ",*DTIME,\n"
        << "                        DROT," << dv0 << "," << dv1
        << ",TEMP,DTEMP,\n"
        << "                        PROPS,PREDEF,DPRED,\n"
        << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
        << sfeh << "};\n"
        << "if(abaqus::AbaqusInterface<tfel::material::ModellingHypothesis::"
        << ModellingHypothesis::toUpperCaseString(h)
        << ",tfel::material::" << mb.getClassName() << ">::exe(d)!=0){\n";
    out << "*PNEWDT = -1.;\n"
        << "return;\n"
        << "}\n";
    if (mfront_omp) {
      if (h == ModellingHypothesis::PLANESTRESS) {
        out << "abaqus::AbaqusReal D[16u] = {DDSDDE[0],DDSDDE[3],0,DDSDDE[6],\n"
            << "                             DDSDDE[1],DDSDDE[4],0,DDSDDE[7],\n"
            << "                             0,0,0,0,\n"
            << "                             "
               "DDSDDE[2],DDSDDE[5],0,DDSDDE[8]};\n"
            << "sm[3]=sm[2];\n"
            << "sm[2]=0;\n"
            << "R.rotateStressesBackward(sm,sg);\n"
            << "R.rotateTangentOperatorBackward(D);\n"
            << "STRESS[0]=sg[0];\n"
            << "STRESS[1]=sg[1];\n"
            << "STRESS[2]=sg[3];\n"
            << "DDSDDE[0]=D[0];\n"
            << "DDSDDE[1]=D[4];\n"
            << "DDSDDE[2]=D[12];\n"
            << "DDSDDE[3]=D[1];\n"
            << "DDSDDE[4]=D[5];\n"
            << "DDSDDE[5]=D[13];\n"
            << "DDSDDE[6]=D[3];\n"
            << "DDSDDE[7]=D[7];\n"
            << "DDSDDE[8]=D[15];\n";
      } else if ((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN) ||
                 (h == ModellingHypothesis::TRIDIMENSIONAL)) {
        out << "R.rotateStressesBackward(sm,STRESS);\n"
            << "R.rotateTangentOperatorBackward(DDSDDE);\n";
      } else {
        throw_if(true, "unsupported hypothesis");
      }
    }
    if (getDebugMode()) {
      out << "std::cout << \"Dt :\" << std::endl;\n"
          << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
          << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n";
    }
    out << "}\n\n";
  }  // end of AbaqusInterface::writeFunctionBase

  void AbaqusInterface::writeFiniteStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const std::string sfeh = "nullptr";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, true);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
        << "STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
        << "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
        << "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n"
        << "}\n\n";
  }

  void AbaqusInterface::writeSmallStrainFunction(std::ostream& out,
                                                 const BehaviourDescription& mb,
                                                 const std::string& name,
                                                 const Hypothesis h) const {
    const std::string sfeh =
        "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, true);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "constexpr auto h = "
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ";\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "abaqus::AbaqusReal pnewdt0(*PNEWDT);\n"
          << "abaqus::AbaqusReal sse0(*SSE);\n"
          << "abaqus::AbaqusReal spd0(*SPD);\n"
          << "abaqus::AbaqusReal scd0(*SCD);\n"
          << "std::vector<abaqus::AbaqusReal> deto0(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sig0(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sv0(*NSTATV);\n"
          << "std::copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
          << "std::copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
          << "std::copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      this->generateMTestFile1(out, mb);
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
        << "STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
        << "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
        << "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n";
    if (this->shallGenerateMTestFileOnFailure(mb)) {
      out << "if(*PNEWDT<1){\n";
      this->generateMTestFileForHypothesis(out, mb, mb.getBehaviourType(), name,
                                           "", h);
      out << "}\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "// computing the tangent operator by pertubation\n"
          << "std::vector<abaqus::AbaqusReal> nD((*NTENS)*(*NTENS));\n"
          << "std::vector<abaqus::AbaqusReal> deto(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sigf(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sigb(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sv(*NSTATV);\n"
          << "std::vector<abaqus::AbaqusReal> D((*NTENS)*(*NTENS));\n"
          << "abaqus::AbaqusReal pnewdt(pnewdt0);\n"
          << "abaqus::AbaqusReal sse(sse0);\n"
          << "abaqus::AbaqusReal spd(spd0);\n"
          << "abaqus::AbaqusReal scd(scd0);\n"
          << "abaqus::AbaqusReal m;\n"
          << "abaqus::AbaqusReal mDt;\n"
          << "abaqus::AbaqusReal mnDt;\n"
          << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigf.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] += " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n"
          << "abaqus::AbaqusData d2 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
          << "                         &sse,&spd,&scd,\n"
          << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
          << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
          << "                         PROPS,PREDEF,DPRED,\n"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
          << sfeh << "};\n"
          << "if(abaqus::AbaqusInterface<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ","
          << "tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
          << "return;\n"
          << "}\n"
          << "abaqus::AbaqusReal pnewdt(pnewdt0);\n"
          << "abaqus::AbaqusReal sse(sse0);\n"
          << "abaqus::AbaqusReal spd(spd0);\n"
          << "abaqus::AbaqusReal scd(scd0);\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] -= " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n"
          << "abaqus::AbaqusData d3 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
          << "                         &sse,&spd,&scd,\n"
          << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
          << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
          << "                         PROPS,PREDEF,DPRED,\n"
          << "if(abaqus::AbaqusInterface<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ","
          << "tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
          << "return;\n"
          << "}\n"
          << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
          << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*"
          << this->strainPerturbationValue << ");\n"
          << "}\n"
          << "}\n"
          << "// comparison\n"
          << "m=0.;\n"
          << "mDt=0.;\n"
          << "mnDt=0.;\n"
          << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
          << "mDt=std::max(mDt,*(DDSDDE+i));\n"
          << "mnDt=std::max(mnDt,nD[i]);\n"
          << "m=std::max(m,std::abs(nD[i]-*(DDSDDE+i)));\n"
          << "}\n"
          << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
          << "std::cout << \"||nDt-Dt|| = \" << m << \" (\" << "
             "100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< std::endl;\n"
          << "std::cout << \"Dt :\" << std::endl;\n"
          << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
          << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << \"nDt :\" << std::endl;\n"
          << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
          << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
          << "std::cout << nD[j*(*NTENS)+i] << \" \";\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n";
    }
    // if(AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)){
    //   if(AbaqusInterfaceBase::getFiniteStrainStrategy(mb)=="Native"){
    // 	out <<
    // "abaqus::AbaqusFiniteStrain::applyNativeFiniteStrainCorrection(DDSDDE,DFGRD1,STRESS,*NTENS);\n";
    //   }
    // }
    out << "}\n\n";
  }

  void AbaqusInterface::writeFiniteRotationSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    tfel::raise_if(h == ModellingHypothesis::PLANESTRESS,
                   "AbaqusInterface::writeFiniteRotationSmallStrainFunction: "
                   "plane stress is not supported yet");
    const auto ps = h == ModellingHypothesis::PLANESTRESS ? "true" : "false";
    const std::string sfeh =
        "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, true);
    out << "{\n"
        << "using namespace abaqus;\n"
        << "AbaqusReal eto[6];\n"
        << "AbaqusReal deto[6];\n"
        << "AbaqusReal CSE[36];\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }

    out << "AbaqusFiniteStrain::computeGreenLagrangeStrain(eto,DFGRD0,*NTENS,"
        << ps << ");\n"
        << "AbaqusFiniteStrain::computeGreenLagrangeStrain(deto,DFGRD1,*NTENS,"
        << ps << ");\n"
        << "AbaqusFiniteStrain::"
           "computeSecondPiolaKirchhoffStressFromCauchyStress(STRESS,DFGRD0,*"
           "NTENS,"
        << ps << ",0);\n"
        << "for(int i=0;i!=*NTENS;++i){\n"
        << "deto[i] -= eto[i];\n"
        << "}\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(STRESS,STATEV,CSE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
        << "eto,deto,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
        << "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
        << "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n";
    out << "if(*PNEWDT>=0.99999){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "AbaqusFiniteStrain::"
           "computeCauchyStressFromSecondPiolaKirchhoffStress(STRESS,DFGRD1,*"
           "NTENS,"
        << ps << ",0);\n"
        << "AbaqusFiniteStrain::computeAbaqusTangentOperatorFromCSE(DDSDDE,CSE,"
           "DFGRD1,STRESS,*NTENS,"
        << ps << ");\n"
        << "}\n"
        << "}\n\n";
  }

  void AbaqusInterface::writeMieheApelLambrechtLogarithmicStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AbaqusInterface::"
                     "writeMieheApelLambrechtLogarithmicStrainFunction: " +
                         m);
    };
    throw_if(h == ModellingHypothesis::PLANESTRESS,
             "plane stress is not supported yet");
    const std::string sfeh =
        "abaqus::AbaqusLogarithmicStrainStressFreeExpansionHandler";
    this->writeFunctionBase(out, mb, name, sfeh, h);
    const auto d = [&h, &throw_if] {
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 3u;
      }
      throw_if(!((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN)),
               "unsupported modelling hypothesis");
      return 2u;
    }();
    const auto n = [&h, &throw_if] {
      if (h == ModellingHypothesis::TRIDIMENSIONAL) {
        return 6u;
      }
      throw_if(!((h == ModellingHypothesis::AXISYMMETRICAL) ||
                 (h == ModellingHypothesis::PLANESTRAIN)),
               "unsupported modelling hypothesis");
      return 4u;
    }();
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeArguments(out, mb, true);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    out << "using namespace abaqus;\n"
        << "using namespace tfel::math;\n"
        << "using namespace tfel::material;\n"
        << "AbaqusReal eto[" << n << "];\n"
        << "AbaqusReal deto[" << n << "];\n"
        << "LogarithmicStrainHandler<" << d << ",AbaqusReal> "
        << "lsh0(LogarithmicStrainHandlerBase::EULERIAN,\n"
        << "     tensor<" << d
        << ",AbaqusReal>::buildFromFortranMatrix(DFGRD0));\n"
        << "LogarithmicStrainHandler<" << d << ",AbaqusReal> "
        << "lsh1(LogarithmicStrainHandlerBase::EULERIAN,\n"
        << "     tensor<" << d
        << ",AbaqusReal>::buildFromFortranMatrix(DFGRD1));\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer pre_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPREPROCESSING);\n";
    }
    out << "lsh0.getHenckyLogarithmicStrain(eto);\n"
        << "lsh1.getHenckyLogarithmicStrain(deto);\n";
    for (unsigned short i = 0; i != n; ++i) {
      out << "deto[" << i << "]-=eto[" << i << "];\n";
    }
    out << "lsh0.convertFromCauchyStress(STRESS);\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
        << "eto,deto,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
        << "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
        << "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n"
        << "if(*PNEWDT>=0.99999){\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "{\n"
          << "BehaviourProfiler::Timer post_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::FINITESTRAINPOSTPROCESSING);\n";
    }
    out << "lsh1.convertToAbaqusTangentModuli(DDSDDE,STRESS);\n"
        << "// converting the stress\n"
        << "lsh1.convertToCauchyStress(STRESS);\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "}\n";
    }
    out << "}\n"
        << "}\n\n";
  }

  void AbaqusInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Abaqus/Abaqus.hxx\"\n"
        << "#include\"MFront/Abaqus/AbaqusConvert.hxx\"\n\n";
  }  // end of AbaqusInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string, std::string>>
  AbaqusInterface::getBehaviourDataConstructorAdditionalVariables() const {
    return {{"DR", "increment of rigid body rotation"}};
  }  // end of AbaqusInterface::getBehaviourDataConstructorAdditionalVariables

  void AbaqusInterface::writeBehaviourDataMainVariablesSetters(
      std::ostream& os, const BehaviourDescription& mb) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov, of;
    os << "void set" << iprefix
       << "BehaviourDataGradients(const NumericType* const " << iprefix
       << "stran)\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataGradientSetter(os, v.first, ov);
      ov += SupportedTypes::getTypeSize(v.first.type, 1u);
    }
    os << "}\n\n";
    os << "void set" << iprefix
       << "BehaviourDataThermodynamicForces(const NumericType* const "
       << iprefix << "stress_,\n"
       << "const NumericType* const )\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += SupportedTypes::getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of AbaqusInterface::writeBehaviourDataMainVariablesSetters

  void AbaqusInterface::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(!o.isNull(),
                   "AbaqusInterface::writeBehaviourDataMainVariablesSetter : "
                   "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "abaqus::UMATImportGradients<hypothesis>::exe(this->" << v.name
         << "," << iprefix << "stran);\n";
    } else {
      os << "abaqus::UMATImportGradients<hypothesis>::exe(this->" << v.name
         << "0," << iprefix << "stran);\n";
    }
  }  // end of AbaqusInterface::writeBehaviourDataGradientSetter

  void AbaqusInterface::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(!o.isNull(),
                   "AbaqusInterface::writeIntegrationDataMainVariablesSetter : "
                   "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "abaqus::UMATImportGradients<hypothesis>::exe(this->d" << v.name
         << "," << iprefix << "dstran);\n";
    } else {
      os << "abaqus::UMATImportGradients<hypothesis>::exe(this->" << v.name
         << "1," << iprefix << "dstran);\n";
    }
  }  // end of AbaqusInterface::writeIntegrationDataGradientSetter

  void AbaqusInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      os << "abaqus::UMATImportThermodynamicForces<hypothesis>::exe(this->"
         << f.name << ",";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o << ");\n";
      } else {
        os << iprefix << "stress_);\n";
      }
    } else {
      tfel::raise(
          "AbaqusInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type");
    }
  }  // end of AbaqusInterface::writeBehaviourDataThermodynamicForceSetter

  void AbaqusInterface::completeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto do_nothing = [&out] { out << "static_cast<void>(ABAQUSDR);\n"; };
    /*
     * We apply the rotation associated to the Jauman corotationnal frame only
     * if:
     * - the behaviour is written in small strain
     * - the finite strain strategy is either undefined or `Native`
     */
    // this is required for gcc 4.7.2
    const auto c = [&mb] {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
          return AbaqusInterfaceBase::getFiniteStrainStrategy(mb) == "Native";
        }
        return true;
      }
      return false;
    }();
    if (!c) {
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false;  // have persistent variables that have to be updated
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if ((flag == SupportedTypes::STENSOR) ||
          (flag == SupportedTypes::TENSOR)) {
        b = true;
        break;
      }
    }
    if (!b) {
      do_nothing();
      return;
    }
    // rotate variables
    out << "const tfel::math::tmatrix<3u,3u,real> abaqus_dr = "
        << "{ABAQUSDR[0],ABAQUSDR[1],ABAQUSDR[2],\n"
           "ABAQUSDR[3],ABAQUSDR[4],ABAQUSDR[5],\n"
           "ABAQUSDR[6],ABAQUSDR[7],ABAQUSDR[8]};\n";
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = SupportedTypes::getTypeFlag(v.type);
      if ((flag == SupportedTypes::STENSOR) ||
          (flag == SupportedTypes::TENSOR)) {
        if (v.arraySize == 1u) {
          out << "this->" << v.name << ".changeBasis(abaqus_dr);\n";
        } else {
          for (unsigned short i = 0; i != v.arraySize; ++i) {
            out << "this->" << v.name << "[" << i
                << "].changeBasis(abaqus_dr);\n";
          }
        }
      }
    }
  }  // end of UMATInterfaceBase::completeBehaviourDataConstructor

  void AbaqusInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if (flag == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a
            << "+" << o << ",this->sig);\n";
      } else {
        out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a
            << ",this->sig);\n";
      }
    } else {
      tfel::raise(
          "AbaqusInterface::exportThermodynamicForce: "
          "unsupported forces type");
    }
  }  // end of AbaqusInterface::exportThermodynamicForce

  void AbaqusInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    auto& l = d.getLibrary(lib);
    insert_if(l.cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(l.include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(l.sources, "abaqus" + name + ".cxx");
    d.headers.push_back("MFront/Abaqus/abaqus" + name + ".hxx");
    insert_if(l.link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(l.link_libraries, tfel::getLibraryInstallName("AbaqusInterface"));
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(l.link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(l.link_libraries, "$(shell " + tfel_config +
                                    " --library-dependency "
                                    "--material --mfront-profiling)");
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(l.epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of AbaqusInterface::getTargetsDescription

  AbaqusInterface::~AbaqusInterface() = default;

}  // end of namespace mfront
