/*!
 * \file   testproxy2.cxx
 * \author Helfer Thomas
 * \date   11 Apr 10
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

TFEL_TESTS_FUNCTION(test1,"TFEL/Tests","suite1")
{
  return true;
}

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  ofstream f("testproxy2.txt");
  if(!f){
    string msg("can't open file 'testproxy2.txt'");
    throw(runtime_error(msg));
  }
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(f,false);
  TestResult r = manager.execute();
  assert(r.success());
  return EXIT_SUCCESS;
} // end of main
