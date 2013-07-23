/*!
 * \file   MFrontDefaultCZMParser.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultCZMParser.hxx"

namespace mfront{

  MFrontDefaultCZMParser::MFrontDefaultCZMParser()
    : MFrontDefaultParserBase()
  {
    this->defineCZMInputVariables();
  }

  std::string
  MFrontDefaultCZMParser::getDescription(void)
  {
    return "this parser is the most generic one as it does not make any restriction "
           "on the behaviour or the integration method that may be used";
  } // end of MFrontDefaultCZMParser::getDescription
  
  std::string
  MFrontDefaultCZMParser::getName(void)
  {
    return "DefaultCZMParser";
  }

  MFrontDefaultCZMParser::~MFrontDefaultCZMParser()
  {} // end of MFrontDefaultCZMParser::~MFrontDefaultCZMParser

} // end of namespace mfront  
