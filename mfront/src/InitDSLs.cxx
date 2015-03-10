/*!
 * \file   mfront/src/InitDSLs.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 sep 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/DSLProxy.hxx"

#include"MFront/DefaultDSL.hxx"
#include"MFront/DefaultCZMDSL.hxx"
#include"MFront/DefaultFiniteStrainDSL.hxx"
#include"MFront/IsotropicMisesCreepDSL.hxx"
#include"MFront/IsotropicMisesPlasticFlowDSL.hxx"
#include"MFront/IsotropicStrainHardeningMisesCreepDSL.hxx"
#include"MFront/MaterialPropertyDSL.hxx"
#include"MFront/MultipleIsotropicMisesFlowsDSL.hxx"
#include"MFront/RungeKuttaDSL.hxx"
#include"MFront/ImplicitDSL.hxx"
#include"MFront/ImplicitDSLII.hxx"
#include"MFront/ImplicitFiniteStrainDSL.hxx"

#include"MFront/ModelDSL.hxx"
#include"MFront/InitDSLs.hxx"

namespace mfront
{

  void
  initParsers(void)
  {
    DSLProxy<DefaultDSL> proxy1("DefaultParser");
    DSLProxy<IsotropicMisesCreepDSL>  proxy2("IsotropicMisesCreepParser");
    DSLProxy<IsotropicMisesPlasticFlowDSL>  proxy3("IsotropicMisesPlasticFlowParser");
    DSLProxy<IsotropicStrainHardeningMisesCreepDSL>  proxy4("IsotropicStrainHardeningMisesCreepParser");
    DSLProxy<MaterialPropertyDSL>  proxy5("MaterialPropertyParser");
    DSLProxy<MultipleIsotropicMisesFlowsDSL>  proxy6("MultipleIsotropicMisesFlowsParser");
    DSLProxy<RungeKuttaDSL>  proxy7("RungeKuttaParser");
    DSLProxy<ImplicitDSL>    proxy8("ImplicitParser");
    DSLProxy<ImplicitDSLII>  proxy9("ImplicitParserII");
    DSLProxy<ModelDSL>       proxy10("ModelParser");
    // CZM
    DSLProxy<DefaultCZMDSL> proxy100("DefaultCZMParser");
    // FiniteStrain
    DSLProxy<DefaultFiniteStrainDSL>  proxy200("DefaultFiniteStrainParser");
    DSLProxy<ImplicitFiniteStrainDSL> proxy201("ImplicitFiniteStrainParser");
  } // end of initParsers 

} // end of namespace mfront
