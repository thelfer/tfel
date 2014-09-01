/*!
 * \file   mfront/include/MFront/MFrontBehaviourInterfaceProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_
#define _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  MFrontBehaviourInterfaceProxy<Interface>::MFrontBehaviourInterfaceProxy()
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mbif.registerInterfaceAlias(Interface::getName(),Interface::getName());
  }

  template<typename Interface>
  MFrontBehaviourInterfaceProxy<Interface>::MFrontBehaviourInterfaceProxy(const std::string& name)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mbif.registerInterfaceAlias(Interface::getName(),name);
  }
  
  template<typename Interface>
  template<typename InputIterator>
  MFrontBehaviourInterfaceProxy<Interface>::MFrontBehaviourInterfaceProxy(const InputIterator b,
									  const InputIterator e)
  {
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    InputIterator p;
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      mbif.registerInterfaceAlias(Interface::getName(),*p);
    }
  } // end of MFrontBehaviourInterfaceProxy<Interface>::MFrontBehaviourInterfaceProxy
  
  template<typename Interface>
  MFrontBehaviourVirtualInterface* 
  MFrontBehaviourInterfaceProxy<Interface>::createInterface()
  {
    return new Interface;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX */
