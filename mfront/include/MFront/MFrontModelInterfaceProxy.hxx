/*!
 * \file   mfront/include/MFront/MFrontModelInterfaceProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
 */

#ifndef _LIB_MFRONTMODELINTERFACEPROXY_HXX_
#define _LIB_MFRONTMODELINTERFACEPROXY_HXX_ 

#include<string>

#include"MFront/MFrontModelInterfaceFactory.hxx"
#include"MFront/MFrontModelVirtualInterface.hxx"

namespace mfront{

  template<typename Interface>
  struct MFrontModelInterfaceProxy
  {
    MFrontModelInterfaceProxy();

    static MFrontModelVirtualInterface* createInterface();
  };

} // end of namespace mfront

#include"MFront/MFrontModelInterfaceProxy.ixx"

#endif /* _LIB_MFRONTMODELINTERFACEPROXY_HXX */
