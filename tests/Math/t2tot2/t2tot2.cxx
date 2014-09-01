/*!
 * \file   tests/Math/t2tot2/t2tot2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   30 jan 2007
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

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/t2tot2.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct T2tot2Test_1D
  : public tfel::tests::TestCase
{
  T2tot2Test_1D()
    : tfel::tests::TestCase("TFEL/Math",
			    "T2tot2Test-1D")
  {} // end of STensorTest8
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    t2tot2<3> t1(1.5);
    t2tot2<3> t2(4.);
    t2tot2<3> t3;
    t3 = t1+0.5*t2;
    TFEL_TESTS_ASSERT(abs(t3(0,0)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(1,0)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(2,0)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(3,0)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(0,1)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(1,1)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(2,1)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(3,1)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(0,2)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(1,2)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(2,2)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(3,2)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(0,3)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(1,3)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(2,3)-3.5)<1.e-14);
    TFEL_TESTS_ASSERT(abs(t3(3,3)-3.5)<1.e-14);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(T2tot2Test_1D,"T2tot2Test-1D");

int main(void){
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("t2tot2.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
