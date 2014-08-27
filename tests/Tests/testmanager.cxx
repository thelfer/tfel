/*!
 * \file   testmanager.cxx
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

#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"

bool
test1(void)
{
  return true;
}

bool
test2(void)
{
  return false;
}

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  TestManager& manager = TestManager::getTestManager();
  shared_ptr<Test> a(new Wrapper1("test1"));
  shared_ptr<Test> b(new Wrapper2("test2"));
  ofstream f("testmanager-3.txt");
  if(!f){
    string msg("can't open file 'testmanager-3.txt'");
    throw(runtime_error(msg));
  }
  manager.addTest("suite1",a);
  manager.addTest("suite1",b);
  manager.addTest("suite2",a);
  manager.addTest("suite3",a);
  manager.addTestOutput("suite1","testmanager-1.txt");
  manager.addTestOutput("testmanager-2.txt");
  manager.addTestOutput("suite3",f,false);
  TestResult r = manager.execute();
  assert(!r.success());
  return EXIT_SUCCESS;
} // end of main
