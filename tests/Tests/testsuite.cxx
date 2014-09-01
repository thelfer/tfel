/*!
 * \file   tests/Tests/testsuite.cxx
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

#include"TFEL/Tests/TestSuite.hxx"
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
  using namespace tfel::tests;
  using namespace tfel::utilities;
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  shared_ptr<Test> a(new Wrapper1("test1"));
  shared_ptr<Test> b(new Wrapper2("test2"));
  TestSuite suite1("suite1");
  TestSuite suite2("suite2");
  TestSuite suite3("suite3");
  suite1.add(a);
  suite1.add(b);
  suite2.add(a);
  suite3.add(b);
  assert(!suite1.execute().success());
  assert(suite2.execute().success());
  assert(!suite3.execute().success());
  return EXIT_SUCCESS;
} // end of main
