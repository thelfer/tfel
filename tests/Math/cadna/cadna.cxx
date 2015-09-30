/*!
 * \file   cadna.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   30 sept. 2015
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<type_traits>
#include<iostream>
#include<cstdlib>
#include<cmath>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/cadna.hxx"

struct CadnaTest final
  : public tfel::tests::TestCase
{
   CadnaTest()
    : tfel::tests::TestCase("TFEL/Math","CadnaTest")
  {} // end of CadnaTest
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace tfel::math;
    using cdouble = cadna::numeric_type<double>;
    TFEL_TESTS_STATIC_ASSERT((std::is_same<ResultType<cdouble,cdouble,OpDiv>::type,
			      cdouble>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<ComputeBinaryResult<cdouble&&,cdouble&&,OpDiv>::Result,
			      cdouble>::value));
    return this->result;
  } // end of execute
}; // end of struct CadnaTest

TFEL_TESTS_GENERATE_PROXY(CadnaTest,"CadnaTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void)
{
  using namespace tfel::tests;
  auto& m = TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("cadna.xml");
  const auto r = m.execute();
  return r.success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
