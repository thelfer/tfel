/*!
  \file   IntegersSet.cxx
  \author Helfer Thomas
  \date   13 mai 2006
  \brief  
*/

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>

#include"TFEL/Utilities/IntegersSet.hxx"


#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct IntegerSetTest
  : public tfel::tests::TestCase
{
  IntegerSetTest()
    : tfel::tests::TestCase("IntegerSetTest")
  {} // end of IntegerSetTest
  tfel::tests::TestResult
  execute()
  {
    using namespace tfel::utilities;
    IntegersSet<> test(12,14);
    TFEL_TESTS_CHECK_EQUAL(test.size(),3);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),1);
    test.add(12);
    TFEL_TESTS_CHECK_EQUAL(test.size(),3);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),1);
    TFEL_TESTS_CHECK_EQUAL(*(test.begin()),12);
    TFEL_TESTS_CHECK_EQUAL(test.position(13),1);
    test.add(17);
    TFEL_TESTS_CHECK_EQUAL(test.size(),4);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),2);
    TFEL_TESTS_CHECK_EQUAL(*(test.begin()),12);
    TFEL_TESTS_CHECK_EQUAL(test.position(17),3);
    test.add(11);
    TFEL_TESTS_CHECK_EQUAL(test.size(),5);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),2);
    test.add(15);
    TFEL_TESTS_CHECK_EQUAL(test.size(),6);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),2);
    test.add(16);
    TFEL_TESTS_CHECK_EQUAL(test.size(),7);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),1);
    test.add(18);
    TFEL_TESTS_CHECK_EQUAL(test.size(),8);
    TFEL_TESTS_CHECK_EQUAL(test.block_number(),1);
    TFEL_TESTS_CHECK_EQUAL(*(test.begin()),11);
    TFEL_TESTS_CHECK_EQUAL(test.position(13),2);
    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(IntegerSetTest,"Name");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
