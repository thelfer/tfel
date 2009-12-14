/*!
 * \file   lambda.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 déc 2006
 */

#include"lambda-header.hxx"

int 
main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  tvector<3u> v(2.34);
  v(1) += 3.;
  v(2) += 1.2;

  cout << x_(v) << endl; 
  cout << y_(v) << endl; 
  cout << z_(v) << endl; 
  
  cout << (x_+y_)(v) << endl;
  cout << (x_-y_)(v) << endl;
  cout << (x_*y_)(v) << endl;
  cout << (x_/y_)(v) << endl;

  cout << (3.2*x_+4.23*y_+2.3*z_)(v) << endl;
  cout << (3.2*x_-4.23*y_+2.3*z_)(v) << endl;
  cout << (3.2*x_*4.23*y_+2.3*z_)(v) << endl;
  cout << (3.2*x_/4.23*y_+2.3*z_)(v) << endl;

  cout << (exp(3.2*sin(x_+x_)))(12.) << "," << 0.17960 << endl;

  printLambdaTraits(exp(3.2*sin(x_+x_)));
  printLambdaTraits(exp(3.2*sin(x_)+y_*x_));
  printLambdaTraits(2.*z_*z_*exp(3.2*sin(x_)+y_));
  
  return EXIT_SUCCESS;
}
