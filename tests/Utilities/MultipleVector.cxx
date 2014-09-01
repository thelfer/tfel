/*! 
 * \file   tests/Utilities/MultipleVector.cxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<cmath>
#include<vector>

#include"TFEL/Utilities/MultipleVector.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct MultipleVectorTest
  : public tfel::tests::TestCase
{
  MultipleVectorTest()
    : tfel::tests::TestCase("TFEL/Utilities","MultipleVectorTest")
  {} // end of MultipleVectorTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::meta;
    using namespace tfel::utilities;

    typedef GenerateTypeList<string,double,int>::type MyList;
    MultipleVector<MyList> mv;
  
    MultipleVector<MyList>::const_iterator<double> p;
    MultipleVector<MyList>::reverse_iterator<double> p1;

    TFEL_TESTS_ASSERT(sizeof(mv)==sizeof(vector<string>)+sizeof(vector<double>)+sizeof(vector<int>));

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
    p-=3;
    TFEL_TESTS_ASSERT(abs(*p-3.)<1.e-14);

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

TFEL_TESTS_GENERATE_PROXY(MultipleVectorTest,"MultipleVector");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("MultipleVector.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
