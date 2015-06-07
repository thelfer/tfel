/*!
 * \file   tests/Tests/stdstreamtestoutput.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief a simple test of the StdStreamTestOutput class
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>

#include"TFEL/Tests/TestSuite.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"
#include"TFEL/Tests/StdStreamTestOutput.hxx"

#if !( defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#define TFEL_TESTS_STATIC static
#else
#define TFEL_TESTS_STATIC
#endif

TFEL_TESTS_STATIC bool test1(void)
{
  return true;
}

TFEL_TESTS_STATIC bool test2(void)
{
  return false;
}

int main(void)
{
  using namespace tfel::tests;
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  std::shared_ptr<Test> a(new Wrapper1("test1"));
  std::shared_ptr<Test> b(new Wrapper2("test2"));
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
