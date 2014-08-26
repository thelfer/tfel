/*!
 * \file   MFrontBehaviourAnalyserProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   19 may 2014
 */

#ifndef _LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX_
#define _LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX_ 

#include<string>

#include"MFront/MFrontBehaviourAnalyserFactory.hxx"
#include"MFront/MFrontBehaviourAnalyser.hxx"

namespace mfront{

  template<typename Analyser>
  struct MFrontBehaviourAnalyserProxy
  {
    MFrontBehaviourAnalyserProxy();

    MFrontBehaviourAnalyserProxy(const std::string&);

    template<typename InputIterator>
    MFrontBehaviourAnalyserProxy(const InputIterator,
				 const InputIterator);

    static MFrontBehaviourAnalyser* createAnalyser();
  };

} // end of namespace mfront

#include"MFront/MFrontBehaviourAnalyserProxy.ixx"

#endif /* _LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX */
