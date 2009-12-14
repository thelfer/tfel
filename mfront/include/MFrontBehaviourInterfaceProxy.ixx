/*!
 * \file   MFrontBehaviourInterfaceProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_
#define _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  MFrontBehaviourInterfaceProxy<Interface>::MFrontBehaviourInterfaceProxy()
  {
    behaviourInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
  }
  
  template<typename Interface>
  MFrontBehaviourVirtualInterface* 
  MFrontBehaviourInterfaceProxy<Interface>::createInterface()
  {
    return new Interface;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX */
