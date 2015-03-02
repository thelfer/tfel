/*!
 * \file   mfront/include/MFront/ModelDSLBase.hxx
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

#ifndef LIB_MFRONTMODELPARSERBASE_HXX_
#define LIB_MFRONTMODELPARSERBASE_HXX_ 

#include<string>
#include<map>

#include"MFront/DSLBase.hxx"
#include"MFront/ModelDSLCommon.hxx"

namespace mfront{

  template<typename Child>
  struct ModelDSLBase
    : public ModelDSLCommon
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

    ModelDSLBase();

    virtual void
    analyseFile(const std::string&,
		const std::vector<std::string>&);

    void
    registerDefaultCallBacks(void);

    void
    registerNewCallBack(const std::string&,const MemberFuncPtr);
    
    CallBackContainer callBacks;
    
    std::set<std::string> registredKeyWords;

  };

} // end of namespace mfront

#include"MFront/ModelDSLBase.ixx"

#endif /* LIB_MFRONTMODELPARSERBASE_HXX_ */

