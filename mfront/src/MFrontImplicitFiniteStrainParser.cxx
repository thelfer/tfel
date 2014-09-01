/*! 
 * \file  mfront/src/MFrontImplicitFiniteStrainParser.cxx
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

#include"MFront/MFrontImplicitFiniteStrainParser.hxx"

namespace mfront{

  MFrontImplicitFiniteStrainParser::MFrontImplicitFiniteStrainParser()
  {
    // input variables
    this->registerVariable("F0",false);
    this->registerVariable("F1",false);
    this->registerVariable("sig",false);
    this->mb.declareAsAFiniteStrainStandardBehaviour();
  } // end of MFrontImplicitFiniteStrainParser::MFrontImplicitFiniteStrainParser

  std::string
  MFrontImplicitFiniteStrainParser::getName(void)
  {
    return "ImplicitFiniteStrain";
  } // end of MFrontImplicitFiniteStrainParser::getName

  std::string
  MFrontImplicitFiniteStrainParser::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of MFrontImplicitFiniteStrainParser::getDescription

} // end of namespace mfront  
