/*! 
 * \file  MFrontImplicitParserII.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 */

#include"MFront/MFrontImplicitParserII.hxx"

namespace mfront{

  MFrontImplicitParserII::MFrontImplicitParserII()
  {
    // input variables
    this->registerVariable("eto");
    this->registerVariable("deto");
    this->registerVariable("sig");
    this->mb.declareAsASmallStrainStandardBehaviour();
  } // end of MFrontImplicitParserII::MFrontImplicitParserII

  std::string
  MFrontImplicitParserII::getName(void)
  {
    return "ImplicitII";
  } // end of MFrontImplicitParserII::getName

  std::string
  MFrontImplicitParserII::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of MFrontImplicitParserII::getDescription

} // end of namespace mfront  
