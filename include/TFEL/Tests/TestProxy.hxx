/*!
 * \file   include/TFEL/Tests/TestProxy.hxx
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

#ifndef LIB_TFEL_TESTS_TESTPROXY_HXX
#define LIB_TFEL_TESTS_TESTPROXY_HXX 1

#include <string>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/TestFunctionWrapper.hxx"

/*!
 * \def TFEL_PP_JOIN
 * \brief this macro joins joins its two arguments together.
 *
 * This macro was taken from the boost library:
 * - http://boost.org/
 *
 * The following piece of macro magic joins the two
 * arguments together, even when one of the arguments is
 * itself a macro (see 16.3.1 in C++ standard).  The key
 * is that macro expansion of macro arguments does not
 * occur in TFEL_PP_DO_JOIN2 but does in TFEL_PP_DO_JOIN.
 *
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define TFEL_PP_JOIN(X, Y) TFEL_PP_DO_JOIN(X, Y)

/*!
 * \def TFEL_PP_DO_JOIN
 * \brief An helper macro for TFEL_PP_JOIN
 * \see TFEL_PP_JOIN
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define TFEL_PP_DO_JOIN(X, Y) TFEL_PP_DO_JOIN2(X, Y)

/*!
 * \def TFEL_PP_DO_JOIN2

 * \see TFEL_PP_JOIN
 * \author Thomas Helfer
 * \date   28 Aug 2006
 */
#define TFEL_PP_DO_JOIN2(X, Y) X##Y

/*!
 * \def TFEL_TESTS_GENERATE_PROXY2
 * \brief generate a proxy for class X
 * \param X  : class for which the proxy is generated
 * \param X2 : stripped class name
 * \param Y  : test suite to which the test is associated
 * \author Thomas Helfer
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY2(X, X2, Y)                    \
  extern tfel::tests::TestProxy<X> TFEL_PP_JOIN(testproxy, X2); \
  tfel::tests::TestProxy<X> TFEL_PP_JOIN(testproxy, X2)(Y)

/*!
 * \def TFEL_TESTS_GENERATE_PROXY3
 * \brief generate a proxy for class X
 * \param X  : class for which the proxy is generated
 * \param X2 : stripped class name
 * \param Y  : test suite to which the test is associated
 * \param Y2 : parameter passed to the class constructor
 * \author Thomas Helfer
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY3(X, X2, Y, Y2)                \
  extern tfel::tests::TestProxy<X> TFEL_PP_JOIN(testproxy, X2); \
  tfel::tests::TestProxy<X> TFEL_PP_JOIN(testproxy, X2)(Y, Y2)

/*!
 * \def TFEL_TESTS_GENERATE_PROXY
 * \brief generate a proxy for class X
 * \param X : class for which the proxy is generated
 * \param Y : test suite to which the test is associated
 * \author Thomas Helfer
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY(X, Y) TFEL_TESTS_GENERATE_PROXY2(X, X, Y)

/*!
 * \def TFEL_TESTS_FUNCTION
 * \brief generate a proxy for function X
 * \param X : test function to be generated
 * \param G : test group
 * \param Y : test suite to which the test is associated
 * \author Thomas Helfer
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_FUNCTION(X, G, Y)                                  \
  TFEL_VISIBILITY_LOCAL bool X();                                     \
  TFEL_TESTS_GENERATE_PROXY3(tfel::tests::TestFunctionWrapper<X>,     \
                             TFEL_PP_JOIN(X, FunctionWrapper), G, Y); \
  bool X()

namespace tfel::tests {

  /*!
   * \brief proxy class for automatic registration of tests
   * \tparam TestType: type of the test to be registred
   */
  template <typename TestType>
  struct TFEL_VISIBILITY_LOCAL TestProxy {
    /*!
     * \brief Default constructor
     * \param[in] n: test suite name
     * \param[in] a: test argument
     */
    template <typename... Arguments>
    TestProxy(const std::string&, Arguments&&...);
  };  // end of struct Test

}  // end of namespace tfel::tests

#include "TFEL/Tests/TestProxy.ixx"

#endif /* LIB_TFEL_TESTS_TESTPROXY_HXX */
