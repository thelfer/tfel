/*! 
 * \file   tests/Utilities/MultipleDeque.cxx
 * \brief
 * \author Helfer Thomas
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<cmath>
#include<vector>

#include"TFEL/Utilities/MultipleDeque.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct MultipleDequeTest
  : public tfel::tests::TestCase
{
  MultipleDequeTest()
    : tfel::tests::TestCase("TFEL/Utilities","MultipleDequeTest")
  {} // end of MultipleDequeTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::meta;

    typedef GenerateTypeList<string,double,int>::type MyList;
    tfel::utilities::MultipleDeque<MyList> mv;
  
    tfel::utilities::MultipleDeque<MyList>::iterator<double> p;
    tfel::utilities::MultipleDeque<MyList>::reverse_iterator<double> p1;

    TFEL_TESTS_ASSERT(sizeof(mv)==sizeof(deque<string>)+sizeof(deque<double>)+sizeof(deque<int>));

    TFEL_TESTS_ASSERT(mv.empty<string>()==true);
    mv.push_back<string>("Youpie");
    mv.push_back<string>("Choupie est la plus belle");
    TFEL_TESTS_ASSERT(mv.empty<string>()==false);
    TFEL_TESTS_ASSERT(mv.size<string>()==2);

    mv.push_back<double>(3.);
    mv.push_back<double>(5.68273);
    mv.push_back<double>(1.2);

    TFEL_TESTS_ASSERT(mv.size<double>()==3);

    TFEL_TESTS_ASSERT(mv.front<string>()=="Youpie");

    mv.front<string>()="Test";
    TFEL_TESTS_ASSERT(mv.front<string>()=="Test");

    mv.clear<string>();
    TFEL_TESTS_ASSERT(mv.empty<string>()==true);

    p=mv.begin<double>();
    TFEL_TESTS_ASSERT(abs(*p-3.)<1.e-14);
    ++p;
    TFEL_TESTS_ASSERT(abs(*p-5.68273)<1.e-14);
    ++p;
    TFEL_TESTS_ASSERT(abs(*p-1.2)<1.e-14);
    ++p;
    TFEL_TESTS_ASSERT(p==mv.end<double>());
 
    p1=mv.rbegin<double>();
    TFEL_TESTS_ASSERT(abs(*p1-1.2)<1.e-14);
    ++p1;
    TFEL_TESTS_ASSERT(abs(*p1-5.68273)<1.e-14);
    ++p1;
    TFEL_TESTS_ASSERT(abs(*p1-3.)<1.e-14);
    ++p1;
    TFEL_TESTS_ASSERT(p1==mv.rend<double>());
    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(MultipleDequeTest,"MultipleDeque");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("MultipleDeque.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
