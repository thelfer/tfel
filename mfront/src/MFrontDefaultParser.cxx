/*!
 * \file   MFrontDefaultParser.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultParser.hxx"

namespace mfront{

  MFrontDefaultParser::MFrontDefaultParser()
    : MFrontDefaultParserBase()
  {
    this->defineSmallStrainInputVariables();
  }

  std::string
  MFrontDefaultParser::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of MFrontDefaultParser::getDescription
  
  std::string
  MFrontDefaultParser::getName(void)
  {
    return "DefaultParser";
  }

  MFrontDefaultParser::~MFrontDefaultParser()
  {} // end of MFrontDefaultParser::~MFrontDefaultParser

} // end of namespace mfront  
