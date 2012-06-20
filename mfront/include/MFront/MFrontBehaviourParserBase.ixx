/*!
 * \file   CasFrontBehaviourParserBase.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSERBASE_IXX_
#define _LIB_MFRONTPARSERBASE_IXX_ 

#include<stdexcept>

namespace mfront{

  template<typename Child>
  void MFrontBehaviourParserBase<Child>::registerNewCallBack(const std::string& keyword,
							     const MemberFuncPtr f)
  {
    using namespace std;
    this->callBacks.insert(make_pair(keyword,f));
    this->registredKeyWords.insert(keyword);
  } // end of registerCallBacks


  template<typename Child>
  void
  MFrontBehaviourParserBase<Child>::registerDefaultCallBacks(void)
  {
    this->registerNewCallBack("Unknown keyword handler",&Child::treatUnknownKeyword);
    this->registerNewCallBack("@Parser",&Child::treatParser);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Library",&Child::treatLibrary);
    this->registerNewCallBack("@Behaviour",&Child::treatBehaviour);
    this->registerNewCallBack("@Author",&Child::treatAuthor);
    this->registerNewCallBack("@Date",&Child::treatDate);
    this->registerNewCallBack("@MFront",&Child::treatMFront);
    this->registerNewCallBack("@Link",&Child::treatLink);
    this->registerNewCallBack("@Includes",&Child::treatIncludes);
    this->registerNewCallBack("@Members",&Child::treatMembers);
    this->registerNewCallBack("@Coef",&Child::treatCoef);
    this->registerNewCallBack("@MaterialProperty",&Child::treatCoef);
    this->registerNewCallBack("@LocalVar",&Child::treatLocalVar);
    this->registerNewCallBack("@Parameter",&Child::treatParameter);
    this->registerNewCallBack("@LocalVariable",&Child::treatLocalVar);
    this->registerNewCallBack("@StateVar",&Child::treatStateVariables);
    this->registerNewCallBack("@StateVariable",&Child::treatStateVariables);
    this->registerNewCallBack("@AuxiliaryStateVar",
			      &Child::treatAuxiliaryStateVariables);
    this->registerNewCallBack("@AuxiliaryStateVariable",
			      &Child::treatAuxiliaryStateVariables);
    this->registerNewCallBack("@ExternalStateVar",&Child::treatExternalStateVariables);
    this->registerNewCallBack("@ExternalStateVariable",
			      &Child::treatExternalStateVariables);
    this->registerNewCallBack("@InitLocalVars",&Child::treatInitLocalVars);
    this->registerNewCallBack("@InitLocalVariables",&Child::treatInitLocalVars);
    this->registerNewCallBack("@Integrator",&Child::treatIntegrator);
    this->registerNewCallBack("@Interface",&Child::treatInterface);
    this->registerNewCallBack("@StaticVar",&Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",&Child::treatStaticVar);
    this->registerNewCallBack("@UseQt",&Child::treatUseQt);
    this->registerNewCallBack("@ComputedVar",&Child::treatComputedVar);
    this->registerNewCallBack("@ComputedVariable",&Child::treatComputedVar);
    this->registerNewCallBack("@Description",&Child::treatDescription);
    this->registerNewCallBack("@UnaryLoadingTest",&Child::treatUnaryLoadingTest);
    this->registerNewCallBack("@Bounds",&Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",&Child::treatPhysicalBounds);
    this->registerNewCallBack("@RequireStiffnessTensor",&Child::treatRequireStiffnessTensor);
    this->registerNewCallBack("@RequireThermalExpansionTensor",&Child::treatRequireThermalExpansionTensor);
    this->registerNewCallBack("@OrthotropicBehaviour",&Child::treatOrthotropicBehaviour);
    this->registerNewCallBack("@IsotropicBehaviour",&Child::treatIsotropicBehaviour);
    this->registerNewCallBack("@Private",&Child::treatPrivate);
    this->registerNewCallBack("@Sources",&Child::treatSources);
    this->registerNewCallBack("@UpdateAuxiliaryStateVars",
			      &Child::treatUpdateAuxiliaryStateVars);
    this->registerNewCallBack("@UpdateAuxiliaryStateVariables",
			      &Child::treatUpdateAuxiliaryStateVars);
  } // end of MFrontBehaviourParserBase<Child>::registerDefaultCallBacks

  template<typename Child>
  void MFrontBehaviourParserBase<Child>::treatDisabledCallBack()
  {
    using namespace std;
    --(this->current);
    string msg("The keyword ");
    msg += this->current->value;
    msg += " has been disabled";
    throw(runtime_error(msg));
  } // end of treatDisabledCallBack

  template<typename Child>
  void MFrontBehaviourParserBase<Child>::disableCallBack(const std::string& name)
  {
    typename CallBackContainer::iterator p;
    p=this->callBacks.find(name);
    if(p==this->callBacks.end()){
      this->registerNewCallBack(name,&Child::treatDisabledCallBack);
      return;
    }
    p->second = &Child::treatDisabledCallBack;
  } // end of disableCallBack

  template<typename Child>
  MFrontBehaviourParserBase<Child>::MFrontBehaviourParserBase()
    : MFrontBehaviourParserCommon()
  {
    // Register callbacks
    this->registerDefaultCallBacks();
  } // end of MFrontBehaviourParserBase<Child>::MFrontBehaviourParserBase

  template<typename Child>
  void MFrontBehaviourParserBase<Child>::treatFile(const std::string fileName_)
  {
    using namespace std;

    this->openFile(fileName_);
    this->fileName = fileName_;
    
    typename CallBackContainer::const_iterator p;
    MemberFuncPtr handler;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      if((this->isMaterialPropertyName(this->current->value))||
	 (this->isInternalStateVariableName(this->current->value))||
	 (this->isAuxiliaryInternalStateVariableName(this->current->value))||
	 (this->isExternalStateVariableName(this->current->value))){
	this->treatVariableMethod();
      } else if (this->isParameterName(this->current->value)){
	this->treatParameterMethod();
      } else {
	p = this->callBacks.find(this->current->value);
	if(p==this->callBacks.end()){
	  handler = &Child::treatUnknownKeyword;
	} else {
	  handler = p->second;
	}
	++(this->current);
	((static_cast<Child *>(this))->*handler)();
      }
    }
  } // end of MFrontBehaviourParserBase<Child>::treatFile

  template<typename Child>
  MFrontBehaviourParserBase<Child>::~MFrontBehaviourParserBase()
  {} // end of MFrontBehaviourParserBase<Child>::~MFrontBehaviourParserBase
  
} // end of namespace mfront

#endif /* _LIB_MFRONTPARSERBASE_IXX */

