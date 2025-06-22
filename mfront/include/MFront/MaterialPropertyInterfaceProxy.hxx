/*!
 * \file   mfront/include/MFront/MaterialPropertyInterfaceProxy.hxx
 * \brief
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTLAWINTERFACEPROXY_HXX
#define LIB_MFRONTLAWINTERFACEPROXY_HXX

#include <string>
#include <memory>

#include "MFront/MaterialPropertyInterfaceFactory.hxx"
#include "MFront/AbstractMaterialPropertyInterface.hxx"

namespace mfront {

  template <typename Interface>
  struct MaterialPropertyInterfaceProxy {
    MaterialPropertyInterfaceProxy();

    MaterialPropertyInterfaceProxy(const std::string&);

    template <typename InputIterator>
    MaterialPropertyInterfaceProxy(const InputIterator, const InputIterator);

    MaterialPropertyInterfaceProxy(const std::string&, const std::string&);

    template <typename InputIterator>
    MaterialPropertyInterfaceProxy(const std::string&,
                                   const InputIterator,
                                   const InputIterator);

    template <typename InputIterator>
    MaterialPropertyInterfaceProxy(const InputIterator,
                                   const InputIterator,
                                   const std::string&);

    template <typename InputIterator, typename InputIterator2>
    MaterialPropertyInterfaceProxy(const InputIterator,
                                   const InputIterator,
                                   const InputIterator2,
                                   const InputIterator2);

    static std::shared_ptr<AbstractMaterialPropertyInterface> createInterface();
  };

}  // end of namespace mfront

#include "MFront/MaterialPropertyInterfaceProxy.ixx"

#endif /* LIB_MFRONTLAWINTERFACEPROXY_HXX */
