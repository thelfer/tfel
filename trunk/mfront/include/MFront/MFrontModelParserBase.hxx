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
  struct MFrontModelParserBase
    : public MFrontModelParserCommon
  {
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

    virtual void
    treatFile(const std::string&,
	      const std::vector<std::string>&);

  protected:

    typedef void (Child::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;

    MFrontModelParserBase();

    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>&);

    void
    registerDefaultCallBacks(void);

    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);
    
    CallBackContainer callBacks;
    
    std::set<std::string> registredKeyWords;

    bool verboseMode;

  };

} // end of namespace mfront

#include"MFront/MFrontModelParserBase.ixx"

#endif /* _LIB_MFRONTMODELPARSERBASE_HXX */

