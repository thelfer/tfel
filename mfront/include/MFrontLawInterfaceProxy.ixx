/*!
 * \file   MFrontLawInterfaceProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTLAWINTERFACEPROXY_IXX_
#define _LIB_MFRONTLAWINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy()
  {
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),Interface::getName());
  }

  template<typename Interface>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name)
  {
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),name);
  }

  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name,
							      const InputIterator b,
							      const InputIterator e)
  {
    InputIterator p;
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),name);
    for(p=b;p!=e;++p){
      lawInterfaceFactory.registerInterfaceDependency(Interface::getName(),*p);
    }
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  
  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e)
  {
    InputIterator p;
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),*p);
    }
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name,
							      const std::string& dep)
  {
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),name);
    lawInterfaceFactory.registerInterfaceDependency(Interface::getName(),dep);
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e,
							      const std::string& dep)
  {
    InputIterator p;
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),*p);
    }
    lawInterfaceFactory.registerInterfaceDependency(Interface::getName(),dep);
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  template<typename InputIterator,
	   typename InputIterator2>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e,
							      const InputIterator2 b2,
							      const InputIterator2 e2)
  {
    InputIterator  p;
    InputIterator2 p2;
    lawInterfaceFactory.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      lawInterfaceFactory.registerInterfaceAlias(Interface::getName(),*p);
    }
    for(p2=b2;p2!=e2;++p2){
      lawInterfaceFactory.registerInterfaceDependency(Interface::getName(),*p2);
    }
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy


  template<typename Interface>
  MFrontLawVirtualInterface* 
  MFrontLawInterfaceProxy<Interface>::createInterface()
  {
    return new Interface;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTLAWINTERFACEPROXY_IXX */
