/*!
 * \file   mfront/src/AsterInterface.cxx
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
#include "MFront/AsterSymbolsGenerator.hxx"
#include "MFront/AsterInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  const char* const AsterInterface::saveTangentOperator =
      "aster::saveTangentOperator";

  const char* const AsterInterface::finiteStrainFormulation =
      "aster::finiteStrainFormulation";

  const char* const AsterInterface::simo_miehe = "aster::SIMO_MIEHE";

  const char* const AsterInterface::grot_gdep = "aster::GROT_GDEP";

  std::string AsterInterface::getName() { return "aster"; }

  std::string AsterInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        return "Aster" + mb.getMaterialName();
      } else {
        return "AsterBehaviour";
      }
    }
    return "Aster" + mb.getLibrary();
  }  // end of AsterInterface::getLibraryName

  std::string AsterInterface::getInterfaceName() const {
    return "Aster";
  }  // end of AsterInterface::getInterfaceName

  std::string AsterInterface::getFunctionNameBasis(
      const std::string& name) const {
    return "aster" + makeLowerCase(name);
  }  // end of AsterInterface::getLibraryName

  AsterInterface::AsterInterface() = default;

  std::pair<bool, AsterInterface::tokens_iterator> AsterInterface::treatKeyword(
      BehaviourDescription& bd,
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AsterInterface::treatKeyword : " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        throw_if((key != "@AsterGenerateMTestFileOnFailure") &&
                     (key != "@AsterCompareToNumericalTangentOperator") &&
                     (key != "@AsterTangentOperatorComparisonCriterium") &&
                     (key != "@AsterTangentOperatorComparisonCriterion") &&
                     (key != "@AsterStrainPerturbationValue") &&
                     (key != "@AsterSaveTangentOperator") &&
                     (key != "@AsterErrorReport") &&
                     (key != "@AsterFiniteStrainFormulation"),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if (key == "@AsterGenerateMTestFileOnFailure") {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(key, current, end));
      return {true, current};
    } else if (key == "@AsterCompareToNumericalTangentOperator") {
      this->compareToNumericalTangentOperator =
          this->readBooleanValue(key, current, end);
      return make_pair(true, current);
    } else if ((key == "@AsterTangentOperatorComparisonCriterium") ||
               (key == "@AsterTangentOperatorComparisonCriterion")) {
      throw_if(
          !this->compareToNumericalTangentOperator,
          "comparison to tangent operator is not enabled at this stage.\n"
          "Use the @AsterCompareToNumericalTangentOperator directive before "
          "@AsterTangentOperatorComparisonCriterion");
      throw_if(current == end, "unexpected end of file");
      this->tangentOperatorComparisonCriterion =
          CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if (key == "@AsterStrainPerturbationValue") {
      throw_if(!this->compareToNumericalTangentOperator,
               "time stepping is not enabled at this stage.\n"
               "Use the @AsterUseTimeSubStepping directive before "
               "@AsterStrainPerturbationValue");
      throw_if(current == end, "unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current, end);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    } else if (key == "@AsterSaveTangentOperator") {
      bd.setAttribute(AsterInterface::saveTangentOperator,
                      this->readBooleanValue(key, current, end), false);
      return {true, current};
    } else if (key == "@AsterErrorReport") {
      this->errorReport = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if (key == "@AsterFiniteStrainFormulation") {
      throw_if(bd.getBehaviourType() !=
                   BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR,
               "the '@AsterFiniteStrainFormulation' is only valid "
               "for finite strain behaviour");
      const auto s = current->value;
      if (s == "SIMO_MIEHE") {
        bd.setAttribute(AsterInterface::finiteStrainFormulation,
                        std::string(AsterInterface::simo_miehe), false);
      } else if ((s == "GROT_GDEP") || (s == "TotalLagrangian")) {
        bd.setAttribute(AsterInterface::finiteStrainFormulation,
                        std::string(AsterInterface::grot_gdep), false);
      } else {
        throw_if(true, "invalid finite strain formuluation '" + s + "'");
      }
      ++(current);
      throw_if(current == end, "unexpected end of file");
      throw_if(current->value != ";",
               "expected ';', read '" + current->value + "'");
      ++(current);
      return {true, current};
    }
    return {false, current};
  }  // end of treatKeyword

  std::set<AsterInterface::Hypothesis>
  AsterInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    // treatment
    std::set<Hypothesis> h;
    // modelling hypotheses handled by the behaviour
    const auto& bh = mb.getModellingHypotheses();
    if (bh.find(ModellingHypothesis::GENERALISEDPLANESTRAIN) != bh.end()) {
      h.insert(ModellingHypothesis::GENERALISEDPLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::AXISYMMETRICAL) != bh.end()) {
      h.insert(ModellingHypothesis::AXISYMMETRICAL);
    }
    if (bh.find(ModellingHypothesis::PLANESTRAIN) != bh.end()) {
      h.insert(ModellingHypothesis::PLANESTRAIN);
    }
    if (bh.find(ModellingHypothesis::PLANESTRESS) != bh.end()) {
      h.insert(ModellingHypothesis::PLANESTRESS);
    }
    if (bh.find(ModellingHypothesis::TRIDIMENSIONAL) != bh.end()) {
      h.insert(ModellingHypothesis::TRIDIMENSIONAL);
    }
    tfel::raise_if(
        h.empty(),
        "AsterInterfaceModellingHypothesesToBeTreated: "
        "no hypotheses selected. This means that the given beahviour "
        "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
        "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
        "make sense to use the Aster interface");
    return h;
  }  // end of AsterInterface::getModellingHypothesesToBeTreated

  void AsterInterface::endTreatment(const BehaviourDescription& mb,
                                    const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "AsterInterface::endTreatment : " + m);
    };
    throw_if(
        !((mb.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
          (mb.getBehaviourType() == BehaviourDescription::COHESIVEZONEMODEL)),
        "AsterInterface::endTreatment : "
        "the aster interface only supports "
        "small and finite strain behaviours "
        "and cohesive zone models");
    if ((this->compareToNumericalTangentOperator) ||
        (mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false))) {
      throw_if(mb.getBehaviourType() !=
                   BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
               "unsupported feature @AsterSaveTangentOperator "
               "and @AsterCompareToNumericalTangentOperator : "
               "those are only valid for small strain beahviours");
    }
    // get the modelling hypotheses to be treated
    const auto& mhs = this->getModellingHypothesesToBeTreated(mb);
    const auto name = [&mb]() -> std::string {
      if (!mb.getLibrary().empty()) {
        return mb.getLibrary() + mb.getClassName();
      }
      return mb.getClassName();
    }();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Aster");

    // header
    const auto header = "include/MFront/Aster/aster" + name + ".hxx";
    ofstream out(header);
    throw_if(!out, "could not open file '" + header + "'");

    out << "/*!\n";
    out << "* \\file   " << header << '\n';
    out << "* \\brief  This file declares the aster interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << '\n';
    out << "* \\date   " << fd.date << '\n';
    out << "*/\n\n";

    const string headerGuard = this->getHeaderGuard(mb);
    out << "#ifndef " << headerGuard << "\n";
    out << "#define " << headerGuard << "\n\n";

    out << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n";
    out << "#include\"MFront/Aster/Aster.hxx\"\n\n";

    out << "#ifdef __cplusplus\n";
    out << "#include\"MFront/Aster/AsterTraits.hxx\"\n";
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/Aster/AsterOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n";

    out << "namespace aster{\n\n";

    if (!mb.areAllMechanicalDataSpecialised(mhs)) {
      this->writeAsterBehaviourTraits(out, mb,
                                      ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mhs) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->writeAsterBehaviourTraits(out, mb, h);
      }
    }

    out << "} // end of namespace aster\n\n";

    out << "#endif /* __cplusplus */\n\n";

    out << "#ifdef __cplusplus\n";
    out << "extern \"C\"{\n";
    out << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    out << "MFRONT_SHAREDOBJ char *" << this->getFunctionNameBasis(name)
        << "_getIntegrationErrorMessage();\n\n";

    out << "MFRONT_SHAREDOBJ void\n"
        << name << "("
        << "aster::AsterReal *const,"       /*< tenseur des contraintes */
        << "aster::AsterReal *const,"       /*< variables internes */
        << "aster::AsterReal *const,"       /*< matrice jacobienne du modèle */
        << "const aster::AsterReal *const," /*< tenseur des déformations totales
                                                au début du pas */
        << "const aster::AsterReal *const," /*< tenseur des incréments de
                                                déformation totale */
        << "const aster::AsterReal *const," /*< incrément de temps */
        << "const aster::AsterReal *const," /*< température au début du pas */
        << "const aster::AsterReal *const," /*< incrément de température */
        << "const aster::AsterReal *const," /*< variables externes au début du
                                                pas */
        << "const aster::AsterReal *const," /*< incréments des variables
                                                externes */
        << "const aster::AsterInt  *const," /*< nombre de composantes du tenseur
                                                des contraintes */
        << "const aster::AsterInt  *const," /*< nombre de variables internes */
        << "const aster::AsterReal *const," /*< propriétés du matériaux */
        << "const aster::AsterInt  *const," /*< nombre de propriétés matériaux
                                             */
        << "const aster::AsterReal *const," /*< matrice de passage du repère
                                                local
                                                de l'élement fini massif au
                                                repère géneral
                                                du maillage */
        << "aster::AsterReal *const," /*< rapport entre le nouveau pas de temps
                                          suggeré et le pas de temps donné en
                                          entrée */
        << "const aster::AsterInt  *const" /*< type de modélisation */
        << ");\n\n";

    out << "MFRONT_SHAREDOBJ void\naster" << makeLowerCase(name) << "("
        << "aster::AsterReal *const,"       /*< tenseur des contraintes */
        << "aster::AsterReal *const,"       /*< variables internes */
        << "aster::AsterReal *const,"       /*< matrice jacobienne du modèle */
        << "const aster::AsterReal *const," /*< tenseur des déformations totales
                                                au début du pas */
        << "const aster::AsterReal *const," /*< tenseur des incréments de
                                                déformation totale */
        << "const aster::AsterReal *const," /*< incrément de temps */
        << "const aster::AsterReal *const," /*< température au début du pas */
        << "const aster::AsterReal *const," /*< incrément de température */
        << "const aster::AsterReal *const," /*< variables externes au début du
                                                pas */
        << "const aster::AsterReal *const," /*< incréments des variables
                                                externes */
        << "const aster::AsterInt  *const," /*< nombre de composantes du tenseur
                                                des contraintes */
        << "const aster::AsterInt  *const," /*< nombre de variables internes */
        << "const aster::AsterReal *const," /*< propriétés du matériaux */
        << "const aster::AsterInt  *const," /*< nombre de propriétés matériaux
                                             */
        << "const aster::AsterReal *const," /*< matrice de passage du repère
                                                local
                                                de l'élement fini massif au
                                                repère géneral
                                                du maillage */
        << "aster::AsterReal *const," /*< rapport entre le nouveau pas de temps
                                          suggeré et le pas de temps donné en
                                          entrée */
        << "const aster::AsterInt  *const" /*< type de modélisation */
        << ");\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";
    out << "#endif /* " << headerGuard << " */\n";
    out.close();

    const auto src = "src/aster" + name + ".cxx";
    out.open(src);
    throw_if(!out, "could not open file '" + src + "'");

    const auto sfeh = [&mb, &throw_if] {
      if (mb.getBehaviourType() ==
          BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
        return "aster::AsterStandardSmallStrainStressFreeExpansionHandler";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
        return "nullptr";
      } else if (mb.getBehaviourType() ==
                 BehaviourDescription::COHESIVEZONEMODEL) {
        return "nullptr";
      } else {
        throw_if(true,
                 "the aster interface only supports "
                 "small and finite strain behaviours and "
                 "cohesive zone models");
      }
    }();

    out << "/*!\n";
    out << "* \\file   " << src << '\n';
    out << "* \\brief  This file implements the aster interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << '\n';
    out << "* \\date   " << fd.date << '\n';
    out << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);

    if (this->compareToNumericalTangentOperator) {
      out << "#include<cmath>\n";
      out << "#include<vector>\n";
    }
    if ((this->compareToNumericalTangentOperator) ||
        (mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false))) {
      out << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n";
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Aster/AsterStressFreeExpansionHandler.hxx\"\n\n";
    out << "#include\"MFront/Aster/AsterInterface.hxx\"\n\n";
    out << "#include\"MFront/Aster/aster" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    AsterSymbolsGenerator sg;
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

    out << "char *" << this->getFunctionNameBasis(name)
        << "_getIntegrationErrorMessage(){\n"
        << "#if (defined __GNUC__) && (!defined __clang__) && "
           "(!defined __INTEL_COMPILER) && (!defined __PGI)\n"
        << "#if __GNUC__ * 10000+__GNUC_MINOR__ * 100 <  40800\n"
        << "static __thread char msg[128];\n"
        << "#else\n"
        << "static thread_local char msg[128];\n"
        << "#endif\n"
        << "#else /* (defined __GNUC__) ...*/\n"
        << "static thread_local char msg[128];\n"
        << "#endif /* (defined __GNUC__) ...*/\n"
        << "return msg;\n"
        << "} // end of " << this->getFunctionNameBasis(name)
        << "_getIntegrationErrorMessage\n\n";

    string dv0;
    string dv1;
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      dv0 = "STRAN";
      dv1 = "DSTRAN";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      dv0 = "F0";
      dv1 = "F1";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      dv0 = "U0";
      dv1 = "DU";
    } else {
      throw_if(true,
               "the aster interface only supports small "
               "and finite strain behaviours and cohesive zone models");
    }

    out << "MFRONT_SHAREDOBJ void\n"
        << name << "("
        << "aster::AsterReal *const STRESS," /*< tenseur des contraintes */
        << "aster::AsterReal *const STATEV," /*< variables internes */
        << "aster::AsterReal *const DDSOE," /*< matrice jacobienne du modèle */
        << "const aster::AsterReal *const " << dv0
        << "," /*< tenseur des déformations totales au début du pas */
        << "const aster::AsterReal *const " << dv1
        << "," /*< tenseur des incréments de déformation totale */
        << "const aster::AsterReal *const DTIME," /*< incrément de temps */
        << "const aster::AsterReal *const TEMP," /*< température au début du pas
                                                  */
        << "const aster::AsterReal *const DTEMP," /*< incrément de température
                                                   */
        << "const aster::AsterReal *const PREDEF," /*< variables externes au
                                                       début du pas */
        << "const aster::AsterReal *const DPRED,"  /*< incréments des variables
                                                       externes */
        << "const aster::AsterInt  *const NTENS,"  /*< nombre de composantes du
                                                       tenseur des contraintes */
        << "const aster::AsterInt  *const NSTATV," /*< nombre de variables
                                                       internes */
        << "const aster::AsterReal *const PROPS," /*< propriétés du matériaux */
        << "const aster::AsterInt  *const NPROPS," /*< nombre de propriétés
                                                       matériaux */
        << "const aster::AsterReal *const DROT,"   /*< matrice de passage du
                                                       repère local
                                                       de l'élement fini massif au
                                                       repère géneral
                                                       du maillage */
        << "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de
                                                 temps
                                                 suggeré et le pas de temps
                                                donné en entrée */
        << "const aster::AsterInt *const NUMMOD" /*< type de modélisation */
        << ")\n";
    out << "{\n";
    out << "char * msg = " << this->getFunctionNameBasis(name)
        << "_getIntegrationErrorMessage();\n";
    if (((getDebugMode()) || (this->compareToNumericalTangentOperator) ||
         (mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false))) &&
        (!this->shallGenerateMTestFileOnFailure(mb))) {
      out << "using namespace std;\n";
    }
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out, mb);
    if ((getDebugMode()) || (this->compareToNumericalTangentOperator)) {
      out << "const bool computeTangentOperator = (*DDSOE>0.5);\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "vector<aster::AsterReal> deto0(*NTENS);\n";
      out << "vector<aster::AsterReal> sig0(*NTENS);\n";
      out << "vector<aster::AsterReal> sv0(*NSTATV);\n";
      out << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n";
      out << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n";
      out << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    if (!mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false)) {
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
          << ">::exe(msg,NTENS,DTIME,DROT,DDSOE," << dv0 << "," << dv1
          << ",TEMP,DTEMP,PROPS,NPROPS,"
          << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NUMMOD,"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
          << sfeh << ")!=0){\n";
      this->generateMTestFile2(out, mb, mb.getBehaviourType(), name, "");
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
    } else {
      out << "if(*(NSTATV)<(*NTENS)*(*NTENS)){\n"
          << "std::cerr << \"aster" << makeLowerCase(name)
          << ": invalid number of state variables "
          << "(can't save tangent operator)\" << std::endl;\n"
          << "*PNEWDT = -1.;\n"
          << "return;\n"
          << "}\n"
          << "aster::AsterInt nNSTATV = "
             "std::max(*(NSTATV)-(*NTENS)*(*NTENS),aster::AsterInt(1));\n"
          << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
          << ">::exe(msg,NTENS,DTIME,DROT,DDSOE," << dv0 << "," << dv1
          << ",TEMP,DTEMP,PROPS,NPROPS,"
          << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS,NUMMOD,"
          << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
          << sfeh << ")!=0){\n";
      this->generateMTestFile2(out, mb, mb.getBehaviourType(), name, "");
      out << "*PNEWDT = -1.;\n"
          << "return;\n"
          << "}\n"
          << "std::copy(DDSOE,DDSOE+(*NTENS)*(*NTENS),STATEV+*(NSTATV)-(*NTENS)"
             "*(*NTENS));\n";
    }
    if (getDebugMode()) {
      out << "if(computeTangentOperator){\n"
          << "aster::AsterInt i;\n"
          << "aster::AsterInt j;\n"
          << "std::cout << \"Dt :\\n\";\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << '\\n';\n"
          << "}\n"
          << "std::cout << '\\n';\n"
          << "}\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "if(computeTangentOperator){\n"
          << "// computing the tangent operator by pertubation\n"
          << "aster::AsterInt i;\n"
          << "aster::AsterInt j;\n"
          << "std::vector<aster::AsterReal> nD((*NTENS)*(*NTENS));\n"
          << "std::vector<aster::AsterReal> deto(*NTENS);\n"
          << "std::vector<aster::AsterReal> sigf(*NTENS);\n"
          << "std::vector<aster::AsterReal> sigb(*NTENS);\n"
          << "std::vector<aster::AsterReal> sv(*NSTATV);\n"
          << "std::vector<aster::AsterReal> D((*NTENS)*(*NTENS));\n"
          << "aster::AsterReal m;\n"
          << "aster::AsterReal mDt;\n"
          << "aster::AsterReal mnDt;\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "copy(sig0.begin(),sig0.end(),sigf.begin());\n"
          << "copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] += " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n";
      if (!mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false)) {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(nullptr,NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,"
               "DTEMP,PROPS,NPROPS,"
            << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0],NUMMOD,"
            << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
            << sfeh << ")!=0){\n";
      } else {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(nullptr,NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,"
               "DTEMP,PROPS,NPROPS,"
            << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigf[0],NUMMOD,"
            << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
            << sfeh << ")!=0){\n";
      }
      out << "return;\n"
          << "}\n"
          << "std::copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "std::copy(sig0.begin(),sig0.end(),sigb.begin());\n"
          << "std::copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] -= " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n";
      if (!mb.getAttribute<bool>(AsterInterface::saveTangentOperator, false)) {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(nullptr,NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,"
               "DTEMP,PROPS,NPROPS,"
            << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],NUMMOD,"
            << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
            << sfeh << ")!=0){\n";
      } else {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(nullptr,NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,"
               "DTEMP,PROPS,NPROPS,"
            << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigb[0],NUMMOD,"
            << this->getFunctionNameBasis(name) << "_getOutOfBoundsPolicy(),"
            << sfeh << ")!=0){\n";
      }
      out << "return;\n"
          << "}\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*"
          << this->strainPerturbationValue << ");\n"
          << "}\n"
          << "}\n"
          << "// comparison\n"
          << "m=0.;\n"
          << "mDt=0.;\n"
          << "mnDt=0.;\n"
          << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
          << "mDt=std::max(mDt,*(DDSOE+i));\n"
          << "mnDt=std::max(mnDt,nD[i]);\n"
          << "m=std::max(m,std::abs(nD[i]-*(DDSOE+i)));\n"
          << "}\n"
          << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
          << "std::cout << \"||nDt-Dt|| = \" << m << \" (\" << "
             "100.*m/(0.5*(mDt+mnDt)) << \"%)\\n\";\n"
          << "std::cout << \"Dt :\\n\";\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "std::cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "std::cout << '\\n';\n"
          << "}\n"
          << "std::cout << \"nDt :\" << '\\n';\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "std::cout << nD[j*(*NTENS)+i] << \" \";\n"
          << "}\n"
          << "std::cout << '\\n';\n"
          << "}\n"
          << "std::cout << std::endl;\n"
          << "}\n"
          << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionNameBasis(name) << "("
        << "aster::AsterReal *const STRESS," /*< tenseur des contraintes */
        << "aster::AsterReal *const STATEV," /*< variables internes */
        << "aster::AsterReal *const DDSOE," /*< matrice jacobienne du modèle */
        << "const aster::AsterReal *const " << dv0
        << "," /*< tenseur des déformations totales au début du pas */
        << "const aster::AsterReal *const " << dv1
        << "," /*< tenseur des incréments de déformation totale */
        << "const aster::AsterReal *const DTIME," /*< incrément de temps */
        << "const aster::AsterReal *const TEMP," /*< température au début du pas
                                                  */
        << "const aster::AsterReal *const DTEMP," /*< incrément de température
                                                   */
        << "const aster::AsterReal *const PREDEF," /*< variables externes au
                                                       début du pas */
        << "const aster::AsterReal *const DPRED,"  /*< incréments des variables
                                                       externes */
        << "const aster::AsterInt  *const NTENS,"  /*< nombre de composantes du
                                                       tenseur des contraintes */
        << "const aster::AsterInt  *const NSTATV," /*< nombre de variables
                                                       internes */
        << "const aster::AsterReal *const PROPS," /*< propriétés du matériaux */
        << "const aster::AsterInt  *const NPROPS," /*< nombre de propriétés
                                                       matériaux */
        << "const aster::AsterReal *const DROT,"   /*< matrice de passage du
                                                       repère local
                                                       de l'élement fini massif au
                                                       repère géneral
                                                       du maillage */
        << "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de
                                                 temps
                                                 suggeré et le pas de temps
                                                donné en entrée */
        << "const aster::AsterInt *const NUMMOD" /*< type de modélisation */
        << ")\n"
        << "{\n"
        << name << "(STRESS,STATEV,DDSOE," << dv0 << "," << dv1
        << ",DTIME,TEMP,DTEMP,\n"
        << "PREDEF,DPRED,NTENS,NSTATV,PROPS,NPROPS,DROT,PNEWDT,NUMMOD);\n"
        << "}\n\n";
    out << "} // end of extern \"C\"\n";
    out.close();
  }  // end of AsterInterface::endTreatment

  void AsterInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "ModellingHypothesis::Hypothesis h;\n"
        << "if( *NUMMOD == 2u ){\n"
        << "  h = ModellingHypothesis::GENERALISEDPLANESTRAIN;\n"
        << "} else if(*NUMMOD==4){\n"
        << "  h = ModellingHypothesis::AXISYMMETRICAL;\n"
        << "} else if(*NUMMOD==5){\n"
        << "  h = ModellingHypothesis::PLANESTRESS;\n"
        << "} else if(*NUMMOD==6){\n"
        << "  h = ModellingHypothesis::PLANESTRAIN;\n"
        << "} else if(*NUMMOD==3){\n"
        << "  h = ModellingHypothesis::TRIDIMENSIONAL;\n"
        << "} else {\n"
        << "  return;\n"
        << "}\n"
        << "mg.setModellingHypothesis(h);\n";
  }  // end of AsterInterface::writeMTestFileGeneratorSetModellingHypothesis

  void AsterInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/Aster/Aster.hxx\"\n\n";
  }  // end of AsterInterface::writeInterfaceSpecificIncludes

  AsterInterface::~AsterInterface() = default;

  void AsterInterface::getTargetsDescription(TargetsDescription& d,
                                             const BehaviourDescription& bd) {
    const auto lib = AsterInterface::getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
#if ASTER_ARCH == 64
    insert_if(d[lib].cppflags, "-DASTER_ARCH=64");
#elif ASTER_ARCH == 32
    insert_if(d[lib].cppflags, "-DASTER_ARCH=32");
#else
#error "AsterInterface::getGlobalIncludes : unsuported architecture"
#endif
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, "aster" + name + ".cxx");
    d.headers.push_back("MFront/Aster/aster" + name + ".hxx");
    insert_if(d[lib].link_libraries,
              tfel::getLibraryInstallName("AsterInterface"));
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(d[lib].link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
#if __cplusplus >= 201703L
    insert_if(d[lib].link_libraries, "$(shell " + tfel_config +
                                         " --library-dependency "
                                         "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(d[lib].link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    // insert_if(d[lib].epts,name);
    insert_if(d[lib].epts, this->getFunctionNameBasis(name));
  }  // end of AsterInterface::getTargetsDescription

  void AsterInterface::writeAsterBehaviourTraits(std::ostream& out,
                                                 const BehaviourDescription& mb,
                                                 const Hypothesis h) const {
    using namespace std;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AsterInterface::writeAsterBehaviourTraits: " + m);
    };
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
    out << "struct AsterTraits<tfel::material::" << mb.getClassName() << "<";
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
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = "
             "aster::STANDARDSTRAINBASEDBEHAVIOUR;"
             "\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = "
             "aster::STANDARDFINITESTRAINBEHAVIOUR;"
             "\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = aster::COHESIVEZONEMODEL;\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
    out << "//! space dimension\n";
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      out << "static " << constexpr_c
          << " unsigned short N = "
             "tfel::material::ModellingHypothesisToSpaceDimension<H>::value;\n";
    } else {
      out << "static " << constexpr_c
          << " unsigned short N = "
             "tfel::material::ModellingHypothesisToSpaceDimension<"
          << "tfel::material::ModellingHypothesis::"
          << ModellingHypothesis::toUpperCaseString(h) << ">::value;\n";
    }
    out << "// tiny vector size\n";
    out << "static " << constexpr_c << " unsigned short TVectorSize = N;\n";
    out << "// symmetric tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::"
           "value;\n";
    out << "// tensor size\n";
    out << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;"
           "\n";
    out << "// size of the driving variable array\n";
    out << "static " << constexpr_c
        << " unsigned short GradientSize = " << mvs.first << ";\n";
    out << "// size of the thermodynamic force variable array (STRESS)\n";
    out << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n";
    if (this->errorReport) {
      out << "static " << constexpr_c
          << " AsterErrorReportPolicy "
             "errorReportPolicy = "
             "ASTER_WRITEONSTDOUT;\n";
    } else {
      out << "static " << constexpr_c
          << " AsterErrorReportPolicy "
             "errorReportPolicy = "
             "ASTER_NOERRORREPORT;\n";
    }
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
          << " AsterSymmetryType type = aster::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " AsterSymmetryType type = aster::ORTHOTROPIC;\n";
    } else {
      throw_if(true,
               "unsupported behaviour type. "
               "The aster interface only support isotropic or "
               "orthotropic behaviour at this time.");
    }
    out << "static " << constexpr_c
        << " AsterFiniteStrainFormulation afsf = aster::";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      if (!mb.hasAttribute(AsterInterface::finiteStrainFormulation)) {
        out << "SIMO_MIEHE;\n";
      } else {
        const auto& afsf = mb.getAttribute<std::string>(
            AsterInterface::finiteStrainFormulation);
        if (afsf == AsterInterface::simo_miehe) {
          out << "SIMO_MIEHE;\n";
        } else if (afsf == AsterInterface::grot_gdep) {
          out << "GROT_GDEP;\n";
        } else {
          throw_if(true,
                   "internal error: unsupported "
                   "finite strain formulation");
        }
      }
    } else {
      out << "UNDEFINEDFINITESTRAINFORMULATION;\n";
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
          << " AsterSymmetryType etype = aster::ISOTROPIC;\n";
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
          << " AsterSymmetryType etype = aster::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= AsterOrthotropicElasticPropertiesOffset<N>::value;\n";
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
      throw_if(true,
               "unsupported behaviour type.\n"
               "The aster interface only support isotropic "
               "or orthotropic behaviour at this time.");
    }
    out << "}; // end of class AsterTraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  AsterInterface::gatherModellingHypothesesAndTests(
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
  }  // end of AsterInterface::gatherModellingHypothesesAndTests

  std::string AsterInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    if (h == ModellingHypothesis::GENERALISEDPLANESTRAIN) {
      return "*NUMMOD == 2u";
    } else if (h == ModellingHypothesis::AXISYMMETRICAL) {
      return "*NUMMOD==4";
    } else if (h == ModellingHypothesis::PLANESTRESS) {
      return "*NUMMOD==5";
    } else if (h == ModellingHypothesis::PLANESTRAIN) {
      return "*NUMMOD==6";
    } else if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "*NUMMOD==3";
    }
    tfel::raise(
        "AsterInterface::getModellingHypothesisTest : "
        "unsupported modelling hypothesis");
  }  // end of UMATInterface::gatherModellingHypothesesAndTests

}  // end of namespace mfront
