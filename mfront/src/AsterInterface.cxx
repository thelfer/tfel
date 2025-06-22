/*!
 * \file   mfront/src/AsterInterface.cxx
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
#include "MFront/AsterInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  std::string AsterInterface::getName() { return "aster"; }

  std::string AsterInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    using namespace std;
    string lib;
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        lib = "Aster" + mb.getMaterialName();
      } else {
        lib = "AsterBehaviour";
      }
    } else {
      lib = "Aster" + mb.getLibrary();
    }
    return lib;
  }  // end of AsterInterface::getLibraryName

  std::string AsterInterface::getInterfaceName() const {
    return "Aster";
  }  // end of AsterInterface::getInterfaceName

  std::string AsterInterface::getFunctionName(const std::string& name) const {
    return "aster" + makeLowerCase(name);
  }  // end of AsterInterface::getLibraryName

  AsterInterface::AsterInterface()
      : compareToNumericalTangentOperator(false),
        strainPerturbationValue(1.e-6),
        tangentOperatorComparisonCriterion(1.e7),
        savesTangentOperator(false),
        errorReport(true) {}  // end of AsterInterface::AsterInterface()

  std::pair<bool, AsterInterface::tokens_iterator> AsterInterface::treatKeyword(
      BehaviourDescription&,
      const std::string& key,
      const std::vector<std::string>& i,
      tokens_iterator current,
      const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error("AsterInterface::treatKeyword : " + m));
      }
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        throw_if((key != "@AsterGenerateMTestFileOnFailure") &&
                     (key != "@AsterCompareToNumericalTangentOperator") &&
                     (key != "@AsterTangentOperatorComparisonCriterium") &&
                     (key != "@AsterTangentOperatorComparisonCriterion") &&
                     (key != "@AsterStrainPerturbationValue") &&
                     (key != "@AsterSaveTangentOperator") &&
                     (key != "@AsterErrorReport"),
                 "unsupported key '" + key + "'");
      } else {
        return {false, current};
      }
    }
    if (key == "@AsterGenerateMTestFileOnFailure") {
      this->generateMTestFile = this->readBooleanValue(key, current, end);
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
      this->savesTangentOperator = this->readBooleanValue(key, current, end);
      return {true, current};
    } else if (key == "@AsterErrorReport") {
      this->errorReport = this->readBooleanValue(key, current, end);
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
    if (h.empty()) {
      throw(std::runtime_error(
          "AsterInterfaceModellingHypothesesToBeTreated : "
          "no hypotheses selected. This means that the given beahviour "
          "can't be used neither in 'AxisymmetricalGeneralisedPlaneStrain' "
          "nor in 'AxisymmetricalGeneralisedPlaneStress', so it does not "
          "make sense to use the Aster interface"));
    }
    return h;
  }  // end of AsterInterface::getModellingHypothesesToBeTreated

  void AsterInterface::endTreatment(const BehaviourDescription& mb,
                                    const FileDescription& fd) const {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::utilities;
    string name;
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) ||
          (mb.getBehaviourType() == BehaviourDescription::COHESIVEZONEMODEL))) {
      throw(
          runtime_error("AsterInterface::endTreatment : "
                        "the aster interface only supports "
                        "small and finite strain behaviours "
                        "and cohesive zone models"));
    }
    if ((this->compareToNumericalTangentOperator) ||
        (this->savesTangentOperator)) {
      if (mb.getBehaviourType() !=
          BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
        throw(
            runtime_error("AsterInterface::endTreatment : "
                          "unsupported feature @AsterSaveTangentOperator "
                          "and @AsterCompareToNumericalTangentOperator : "
                          "those are only valid for small strain beahviours"));
      }
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    if (!mb.getLibrary().empty()) {
      name += mb.getLibrary();
    }
    name += mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Aster");

    // header
    string fileName("aster");
    fileName += name;
    fileName += ".hxx";
    ofstream out("include/MFront/Aster/" + fileName);
    if (!out) {
      string msg("AsterInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   " << fileName << '\n';
    out << "* \\brief  This file declares the aster interface for the "
        << mb.getClassName() << " behaviour law\n";
    out << "* \\author " << fd.authorName << '\n';
    out << "* \\date   " << fd.date << '\n';
    out << "*/\n\n";

    const string header = this->getHeaderDefine(mb);
    out << "#ifndef " << header << "\n";
    out << "#define " << header << "\n\n";

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

    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      this->writeAsterBehaviourTraits(out, mb,
                                      ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for (const auto& h : mh) {
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
    this->writeSetParametersFunctionsDeclarations(out, name, mb);

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
                                              local de l'élement fini massif au
                                              repère géneral du maillage */
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
                                              local de l'élement fini massif au
                                              repère géneral du maillage */
        << "aster::AsterReal *const," /*< rapport entre le nouveau pas de temps
                                        suggeré et le pas de temps donné en
                                        entrée */
        << "const aster::AsterInt  *const" /*< type de modélisation */
        << ");\n\n";

    out << "#ifdef __cplusplus\n";
    out << "}\n";
    out << "#endif /* __cplusplus */\n\n";

    out << "#endif /* " << header << " */\n";

    out.close();

    fileName = "aster";
    fileName += name;
    fileName += ".cxx";

    out.open("src/" + fileName);
    if (!out) {
      string msg("AsterInterface::endTreatment : ");
      msg += "could not open file ";
      msg += fileName;
      throw(runtime_error(msg));
    }

    string sfeh;
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      sfeh = "aster::AsterStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      sfeh = "nullptr";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      sfeh = "nullptr";
    } else {
      string msg(
          "AsterInterface::endTreatment : "
          "the aster interface only supports small and finite strain "
          "behaviours and cohesive zone models");
      throw(runtime_error(msg));
    }

    out << "/*!\n";
    out << "* \\file   " << fileName << '\n';
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
        (this->savesTangentOperator)) {
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

    this->generateUMATxxGeneralSymbols(out, name, mb, fd);
    if (!mb.areAllMechanicalDataSpecialised(mh)) {
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out, name, uh, mb, fd);
    }
    for (const auto& h : mh) {
      if (mb.hasSpecialisedMechanicalData(h)) {
        this->generateUMATxxSymbols(out, name, h, mb, fd);
      }
    }

    this->writeSetParametersFunctionsImplementations(out, name, mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    string dv0;
    string dv1;
    if (mb.getBehaviourType() ==
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      dv0 = "STRAN";
      dv1 = "DSTRAN";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      dv0 = "F0";
      dv1 = "F1";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      dv0 = "U0";
      dv1 = "DU";
    } else {
      throw(runtime_error(
          "AsterInterface::endTreatment : "
          "the aster interface only supports small "
          "and finite strain behaviours and cohesive zone models"));
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
                                                     repère local   de l'élement
                                                     fini massif au repère géneral
                                                     du maillage */
        << "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de
                                               temps
                                               suggeré et le pas de temps donné
                                               en entrée */
        << "const aster::AsterInt *const NUMMOD" /*< type de modélisation */
        << ")\n";
    out << "{\n";
    if (((getDebugMode()) || (this->compareToNumericalTangentOperator) ||
         (this->savesTangentOperator)) &&
        (!this->generateMTestFile)) {
      out << "using namespace std;\n";
    }
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "using mfront::BehaviourProfiler;\n";
      out << "using tfel::material::" << mb.getClassName() << "Profiler;\n";
      out << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
          << "Profiler::getProfiler(),\n"
          << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
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
    if (!this->savesTangentOperator) {
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
          << ">::exe(NTENS,DTIME,DROT,DDSOE," << dv0 << "," << dv1
          << ",TEMP,DTEMP,PROPS,NPROPS,"
          << "PREDEF,DPRED,STATEV,NSTATV,STRESS,NUMMOD,"
          << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
          << ")!=0){\n";
      this->generateMTestFile2(out, mb.getBehaviourType(), name, "", mb);
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
    } else {
      out << "if(*(NSTATV)<(*NTENS)*(*NTENS)){\n"
          << "string msg(\"aster" << makeLowerCase(name)
          << ": invalid number of state variables (can't save tangent "
             "operator)\");\n"
          << "throw(runtime_error(msg));\n"
          << "}\n";
      out << "aster::AsterInt nNSTATV = "
             "max(*(NSTATV)-(*NTENS)*(*NTENS),aster::AsterInt(1));\n";
      out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
          << ">::exe(NTENS,DTIME,DROT,DDSOE," << dv0 << "," << dv1
          << ",TEMP,DTEMP,PROPS,NPROPS,"
          << "PREDEF,DPRED,STATEV,&nNSTATV,STRESS,NUMMOD,"
          << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
          << ")!=0){\n";
      this->generateMTestFile2(out, mb.getBehaviourType(), name, "", mb);
      out << "*PNEWDT = -1.;\n";
      out << "return;\n";
      out << "}\n";
      out << "copy(DDSOE,DDSOE+(*NTENS)*(*NTENS),STATEV+*(NSTATV)-(*NTENS)*(*"
             "NTENS));\n";
    }
    if (getDebugMode()) {
      out << "if(computeTangentOperator){\n"
          << "aster::AsterInt i;\n"
          << "aster::AsterInt j;\n"
          << "cout << \"Dt :\" << endl;\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "cout << endl;\n"
          << "}\n"
          << "cout << endl;\n"
          << "}\n";
    }
    if (this->compareToNumericalTangentOperator) {
      out << "if(computeTangentOperator){\n"
          << "// computing the tangent operator by pertubation\n"
          << "aster::AsterInt i;\n"
          << "aster::AsterInt j;\n"
          << "vector<aster::AsterReal> nD((*NTENS)*(*NTENS));\n"
          << "vector<aster::AsterReal> deto(*NTENS);\n"
          << "vector<aster::AsterReal> sigf(*NTENS);\n"
          << "vector<aster::AsterReal> sigb(*NTENS);\n"
          << "vector<aster::AsterReal> sv(*NSTATV);\n"
          << "vector<aster::AsterReal> D((*NTENS)*(*NTENS));\n"
          << "aster::AsterReal m;\n"
          << "aster::AsterReal mDt;\n"
          << "aster::AsterReal mnDt;\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "copy(sig0.begin(),sig0.end(),sigf.begin());\n"
          << "copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] += " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n";
      if (!this->savesTangentOperator) {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,"
               "NPROPS,"
            << "PREDEF,DPRED,&sv[0],NSTATV,&sigf[0],NUMMOD,"
            << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
            << ")!=0){\n";
      } else {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,"
               "NPROPS,"
            << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigf[0],NUMMOD,"
            << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
            << ")!=0){\n";
      }
      out << "return;\n"
          << "}\n"
          << "copy(deto0.begin(),deto0.end(),deto.begin());\n"
          << "copy(sig0.begin(),sig0.end(),sigb.begin());\n"
          << "copy(sv0.begin(),sv0.end(),sv.begin());\n"
          << "deto[i] -= " << this->strainPerturbationValue << ";\n"
          << "D[0] = 0.;\n";
      if (!this->savesTangentOperator) {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,"
               "NPROPS,"
            << "PREDEF,DPRED,&sv[0],NSTATV,&sigb[0],NUMMOD,"
            << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
            << ")!=0){\n";
      } else {
        out << "if(aster::AsterInterface<tfel::material::" << mb.getClassName()
            << ">::exe(NTENS,DTIME,DROT,&D[0],STRAN,&deto[0],TEMP,DTEMP,PROPS,"
               "NPROPS,"
            << "PREDEF,DPRED,&sv[0],&nNSTATV,&sigb[0],NUMMOD,"
            << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh
            << ")!=0){\n";
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
          << "mDt=max(mDt,*(DDSOE+i));\n"
          << "mnDt=max(mnDt,nD[i]);\n"
          << "m=max(m,abs(nD[i]-*(DDSOE+i)));\n"
          << "}\n"
          << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
          << "cout << \"||nDt-Dt|| = \" << m << \" (\" << "
             "100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< endl;\n"
          << "cout << \"Dt :\" << endl;\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "cout << *(DDSOE+j*(*NTENS)+i) << \" \";\n"
          << "}\n"
          << "cout << endl;\n"
          << "}\n"
          << "cout << \"nDt :\" << endl;\n"
          << "for(i=0;i!=*NTENS;++i){\n"
          << "for(j=0;j!=*NTENS;++j){\n"
          << "cout << nD[j*(*NTENS)+i] << \" \";\n"
          << "}\n"
          << "cout << endl;\n"
          << "}\n"
          << "cout << endl;\n"
          << "}\n"
          << "}\n";
    }
    out << "}\n\n";
    out << "MFRONT_SHAREDOBJ void\n"
        << this->getFunctionName(name) << "("
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
                                                     repère local   de l'élement
                                                     fini massif au repère géneral
                                                     du maillage */
        << "aster::AsterReal *const PNEWDT," /*< rapport entre le nouveau pas de
                                               temps
                                               suggeré et le pas de temps donné
                                               en entrée */
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
    insert_if(d[lib].cppflags, "$(shell " + tfel_config + " --includes)");
#if ASTER_ARCH == 64
    insert_if(d[lib].cppflags, "-DASTER_ARCH=64");
#elif ASTER_ARCH == 32
    insert_if(d[lib].cppflags, "-DASTER_ARCH=32");
#else
#error "AsterInterface::getGlobalIncludes : unsuported architecture"
#endif
    insert_if(d[lib].sources, "aster" + name + ".cxx");
    d.headers.push_back("MFront/Aster/aster" + name + ".hxx");
    insert_if(d[lib].ldflags,
              "-l" + tfel::getLibraryInstallName("AsterInterface"));
    if (this->generateMTestFile) {
      insert_if(d[lib].ldflags,
                "-l" + tfel::getLibraryInstallName("MTestFileGenerator"));
    }
    insert_if(d[lib].ldflags, "$(shell " + tfel_config +
                                  " --libs --material --mfront-profiling)");
    // insert_if(d[lib].epts,name);
    insert_if(d[lib].epts, this->getFunctionName(name));
  }  // end of AsterInterface::getTargetsDescription

  std::pair<std::vector<UMATInterfaceBase::UMATMaterialProperty>,
            SupportedTypes::TypeSize>
  AsterInterface::buildMaterialPropertiesList(const BehaviourDescription& mb,
                                              const Hypothesis h) const {
    auto throw_if = [](const bool b, const std::string& m) {
      if (b) {
        throw(std::runtime_error(
            "AsterInterface::buildMaterialPropertiesList: " + m));
      }
    };
    if (h == ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
      const auto ah = this->getModellingHypothesesToBeTreated(mb);
      std::set<Hypothesis> uh;
      for (const auto& lh : ah) {
        if (!mb.hasSpecialisedMechanicalData(lh)) {
          uh.insert(lh);
        }
      }
      throw_if(
          uh.empty(),
          "internal error : the mechanical behaviour says that not "
          "all handled mechanical data are specialised, but we found none.");
      // material properties for all the selected hypothesis
      auto mpositions = std::vector<std::pair<std::vector<UMATMaterialProperty>,
                                              SupportedTypes::TypeSize>>{};
      for (const auto& lh : uh) {
        mpositions.push_back(this->buildMaterialPropertiesList(mb, lh));
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
          throw_if(
              o1 != o2,
              "incompatible offset for material property '" + mp.name +
                  "' (aka '" + mp1.name +
                  "'). This is one pitfall of the umat interface. "
                  "To by-pass this limitation, you may want to explicitely "
                  "specialise some modelling hypotheses");
        }
      }
      return mfirst;
    }
    auto res = std::pair<std::vector<UMATMaterialProperty>,
                         SupportedTypes::TypeSize>{};
    auto& mprops = res.first;
    throw_if((h != ModellingHypothesis::GENERALISEDPLANESTRAIN) &&
                 (h != ModellingHypothesis::AXISYMMETRICAL) &&
                 (h != ModellingHypothesis::PLANESTRAIN) &&
                 (h != ModellingHypothesis::PLANESTRESS) &&
                 (h != ModellingHypothesis::TRIDIMENSIONAL),
             std::string("unsupported modelling hypothesis") +
                 ((h == ModellingHypothesis::UNDEFINEDHYPOTHESIS)
                      ? " (default)"
                      : " '" + ModellingHypothesis::toString(h) + "'"));
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
        } else if ((h != ModellingHypothesis::
                             AXISYMMETRICALGENERALISEDPLANESTRAIN) &&
                   (h != ModellingHypothesis::GENERALISEDPLANESTRAIN) &&
                   (h != ModellingHypothesis::AXISYMMETRICAL) &&
                   (h != ModellingHypothesis::PLANESTRAIN) &&
                   (h != ModellingHypothesis::PLANESTRESS)) {
          throw_if(true, "unsupported modelling hypothesis");
        }
      } else {
        throw_if(true, "unsupported behaviour symmetry type");
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
        throw_if(true, "unsupported behaviour symmetry type");
      }
    }
    if (!mprops.empty()) {
      const auto& m = mprops.back();
      res.second = m.offset;
      res.second += this->getTypeSize(m.type, m.arraySize);
    }
    this->completeMaterialPropertiesList(mprops, mb, h);
    return res;
  }  // end of AsterInterface::buildMaterialPropertiesList

  void AsterInterface::writeUMATxxAdditionalSymbols(
      std::ostream&,
      const std::string&,
      const Hypothesis,
      const BehaviourDescription&,
      const FileDescription&) const {
  }  // end of AsterInterface::writeUMATxxAdditionalSymbols

  void AsterInterface::writeUMATxxSpecificSymbols(
      std::ostream& out,
      const std::string& name,
      const BehaviourDescription&,
      const FileDescription&) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << this->getFunctionName(name)
        << "_savesTangentOperator = ";
    if (this->savesTangentOperator) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
  }  // end of AsterInterface::writeUMATxxSpecificSymbols

  void AsterInterface::writeAsterBehaviourTraits(std::ostream& out,
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
        BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = "
             "aster::SMALLSTRAINSTANDARDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = "
             "aster::FINITESTRAINSTANDARDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "static " << constexpr_c
          << " AsterBehaviourType btype = aster::COHESIVEZONEMODEL;\n";
    } else {
      throw(
          runtime_error("AsterInterface::writeAsterBehaviourTraits : "
                        "unsupported behaviour type"));
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
    if (this->errorReport) {
      out << "static " << constexpr_c
          << " AsterErrorReportPolicy errorReportPolicy = "
             "ASTER_WRITEONSTDOUT;\n";
    } else {
      out << "static " << constexpr_c
          << " AsterErrorReportPolicy errorReportPolicy = "
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
      string msg("AsterInterface::endTreatment : ");
      msg += "unsupported behaviour type.\n";
      msg +=
          "The aster interface only support isotropic or orthotropic behaviour "
          "at this time.";
      throw(runtime_error(msg));
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
      string msg("AsterInterface::endTreatment : ");
      msg += "unsupported behaviour type.\n";
      msg +=
          "The aster interface only support isotropic or orthotropic behaviour "
          "at this time.";
      throw(runtime_error(msg));
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
      const auto h = this->getModellingHypothesesToBeTreated(mb);
      for (const auto& mh : h) {
        res.insert({mh, this->getModellingHypothesisTest(mh)});
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
    throw(
        std::runtime_error("AsterInterface::getModellingHypothesisTest : "
                           "unsupported modelling hypothesis"));
  }  // end of UMATInterface::gatherModellingHypothesesAndTests

}  // end of namespace mfront
