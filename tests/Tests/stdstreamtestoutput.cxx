/*!
 * \file   stdstreamtestoutput.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief a simple test of the StdStreamTestOutput class
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>

#include"TFEL/Tests/TestSuite.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"
#include"TFEL/Tests/StdStreamTestOutput.hxx"

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
  using namespace tfel::tests;
  using namespace tfel::utilities;
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  shared_ptr<Test> a(new Wrapper1("test1"));
  shared_ptr<Test> b(new Wrapper2("test2"));
  StdStreamTestOutput output1("stdstreamtestoutput1.txt");
  StdStreamTestOutput output2("stdstreamtestoutput2.txt");
  StdStreamTestOutput output3("stdstreamtestoutput3.txt");
  TestSuite suite1("suite1");
  TestSuite suite2("suite2");
  TestSuite suite3("suite3");
  suite1.add(a);
  suite1.add(b);
  suite2.add(a);
  suite3.add(b);
  assert(!suite1.execute(output1).success());
  assert(suite2.execute(output2).success());
  assert(!suite3.execute(output3).success());
  return EXIT_SUCCESS;
} // end of main
