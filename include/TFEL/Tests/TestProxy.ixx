/*!
 * \file   include/TFEL/Tests/TestProxy.ixx
 * \author Helfer Thomas
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

namespace tfel {

  namespace tests {

    template <typename T>
    TestProxy<T>::TestProxy(const std::string& n) {
      auto& tm = TestManager::getTestManager();
      tm.addTest(n, std::shared_ptr<Test>(new T()));
    }  // end of TestProxy<T>::TestProxy

    template <typename T>
    template <typename T2>
    TestProxy<T>::TestProxy(const std::string& n, const T2& t) {
      auto& tm = TestManager::getTestManager();
      tm.addTest(n, std::shared_ptr<Test>(new T(t)));
    }  // end of TestProxy<T>::TestProxy

    template <typename T>
    template <typename T2, typename T3>
    TestProxy<T>::TestProxy(const std::string& n, const T2& t1, const T3& t2) {
      auto& tm = TestManager::getTestManager();
      tm.addTest(n, std::shared_ptr<Test>(new T(t1, t2)));
    }  // end of TestProxy<T>::TestProxy

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTPROXY_IXX */
