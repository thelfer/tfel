/*!
 * \file   mfront/include/MFront/BehaviourInterfaceProxy.ixx
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

#ifndef LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_
#define LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  BehaviourInterfaceProxy<Interface>::BehaviourInterfaceProxy()
  {
    typedef BehaviourInterfaceFactory MBIF;
    auto& mbif = MBIF::getBehaviourInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
  }

  template<typename Interface>
  BehaviourInterfaceProxy<Interface>::BehaviourInterfaceProxy(const std::string& name)
  {
    typedef BehaviourInterfaceFactory MBIF;
    auto& mbif = MBIF::getBehaviourInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
  }
  
  template<typename Interface>
  template<typename InputIterator>
  BehaviourInterfaceProxy<Interface>::BehaviourInterfaceProxy(const InputIterator b,
							      const InputIterator e)
  {
    typedef BehaviourInterfaceFactory MBIF;
    auto& mbif = MBIF::getBehaviourInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(auto p=b;p!=e;++p){
      mbif.registerInterfaceAlias(Interface::getName(),*p);
    }
  } // end of BehaviourInterfaceProxy<Interface>::BehaviourInterfaceProxy
  
  template<typename Interface>
  std::shared_ptr<AbstractBehaviourInterface>
  BehaviourInterfaceProxy<Interface>::createInterface()
  {
    return std::make_shared<Interface>();
  }

} // end of namespace mfront

#endif /* LIB_MFRONTBEHAVIOURINTERFACEPROXY_IXX_ */
