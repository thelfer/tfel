/*!
 * \file   mfront/include/MFront/MFrontLawInterfaceProxy.hxx
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

#ifndef _LIB_MFRONTLAWINTERFACEPROXY_HXX_
#define _LIB_MFRONTLAWINTERFACEPROXY_HXX_ 

#include<string>

#include"MFront/MFrontLawInterfaceFactory.hxx"
#include"MFront/MFrontLawVirtualInterface.hxx"

namespace mfront{

  template<typename Interface>
  struct MFrontLawInterfaceProxy
  {
    MFrontLawInterfaceProxy();

    MFrontLawInterfaceProxy(const std::string&);

    template<typename InputIterator>
    MFrontLawInterfaceProxy(const InputIterator,
			    const InputIterator);

    MFrontLawInterfaceProxy(const std::string&,
			    const std::string&);

    template<typename InputIterator>
    MFrontLawInterfaceProxy(const std::string&,
			    const InputIterator,
			    const InputIterator);

    template<typename InputIterator>
    MFrontLawInterfaceProxy(const InputIterator,
			    const InputIterator,
			    const std::string&);

    template<typename InputIterator,
	     typename InputIterator2>
    MFrontLawInterfaceProxy(const InputIterator,
			    const InputIterator,
			    const InputIterator2,
			    const InputIterator2);

    static MFrontLawVirtualInterface* createInterface();
  };

} // end of namespace mfront

#include"MFront/MFrontLawInterfaceProxy.ixx"

#endif /* _LIB_MFRONTLAWINTERFACEPROXY_HXX */
