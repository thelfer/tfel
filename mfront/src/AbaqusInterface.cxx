/*!
 * \file   mfront/src/AbaqusInterface.cxx
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

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

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
#include "MFront/AbaqusInterface.hxx"

namespace mfront {

  static void writeUMATArguments(
      std::ostream& out,
      const BehaviourDescription::BehaviourType& t,
      const AbaqusInterface::FiniteStrainStrategy fss,
      const bool f) {
    if (f) {
      out << "(abaqus::AbaqusReal *const STRESS,\n"
          << " abaqus::AbaqusReal *const STATEV,\n"
          << " abaqus::AbaqusReal *const DDSDDE,\n"
          << " abaqus::AbaqusReal *const SSE,\n"
          << " abaqus::AbaqusReal *const SPD,\n"
          << " abaqus::AbaqusReal *const SCD,\n"
          << " abaqus::AbaqusReal *const RPL,\n"
          << " abaqus::AbaqusReal *const DDSDDT,\n"
          << " abaqus::AbaqusReal *const DRPLDE,\n"
          << " abaqus::AbaqusReal *const DRPLDT,\n"
          << " const abaqus::AbaqusReal *const STRAN,\n"
          << " const abaqus::AbaqusReal *const DSTRAN,\n"
          << " const abaqus::AbaqusReal *const TIME,\n"
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
      out << "(abaqus::AbaqusReal *const STRESS,\n"
          << " abaqus::AbaqusReal *const STATEV,\n"
          << " abaqus::AbaqusReal *const DDSDDE,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n"
          << " abaqus::AbaqusReal *const,\n";
      if (t == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
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
      if (t == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
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
      if ((t == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) ||
          ((fss != AbaqusInterfaceBase::NATIVEFINITESTRAINSTRATEGY) &&
           (fss != AbaqusInterfaceBase::UNDEFINEDSTRATEGY))) {
        out << " const abaqus::AbaqusInt  *const KSTEP,\n";
      } else {
        out << " const abaqus::AbaqusInt  *const,\n";
      }
      out << "       abaqus::AbaqusInt  *const,\n"
          << "const int)";
    }
  }  // end of writeUMATArguments

  static void writeUMATArguments(std::ostream& out) {
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
  }  // end of writeUMATArguments

  std::string AbaqusInterface::getName() { return "abaqus"; }

  std::string AbaqusInterface::getInterfaceName() const {
    return "Abaqus";
  }  // end of AbaqusInterface::getInterfaceName

  std::pair<bool, AbaqusInterface::tokens_iterator>
  AbaqusInterface::treatKeyword(BehaviourDescription&,
                                const std::string& key,
                                const std::vector<std::string>& i,
                                tokens_iterator current,
                                const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("AbaqusInterface::treatKeyword: " + m));
      }
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        auto keys = AbaqusInterfaceBase::getCommonKeywords();
        keys.insert(keys.end(), {"@AbaqusGenerateMTestFileOnFailure",
                                 "@AbaqusCompareToNumericalTangentOperator",
                                 "@AbaqusTangentOperatorComparisonCriterium",
                                 "@AbaqusTangentOperatorComparisonCriterion",
                                 "@AbaqusStrainPerturbationValue"});
        throw_if(std::find(keys.begin(), keys.end(), key) == keys.end(),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    const auto r = AbaqusInterfaceBase::treatCommonKeywords(key, current, end);
    if (r.first) {
      return r;
    }
    if (key == "@AbaqusGenerateMTestFileOnFailure") {
      this->generateMTestFile = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if (key == "@AbaqusCompareToNumericalTangentOperator") {
      this->compareToNumericalTangentOperator =
          this->readBooleanValue(key, current, end);
      return make_pair(true, current);
    } else if ((key == "@AbaqusTangentOperatorComparisonCriterium") ||
               (key == "@AbaqusTangentOperatorComparisonCriterion")) {
      throw_if(
          !this->compareToNumericalTangentOperator,
          "comparison to tangent operator is not enabled at this stage.\n"
          "Use the @AbaqusCompareToNumericalTangentOperator directive before "
          "@AbaqusTangentOperatorComparisonCriterion");
      throw_if(current == end, "unexpected end of file");
      this->tangentOperatorComparisonCriterion =
          CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if (key == "@AbaqusStrainPerturbationValue") {
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
      if (b) {
        throw(std::runtime_error("AbaqusInterface::endTreatment: " + m));
      }
    };
    throw_if(
        (mb.getBehaviourType() !=
         BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
            (this->fss != UNDEFINEDSTRATEGY),
        "finite strain strategy is only supported for small strain behaviours");
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)),
             "the abaqus interface only supports small and "
             "finite strain behaviours");
    if (this->compareToNumericalTangentOperator) {
      throw_if(mb.getBehaviourType() !=
                   BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
               "unsupported feature @AbaqusSaveTangentOperator "
               "and @AbaqusCompareToNumericalTangentOperator : "
               "those are only valid for small strain beahviours");
    }
    throw_if((mb.getSymmetryType() != mfront::ORTHOTROPIC) &&
                 (this->omp != UNDEFINEDORTHOTROPYMANAGEMENTPOLICY),
             "orthotropy management policy is only valid "
             "for orthotropic behaviour");
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (((mb.getBehaviourType() ==
            BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
           (this->fss != UNDEFINEDSTRATEGY)) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)) {
        throw_if(
            this->omp != MFRONTORTHOTROPYMANAGEMENTPOLICY,
            "orthotropic finite strain behaviours are only supported with the "
            "'MFront' othotropy management policy. See the "
            "'@AbaqusOrthotropyManagementPolicy' for details");
      }
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
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
#ifdef TFEL_APPEND_VERSION
        const auto fn = root + "/share/doc/mfront-" VERSION "/abaqus/umat.cpp";
#else  /* TFEL_APPEND_VERSION */
        const auto fn = root + "/share/doc/mfront/abaqus/umat.cpp";
#endif /* TFEL_APPEND_VERSION */
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

    const auto header = this->getHeaderDefine(mb);
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
    if (this->fss != UNDEFINEDSTRATEGY) {
      out << "#include\"MFront/Abaqus/AbaqusFiniteStrain.hxx\"\n\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace abaqus{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      this->writeAbaqusBehaviourTraits(
          out, mb, ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mh) {
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
    this->writeSetParametersFunctionsDeclarations(out, name, mb);

    for (const auto h : mh) {
      out << "MFRONT_SHAREDOBJ void\n"
          << this->getFunctionNameForHypothesis(name, h);
      writeUMATArguments(out);
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
    if (this->omp != UNDEFINEDORTHOTROPYMANAGEMENTPOLICY) {
      out << "#include\"MFront/Abaqus/AbaqusRotation.hxx\"\n\n";
    }
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/Abaqus/AbaqusInterface.hxx\"\n\n"
        << "#include\"MFront/Abaqus/abaqus" << name << ".hxx\"\n\n";

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

    for (const auto h : mh) {
      if ((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
          ((this->fss == UNDEFINEDSTRATEGY) ||
           (this->fss == NATIVEFINITESTRAINSTRATEGY))) {
        this->writeUMATSmallStrainFunction(out, mb, name, h);
      } else if ((mb.getBehaviourType() ==
                  BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                 ((this->fss != UNDEFINEDSTRATEGY) &&
                  (this->fss != NATIVEFINITESTRAINSTRATEGY))) {
        if (this->fss == FINITEROTATIONSMALLSTRAIN) {
          this->writeUMATFiniteRotationSmallStrainFunction(out, mb, name, h);
        } else {
          throw_if(true, "unsupported finite strain strategy !");
        }
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        this->writeUMATFiniteStrainFunction(out, mb, name, h);
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

  void AbaqusInterface::writeUMATFunctionBase(std::ostream& out,
                                              const BehaviourDescription& mb,
                                              const std::string& name,
                                              const std::string& sfeh,
                                              const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(
            std::runtime_error("AbaqusInterface::writeUMATFunctionBase: " + m));
      }
    };
    std::string dv0, dv1, sig, statev, nstatev;
    const auto btype = mb.getBehaviourType();
    out << "static void\n"
        << name << "_base" << this->getFunctionNameForHypothesis("", h);
    writeUMATArguments(out, btype, this->fss, false);
    out << "{\n";
    if ((btype == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) ||
        ((btype == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
         ((this->fss != AbaqusInterfaceBase::NATIVEFINITESTRAINSTRATEGY) &&
          (this->fss != AbaqusInterfaceBase::UNDEFINEDSTRATEGY)))) {
      out << "#ifndef MFRONT_ABAQUS_NORUNTIMECHECKS\n"
          << "if(KSTEP[2]!=1){\n"
          << "std::cerr << \"the " << name << " behaviour is only "
          << "valid in finite strain analysis\\n\";\n"
          << "*PNEWDT=-1;\n"
          << "return;\n"
          << "}\n"
          << "#endif /* MFRONT_ABAQUS_NORUNTIMECHECKS */\n";
    }
    if (this->omp == MFRONTORTHOTROPYMANAGEMENTPOLICY) {
      if (btype == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        // turning the deformation and the deformation gradient
        // increment to the material frame
        if (h == ModellingHypothesis::PLANESTRESS) {
          out << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
          out << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
          out << "if(*NSTATV<6){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
      } else if (btype == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
        // turning the deformation gradients to the material frame
        if (h == ModellingHypothesis::PLANESTRESS) {
          out << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
          out << "if(*NSTATV<2){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
          out << "if(*NSTATV<6){\n"
              << "std::cerr << \"" << name
              << this->getFunctionNameForHypothesis("", h) << ": \"\n"
              << "          << \"invalid number of state variables\\n\";\n"
              << "*PNEWDT = -1.;\n"
              << "return;\n"
              << "}\n"
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
      if (btype == BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        dv0 = "STRAN";
        dv1 = "DSTRAN";
      } else if (btype == BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
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
        << "                        *NTENS,*NPROPS," << nstatev << ",*DTIME,\n"
        << "                        DROT," << dv0 << "," << dv1
        << ",TEMP,DTEMP,\n"
        << "                        PROPS,PREDEF,DPRED,\n"
        << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
        << "if(abaqus::AbaqusInterface<tfel::material::ModellingHypothesis::"
        << ModellingHypothesis::toUpperCaseString(h)
        << ",tfel::material::" << mb.getClassName() << ">::exe(d)!=0){\n";
    out << "*PNEWDT = -1.;\n"
        << "return;\n"
        << "}\n";
    if (this->omp == MFRONTORTHOTROPYMANAGEMENTPOLICY) {
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
  }  // end of AbaqusInterface::writeUMATFunctionBase

  void AbaqusInterface::writeUMATFiniteStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const std::string sfeh = "nullptr";
    this->writeUMATFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeUMATArguments(out, BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
                       this->fss, true);
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

  void AbaqusInterface::writeUMATSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    const std::string sfeh =
        "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeUMATArguments(out, BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
                       this->fss, true);
    out << "{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n"
          << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
          << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "abaqus::AbaqusReal pnewdt0(*PNEWDT);\n"
          << "std::vector<abaqus::AbaqusReal> deto0(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sig0(*NTENS);\n"
          << "std::vector<abaqus::AbaqusReal> sv0(*NSTATV);\n"
          << "std::copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
          << "std::copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
          << "std::copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if (this->generateMTestFile) {
      this->generateMTestFile1(out);
    }
    out << name << "_base" << this->getFunctionNameForHypothesis("", h)
        << "(STRESS,STATEV,DDSDDE,SSE,SPD,SCD,RPL,DDSDDT,DRPLDE,DRPLDT,\n"
        << "STRAN,DSTRAN,TIME,DTIME,TEMP,DTEMP,PREDEF,DPRED,CMNAME,\n"
        << "NDI,NSHR,NTENS,NSTATV,PROPS,NPROPS,COORDS,DROT,PNEWDT,\n"
        << "CELENT,DFGRD0,DFGRD1,NOEL,NPT,LAYER,KSPT,KSTEP,KINC,size);\n";
    if (this->generateMTestFile) {
      out << "if(*PNEWDT<1){\n";
      this->generateMTestFile2(out, mb.getBehaviourType(), name, "", mb);
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
          << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
          << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
          << "                         PROPS,PREDEF,DPRED,\n"
          << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
          << "};\n"
          << "if(abaqus::AbaqusInterface<tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ","
          << "tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
          << "return;\n"
          << "}\n"
          << "abaqus::AbaqusReal pnewdt(pnewdt0);\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] -= " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n"
          << "abaqus::AbaqusData d3 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
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
    if (this->fss == NATIVEFINITESTRAINSTRATEGY) {
      out << "abaqus::AbaqusFiniteStrain::applyNativeFiniteStrainCorrection("
             "DDSDDE,DFGRD1,STRESS,*NTENS);\n";
    }
    out << "}\n\n";
  }

  void AbaqusInterface::writeUMATFiniteRotationSmallStrainFunction(
      std::ostream& out,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    if (h == ModellingHypothesis::PLANESTRESS) {
      throw(std::runtime_error(
          "AbaqusInterface::writeUMATFiniteRotationSmallStrainFunction: "
          "plane stress is not supported yet"));
    }
    const auto ps = h == ModellingHypothesis::PLANESTRESS ? "true" : "false";
    const std::string sfeh =
        "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    this->writeUMATFunctionBase(out, mb, name, sfeh, h);
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameForHypothesis(name, h);
    writeUMATArguments(out, BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR,
                       this->fss, true);
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

    out << "static_cast<void>(STRAN);\n"
        << "static_cast<void>(DSTRAN);\n"
        << "AbaqusFiniteStrain::computeGreenLagrangeStrain(eto,DFGRD0,*NTENS,"
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
        << ps << ",0);\n";
    out << "AbaqusFiniteStrain::computeAbaqusTangentOperatorFromCSE(DDSDDE,CSE,"
           "DFGRD1,STRESS,*NTENS,"
        << ps << ");\n";
    out << "}\n";
    out << "}\n\n";
  }

  void AbaqusInterface::writeUMATxxBehaviourTypeSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "AbaqusInterface::writeUMATxxBehaviourTypeSymbols: " + m));
      }
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->fss == UNDEFINEDSTRATEGY) {
        out << "1u;\n\n";
      } else {
        out << "2u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "2u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AbaqusInterface::writeUMATxxBehaviourTypeSymbols

  void AbaqusInterface::writeUMATxxBehaviourKinematicSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription& mb) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "AbaqusInterface::writeUMATxxBehaviourKinematicSymbols: " + m));
      }
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      if (this->fss == UNDEFINEDSTRATEGY) {
        out << "1u;\n\n";
      } else {
        out << "3u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "3u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AbaqusInterface::writeUMATxxBehaviourKinematicSymbols

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
       << "stress_,\n"
       << "                                                        const Type* "
          "const )\n"
       << "{\n";
    for (const auto& v : mb.getMainVariables()) {
      this->writeBehaviourDataThermodynamicForceSetter(os, v.second, of);
      of += this->getTypeSize(v.second.type, 1u);
    }
    os << "}\n\n";
  }  // end of AbaqusInterface::writeBehaviourDataMainVariablesSetters

  void AbaqusInterface::writeBehaviourDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!o.isNull()) {
      throw(std::runtime_error(
          "AbaqusInterface::writeBehaviourDataMainVariablesSetter : "
          "only one driving variable supported"));
    }
    if (v.increment_known) {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->"
         << v.name << "," << iprefix << "stran);\n";
    } else {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->"
         << v.name << "0," << iprefix << "stran);\n";
    }
  }  // end of AbaqusInterface::writeBehaviourDataDrivingVariableSetter

  void AbaqusInterface::writeIntegrationDataDrivingVariableSetter(
      std::ostream& os,
      const DrivingVariable& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (!o.isNull()) {
      throw(std::runtime_error(
          "AbaqusInterface::writeIntegrationDataMainVariablesSetter : "
          "only one driving variable supported"));
    }
    if (v.increment_known) {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->d"
         << v.name << "," << iprefix << "dstran);\n";
    } else {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->"
         << v.name << "1," << iprefix << "dstran);\n";
    }
  }  // end of AbaqusInterface::writeIntegrationDataDrivingVariableSetter

  void AbaqusInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (this->getTypeFlag(f.type) == SupportedTypes::Stensor) {
      os << "abaqus::UMATImportThermodynamicForces<hypothesis>::exe(this->"
         << f.name << ",";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o << ");\n";
      } else {
        os << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error(
          "AbaqusInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type"));
    }
  }  // end of AbaqusInterface::writeBehaviourDataThermodynamicForceSetter

  void AbaqusInterface::completeBehaviourDataConstructor(
      std::ostream& out,
      const Hypothesis h,
      const BehaviourDescription& mb) const {
    auto do_nothing = [&out]() { out << "static_cast<void>(ABAQUSDR);\n"; };
    /*
     * We apply the rotation associated to the Jauman corotationnal frame only
     * if:
     * - the behaviour is written in small strain
     * - the finite strain strategy is either undefined or `Native`
     */
    const auto c = (mb.getBehaviourType() ==
                    BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) &&
                   ((this->fss == UNDEFINEDSTRATEGY) ||
                    (this->fss == NATIVEFINITESTRAINSTRATEGY));
    if (!c) {
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false;  // have persistent variables that have to be updated
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = this->getTypeFlag(v.type);
      if ((flag == SupportedTypes::Stensor) ||
          (flag == SupportedTypes::Tensor)) {
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
           "{ABAQUSDR[0],ABAQUSDR[1],ABAQUSDR[2],\n"
           "                                                        "
           "ABAQUSDR[3],ABAQUSDR[4],ABAQUSDR[5],\n"
           "                                                        "
           "ABAQUSDR[6],ABAQUSDR[7],ABAQUSDR[8]};\n";
    for (const auto& v : d.getPersistentVariables()) {
      const auto flag = this->getTypeFlag(v.type);
      if ((flag == SupportedTypes::Stensor) ||
          (flag == SupportedTypes::Tensor)) {
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
    const auto flag = this->getTypeFlag(f.type);
    if (flag == SupportedTypes::Stensor) {
      if (!o.isNull()) {
        out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a
            << "+" << o << ",this->sig);\n";
      } else {
        out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a
            << ",this->sig);\n";
      }
    } else {
      throw(
          std::runtime_error("AbaqusInterface::exportThermodynamicForce: "
                             "unsupported forces type"));
    }
  }  // end of AbaqusInterface::exportThermodynamicForce

  void AbaqusInterface::getTargetsDescription(TargetsDescription& d,
                                              const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags, "$(shell " + tfel_config + " --includes)");
    insert_if(d[lib].sources, "abaqus" + name + ".cxx");
    d.headers.push_back("MFront/Abaqus/abaqus" + name + ".hxx");
    insert_if(d[lib].ldflags,
              "-l" + tfel::getLibraryInstallName("AbaqusInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].ldflags,
                "-l" + tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
    for (const auto h : this->getModellingHypothesesToBeTreated(bd)) {
      insert_if(d[lib].epts, this->getFunctionNameForHypothesis(name, h));
    }
  }  // end of AbaqusInterface::getTargetsDescription

  AbaqusInterface::~AbaqusInterface() = default;

}  // end of namespace mfront
