/*!
 * \file   MFrontBehaviourParserBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 nov 2006
 */

#ifndef _LIB_MFRONTPARSERBASE_HXX_
#define _LIB_MFRONTPARSERBASE_HXX_ 

#include"MFront/MFrontBehaviourParserCommon.hxx"

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

    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

    virtual ~MFrontBehaviourParserBase();

    MFrontBehaviourParserBase();

  public:

    virtual void
    analyseFile(const std::string&);

  };

} // end of namespace mfront  

#include"MFront/MFrontBehaviourParserBase.ixx"

#endif /* _LIB_MFRONTBASE_HXX */

