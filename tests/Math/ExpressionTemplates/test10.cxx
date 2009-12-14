/*!
 * \file   test10.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   17 oct 2006
 */

#include"test10-header.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;

  vector<double> v1(4);
  vector<double> v2(4);
  vector<double> v3(4);

  v1(0) = 12.12;
  v1(1) = 12.234;
  v1(2) = 22.4322;
  v1(3) = 2.2112;
  v2(0) = 2.;
  v2(1) = 4.;
  v2(2) = 5.;
  v2(3) = 6.;
  
  //  function(v1);
  function(1.25*v1);
  function(cos(v2));
  function(v2*v1);
  //  function(cos(v2)*v1);
  
  //  function(Exp(Sin(1.25*v1-cos(v2)*v1))/3.);

  cout << "Octave result : " <<  0.88321 << "," << 0.12724 << "," <<  0.45399 <<  "," << 0.60610 << endl;


  return EXIT_SUCCESS;

}
