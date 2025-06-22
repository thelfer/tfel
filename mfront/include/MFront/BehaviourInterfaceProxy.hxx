/*!
 * \file   mfront/include/MFront/BehaviourInterfaceProxy.hxx
 * \brief
 * \author Helfer Thomas
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_
#define LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_

#include <string>
#include <memory>

#include "MFront/BehaviourInterfaceFactory.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"

namespace mfront {

  template <typename Interface>
  struct BehaviourInterfaceProxy {
    BehaviourInterfaceProxy();

    BehaviourInterfaceProxy(const std::string&);

    template <typename InputIterator>
    BehaviourInterfaceProxy(const InputIterator, const InputIterator);

    static std::shared_ptr<AbstractBehaviourInterface> createInterface();
  };

}  // end of namespace mfront

#include "MFront/BehaviourInterfaceProxy.ixx"

#endif /* LIB_MFRONTBEHAVIOURINTERFACEPROXY_HXX_ */
