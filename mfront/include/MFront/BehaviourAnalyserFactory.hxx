/*!
 * \file   mfront/include/MFront/BehaviourAnalyserFactory.hxx
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

#ifndef LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_
#define LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_ 

#include<vector>
#include<string>
#include<map>

#include"MFront/MFrontConfig.hxx"
#include"MFront/BehaviourAnalyser.hxx"

namespace mfront{

  struct MFRONT_VISIBILITY_EXPORT BehaviourAnalyserFactory
  {
    typedef BehaviourAnalyser* (*AnalyserCreator)(void);
    
    static BehaviourAnalyserFactory&
    getBehaviourAnalyserFactory();

    std::vector<std::string>
    getRegistredAnalysers(void);

    void registerAnalyserCreator(const std::string&,
				 AnalyserCreator);
    
    void
    registerAnalyserAlias(const std::string&,
			  const std::string&);

    BehaviourAnalyser* 
    getAnalyserPtr(const std::string&);
    //! destructor
    ~BehaviourAnalyserFactory();
    
  private:

    typedef std::map<std::string,std::string> AliasContainer;
    typedef std::map<std::string,AnalyserCreator> AnalyserCreatorsContainer;
    typedef std::map<std::string,BehaviourAnalyser *> AnalyserContainer;

    TFEL_VISIBILITY_LOCAL
    BehaviourAnalyserFactory();

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

#endif /* LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_ */

