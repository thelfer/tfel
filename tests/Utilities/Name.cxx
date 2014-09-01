/*!
 * \file   tests/Utilities/Name.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   20 jui 2006
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

#include<cassert>
#include<cstdlib>
#include<cassert>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/TypeTraits/Promote.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

struct NameTest
  : public tfel::tests::TestCase
{
  NameTest()
    : tfel::tests::TestCase("TFEL/Utilities","NameTest")
  {} // end of NameTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::typetraits;
    using namespace tfel::math;
    using namespace tfel::utilities;
    TFEL_TESTS_CHECK_EQUAL((Name<Promote<float,long double>::type>::getName()),"long double");
    TFEL_TESTS_CHECK_EQUAL((Name<Promote<long double,float>::type>::getName()),"long double");
    TFEL_TESTS_CHECK_EQUAL((Name<ResultType<long double,float,OpMult>::type>::getName()),"long double");
    TFEL_TESTS_CHECK_EQUAL((Name<ResultType<float,long double,OpMult>::type>::getName()),"long double");
    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(NameTest,"Name");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Name.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
