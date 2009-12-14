/*!
 * \file   MFrontBehaviourInterfaceProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_
#define _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_ 

#include<string>

#include"MFrontBehaviourInterfaceFactory.hxx"
#include"MFrontBehaviourVirtualInterface.hxx"

namespace mfront{

  template<typename Interface>
  struct MFrontBehaviourInterfaceProxy
  {
    MFrontBehaviourInterfaceProxy();

    static MFrontBehaviourVirtualInterface* createInterface();
  };

} // end of namespace mfront

#include"MFrontBehaviourInterfaceProxy.ixx"

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX */
