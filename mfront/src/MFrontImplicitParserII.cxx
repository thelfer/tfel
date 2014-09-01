/*! 
 * \file  mfront/src/MFrontImplicitParserII.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"MFront/MFrontImplicitParserII.hxx"

namespace mfront{

  MFrontImplicitParserII::MFrontImplicitParserII()
  {
    // input variables
    this->registerVariable("eto",false);
    this->registerVariable("deto",false);
    this->registerVariable("sig",false);
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
