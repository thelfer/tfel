/*!
 * \file   MFrontBehaviourParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSERBASE_HXX_
#define _LIB_MFRONTPARSERBASE_HXX_ 

#include"MFrontBehaviourParserCommon.hxx"

namespace mfront{

  template<typename Child>
  class MFrontBehaviourParserBase
    : public MFrontBehaviourParserCommon
  {

  protected:
    
    typedef void (Child::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;

    CallBackContainer callBacks;

    void registerDefaultCallBacks(void);

    void treatDisabledCallBack(void);

    void registerNewCallBack(const std::string&,const MemberFuncPtr);

    void disableCallBack(const std::string&);

    virtual ~MFrontBehaviourParserBase();

    MFrontBehaviourParserBase();

  public:

    void treatFile(const std::string);

  };

} // end of namespace mfront  

#include"MFrontBehaviourParserBase.ixx"

#endif /* _LIB_MFRONTBASE_HXX */

