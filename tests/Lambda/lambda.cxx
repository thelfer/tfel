/*!
 * \file   tests/Lambda/lambda.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 déc 2006
 */

#include<cassert>
#include<cstdlib>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/lambda.hxx"
#include"TFEL/Math/functions.hxx"

int 
main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  tvector<3u> v(2.34);
  v(1) += 3.;
  v(2) += 1.2;

  assert(abs(x_(v)-2.34)<1.e-14); 
  assert(abs(y_(v)-5.34)<1.e-14); 
  assert(abs(z_(v)-3.54)<1.e-14); 
  
  assert(abs((x_+y_)(v)-7.68)    <1.e-14);
  assert(abs((x_-y_)(v)+3)       <1.e-14);
  assert(abs((x_*y_)(v)-12.4956) <1.e-3);
  assert(abs((x_/y_)(v)-0.438202)<1.e-3);

  assert(abs((3.2*x_+4.23*y_+2.3*z_)(v)-38.2182)<1.e-3);
  assert(abs((3.2*x_-4.23*y_+2.3*z_)(v)+6.9582)<1.e-3);
  assert(abs((3.2*x_*4.23*y_+2.3*z_)(v)-177.282)<1.e-3);
  assert(abs((3.2*x_/4.23*y_+2.3*z_)(v)-17.5949)<1.e-3);
  assert(abs((exp(3.2*sin(x_+x_)))(12.)-0.055142)<1.e-3);
  
  return EXIT_SUCCESS;
}
