/*!
 * \file   MFrontModelParserBase.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#ifndef _LIB_MFRONTMODELPARSERBASE_IXX_
#define _LIB_MFRONTMODELPARSERBASE_IXX_ 

#include<sstream>
#include<stdexcept>

namespace mfront{

  template<typename Child>
  void
  MFrontModelParserBase<Child>::registerDefaultCallBacks(void)
  {
    this->registerNewCallBack("@Parser",&Child::treatParser);
    this->registerNewCallBack("@Model",&Child::treatModel);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Author",&Child::treatAuthor);
    this->registerNewCallBack("@Date",&Child::treatDate);
    this->registerNewCallBack("@Includes",&Child::treatIncludes);
    this->registerNewCallBack("@MembersFunc",&Child::treatMembers);
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
    this->registerNewCallBack("@GlobalParameter",
			      &Child::treatGlobalParameter);
    this->registerNewCallBack("@LocalParameter",
			      &Child::treatLocalParameter);
    this->registerNewCallBack("@ConstantMaterialProperty",
			      &Child::treatConstantMaterialProperty);
    this->registerNewCallBack("@Domain",&Child::treatDomain);
    this->registerNewCallBack("@Domains",&Child::treatDomains);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Output",&Child::treatOutput);
    this->registerNewCallBack("@Input",&Child::treatInput);
    this->registerNewCallBack("@Function",&Child::treatFunction);
    this->registerNewCallBack("@MaterialLaw",&Child::treatMaterialLaw);
  } // end of MFrontModelParserBase<Child>::registerDefaultCallBacks()

  template<typename Child>
  void
  MFrontModelParserBase<Child>::getKeywordsList(std::vector<std::string>& k) const
  {
    typename CallBackContainer::const_iterator p;
    for(p=this->callBacks.begin();p!=this->callBacks.end();++p){
      k.push_back(p->first);
    }
  } // end of MFrontModelParserBase<Child>::getKeywordsList


  template<typename Child>
  void
  MFrontModelParserBase<Child>::treatFile(const std::string& fileName_,
					  const std::vector<std::string>& ecmds) 
  {
    this->analyseFile(fileName_,ecmds);
    this->writeOutputFiles();
  } // end of Child::treatFile

  template<typename Child>
  void
  MFrontModelParserBase<Child>::analyseFile(const std::string& fileName_,
						const std::vector<std::string>& ecmds)
  {
    using namespace std;
    typename CallBackContainer::const_iterator p;
    typename VarContainer::const_iterator p2;
    MemberFuncPtr handler = 0;
    this->fileName = fileName_;
    this->openFile(this->fileName,ecmds);
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
	for(p2=this->globalParameters.begin();(p2!=this->globalParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatGlobalParameterMethod;
	  } else {
	    ++p2;
	  }
	}
	for(p2=this->localParameters.begin();(p2!=this->localParameters.end())&&(!found);){
	  if(p2->name==this->current->value){
	    found = true;
	    this->currentVar = this->current->value;
	    handler = &Child::treatLocalParameterMethod;
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
      ++(this->current);
      (static_cast<Child*>(this)->*handler)();
    }
  } // end of MFrontModelParserBase<Child>::analyseFile

  template<typename Child>
  void MFrontModelParserBase<Child>::registerNewCallBack(const std::string& keyword,
							    const MemberFuncPtr f)
  {
    using namespace std;
    this->callBacks.insert(make_pair(keyword,f));
    this->registredKeyWords.insert(keyword);
  } // end of registerCallBacks

  template<typename Child>
  MFrontModelParserBase<Child>::MFrontModelParserBase()
  {
    this->registerDefaultCallBacks();
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTMODELPARSERBASE_IXX */

