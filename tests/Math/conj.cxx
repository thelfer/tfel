/*!
 * \file   conj.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Sep 2006
 */

#include <iostream>
#include <cstdlib>

#include "Math/General/Complex.hxx"
#include "Math/General/BasicOperations.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  cout << conj(12) << endl;
  cout << conj(Complex<int>(12,13)) << endl;

  return EXIT_SUCCESS;
}
