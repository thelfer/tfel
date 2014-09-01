/*!
 * \file   mfront/include/MFront/MFrontBehaviourAnalyserProxy.hxx
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
