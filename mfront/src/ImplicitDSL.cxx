/*! 
 * \file  mfront/src/ImplicitDSL.cxx
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

#include"MFront/ImplicitDSL.hxx"

namespace mfront{

  ImplicitDSL::ImplicitDSL()
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setParserName("Implicit");
    // input variables
    this->registerVariable("eto",false);
    this->registerVariable("deto",false);
    this->registerVariable("sig",false);
    this->mb.declareAsASmallStrainStandardBehaviour();
    // Default state variable
    this->registerVariable("eel",false);
    this->registerVariable("deel",false);
    VariableDescription eel("StrainStensor","eel",1u,0u);
    eel.description = "elastic strain";
    this->mb.addStateVariable(h,eel);
    this->mb.setGlossaryName(h,"eel","ElasticStrain");
  } // end of ImplicitDSL::ImplicitDSL

  std::string
  ImplicitDSL::getName(void)
  {
    return "Implicit";
  } // end of ImplicitDSL::getName

  std::string
  ImplicitDSL::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of ImplicitDSL::getDescription

  ImplicitDSL::~ImplicitDSL()
  {}

} // end of namespace mfront  

