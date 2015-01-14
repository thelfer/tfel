/*!
 * \file   tests/Math/qt.cxx
 * \brief  
 * 
 * \author Helfer thomas
 * \date   06 Jun 2006
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
#include<cmath>
#include<cassert>

#include"TFEL/Math/qt.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct qtTest
  : public tfel::tests::TestCase
{
  qtTest()
    : tfel::tests::TestCase("TFEL/Math","qt")
  {} // end of qtTest
  tfel::tests::TestResult
  execute()
  {
    using namespace tfel::math; 
    constexpr qt<Mass> m1(100.);
    constexpr qt<Mass> m2(100.);
    constexpr qt<Mass> m3 = m1+0.5*m2;
    constexpr qt<Acceleration,unsigned short> a(2);
    constexpr qt<Force> f = m1*a;
    TFEL_TESTS_ASSERT(std::abs(m3.getValue()-150.)<1.e-14);
    TFEL_TESTS_ASSERT(std::abs(f.getValue()-200.)<1.e-14);
    TFEL_TESTS_ASSERT((std::abs(std::cos(qt<NoUnit>(12.))-std::cos(12.))<1.e-14));
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(qtTest,"qtTest");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("qt.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

