/*!
 * \file   mfront/include/MFront/MaterialPropertyInterfaceProxy.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTLAWINTERFACEPROXY_IXX
#define LIB_MFRONTLAWINTERFACEPROXY_IXX

namespace mfront {

  template <typename Interface>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy() {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    mlif.registerInterfaceAlias(Interface::getName(), Interface::getName());
  }

  template <typename Interface>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const std::string& name) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    mlif.registerInterfaceAlias(Interface::getName(), name);
  }

  template <typename Interface>
  template <typename InputIterator>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const std::string& name, const InputIterator b, const InputIterator e) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    mlif.registerInterfaceAlias(Interface::getName(), name);
    for (p = b; p != e; ++p) {
      mlif.registerInterfaceDependency(Interface::getName(), *p);
    }
  }  // end of MaterialPropertyInterfaceProxy

  template <typename Interface>
  template <typename InputIterator>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const InputIterator b, const InputIterator e) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    for (p = b; p != e; ++p) {
      mlif.registerInterfaceAlias(Interface::getName(), *p);
    }
  }  // end of MaterialPropertyInterfaceProxy

  template <typename Interface>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const std::string& name, const std::string& dep) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    mlif.registerInterfaceAlias(Interface::getName(), name);
    mlif.registerInterfaceDependency(Interface::getName(), dep);
  }  // end of MaterialPropertyInterfaceProxy

  template <typename Interface>
  template <typename InputIterator>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const InputIterator b, const InputIterator e, const std::string& dep) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    InputIterator p;
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    for (p = b; p != e; ++p) {
      mlif.registerInterfaceAlias(Interface::getName(), *p);
    }
    mlif.registerInterfaceDependency(Interface::getName(), dep);
  }  // end of MaterialPropertyInterfaceProxy

  template <typename Interface>
  template <typename InputIterator, typename InputIterator2>
  MaterialPropertyInterfaceProxy<Interface>::MaterialPropertyInterfaceProxy(
      const InputIterator b,
      const InputIterator e,
      const InputIterator2 b2,
      const InputIterator2 e2) {
    typedef MaterialPropertyInterfaceFactory MLIF;
    auto& mlif = MLIF::getMaterialPropertyInterfaceFactory();
    InputIterator p;
    InputIterator2 p2;
    mlif.registerInterfaceCreator(Interface::getName(), &createInterface);
    for (p = b; p != e; ++p) {
      mlif.registerInterfaceAlias(Interface::getName(), *p);
    }
    for (p2 = b2; p2 != e2; ++p2) {
      mlif.registerInterfaceDependency(Interface::getName(), *p2);
    }
  }  // end of MaterialPropertyInterfaceProxy

  template <typename Interface>
  std::shared_ptr<AbstractMaterialPropertyInterface>
  MaterialPropertyInterfaceProxy<Interface>::createInterface() {
    return std::make_shared<Interface>();
  }

}  // end of namespace mfront

#endif /* LIB_MFRONTLAWINTERFACEPROXY_IXX */
