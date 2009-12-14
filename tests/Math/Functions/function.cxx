/*!
 * \file   function.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#include<iostream>
#include<cstdlib>

#include "Math/functions.hxx"
#include "Utilities/Name.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  USING_TFEL_FUNCTIONS;
  
  cout << name(sin+cos)                      << " " << sizeof(sin+cos)      << endl;
  cout << name(exp[sin])                     << " " << sizeof(exp[sin])     << endl;
  cout << name(exp[sin+cos])                 << " " << sizeof(exp[sin+cos]) << endl;
//   cout << name(3.42*exp[2.5*sin-3.*cos*tan]) << " " 
//        << sizeof(3.42*exp[2.5*sin-3.*cos*tan]) << endl;

  // Comparison with Octave results
  cout << (sin+cos)(3.4312)      << ", " << -1.2439  << endl;
  cout << (-(sin+cos))(3.4312)   << ", " <<  1.2439  << endl;
  cout << (exp[sin])(3.4312)     << ", " <<  0.75158 << endl;
  cout << (exp[sin+cos])(3.4312) << ", " <<  0.28825 << endl;
//   cout << (3.42*exp[2.5*sin-3.*cos*tan])(3.4312) << ", " << 3.9449 << endl;

  return EXIT_SUCCESS;

}
