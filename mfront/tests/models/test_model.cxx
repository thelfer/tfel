/*!
 * \file   test_model.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   23 juin 2016
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<iostream>
#include<stdexcept>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Material/MaterialException.hxx"
#include"MFront/B4C_ConcentrationModel.hxx"
#include"MFront/SiC_IrradiationSwellingModel_GoFaster.hxx"
#include"MFront/UO2_Shrinkage_RAPHAEL2008.hxx"
#include"MFront/FastNeutronFluence.hxx"
#include"MFront/NeutronFluence.hxx"

struct B4CConcentrationModelTest final
  : public tfel::tests::TestCase
{
  B4CConcentrationModelTest()
    : tfel::tests::TestCase("MFront/Model","B4CConcentrationModelTest")
  {} // end of B4CConcentrationModelTest
  virtual tfel::tests::TestResult
  execute(void) override
  {
    mfront::B4C_ConcentrationModel<double> cm;
    double bu  = 0;
    double b10 = 0;
    TFEL_TESTS_CHECK_THROW((cm.Compute(bu,b10,-1.,0.,0.,0.,3600)),
			   tfel::material::OutOfBoundsException);
    return this->result;
  } // end of execute
  virtual ~B4CConcentrationModelTest() = default;
};

TFEL_TESTS_GENERATE_PROXY(B4CConcentrationModelTest,"B4CConcentrationModelTest");

int main(void){
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("test-model.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}
