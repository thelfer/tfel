/*! 
 * \file  mfront/src/ImplicitFiniteStrainDSL.cxx
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

#include"MFront/ImplicitFiniteStrainDSL.hxx"

namespace mfront{

  ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL()
  {
    this->mb.setParserName("ImplicitFiniteStrain");
    // input variables
    this->registerVariable("F0",false);
    this->registerVariable("F1",false);
    this->registerVariable("sig",false);
    this->mb.declareAsAFiniteStrainStandardBehaviour();
  } // end of ImplicitFiniteStrainDSL::ImplicitFiniteStrainDSL

  std::string
  ImplicitFiniteStrainDSL::getName(void)
  {
    return "ImplicitFiniteStrain";
  } // end of ImplicitFiniteStrainDSL::getName

  std::string
  ImplicitFiniteStrainDSL::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of ImplicitFiniteStrainDSL::getDescription

  ImplicitFiniteStrainDSL::~ImplicitFiniteStrainDSL() noexcept
  {}

} // end of namespace mfront  
