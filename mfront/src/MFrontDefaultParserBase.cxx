/*!
 * \file   mfront/src/MFrontDefaultParserBase.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   22 jullet 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
    this->registerNewCallBack("@MaterialLaw",
			      &MFrontDefaultParserBase::treatMaterialLaw);
    this->registerNewCallBack("@TangentOperator",
			      &MFrontDefaultParserBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
			      &MFrontDefaultParserBase::treatIsTangentOperatorSymmetric);
  }

  void
  MFrontDefaultParserBase::writeBehaviourParserSpecificIncludes()
  {
    MFrontBehaviourParserCommon::writeBehaviourParserSpecificIncludes();
  } // end of MFrontDefaultParserBase::writeBehaviourParserSpecificIncludes

  void
  MFrontDefaultParserBase::treatProvidesTangentOperator()
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    this->readSpecifiedToken("MFrontBehaviourParserBaseCommon::"
			     "treatProvidesTangentOperator",";");
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasAttribute(*ph,MechanicalBehaviourData::hasConsistentTangentOperator)){
	this->throwRuntimeError("MFrontDefaultParserBase::treatProvidesTangentOperator",
				"one of the key @ProvidesTangentOperator "
				"or @ProvidesSymmetricTangentOperator "
				"has already been called");
      }
      this->mb.setAttribute(*ph,MechanicalBehaviourData::hasConsistentTangentOperator,true);
    }
  } // end of MFrontDefaultParserBase::treatProvidesTangentOperator

  void
  MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator()
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    this->checkNotEndOfFile("MFrontBehaviourParserBaseCommon::"
			    "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken("MFrontBehaviourParserBaseCommon::"
			     "treatProvidesSymmetricTangentOperator",";");
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasAttribute(*ph,MechanicalBehaviourData::hasConsistentTangentOperator)){
	this->throwRuntimeError("MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator",
				"one of the key @ProvidesTangentOperator "
				"or @ProvidesSymmetricTangentOperator "
				"has already been called");
      }
      this->mb.setAttribute(*ph,MechanicalBehaviourData::hasConsistentTangentOperator,true);
      this->mb.setAttribute(*ph,MechanicalBehaviourData::isConsistentTangentOperatorSymmetric,true);
    }
  } // end of MFrontDefaultParserBase::treatProvidesSymmetricTangentOperator

  MFrontDefaultParserBase::~MFrontDefaultParserBase()
  {} // end of MFrontDefaultParserBase::~MFrontDefaultParserBase

} // end of namespace mfront  
