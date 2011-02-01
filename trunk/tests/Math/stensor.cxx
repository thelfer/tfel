#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/stensor.hxx"
#include "function_tests.hxx"

int main(void){

  using namespace std;  
  using namespace tfel::math;  

  typedef stensor<1> stensor1;
  typedef stensor<1,unsigned short> stensor2;

  stensor1 v1;
  stensor2 v2;
  stensor1 v3;

  v2(0)=2;
  v2(1)=1;
  v2(2)=5;

  v1(0)=4.;
  v1(1)=1.;
  v1(2)=25.;

  v3 = v1 + v2;

  assert(abs(v3(0)-6)<1.e-14);
  assert(abs(v3(1)-2)<1.e-14);
  assert(abs(v3(2)-30)<1.e-14);

  return EXIT_SUCCESS;

}
