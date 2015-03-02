/*!
 * \file   mfront/include/MFront/BehaviourAnalyserProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mai 2014x
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX_
#define LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX_ 

namespace mfront{

  template<typename Analyser>
  BehaviourAnalyserProxy<Analyser>::BehaviourAnalyserProxy()
  {
    typedef BehaviourAnalyserFactory MBAF;
    auto& mbaf = MBAF::getBehaviourAnalyserFactory();
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    mbaf.registerAnalyserAlias(Analyser::getName(),Analyser::getName());
  }
  
  template<typename Analyser>
  BehaviourAnalyserProxy<Analyser>::BehaviourAnalyserProxy(const std::string& name)
  {
    typedef BehaviourAnalyserFactory MBAF;
    auto& mbaf = MBAF::getBehaviourAnalyserFactory();
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    mbaf.registerAnalyserAlias(Analyser::getName(),name);
  }
  
  template<typename Analyser>
  template<typename InputIterator>
  BehaviourAnalyserProxy<Analyser>::BehaviourAnalyserProxy(const InputIterator b,
									const InputIterator e)
  {
    typedef BehaviourAnalyserFactory MBAF;
    auto& mbaf = MBAF::getBehaviourAnalyserFactory();
    InputIterator p;
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    for(p=b;p!=e;++p){
      mbaf.registerAnalyserAlias(Analyser::getName(),*p);
    }
  } // end of BehaviourAnalyserProxy<Analyser>::BehaviourAnalyserProxy

  template<typename Analyser>
  BehaviourAnalyser* 
  BehaviourAnalyserProxy<Analyser>::createAnalyser()
  {
    return new Analyser;
  }

} // end of namespace mfront

#endif /* LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX_ */
