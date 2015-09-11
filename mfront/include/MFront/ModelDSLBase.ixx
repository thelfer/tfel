/*!
 * \file   mfront/include/MFront/ModelDSLBase.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTMODELPARSERBASE_IXX_
#define LIB_MFRONTMODELPARSERBASE_IXX_ 

#include<sstream>
#include<stdexcept>
#include"MFront/AbstractModelInterface.hxx"

namespace mfront{

  template<typename Child>
  void
  ModelDSLBase<Child>::registerDefaultCallBacks(void)
  {
    this->registerNewCallBack("@Parser",&Child::treatParser);
    this->registerNewCallBack("@Model",&Child::treatModel);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Author",&Child::treatAuthor);
    this->registerNewCallBack("@Date",&Child::treatDate);
    this->registerNewCallBack("@Includes",&Child::treatIncludes);
    this->registerNewCallBack("@StaticVar",&Child::treatStaticVar);
    this->registerNewCallBack("@StaticVariable",&Child::treatStaticVar);
    this->registerNewCallBack("@Description",&Child::treatDescription);
    this->registerNewCallBack("@Import",&Child::treatImport);
    this->registerNewCallBack("@Bounds",
			      &Child::treatBounds);
    this->registerNewCallBack("@PhysicalBounds",
			      &Child::treatPhysicalBounds);
    this->registerNewCallBack("@ConstantMaterialProperty",
			      &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Parameter",
			      &Child::treatParameter);
    this->registerNewCallBack("@ConstantMaterialProperty",
			      &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Domain",&Child::treatDomain);
    this->registerNewCallBack("@Domains",&Child::treatDomains);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Output",&Child::treatOutput);
    this->registerNewCallBack("@Input",&Child::treatInput);
    this->registerNewCallBack("@Function",&Child::treatFunction);
    this->registerNewCallBack("@MaterialLaw",&Child::treatMaterialLaw);
  } // end of ModelDSLBase<Child>::registerDefaultCallBacks()

  template<typename Child>
  void
  ModelDSLBase<Child>::getKeywordsList(std::vector<std::string>& k) const
  {
    for(auto c :this->callBacks){
      k.push_back(c.first);
    }
  } // end of ModelDSLBase<Child>::getKeywordsList

  template<typename Child>
  void
  ModelDSLBase<Child>::importFile(const std::string& fileName_,
				   const std::vector<std::string>& ecmds)
  {
    this->fileName = fileName_;
    this->openFile(this->fileName,ecmds);
    this->analyse();
  } // end of ModelDSLBase<Child>::importFile
    
  template<typename Child>
  void
  ModelDSLBase<Child>::analyseString(const std::string& s)
  {
    this->fileName = "user defined string";
    this->parseString(s);
    this->analyse();
  } // end of ModelDSLBase<Child>::analyseString

  template<typename Child>
  void
  ModelDSLBase<Child>::analyse()
  {
    using namespace std;
    typename CallBackContainer::const_iterator p;
    typename VariableDescriptionContainer::const_iterator p2;
    MemberFuncPtr handler = nullptr;
    // strip comments from file
    this->stripComments();
    // begin treatement
    this->current = this->fileTokens.begin();
    while(this->current != this->fileTokens.end()){
      p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	bool found = false;
	for(p2=this->outputs.begin();(p2!=this->outputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatOutputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->inputs.begin();(p2!=this->inputs.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatInputMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->parameters.begin();(p2!=this->parameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->constantMaterialProperties.begin();(p2!=this->constantMaterialProperties.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatConstantMaterialPropertyMethod;
	  } else {
	    ++p2;
	  }
	}
	if(!found){
	  handler = &Child::treatUnknownKeyword;
	}
      } else {
	handler = p->second;
      }
      this->currentComment = this->current->comment;
      ++(this->current);
      try{
	(static_cast<Child*>(this)->*handler)();
      } catch(...){
	this->currentComment.clear();
	throw;
      }
      this->currentComment.clear();
    }
  } // end of ModelDSLBase<Child>::analyse

  template<typename Child>
  void
  ModelDSLBase<Child>::analyseFile(const std::string& fileName_,
				   const std::vector<std::string>& ecmds)
  {
    this->importFile(fileName_,ecmds);
    for(const auto & i : this->interfaces){
      i.second->getTargetsDescription(this->td,*this);
    }
    for(const auto& l : this->td){
      for(const auto& deps : this->librariesDependencies){
	if("-l"+l.name!=deps){
	  this->td[l.name].ldflags.push_back(deps);
	}
      }
    }
  }

  template<typename Child>
  void ModelDSLBase<Child>::registerNewCallBack(const std::string& keyword,
							    const MemberFuncPtr f)
  {
    using namespace std;
    this->callBacks.insert(make_pair(keyword,f));
    this->registredKeyWords.insert(keyword);
  } // end of registerCallBacks

  template<typename Child>
  ModelDSLBase<Child>::ModelDSLBase()
  {
    this->registerDefaultCallBacks();
  }

} // end of namespace mfront

#endif /* LIB_MFRONTMODELPARSERBASE_IXX_ */

