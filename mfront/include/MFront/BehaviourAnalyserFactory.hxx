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

#include<map>
#include<vector>
#include<string>
#include<memory>

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  // forward declaration
  struct BehaviourAnalyser;

  struct MFRONT_VISIBILITY_EXPORT BehaviourAnalyserFactory
  {
    typedef std::shared_ptr<BehaviourAnalyser> (*AnalyserCreator)(void);
    
    static BehaviourAnalyserFactory&
    getBehaviourAnalyserFactory();

    std::vector<std::string>
    getRegistredAnalysers(void);

    void registerAnalyserCreator(const std::string&,
				 AnalyserCreator);
    
    void
    registerAnalyserAlias(const std::string&,
			  const std::string&);
    
    std::shared_ptr<BehaviourAnalyser>
    getAnalyser(const std::string&);
    //! destructor
    ~BehaviourAnalyserFactory();
    
  private:

    typedef std::map<std::string,AnalyserCreator> AnalyserCreatorsContainer;
    typedef std::map<std::string,std::string> AliasContainer;

    TFEL_VISIBILITY_LOCAL
    BehaviourAnalyserFactory();

    TFEL_VISIBILITY_LOCAL
    AnalyserCreatorsContainer&
    getAnalyserCreatorsMap(void);

    TFEL_VISIBILITY_LOCAL
    AliasContainer&
    getAliasesMap(void);

  };

} // end of namespace mfront  

#endif /* LIB_MFRONBEHAVIOURTANALYSERFACTORY_HXX_ */

