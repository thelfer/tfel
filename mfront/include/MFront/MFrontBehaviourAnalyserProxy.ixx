/*!
 * \file   mfront/include/MFront/MFrontBehaviourAnalyserProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 mai 2014x
 */

#ifndef _LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX_
#define _LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX_ 

namespace mfront{

  template<typename Analyser>
  MFrontBehaviourAnalyserProxy<Analyser>::MFrontBehaviourAnalyserProxy()
  {
    typedef MFrontBehaviourAnalyserFactory MBAF;
    MBAF& mbaf = MBAF::getMFrontBehaviourAnalyserFactory();
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    mbaf.registerAnalyserAlias(Analyser::getName(),Analyser::getName());
  }
  
  template<typename Analyser>
  MFrontBehaviourAnalyserProxy<Analyser>::MFrontBehaviourAnalyserProxy(const std::string& name)
  {
    typedef MFrontBehaviourAnalyserFactory MBAF;
    MBAF& mbaf = MBAF::getMFrontBehaviourAnalyserFactory();
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    mbaf.registerAnalyserAlias(Analyser::getName(),name);
  }
  
  template<typename Analyser>
  template<typename InputIterator>
  MFrontBehaviourAnalyserProxy<Analyser>::MFrontBehaviourAnalyserProxy(const InputIterator b,
									const InputIterator e)
  {
    typedef MFrontBehaviourAnalyserFactory MBAF;
    MBAF& mbaf = MBAF::getMFrontBehaviourAnalyserFactory();
    InputIterator p;
    mbaf.registerAnalyserCreator(Analyser::getName(),&createAnalyser);
    for(p=b;p!=e;++p){
      mbaf.registerAnalyserAlias(Analyser::getName(),*p);
    }
  } // end of MFrontBehaviourAnalyserProxy<Analyser>::MFrontBehaviourAnalyserProxy

  template<typename Analyser>
  MFrontBehaviourAnalyser* 
  MFrontBehaviourAnalyserProxy<Analyser>::createAnalyser()
  {
    return new Analyser;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTBEHAVIOURANALYSERPROXY_IXX */
