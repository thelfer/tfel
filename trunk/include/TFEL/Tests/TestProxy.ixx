/*!
 * \file   TestProxy.ixx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTPROXY_IXX
#define _LIB_TFEL_TESTS_TESTPROXY_IXX 1

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Tests/TestManager.hxx"

namespace tfel
{

  namespace tests
  {
    
    template<typename T>
    TestProxy<T>::TestProxy(const std::string& n)
    {
      using namespace tfel::utilities;
      TestManager& tm = TestManager::getTestManager();
      tm.addTest(n,shared_ptr<Test>(new T()));
    } // end of TestProxy<T>::TestProxy

    template<typename T>
    template<typename T2>
    TestProxy<T>::TestProxy(const std::string& n,
			    const T2& t)
    {
      using namespace tfel::utilities;
      TestManager& tm = TestManager::getTestManager();
      tm.addTest(n,shared_ptr<Test>(new T(t)));
    } // end of TestProxy<T>::TestProxy

    template<typename T>
    template<typename T2,
	     typename T3>
    TestProxy<T>::TestProxy(const std::string& n,
			    const T2& t1,
			    const T3& t2)
    {
      using namespace tfel::utilities;
      TestManager& tm = TestManager::getTestManager();
      tm.addTest(n,shared_ptr<Test>(new T(t1,t2)));
    } // end of TestProxy<T>::TestProxy
    
  } // end of namespace test

} // end of namespace tfel

#include"TFEL/Tests/TestProxy.ixx"

#endif  /*_LIB_TFEL_TESTS_TESTPROXY_IXX */
