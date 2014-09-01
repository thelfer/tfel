/*!
 * \file   tests/Math/parser9.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   21 sep 2007
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

#include"TFEL/Math/Evaluator.hxx"

struct ParserTest
  : public tfel::tests::TestCase
{
  ParserTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "ParserTest")
  {} // end of ParserTest

  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::math;
    using namespace tfel::math::parser;
    vector<string> var(1,"x");
    shared_ptr<ExternalFunctionManager> manager(new ExternalFunctionManager());
    manager->operator[]("u") = shared_ptr<ExternalFunction>(new Evaluator(var,"cos(x)",manager));
    manager->operator[]("g") = shared_ptr<ExternalFunction>(new Evaluator(var,"f(x)",manager));
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"f(x)",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"g(x)",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"g(f(x))",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"g(u(g(x)))",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"g(u(f(x)))",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    manager->operator[]("f") = shared_ptr<ExternalFunction>(new Evaluator(var,"g(f(x)+1)",manager));
    manager->operator[]("g") = shared_ptr<ExternalFunction>(new Evaluator(var,"sinh(x)",manager));
    TFEL_TESTS_CHECK_THROW(manager->operator[]("f")->checkCyclicDependency("f"),exception);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(ParserTest,
			  "ParserTest9");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Parser9.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
