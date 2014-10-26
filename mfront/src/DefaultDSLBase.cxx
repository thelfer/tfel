/*!
 * \file   mfront/src/DefaultDSLBase.cxx
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

#include"MFront/AbstractBehaviourInterface.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/DefaultDSLBase.hxx"

namespace mfront{

  DefaultDSLBase::DefaultDSLBase()
    : BehaviourDSLBase<DefaultDSLBase>()
  {
    this->registerNewCallBack("@ProvidesTangentOperator",
			      &DefaultDSLBase::treatProvidesTangentOperator);
    this->registerNewCallBack("@ProvidesSymmetricTangentOperator",
			      &DefaultDSLBase::treatProvidesSymmetricTangentOperator);
    this->registerNewCallBack("@MaterialLaw",
			      &DefaultDSLBase::treatMaterialLaw);
    this->registerNewCallBack("@TangentOperator",
			      &DefaultDSLBase::treatTangentOperator);
    this->registerNewCallBack("@IsTangentOperatorSymmetric",
			      &DefaultDSLBase::treatIsTangentOperatorSymmetric);
  }

  void
  DefaultDSLBase::writeBehaviourParserSpecificIncludes()
  {
    BehaviourDSLCommon::writeBehaviourParserSpecificIncludes();
  } // end of DefaultDSLBase::writeBehaviourParserSpecificIncludes

  void
  DefaultDSLBase::treatProvidesTangentOperator()
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    this->readSpecifiedToken("BehaviourDSLBaseCommon::"
			     "treatProvidesTangentOperator",";");
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasAttribute(*ph,BehaviourData::hasConsistentTangentOperator)){
	this->throwRuntimeError("DefaultDSLBase::treatProvidesTangentOperator",
				"one of the key @ProvidesTangentOperator "
				"or @ProvidesSymmetricTangentOperator "
				"has already been called");
      }
      this->mb.setAttribute(*ph,BehaviourData::hasConsistentTangentOperator,true);
    }
  } // end of DefaultDSLBase::treatProvidesTangentOperator

  void
  DefaultDSLBase::treatProvidesSymmetricTangentOperator()
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    this->checkNotEndOfFile("BehaviourDSLBaseCommon::"
			    "treatProvidesSymmetricTangentOperator");
    this->readSpecifiedToken("BehaviourDSLBaseCommon::"
			     "treatProvidesSymmetricTangentOperator",";");
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasAttribute(*ph,BehaviourData::hasConsistentTangentOperator)){
	this->throwRuntimeError("DefaultDSLBase::treatProvidesSymmetricTangentOperator",
				"one of the key @ProvidesTangentOperator "
				"or @ProvidesSymmetricTangentOperator "
				"has already been called");
      }
      this->mb.setAttribute(*ph,BehaviourData::hasConsistentTangentOperator,true);
      this->mb.setAttribute(*ph,BehaviourData::isConsistentTangentOperatorSymmetric,true);
    }
  } // end of DefaultDSLBase::treatProvidesSymmetricTangentOperator

  DefaultDSLBase::~DefaultDSLBase()
  {} // end of DefaultDSLBase::~DefaultDSLBase

} // end of namespace mfront  
