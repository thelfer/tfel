/*! 
 * \file  mfront/src/MFrontImplicitFiniteStrainParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
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
