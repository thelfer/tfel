/*!
 * \file   tests/Utilities/tuple.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   24 fév 2007
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<string>
#include<cmath>

#include<cassert>

#include"TFEL/Utilities/Tuple.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct TupleTest
  : public tfel::tests::TestCase
{
  TupleTest()
    : tfel::tests::TestCase("TFEL/Utilities","TupleTest")
  {} // end of MyTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using tfel::utilities::tuple;
    using tfel::utilities::makeTuple;

    tuple<double,string> t(12.,"toto");
    tuple<double,string> t2;

    TFEL_TESTS_ASSERT(abs(t.get<0>()-12.)<1.e-14);
    TFEL_TESTS_CHECK_EQUAL(t.get<1>(),"toto");
  
    t.set<0>(13.);
    t.set<1>("tata");

    TFEL_TESTS_ASSERT(abs(t.get<0>()-13.)<1.e-14);
    TFEL_TESTS_CHECK_EQUAL(t.get<1>(),"tata");

    t2=makeTuple(123.,std::string("titi"));
    TFEL_TESTS_ASSERT(abs(t2.get<0>()-123.)<1.e-14);
    TFEL_TESTS_CHECK_EQUAL(t2.get<1>(),"titi");
  
    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(TupleTest,"Tuple");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("tuple.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
