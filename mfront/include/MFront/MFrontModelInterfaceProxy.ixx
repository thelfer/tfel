/*!
 * \file   mfront/include/MFront/MFrontModelInterfaceProxy.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTMODELINTERFACEPROXY_IXX_
#define _LIB_MFRONTMODELINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  MFrontModelInterfaceProxy<Interface>::MFrontModelInterfaceProxy()
  {
    typedef MFrontModelInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontModelInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
  }
  
  template<typename Interface>
  MFrontModelVirtualInterface* 
  MFrontModelInterfaceProxy<Interface>::createInterface()
  {
    return new Interface;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTMODELINTERFACEPROXY_IXX */
