#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/General/Complex.hxx"

struct TensorTest
  : public tfel::tests::TestCase
{
  TensorTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "TensorTest")
  {} // end of TensorTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;  
    using namespace tfel::math;  
    typedef tensor<1>                tensor1;
    typedef tensor<1,unsigned short> tensor2;
    tensor1 v1;
    tensor2 v2;
    tensor1 v3;
    v2(0)=2;
    v2(1)=1;
    v2(2)=5;
    v1(0)=4.;
    v1(1)=1.;
    v1(2)=25.;
    v3 = v1 + v2;
    TFEL_TESTS_ASSERT(abs(v3(0)-6)<1.e-14);
    TFEL_TESTS_ASSERT(abs(v3(1)-2)<1.e-14);
    TFEL_TESTS_ASSERT(abs(v3(2)-30)<1.e-14);
    return this->result;
  } // end of execute
};

struct TensorTest2
  : public tfel::tests::TestCase
{
  TensorTest2()
    : tfel::tests::TestCase("TFEL/Math",
			    "TensorTest2")
  {} // end of TensorTest2
  tfel::tests::TestResult
  execute()
  {
    using namespace tfel::math;
    using namespace std;
    tensor<1,Complex<float> > s;
    tensor<1,Complex<float> > s2;
    s(0) = Complex<float>(2.f);
    s(1) = Complex<float>(3.1415929f);
    s(2) = Complex<float>(4231.421f);
    s2 = 0.5f*s;
    TFEL_TESTS_ASSERT((abs(real(s2(0)-Complex<float>(1.f)))<1.e-3f));
    TFEL_TESTS_ASSERT(abs(imag(s2(0)))<1.e-3);
    TFEL_TESTS_ASSERT((abs(real(s2(1)-Complex<float>(1.5708f)))<1.e-3f));
    TFEL_TESTS_ASSERT(abs(imag(s2(1)))<1.e-3);
    TFEL_TESTS_ASSERT((abs(real(s2(2)-Complex<float>(2115.7f)))<1.f));
    TFEL_TESTS_ASSERT(abs(imag(s2(2)))<1.e-3f);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TensorTest,"TensorTest");
TFEL_TESTS_GENERATE_PROXY(TensorTest2,"TensorTest2");

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
