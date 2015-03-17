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

#ifndef LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX_
#define LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX_ 

#include<string>
#include<memory>

#include"MFront/BehaviourAnalyserFactory.hxx"

namespace mfront{

  // forward declaration
  struct BehaviourAnalyser;

  template<typename Analyser>
  struct BehaviourAnalyserProxy
  {
    BehaviourAnalyserProxy();

    BehaviourAnalyserProxy(const std::string&);

    template<typename InputIterator>
    BehaviourAnalyserProxy(const InputIterator,
				 const InputIterator);

    static std::shared_ptr<BehaviourAnalyser> createAnalyser();
  };

} // end of namespace mfront

#include"MFront/BehaviourAnalyserProxy.ixx"

#endif /* LIB_MFRONTBEHAVIOURANALYSERPROXY_HXX_ */
