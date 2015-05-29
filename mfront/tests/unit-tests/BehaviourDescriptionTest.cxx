/*! 
 * \file   mfront/tests/unit-tests/BehaviourDescriptionTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief  29 may 2015
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

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct BehaviourDescriptionTest
  : public tfel::tests::TestCase
{
  BehaviourDescriptionTest()
    : tfel::tests::TestCase("MFront",
			    "BehaviourDescriptionTest")
  {} // end of BehaviourDescriptionTest

  tfel::tests::TestResult
  execute()
  {
    return this->result;
  } // end of execute
};


TFEL_TESTS_GENERATE_PROXY(BehaviourDescriptionTest,
			  "BehaviourDescriptionTest");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("BehaviourDescription.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
