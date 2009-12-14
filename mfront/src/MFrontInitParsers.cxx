/*!
 * \file   MFrontInitParsers.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 sep 2008
 */

#include"MFrontParserProxy.hxx"

#include"MFrontDefaultParser.hxx"
#include"MFrontIsotropicMisesCreepParser.hxx"
#include"MFrontIsotropicMisesPlasticFlowParser.hxx"
#include"MFrontIsotropicStrainHardeningMisesCreepParser.hxx"
#include"MFrontMaterialLawParser.hxx"
#include"MFrontMultipleIsotropicMisesFlowsParser.hxx"
#include"MFrontRungeKuttaParser.hxx"
#include"MFrontImplicitParser.hxx"

#ifdef HAVE_PLEIADES
#include"MFrontPleiadesModelParser.hxx"
#include"MFrontPleiadesModelParserArchitecture1.4.hxx"
#endif /* HAVE_PLEIADES */

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
    MFrontParserProxy<MFrontImplicitParser>  proxy8;
#ifdef HAVE_PLEIADES
    MFrontParserProxy<MFrontPleiadesModelParserArchitecture1_4> proxy9;
    MFrontParserProxy<MFrontPleiadesModelParser> proxy10;
#endif /* HAVE_PLEIADES */
  } // end of initParsers 

} // end of namespace mfront
