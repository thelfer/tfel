/*! 
 * \file  MFrontImplicitParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 févr. 2013
 */

#include"MFront/MFrontImplicitParser.hxx"

namespace mfront{

  MFrontImplicitParser::MFrontImplicitParser()
  {
    using namespace std;
    typedef map<string,string>::value_type MVType;
    // Default state variable
    this->registerVariable("eel");
    this->registerVariable("deel");
    this->mb.getStateVariables().push_back(VarHandler("StrainStensor","eel",1u,0u));
    this->glossaryNames.insert(MVType("eel","ElasticStrain"));
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

