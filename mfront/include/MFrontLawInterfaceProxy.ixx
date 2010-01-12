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
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mlif.registerInterfaceAlias(Interface::getName(),Interface::getName());
  }

  template<typename Interface>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mlif.registerInterfaceAlias(Interface::getName(),name);
  }

  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name,
							      const InputIterator b,
							      const InputIterator e)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mlif.registerInterfaceAlias(Interface::getName(),name);
    for(p=b;p!=e;++p){
      mlif.registerInterfaceDependency(Interface::getName(),*p);
    }
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  
  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      mlif.registerInterfaceAlias(Interface::getName(),*p);
    }
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const std::string& name,
							      const std::string& dep)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    mlif.registerInterfaceAlias(Interface::getName(),name);
    mlif.registerInterfaceDependency(Interface::getName(),dep);
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  template<typename InputIterator>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e,
							      const std::string& dep)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      mlif.registerInterfaceAlias(Interface::getName(),*p);
    }
    mlif.registerInterfaceDependency(Interface::getName(),dep);
  } // end of MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy

  template<typename Interface>
  template<typename InputIterator,
	   typename InputIterator2>
  MFrontLawInterfaceProxy<Interface>::MFrontLawInterfaceProxy(const InputIterator b,
							      const InputIterator e,
							      const InputIterator2 b2,
							      const InputIterator2 e2)
  {
    typedef MFrontLawInterfaceFactory MLIF;
    MLIF& mlif = MLIF::getMFrontLawInterfaceFactory();
    InputIterator  p;
    InputIterator2 p2;
    mlif.registerInterfaceCreator(Interface::getName(),&createInterface);
    for(p=b;p!=e;++p){
      mlif.registerInterfaceAlias(Interface::getName(),*p);
    }
    for(p2=b2;p2!=e2;++p2){
      mlif.registerInterfaceDependency(Interface::getName(),*p2);
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
