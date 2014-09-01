/*! 
 * \file  mfront/src/MFrontImplicitParser.cxx
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

#include"MFront/MFrontImplicitParser.hxx"

namespace mfront{

  MFrontImplicitParser::MFrontImplicitParser()
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
  } // end of MFrontImplicitParser::MFrontImplicitParser

  std::string
  MFrontImplicitParser::getName(void)
  {
    return "Implicit";
  } // end of MFrontImplicitParser::getName

  std::string
  MFrontImplicitParser::getDescription(void)
  {
    return "this parser provides a generic integrator based on a theta method.";
  } // end of MFrontImplicitParser::getDescription

} // end of namespace mfront  

