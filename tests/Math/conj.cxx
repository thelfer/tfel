/*!
 * \file   conj.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Sep 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cmath>
#include <cstdlib>
#include <cassert>

#include"TFEL/Math/General/Complex.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

#include<iostream>

int main(void)
{
  using namespace tfel::math;

  assert(real(conj(12)-Complex<int>(12,0))==0);
  assert(imag(conj(12)-Complex<int>(12,0))==0);
  assert(real(conj(Complex<int>(0,12))-Complex<int>(0,-12))==0);
  assert(imag(conj(Complex<int>(0,12))-Complex<int>(0,-12))==0);

  return EXIT_SUCCESS;
}
