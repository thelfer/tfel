/*!
 * \file   testcase.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief a simple test of the TestCase class
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<string>
#include<cstdlib>
#include<cassert>
#include<fstream>
#include<stdexcept>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct MyTest
  : public tfel::tests::TestCase
{
  MyTest()
    : TestCase("TFEL/Tests","MyTest")
  {} // end of MyTest
  tfel::tests::TestResult
  execute()
  {
    TFEL_TESTS_ASSERT(1==1);
    TFEL_TESTS_ASSERT(1!=2);
    TFEL_TESTS_CALLMETHOD(test);
    TFEL_TESTS_CHECK_THROW(throw(std::runtime_error("test")),
			  std::runtime_error);
    TFEL_TESTS_CHECK_THROW(throw(std::runtime_error("test")),
			  std::exception);
    TFEL_TESTS_CHECK_EQUAL(std::string("test"),"test");
    return this->result;
  } // end of execute()
private:
  bool
  test() const
  {
    return true;
  };
}; // end of struct MyTest 

TFEL_TESTS_GENERATE_PROXY(MyTest,"TestSuite1");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  using namespace std;
  using namespace tfel::tests;
  ofstream f("testcase.txt");
  if(!f){
    string msg("can't open file 'testproxy.txt'");
    throw(runtime_error(msg));
  }
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(f,false);
  TestResult r = manager.execute();
  assert(r.success());
  return EXIT_SUCCESS;
} // end of main
