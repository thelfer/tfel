/*! 
 * \file  IsConstCallable.cxx
 * \brief
 * \author Helfer Thomas
 * \date   15 févr. 2015
 */
    
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Metaprogramming/ResultOf.hxx"

struct ConstCallableType
{
  void operator()(double) const;
};

struct IsConstCallableTest final
  : public tfel::tests::TestCase
{
  IsConstCallableTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "IsConstCallableTest")
  {} // end of IsConstCallableTest
  virtual tfel::tests::TestResult
  execute() override
  {
    using namespace tfel::meta;
    TFEL_TESTS_STATIC_ASSERT((!IsConstCallable<std::string,double>::cond));
    TFEL_TESTS_STATIC_ASSERT((IsConstCallable<ConstCallableType,double>::cond));
    TFEL_TESTS_STATIC_ASSERT((IsConstCallable<ConstCallableType,int>::cond));
    TFEL_TESTS_STATIC_ASSERT((!IsConstCallable<ConstCallableType,std::string>::cond));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<ResultOf<std::string,double>::type,
					   InvalidType>::value));
    TFEL_TESTS_STATIC_ASSERT((std::is_same<ResultOf<ConstCallableType,double>::type,
					   void>::value));
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(IsConstCallableTest,
			  "IsConstCallableTest");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  auto& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("IsConstCallable.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

