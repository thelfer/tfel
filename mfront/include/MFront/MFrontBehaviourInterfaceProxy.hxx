/*!
 * \file   mfront/include/MFront/MFrontBehaviourInterfaceProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_
#define _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_ 

#include<string>

#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  template<typename Interface>
  struct MFrontBehaviourInterfaceProxy
  {
    MFrontBehaviourInterfaceProxy();

    MFrontBehaviourInterfaceProxy(const std::string&);

    template<typename InputIterator>
    MFrontBehaviourInterfaceProxy(const InputIterator,
				  const InputIterator);

    static MFrontBehaviourVirtualInterface* createInterface();
  };

} // end of namespace mfront

#include"MFront/MFrontBehaviourInterfaceProxy.ixx"

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX */
