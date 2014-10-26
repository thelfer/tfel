/*!
 * \file   mfront/include/MFront/BehaviourAnalyserProxy.hxx
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

#include"MFront/BehaviourAnalyserFactory.hxx"
#include"MFront/BehaviourAnalyser.hxx"

namespace mfront{

  template<typename Analyser>
  struct BehaviourAnalyserProxy
  {
    BehaviourAnalyserProxy();

    BehaviourAnalyserProxy(const std::string&);

    template<typename InputIterator>
    BehaviourAnalyserProxy(const InputIterator,
				 const InputIterator);

    static BehaviourAnalyser* createAnalyser();
  };

} // end of namespace mfront

#include"MFront/BehaviourAnalyserProxy.ixx"

#endif /* _LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX */
