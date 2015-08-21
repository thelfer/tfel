/*! 
 * \file  fsarray.cxx
 * \brief
 * \author Helfer Thomas
 * \date   07 janv. 2015
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cmath>
#include<cstdlib>
#include<cassert>
#include<iostream>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/stensor.hxx"

template<int N>
struct structify
{
  static constexpr int value = N; 
};

struct FSArrayTest final
  : public tfel::tests::TestCase
{
  FSArrayTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "FSArrayTest")
  {} // end of FSArrayTest
  virtual tfel::tests::TestResult
  execute() override
  {
#if (!defined _MSC_VER) && (!defined _LIBCPP_VERSION)
    this->test1();
    this->test2();
    this->test3();
#endif
    return this->result;
  } // end of execute
#if (!defined _MSC_VER) && (!defined _LIBCPP_VERSION)
  void test1(void)
  {
    using namespace std;
    using namespace tfel::math;
    constexpr fsarray<3u,int> a1(0);
    constexpr fsarray<3u,int> a2{a1};
    constexpr fsarray<3u,int> a3({3,2,6});
    constexpr fsarray<3u,int> a4{a3};
    constexpr fsarray<3u,int> a5 = {5,3,9};
    constexpr fsarray<3u,int> a6{a5};
    TFEL_TESTS_ASSERT(structify<a1[0]>::value==0);
    TFEL_TESTS_ASSERT(structify<a1[1]>::value==0);
    TFEL_TESTS_ASSERT(structify<a1[2]>::value==0);
    TFEL_TESTS_ASSERT(structify<a2[0]>::value==0);
    TFEL_TESTS_ASSERT(structify<a2[1]>::value==0);
    TFEL_TESTS_ASSERT(structify<a2[2]>::value==0);
    TFEL_TESTS_ASSERT(structify<a3[0]>::value==3);
    TFEL_TESTS_ASSERT(structify<a3[1]>::value==2);
    TFEL_TESTS_ASSERT(structify<a3[2]>::value==6);
    TFEL_TESTS_ASSERT(structify<a4[0]>::value==3);
    TFEL_TESTS_ASSERT(structify<a4[1]>::value==2);
    TFEL_TESTS_ASSERT(structify<a4[2]>::value==6);
    TFEL_TESTS_ASSERT(structify<a5[0]>::value==5);
    TFEL_TESTS_ASSERT(structify<a5[1]>::value==3);
    TFEL_TESTS_ASSERT(structify<a5[2]>::value==9);
    TFEL_TESTS_ASSERT(structify<a6[0]>::value==5);
    TFEL_TESTS_ASSERT(structify<a6[1]>::value==3);
    TFEL_TESTS_ASSERT(structify<a6[2]>::value==9);
    TFEL_TESTS_ASSERT(*(a5.begin()+0)==5);
    TFEL_TESTS_ASSERT(*(a5.begin()+1)==3);
    TFEL_TESTS_ASSERT(*(a5.begin()+2)==9);
    TFEL_TESTS_ASSERT(*(a5.rbegin()+0)==9);
    TFEL_TESTS_ASSERT(*(a5.rbegin()+1)==3);
    TFEL_TESTS_ASSERT(*(a5.rbegin()+2)==5);
  } // end of test1
  void test2(void)
  {
    using namespace tfel::math;
    typedef unsigned short usint;
    constexpr fsarray<3u,unsigned short> a1{{usint(2),usint(3),usint(4)}};
    constexpr fsarray<3u,int> a2{a1};
    TFEL_TESTS_ASSERT(structify<a1[0]>::value==2);
    TFEL_TESTS_ASSERT(structify<a1[1]>::value==3);
    TFEL_TESTS_ASSERT(structify<a1[2]>::value==4);
    TFEL_TESTS_ASSERT(structify<a2[0]>::value==2);
    TFEL_TESTS_ASSERT(structify<a2[1]>::value==3);
    TFEL_TESTS_ASSERT(structify<a2[2]>::value==4);
  } // end of test2
  void test3(void)
  {
    using namespace tfel::math;
    typedef unsigned short usint;
    constexpr fsarray<3u,unsigned short> a1{{usint(2),usint(3),usint(4)}};
    constexpr fsarray<3u,int> a2{a1};
  }  
#endif
};

TFEL_TESTS_GENERATE_PROXY(FSArrayTest,"FSArrayTest");

int main(void){
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("fsarray.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

