/*!
 * \file   MFrontModelParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#ifndef _LIB_MFRONTMODELPARSERBASE_HXX_
#define _LIB_MFRONTMODELPARSERBASE_HXX_ 

#include<string>
#include<map>

#include"ParserBase.hxx"

namespace mfront{

  template<typename Child>
  class MFrontModelParserBase
    : public ParserBase
  {
  protected:
    typedef void (Child::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;
    
    void
    treatModel(void);

    void
    registerDefaultCallBacks(void);

    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);

    void
    treatFile(const std::string&);
    
    std::set<std::string> registredKeyWords;

    CallBackContainer callBacks;
    
    bool verboseMode;

    MFrontModelParserBase();
    
  };

} // end of namespace mfront

#include"MFront/MFrontModelParserBase.ixx"

#endif /* _LIB_MFRONTMODELPARSERBASE_HXX */

