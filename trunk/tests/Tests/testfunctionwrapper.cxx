/*!
 * \file   testfunctionwrapper.cxx
 * \author Helfer Thomas <helferthomas@free.fr>
 * \date   10 Apr 10
 * \brief a simple test of the TestFunctionWrapper class
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<cassert>
#include<memory>

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
  typedef TestFunctionWrapper<test1> Wrapper1;
  typedef TestFunctionWrapper<test2> Wrapper2;
  auto_ptr<Test> a(new Wrapper1("test1"));
  auto_ptr<Test> b(new Wrapper2("test2"));
  // first test
  assert(a->name()=="test1");
  assert(a->execute().success());
  // second test
  assert(b->name()=="test2");
  assert(!b->execute().success());
  return EXIT_SUCCESS;
} // end of main
