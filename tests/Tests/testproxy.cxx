/*!
 * \file   tests/Tests/testproxy.cxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief a simple test of the TestSuite class
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
#include<stdexcept>

#include<fstream>

#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"
#include"TFEL/Tests/TestFunctionWrapper.hxx"

#if !( defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#define TFEL_TESTS_STATIC static
#else
#define TFEL_TESTS_STATIC
#endif

TFEL_TESTS_STATIC bool test1(void)
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
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(f,false);
  TestResult r = manager.execute();
  assert(r.success());
  return EXIT_SUCCESS;
} // end of main
