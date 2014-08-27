/*!
 * \file   testproxy.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief a simple test of the TestSuite class
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<stdexcept>

#include<fstream>

#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"

bool
test1(void)
{
  return true;
}

TFEL_TESTS_GENERATE_PROXY3(tfel::tests::TestFunctionWrapper<test1>,
			   Test1FunctionWrapper,"TFEL/Tests","suite1");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  ofstream f("testproxy.txt");
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
