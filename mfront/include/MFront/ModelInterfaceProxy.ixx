/*!
 * \file   mfront/include/MFront/ModelInterfaceProxy.ixx
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

#ifndef _LIB_MFRONTMODELINTERFACEPROXY_IXX_
#define _LIB_MFRONTMODELINTERFACEPROXY_IXX_ 

namespace mfront{

  template<typename Interface>
  ModelInterfaceProxy<Interface>::ModelInterfaceProxy()
  {
    typedef ModelInterfaceFactory MBIF;
    auto& mbif = MBIF::getModelInterfaceFactory();
    mbif.registerInterfaceCreator(Interface::getName(),&createInterface);
  }
  
  template<typename Interface>
  AbstractModelInterface* 
  ModelInterfaceProxy<Interface>::createInterface()
  {
    return new Interface;
  }

} // end of namespace mfront

#endif /* _LIB_MFRONTMODELINTERFACEPROXY_IXX */
