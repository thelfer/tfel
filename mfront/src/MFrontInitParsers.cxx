/*!
 * \file   mfront/src/MFrontInitParsers.cxx
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

#include"MFront/MFrontParserProxy.hxx"

#include"MFront/MFrontDefaultParser.hxx"
#include"MFront/MFrontDefaultCZMParser.hxx"
#include"MFront/MFrontDefaultFiniteStrainParser.hxx"
#include"MFront/MFrontIsotropicMisesCreepParser.hxx"
#include"MFront/MFrontIsotropicMisesPlasticFlowParser.hxx"
#include"MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx"
#include"MFront/MFrontMaterialLawParser.hxx"
#include"MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx"
#include"MFront/MFrontRungeKuttaParser.hxx"
#include"MFront/MFrontImplicitParser.hxx"
#include"MFront/MFrontImplicitParserII.hxx"
#include"MFront/MFrontImplicitFiniteStrainParser.hxx"

#include"MFront/MFrontModelParser.hxx"

namespace mfront
{

  void
  initParsers(void)
  {
    MFrontParserProxy<MFrontDefaultParser> proxy1;
    MFrontParserProxy<MFrontIsotropicMisesCreepParser>  proxy2;
    MFrontParserProxy<MFrontIsotropicMisesPlasticFlowParser>  proxy3;
    MFrontParserProxy<MFrontIsotropicStrainHardeningMisesCreepParser>  proxy4;
    MFrontParserProxy<MFrontMaterialLawParser>  proxy5;
    MFrontParserProxy<MFrontMultipleIsotropicMisesFlowsParser>  proxy6;
    MFrontParserProxy<MFrontRungeKuttaParser>  proxy7;
    MFrontParserProxy<MFrontImplicitParser>    proxy8;
    MFrontParserProxy<MFrontImplicitParserII>  proxy9;
    MFrontParserProxy<MFrontModelParser>       proxy10;
    // CZM
    MFrontParserProxy<MFrontDefaultCZMParser> proxy100;
    // FiniteStrain
    MFrontParserProxy<MFrontDefaultFiniteStrainParser>  proxy200;
    MFrontParserProxy<MFrontImplicitFiniteStrainParser> proxy201;
  } // end of initParsers 

} // end of namespace mfront
