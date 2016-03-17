/*! 
 * \file  tests/Math/tvector6.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/Math/tvector.hxx"

struct TVector6 final
  : public tfel::tests::TestCase
{
  TVector6()
    : tfel::tests::TestCase("TFEL/Math",
			    "TVector6")
  {} // end of TVector6

  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace tfel::fsalgo;
    using namespace tfel::math;
    int vx[3u] = {1,0,0};
    int vy[3u] = {0,1,0};
    int vz[3u] = {0,0,1};
    tvector<3u,int> x(vx);
    tvector<3u,int> y(vy);
    tvector<3u,int> z(vz);
    TFEL_TESTS_ASSERT(equal<2u>::exe(z.begin(),cross_product(x,y).begin()));
    TFEL_TESTS_ASSERT(equal<2u>::exe(x.begin(),cross_product(y,z).begin()));
    TFEL_TESTS_ASSERT(equal<2u>::exe(y.begin(),cross_product(z,x).begin()));
#pragma message("HERE")
    // TFEL_TESTS_ASSERT(equal<2u>::exe((-x).begin(),
    // 				     cross_product(z,y).begin()));
    // TFEL_TESTS_ASSERT(equal<2u>::exe((-y).begin(),
    // 				     cross_product(x,z).begin()));
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TVector6,
			  "TVector6");

/* coverity [root_function]*/
int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("tvector6.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

