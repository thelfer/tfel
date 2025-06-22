/*!
 * \file   BehaviourBrickProviderTest.cxx
 * \brief
 * \author Thomas Helfer
 * \date   26 juin 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"

#include "MFront/BehaviourBrick/Provider.hxx"
#include "MFront/BehaviourBrick/Requirement.hxx"
#include "MFront/BehaviourBrick/RequirementManager.hxx"

struct BehaviourBrickProviderTest final : public tfel::tests::TestCase {
  BehaviourBrickProviderTest()
      : tfel::tests::TestCase("MFront", "BehaviourBrickProviderTest") {
  }  // end of BehaviourBrickProviderTest

  tfel::tests::TestResult execute() override {
    using namespace mfront;
    using namespace mfront::bbrick;
    this->test<MaterialPropertyProvider>(ProviderIdentifier::MATERIALPROPERTY);
    this->test<IntegrationVariableProvider>(
        ProviderIdentifier::INTEGRATIONVARIABLE);
    this->test<AuxiliaryStateVariableProvider>(
        ProviderIdentifier::AUXILIARYSTATEVARIABLE);
    this->test<IntegrationVariableProvider>(
        ProviderIdentifier::INTEGRATIONVARIABLE);
    this->test<LocalVariableProvider>(ProviderIdentifier::LOCALVARIABLE);
    this->test2<StaticVariableProvider>(ProviderIdentifier::STATICVARIABLE);
    this->test2<ParameterProvider>(ProviderIdentifier::PARAMETER);
    //    this->test3();
    this->test4();
    this->test5();
    return this->result;
  }  // end of execute
 private:
  using ProviderIdentifier = mfront::bbrick::ProviderIdentifier;
  /*!
   * \brief a simple test to see if the ProviderType::getIdentifier method
   * returns the expected identifier
   */
  template <typename ProviderType>
  void test(const ProviderIdentifier i) {
    const ProviderType p{"real", "test", "test", 1u};
    TFEL_TESTS_ASSERT(p.getIdentifier() == i);
  }  // end of test
  /*!
   * \brief a simple test to see if the ProviderType::getIdentifier method
   * returns the expected identifier
   */
  template <typename ProviderType>
  void test2(const ProviderIdentifier i) {
    const ProviderType p{"real", "test", "test"};
    TFEL_TESTS_ASSERT(p.getIdentifier() == i);
  }  // end of test
  // /*!
  //  * \brief a simple test to see if the
  //  * MaterialLawProvider::getIdentifier method returns the expected
  //  * identifier.
  //  */
  // void test3(){
  //   using namespace mfront::bbrick;
  //   const auto p = MaterialLawProvider{};
  //   TFEL_TESTS_ASSERT(p.getIdentifier()==ProviderIdentifier::MATERIALLAW);
  // } // end of test3
  void test4() {
    using namespace mfront::bbrick;
    const MaterialPropertyProvider p{"stress", "young", "YoungModulus", 1u};
    TFEL_TESTS_ASSERT(p.name == "young");
    TFEL_TESTS_ASSERT(p.ename == "YoungModulus");
    TFEL_TESTS_ASSERT(p.type == "stress");
    // unsupported type
    TFEL_TESTS_CHECK_THROW(
        (MaterialPropertyProvider{"invalid_type", "young", "YoungModulus", 1u}
             .name == "young"),
        std::runtime_error);
    // everything is okay
    TFEL_TESTS_ASSERT((p.handleRequirement(
        Requirement{"stress",
                    "YoungModulus",
                    1u,
                    {ProviderIdentifier::MATERIALPROPERTY,
                     ProviderIdentifier::INTEGRATIONVARIABLE}},
        false)));
    // the array size does not match
    TFEL_TESTS_CHECK_THROW(
        p.handleRequirement(Requirement{"stress",
                                        "YoungModulus",
                                        2u,
                                        {ProviderIdentifier::MATERIALPROPERTY}},
                            false),
        std::runtime_error);
    // requirement does not allow a MaterialProperty provider
    TFEL_TESTS_CHECK_THROW(
        p.handleRequirement(
            Requirement{"stress",
                        "YoungModulus",
                        1u,
                        {ProviderIdentifier::INTEGRATIONVARIABLE}},
            false),
        std::runtime_error);
    // type does not match
    TFEL_TESTS_CHECK_THROW(
        p.handleRequirement(Requirement{"StressStensor",
                                        "YoungModulus",
                                        1u,
                                        {ProviderIdentifier::MATERIALPROPERTY}},
                            false),
        std::runtime_error);
    // units does not match, but this is allowed
    TFEL_TESTS_ASSERT((p.handleRequirement(
        Requirement{
            "real", "YoungModulus", 1u, {ProviderIdentifier::MATERIALPROPERTY}},
        false)));
    // units does not match, but this is not allowed
    TFEL_TESTS_CHECK_THROW(
        p.handleRequirement(Requirement{"real",
                                        "YoungModulus",
                                        1u,
                                        {ProviderIdentifier::MATERIALPROPERTY}},
                            true),
        std::runtime_error);
  }
  void test5() {
    using namespace mfront::bbrick;
    RequirementManager m;
    m.addMaterialPropertyProvider("real", "young", "YoungModulus", 1u);
    TFEL_TESTS_ASSERT(m.getProvider("YoungModulus").getIdentifier() ==
                      ProviderIdentifier::MATERIALPROPERTY);
    TFEL_TESTS_CHECK_THROW(m.getProvider("unknown"), std::runtime_error);
    // young modulus has already been used
    TFEL_TESTS_CHECK_THROW(
        m.addIntegrationVariableProvider("real", "young", "YoungModulus", 1u),
        std::runtime_error);
  }
};

TFEL_TESTS_GENERATE_PROXY(BehaviourBrickProviderTest,
                          "BehaviourBrickProviderTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace tfel::tests;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(std::cout);
  manager.addXMLTestOutput("BehaviourBrickProvider.xml");
  TestResult r = manager.execute();
  if (!r.success()) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}  // end of main
