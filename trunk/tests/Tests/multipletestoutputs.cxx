/*!
 * \file   multipletestoutputs.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief a simple test of the MultipleTestOutputs class
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>

#include"TFEL/Tests/TestSuite.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"
#include"TFEL/Tests/StdStreamTestOutput.hxx"
#include"TFEL/Tests/MultipleTestOutputs.hxx"

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
  shared_ptr<Test> a(new Wrapper1("test1"));
  shared_ptr<Test> b(new Wrapper2("test2"));
  MultipleTestOutputs output;
  output.addTestOutput(shared_ptr<TestOutput>(new StdStreamTestOutput("multipletestoutputs.txt")));
  TestSuite suite1("suite1");
  suite1.add(a);
  suite1.add(b);
  assert(!suite1.execute(output).success());
  return EXIT_SUCCESS;
} // end of main

