/*!
 * \file   include/TFEL/Tests/TestProxy.ixx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TESTPROXY_IXX
#define LIB_TFEL_TESTS_TESTPROXY_IXX 1

#include <memory>
#include "TFEL/Tests/TestManager.hxx"

namespace tfel::tests {

  template <typename TestType>
  template <typename... Arguments>
  TestProxy<TestType>::TestProxy(const std::string& n, Arguments&&... a) {
    auto& tm = TestManager::getTestManager();
    tm.addTest(n, std::make_shared<TestType>(std::forward<Arguments>(a)...));
  }  // end of TestProxy<T>::TestProxy

}  // end of namespace tfel::tests

#endif /* LIB_TFEL_TESTS_TESTPROXY_IXX */
