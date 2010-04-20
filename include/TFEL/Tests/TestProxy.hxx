/*!
 * \file   TestProxy.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTPROXY_HXX
#define _LIB_TFEL_TESTS_TESTPROXY_HXX 1

#include<string>

#include"TFEL/Tests/TestFunctionWrapper.hxx"

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
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */
#define TFEL_PP_JOIN(X,Y) TFEL_PP_DO_JOIN( X, Y )

/*!
 * \def TFEL_PP_DO_JOIN
 * \brief An helper macro for TFEL_PP_JOIN
 * \see TFEL_PP_JOIN
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */
#define TFEL_PP_DO_JOIN(X,Y) TFEL_PP_DO_JOIN2(X,Y)

/*!
 * \def TFEL_PP_DO_JOIN2

 * \see TFEL_PP_JOIN
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */
#define TFEL_PP_DO_JOIN2(X,Y) X##Y

/*!
 * \def TFEL_TESTS_GENERATE_PROXY2
 * \brief generate a proxy for class X
 * \param X  : class for which the proxy is generated
 * \param X2 : stripped class name
 * \param Y  : test suite to which the test is associated
 * \author Helfer Thomas
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY2(X,X2,Y)                   \
  tfel::tests::TestProxy< X > TFEL_PP_JOIN(testproxy,X2)(Y)

/*!
 * \def TFEL_TESTS_GENERATE_PROXY3
 * \brief generate a proxy for class X
 * \param X  : class for which the proxy is generated
 * \param X2 : stripped class name
 * \param Y  : test suite to which the test is associated
 * \param Y2 : parameter passed to the class constructor
 * \author Helfer Thomas
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY3(X,X2,Y,Y2)			\
  tfel::tests::TestProxy< X > TFEL_PP_JOIN(testproxy,X2)(Y,Y2)

/*!
 * \def TFEL_TESTS_GENERATE_PROXY
 * \brief generate a proxy for class X
 * \param X : class for which the proxy is generated
 * \param Y : test suite to which the test is associated
 * \author Helfer Thomas
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY(X,Y)                     \
  TFEL_TESTS_GENERATE_PROXY2(X,X,Y)

/*!
 * \def TFEL_TESTS_GENERATE_PROXY
 * \brief generate a proxy for class X
 * \param X : class for which the proxy is generated
 * \param Y2 : parameter passed to the class constructor
 * \param Y : test suite to which the test is associated
 * \author Helfer Thomas
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_GENERATE_PROXY4(X,Y,Y2)	          \
  TFEL_TESTS_GENERATE_PROXY2(X,X,Y,Y2)

/*!
 * \def TFEL_TESTS_FUNCTION
 * \brief generate a proxy for class X
 * \param X : test function to be generated
 * \param Y : test suite to which the test is associated
 * \author Helfer Thomas
 * \date   11 Apr 2010
 */
#define TFEL_TESTS_FUNCTION(X,Y)				    \
  bool __attribute__ ((visibility("hidden"))) X(void);	            \
  TFEL_TESTS_GENERATE_PROXY3(tfel::tests::TestFunctionWrapper<X>,   \
                             TFEL_PP_JOIN(X,FunctionWrapper),Y,#X); \
  bool X(void)

namespace tfel
{

  namespace tests
  {
    
    /*!
     * proxy class for automatic registration of tests
     */
    template<typename T>
    struct __attribute__ ((visibility("hidden"))) TestProxy
    {
      /*!
       * Default constructor
       * \param n : test suite name
       */
      TestProxy(const std::string&);
      /*!
       * Default constructor
       * \param n : test suite name
       * \param t : test argument 
       */
      template<typename T2>
      TestProxy(const std::string&,
		const T2&);
      /*!
       * Default constructor
       * \param n  : test suite name
       * \param t1 : first test argument 
       * \param t2 : second test argument 
       */
      template<typename T2,
	       typename T3>
      TestProxy(const std::string&,
		const T2&,
		const T3&);
    }; // end of struct Test
    
  } // end of namespace test

} // end of namespace tfel

#include"TFEL/Tests/TestProxy.ixx"

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
