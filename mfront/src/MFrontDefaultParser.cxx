/*!
 * \file   MFrontBase.cxx
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
    : MFrontBehaviourParserBase<MFrontDefaultParser>()
  {
    this->registerNewCallBack("@ProvidesTangentOperator",
			      &MFrontDefaultParser::treatProvidesTangentOperator);
    this->registerNewCallBack("@ProvidesSymmetricTangentOperator",
			      &MFrontDefaultParser::treatProvidesSymmetricTangentOperator);
    this->defineSmallStrainInputVariables();
  }

  void
  MFrontDefaultParser::treatProvidesTangentOperator()
  {
    using namespace std;
    if(this->hasConsistantTangentOperator){
      string msg("MFrontDefaultParser::treatProvidesTangentOperator : ");
      msg += "one of the key @ProvidesTangentOperator or "
	"@ProvidesSymmetricTangentOperator ";
      msg += "has already been called";
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::"
			    "treatProvidesTangentOperator");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::"
			     "treatProvidesTangentOperator",";");
    this->hasConsistantTangentOperator = true;
  } // end of MFrontDefaultParser::treatProvidesTangentOperator

  void
  MFrontDefaultParser::treatProvidesSymmetricTangentOperator()
  {
    using namespace std;
    if(this->hasConsistantTangentOperator){
      string msg("MFrontDefaultParser::"
		 "treatProvidesSymmetricTangentOperator : ");
      msg += "one of the key @ProvidesTangentOperator "
	"or @ProvidesSymmetricTangentOperator ";
      msg += "has already been called";
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::"
			    "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::"
			     "treatProvidesSymmetricTangentOperator",";");
    this->hasConsistantTangentOperator = true;
    this->isConsistantTangentOperatorSymmetric = true;
  } // end of MFrontDefaultParser::treatProvidesSymmetricTangentOperator

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
