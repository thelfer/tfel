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

namespace mfront{

  template<typename Child>
  void MFrontModelParserBase<Child>::treatFile(const std::string& fileName_)
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
      p = this->callBacks.find(this->current->value);
      if(p==this->callBacks.end()){
	handler = &Child::treatUnknownKeyword;
      } else {
	handler = p->second;
      }
      ++(this->current);
      ((static_cast<Child *>(this))->*handler)();
    }
  } // end of class MFrontModelParserBase<Child>::treatFile(const std::string& fileName_)

  template<typename Child>
  void MFrontModelParserBase<Child>::treatModel(void)
  {
    using namespace std;
    this->className = this->readOnlyOneToken();
    if(!isValidIdentifier(this->className)){
      ostringstream msg;
      msg << "Given model name is not valid : "
	  <<  this->current->value
	  <<  "\nError at line : " << this->current->line << "\n";
      throw(runtime_error(msg.str()));
    }
  } // end of MFrontModelParserBase<Child>::treatModel

  template<typename Child>
  void MFrontModelParserBase<Child>::registerDefaultCallBacks(void)
  {
    this->registerNewCallBack("Unknown keyword handler",&Child::treatUnknownKeyword);
    this->registerNewCallBack("@Parser",&Child::treatParser);
    this->registerNewCallBack("@Model",&Child::treatModel);
    this->registerNewCallBack("@Material",&Child::treatMaterial);
    this->registerNewCallBack("@Author",&Child::treatAuthor);
    this->registerNewCallBack("@Date",&Child::treatDate);
    this->registerNewCallBack("@Includes",&Child::treatIncludes);
    this->registerNewCallBack("@MembersFunc",&Child::treatMembers);
    this->registerNewCallBack("@StaticVar",&Child::treatStaticVar);
    this->registerNewCallBack("@Description",&Child::treatDescription);
    this->registerNewCallBack("@LocalVar",&Child::treatLocalVar);
    this->registerNewCallBack("@InitLocalVars",&Child::treatInitLocalVars);
  } // end of MFrontModelParserBase<Child>::registerDefaultCallBacks()

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

