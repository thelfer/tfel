/*!
 * \file   mfront/include/MFront/MFrontBehaviourAnalyserFactory.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   19 may 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_
#define _LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/MFrontBehaviourAnalyser.hxx"

namespace mfront{

  struct TFEL_VISIBILITY_EXPORT MFrontBehaviourAnalyserFactory
  {
    typedef MFrontBehaviourAnalyser* (*AnalyserCreator)(void);
    
    static MFrontBehaviourAnalyserFactory&
    getMFrontBehaviourAnalyserFactory();

    std::vector<std::string>
    getRegistredAnalysers(void);

    void registerAnalyserCreator(const std::string&,
				 AnalyserCreator);
    
    void
    registerAnalyserAlias(const std::string&,
			  const std::string&);

    MFrontBehaviourAnalyser* 
    getAnalyserPtr(const std::string&);
    
    void
    reset(void);

    void
    clear(void);

    ~MFrontBehaviourAnalyserFactory();
    
  private:

    typedef std::map<std::string,std::string> AliasContainer;
    typedef std::map<std::string,AnalyserCreator> AnalyserCreatorsContainer;
    typedef std::map<std::string,MFrontBehaviourAnalyser *> AnalyserContainer;

    TFEL_VISIBILITY_LOCAL
    MFrontBehaviourAnalyserFactory();

    TFEL_VISIBILITY_LOCAL
    AnalyserCreatorsContainer&
    getAnalyserCreatorsMap(void);

    TFEL_VISIBILITY_LOCAL
    AnalyserContainer&
    getAnalysersMap(void);

    TFEL_VISIBILITY_LOCAL
    AliasContainer&
    getAliasesMap(void);

  };

} // end of namespace mfront  

#endif /* _LIB_MFRONTBEHAVIOURANALYSERFACTORY_HXX */

