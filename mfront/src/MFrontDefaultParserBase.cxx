/*!
 * \file   MFrontDefaultParserBase.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   22 jullet 2013
 */

#include<string>
#include<fstream>
#include<stdexcept>

#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontDefaultParserBase.hxx"

namespace mfront{

  MFrontDefaultParserBase::MFrontDefaultParserBase()
    : MFrontBehaviourParserBase<MFrontDefaultParserBase>()
  {
    this->registerNewCallBack("@ProvidesTangentOperator",
			      &MFrontDefaultParserBase::treatProvidesTangentOperator);
    this->registerNewCallBack("@ProvidesSymmetricTangentOperator",
			      &MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator);
  }

  void
  MFrontDefaultParserBase::treatProvidesTangentOperator()
  {
    using namespace std;
    if(this->hasConsistantTangentOperator){
      string msg("MFrontDefaultParserBase::treatProvidesTangentOperator : ");
      msg += "one of the key @ProvidesTangentOperator or "
	"@ProvidesSymmetricTangentOperator ";
      msg += "has already been called";
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserBaseCommon::"
			    "treatProvidesTangentOperator");
    this->readSpecifiedToken("MFrontBehaviourParserBaseCommon::"
			     "treatProvidesTangentOperator",";");
    this->hasConsistantTangentOperator = true;
  } // end of MFrontDefaultParserBase::treatProvidesTangentOperator

  void
  MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator()
  {
    using namespace std;
    if(this->hasConsistantTangentOperator){
      string msg("MFrontDefaultParserBase::"
		 "treatProvidesSymmetricTangentOperator : ");
      msg += "one of the key @ProvidesTangentOperator "
	"or @ProvidesSymmetricTangentOperator ";
      msg += "has already been called";
      throw(runtime_error(msg));
    }
    this->checkNotEndOfFile("MFrontBehaviourParserBaseCommon::"
			    "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken("MFrontBehaviourParserBaseCommon::"
			     "treatProvidesSymmetricTangentOperator",";");
    this->hasConsistantTangentOperator = true;
    this->isConsistantTangentOperatorSymmetric = true;
  } // end of MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator

  MFrontDefaultParserBase::~MFrontDefaultParserBase()
  {} // end of MFrontDefaultParserBase::~MFrontDefaultParserBase

} // end of namespace mfront  
