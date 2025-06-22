/*!
 * \file   mfront/src/InitDSLs.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13 sep 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MISSING_STD_THREADS_SUPPORT
#include <mutex>
#endif /* TFEL_MISSING_STD_THREADS_SUPPORT */
#include "MFront/DSLProxy.hxx"

#include "MFront/DefaultDSL.hxx"
#include "MFront/DefaultCZMDSL.hxx"
#include "MFront/DefaultFiniteStrainDSL.hxx"
#include "MFront/DefaultGenericBehaviourDSL.hxx"
#include "MFront/IsotropicMisesCreepDSL.hxx"
#include "MFront/IsotropicMisesPlasticFlowDSL.hxx"
#include "MFront/IsotropicStrainHardeningMisesCreepDSL.hxx"
#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/MultipleIsotropicMisesFlowsDSL.hxx"
#include "MFront/RungeKuttaDSL.hxx"
#include "MFront/RungeKuttaFiniteStrainDSL.hxx"
#include "MFront/RungeKuttaGenericBehaviourDSL.hxx"
#include "MFront/ImplicitDSL.hxx"
#include "MFront/ImplicitDSLII.hxx"
#include "MFront/ImplicitFiniteStrainDSL.hxx"
#include "MFront/ImplicitGenericBehaviourDSL.hxx"

#include "MFront/ModelDSL.hxx"
#include "MFront/InitDSLs.hxx"

namespace mfront {

  void initDSLs() {
    static bool init = false;
#ifndef TFEL_MISSING_STD_THREADS_SUPPORT
    static std::mutex m;
    std::lock_guard<std::mutex> lock(m);
#endif /* TFEL_MISSING_STD_THREADS_SUPPORT */
    if (init) {
      return;
    }
    if (!init) {
      DSLProxy<DefaultDSL> proxy1(
          std::vector<std::string>{"Default", "DefaultParser"});
      DSLProxy<IsotropicMisesCreepDSL> proxy2("IsotropicMisesCreepParser");
      DSLProxy<IsotropicMisesPlasticFlowDSL> proxy3(std::vector<std::string>{
          "IsotropicMisesPlasticFlowParser", "IsotropicMisesPlasticFlow"});
      DSLProxy<IsotropicStrainHardeningMisesCreepDSL> proxy4(
          "IsotropicStrainHardeningMisesCreepParser");
      DSLProxy<MaterialPropertyDSL> proxy5(std::vector<std::string>{
          "MaterialProperty", "MaterialPropertyParser"});
      DSLProxy<MultipleIsotropicMisesFlowsDSL> proxy6(
          "MultipleIsotropicMisesFlowsParser");
      DSLProxy<RungeKuttaDSL> proxy7(
          std::vector<std::string>{"RungeKuttaParser"});
      DSLProxy<RungeKuttaFiniteStrainDSL> proxy8;
      DSLProxy<ImplicitDSL> proxy9("ImplicitParser");
      DSLProxy<ImplicitDSLII> proxy10("ImplicitParserII");
      DSLProxy<ModelDSL> proxy11("ModelParser");
      // CZM
      DSLProxy<DefaultCZMDSL> proxy100(
          std::vector<std::string>{"DefaultCZMParser", "DefaultCZM"});
      // FiniteStrain
      DSLProxy<DefaultFiniteStrainDSL> proxy200(std::vector<std::string>{
          "DefaultFiniteStrain", "DefaultFiniteStrainParser"});
      DSLProxy<ImplicitFiniteStrainDSL> proxy201("ImplicitFiniteStrainParser");
      // Generic behaviours
      DSLProxy<DefaultGenericBehaviourDSL> proxy300(std::vector<std::string>{
          "DefaultGenericBehaviour", "DefaultGenericBehaviourParser"});
      DSLProxy<ImplicitGenericBehaviourDSL> proxy301(std::vector<std::string>{
          "ImplicitGenericBehaviour", "ImplicitGenericBehaviourParser"});
      DSLProxy<RungeKuttaGenericBehaviourDSL> proxy302(std::vector<std::string>{
          "RungeKuttaGenericBehaviour", "RungeKuttaGenericBehaviourParser"});
      init = true;
    }
  }  // end of initDSLs

}  // end of namespace mfront
