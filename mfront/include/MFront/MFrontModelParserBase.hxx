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

#include"MFront/ParserBase.hxx"
#include"MFront/MFrontModelParserCommon.hxx"

namespace mfront{

  template<typename Child>
  class MFrontModelParserBase
    : public MFrontModelParserCommon
  {
  protected:
    typedef void (Child::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;

    void
    registerDefaultCallBacks(void);

    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);

    virtual void
    treatFile(const std::string&);
    
    std::set<std::string> registredKeyWords;

    CallBackContainer callBacks;
    
    MFrontModelParserBase();

    bool verboseMode;

  };

} // end of namespace mfront

#include"MFront/MFrontModelParserBase.ixx"

#endif /* _LIB_MFRONTMODELPARSERBASE_HXX */

