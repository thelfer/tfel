/*! 
 * \file  tests/Math/stensor_changeBasis.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 mai 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cmath>
#include<fstream>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"

struct StensorChangeBasisTest
  : public tfel::tests::TestCase
{
  StensorChangeBasisTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "StensorChangeBasisTest")
  {} // end of StensorChangeBasisTest

  tfel::tests::TestResult
  execute()
  {
    using namespace tfel::math;
    typedef double real;
    const real eps = 1.e-4;
    const real phi = M_PI/7.;
    const real co = cos(phi);
    const real si = sin(phi);
    {
      // test 2D
      tmatrix<3u,3u,real> r(real(0));
      const real sv[4] = {20e7,5.12e7,32.98e7,0.};
      stensor<2u,real> s(sv);
      r(0,0) = co;
      r(0,1) = -si;
      r(2,2) = 1.;
      r(1,0) = si;
      r(1,1) = co;
      s.changeBasis(r);
      TFEL_TESTS_ASSERT(abs(s[0]-(co*co*sv[0]+si*si*sv[1]))<eps);
      TFEL_TESTS_ASSERT(abs(s[1]-(si*si*sv[0]+co*co*sv[1]))<eps);
      TFEL_TESTS_ASSERT(abs(s[2]-sv[2])<eps);
      TFEL_TESTS_ASSERT(abs(s[3]+(sqrt(real(2))*si*co*(sv[0]-sv[1])))<eps);
    }
    {
      // test 3D
      tmatrix<3u,3u,real> r(real(0));
      const real sv[6] = {20e7,5.12e7,32.98e7,
			  0,0,0};
      stensor<3u,real> s(sv);
      r(0,0) = co;
      r(0,2) = -si;
      r(1,1) = 1.;
      r(2,0) = si;
      r(2,2) = co;
      s.changeBasis(r);
      TFEL_TESTS_ASSERT(abs(s[0]-(co*co*sv[0]+si*si*sv[2]))<eps);
      TFEL_TESTS_ASSERT(abs(s[1]-sv[1])<eps);
      TFEL_TESTS_ASSERT(abs(s[2]-(si*si*sv[0]+co*co*sv[2]))<eps);
      TFEL_TESTS_ASSERT(abs(s[3])<eps);
      TFEL_TESTS_ASSERT(abs(s[4]+(sqrt(real(2))*si*co*(sv[0]-sv[2])))<eps);
      TFEL_TESTS_ASSERT(abs(s[5])<eps);
    }
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(StensorChangeBasisTest,
			  "StensorChangeBasisTest");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("StensorChangeBasis.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

