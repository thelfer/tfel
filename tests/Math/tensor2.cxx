#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Utilities/ToString.hxx"

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/Tensor/TVFS.hxx"
#include"TFEL/Math/General/Complex.hxx"

struct RightCauchyGreenTensorTest
  : public tfel::tests::TestCase
{
  RightCauchyGreenTensorTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "RightCauchyGreenTensorTest")
  {} // end of RightCauchyGreenTensorTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;  
    using namespace tfel::math;  
    tensor<2,double> F(real(0));
    F(0)=1.300;
    F(1)=0.650;
    F(2)=0.;
    F(3)=-0.3750;
    F(4)=0.750;
    const stensor<2,double> e = computeRightCauchyGreenTensor(F);
    const stensor<2,double> U = square_root(e);
    return this->result;
  } // end of execute
};

struct RightCauchyGreenTensorTest2
  : public tfel::tests::TestCase
{
  RightCauchyGreenTensorTest2()
    : tfel::tests::TestCase("TFEL/Math",
			    "RightCauchyGreenTensorTest2")
  {} // end of RightCauchyGreenTensorTest2
  tfel::tests::TestResult
  execute()
  {
    using namespace std;  
    using namespace tfel::math;  
    tensor<3,double> F(real(0));
    F(0)=1;
    F(1)=1;
    F(2)=1.5;
    F(4)=-0.333;
    F(3)=0.495;
    F(5)=0.5;
    F(6)=0.959;
    F(7)=-0.247;
    const stensor<3,double> e = computeRightCauchyGreenTensor(F);
    const stensor<3,double> U = square_root(e);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorTest,"RightCauchyGreenTensorTest");
TFEL_TESTS_GENERATE_PROXY(RightCauchyGreenTensorTest2,"RightCauchyGreenTensorTest2");

int main(void){
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Tensor.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
