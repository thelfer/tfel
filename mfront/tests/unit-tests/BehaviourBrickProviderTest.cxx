/*!
 * \file   BehaviourBrickProviderTest.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   26 juin 2015
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
#include<stdexcept>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"MFront/BehaviourBrick/Provider.hxx"

struct BehaviourBrickProviderTest final
  : public tfel::tests::TestCase
{
  BehaviourBrickProviderTest()
    : tfel::tests::TestCase("MFront",
			    "BehaviourBrickProviderTest")
  {} // end of BehaviourBrickProviderTest

  tfel::tests::TestResult
  execute() override
  {
    using namespace mfront;
    using namespace mfront::bbrick;
    this->test<MaterialPropertyProvider>(ProviderIdentifier::MATERIALPROPERTY);
    this->test<StateVariableProvider>(ProviderIdentifier::STATEVARIABLE);
    this->test<AuxiliaryStateVariableProvider>(ProviderIdentifier::AUXILIARYSTATEVARIABLE);
    this->test<IntegrationVariableProvider>(ProviderIdentifier::INTEGRATIONVARIABLE);
    this->test<LocalVariableProvider>(ProviderIdentifier::LOCALVARIABLE);
    this->test2<StaticVariableProvider>(ProviderIdentifier::STATICVARIABLE);
    this->test2<ParameterProvider>(ProviderIdentifier::PARAMETER);
    //    this->test3();
    return this->result;
  } // end of execute
 private:
  using ProviderIdentifier =  mfront::bbrick::ProviderIdentifier;
  /*!
   * \brief a simple test to see if the ProviderType::getIdentifier method
   * returns the expected identifier
   */
  template<typename ProviderType>
  void test(const ProviderIdentifier i){
    const auto p  = ProviderType{"real","test","test",1u};
    TFEL_TESTS_ASSERT(p.getIdentifier()==i);
  } // end of test
  /*!
   * \brief a simple test to see if the ProviderType::getIdentifier method
   * returns the expected identifier
   */
  template<typename ProviderType>
  void test2(const ProviderIdentifier i){
    const auto p  = ProviderType{"real","test","test"};
    TFEL_TESTS_ASSERT(p.getIdentifier()==i);
  } // end of test
  // /*!
  //  * \brief a simple test to see if the
  //  * MaterialLawProvider::getIdentifier method returns the expected
  //  * identifier.
  //  */
  // void test3(void){
  //   using namespace mfront::bbrick;
  //   const auto p = MaterialLawProvider{};
  //   TFEL_TESTS_ASSERT(p.getIdentifier()==ProviderIdentifier::MATERIALLAW);
  // } // end of test3
};

TFEL_TESTS_GENERATE_PROXY(BehaviourBrickProviderTest,
			  "BehaviourBrickProviderTest");

int main(void)
{
  using namespace std;
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("BehaviourBrickProvider.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
