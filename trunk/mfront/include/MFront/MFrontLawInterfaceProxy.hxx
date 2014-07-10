/*!
 * \file   MFrontLawInterfaceProxy.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   09 nov 2006
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
