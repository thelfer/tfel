/*!
 * \file   mfront/include/MFront/MFrontBehaviourParserBase.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSERBASE_IXX_
#define _LIB_MFRONTPARSERBASE_IXX_ 

#include<stdexcept>
#include<algorithm>

#include"MFront/MFrontLogStream.hxx"

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
    this->registerNewCallBack("@Parser",&Child::treatParser);
    this->registerNewCallBack("@ModellingHypothesis",
			      &Child::treatModellingHypothesis);
    this->registerNewCallBack("@ModellingHypotheses",
			      &Child::treatModellingHypotheses);
    this->registerNewCallBack("@Import",&Child::treatImport);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Library",&Child::treatLibrary);
    this->registerNewCallBack("@Profiling",
			      &Child::treatProfiling);
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
    this->registerNewCallBack("@LocalVariable",&Child::treatLocalVar);
    this->registerNewCallBack("@Parameter",&Child::treatParameter);
    this->registerNewCallBack("@StateVar",&Child::treatStateVariable);
    this->registerNewCallBack("@StateVariable",&Child::treatStateVariable);
    this->registerNewCallBack("@AuxiliaryStateVar",
			      &Child::treatAuxiliaryStateVariable);
    this->registerNewCallBack("@AuxiliaryStateVariable",
			      &Child::treatAuxiliaryStateVariable);
    this->registerNewCallBack("@ExternalStateVar",&Child::treatExternalStateVariable);
    this->registerNewCallBack("@ExternalStateVariable",
			      &Child::treatExternalStateVariable);
    this->registerNewCallBack("@InitLocalVars",
			      &Child::treatInitLocalVariables);
    this->registerNewCallBack("@InitLocalVariables",
			      &Child::treatInitLocalVariables);
    this->registerNewCallBack("@Integrator",&Child::treatIntegrator);
    this->registerNewCallBack("@Interface",&Child::treatInterface);
    this->registerNewCallBack("@StaticVar",&Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",&Child::treatStaticVar);
    this->registerNewCallBack("@IntegerConstant",
			      &Child::treatIntegerConstant);
    this->registerNewCallBack("@UseQt",&Child::treatUseQt);
    this->registerNewCallBack("@Description",&Child::treatDescription);
    this->registerNewCallBack("@Bounds",&Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",&Child::treatPhysicalBounds);
    this->registerNewCallBack("@RequireStiffnessOperator",
			      &Child::treatRequireStiffnessOperator);
    this->registerNewCallBack("@RequireStiffnessTensor",
			      &Child::treatRequireStiffnessTensor);
    this->registerNewCallBack("@RequireThermalExpansionCoefficientTensor",
			      &Child::treatRequireThermalExpansionCoefficientTensor);
    this->registerNewCallBack("@OrthotropicBehaviour",&Child::treatOrthotropicBehaviour);
    this->registerNewCallBack("@IsotropicElasticBehaviour",&Child::treatIsotropicElasticBehaviour);
    this->registerNewCallBack("@IsotropicBehaviour",&Child::treatIsotropicBehaviour);
    this->registerNewCallBack("@PredictionOperator",&Child::treatPredictionOperator);
    this->registerNewCallBack("@Private",&Child::treatPrivate);
    this->registerNewCallBack("@Sources",&Child::treatSources);
    this->registerNewCallBack("@UpdateAuxiliaryStateVars",
			      &Child::treatUpdateAuxiliaryStateVariables);
    this->registerNewCallBack("@UpdateAuxiliaryStateVariables",
			      &Child::treatUpdateAuxiliaryStateVariables);
    this->registerNewCallBack("@ComputeThermalExpansion",
			      &Child::treatComputeThermalExpansion);
  } // end of MFrontBehaviourParserBase<Child>::registerDefaultCallBacks

  template<typename Child>
  void
  MFrontBehaviourParserBase<Child>::getKeywordsList(std::vector<std::string>& k) const
  {
    typename CallBackContainer::const_iterator p;
    for(p=this->callBacks.begin();p!=this->callBacks.end();++p){
      k.push_back(p->first);
    }
  } // end of MFrontBehaviourParserBase<Child>::getKeywordsList

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
  void MFrontBehaviourParserBase<Child>::treatFile(const std::string& fileName_,
						   const std::vector<std::string>& ecmds)
  {
    this->analyseFile(fileName_,ecmds);
    this->writeOutputFiles();
  } // end of MFrontBehaviourParserBase<Child>::treatFile

  template<typename Child>
  void
  MFrontBehaviourParserBase<Child>::analyseFile(const std::string& fileName_,
						const std::vector<std::string>& ecmds)
  {
    using namespace std;
    typename CallBackContainer::const_iterator p;
    const vector<Hypothesis>& mh = ModellingHypothesis::getModellingHypotheses();
    vector<string> hn(mh.size());
    vector<Hypothesis>::const_iterator pmh;
    vector<string>::iterator phn;      
    for(pmh=mh.begin(),phn=hn.begin();pmh!=mh.end();++pmh,++phn){
      *phn = ModellingHypothesis::toString(*pmh);
    }
    MemberFuncPtr handler;
    // opening file
    this->fileName = fileName_;
    this->openFile(this->fileName,ecmds);
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      if(find(hn.begin(),hn.end(),this->current->value)!=hn.end()){
	const Hypothesis h = ModellingHypothesis::fromString(this->current->value);
	++(this->current);
	this->checkNotEndOfFile("MFrontBehaviourParserBase<Child>::analyseFile");
	this->readSpecifiedToken("MFrontBehaviourParserBase<Child>::analyseFile","::");
	if(!this->isCallableVariable(h,this->current->value)){
	  string msg("MFrontBehaviourParserBase<Child>::analyseFile : "
		     "no variable named '"+this->current->value+"' "
		     "for hypothesis '"+ModellingHypothesis::toString(h)+"'");
	  throw(runtime_error(msg));
	}
	if(this->mb.isParameterName(h,this->current->value)){
	  this->treatParameterMethod(h);
	} else {
	  this->treatVariableMethod(h);
	}
      } else { 
	const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	if(this->isCallableVariable(h,this->current->value)){
	  if(this->mb.isParameterName(h,this->current->value)){
	    this->treatParameterMethod(h);
	  } else {
	    this->treatVariableMethod(h);
	  }
	} else {
	  p = this->callBacks.find(this->current->value);
	  if(p==this->callBacks.end()){
	    if(getVerboseMode()>=VERBOSE_DEBUG){
	      ostream& log = getLogStream();
	      log << "treating unknown keyword" << endl;
	    }
	    handler = &Child::treatUnknownKeyword;
	  } else {
	    if(getVerboseMode()>=VERBOSE_DEBUG){
	      ostream& log = getLogStream();
	      log << "treating keyword : " << this->current->value << endl;
	    }
	    handler = p->second;
	  }
	  this->currentComment = this->current->comment;
	  ++(this->current);
	  try{
	    ((static_cast<Child *>(this))->*handler)();
	  } catch(...){
	    this->currentComment.clear();
	    throw;
	  }
	  this->currentComment.clear();
	}
      }
    }
  } // end of MFrontBehaviourParserBase<Child>::analyseFile

  template<typename Child>
  MFrontBehaviourParserBase<Child>::~MFrontBehaviourParserBase()
  {} // end of MFrontBehaviourParserBase<Child>::~MFrontBehaviourParserBase
  
} // end of namespace mfront

#endif /* _LIB_MFRONTPARSERBASE_IXX */

