/*!
 * \file   Range.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2006
 */

#include<iostream>
#include<cstdlib>

#include"Math/Vector/Range.hxx"
#include"Math/vector.hxx"

int main(void){

  using namespace tfel::math;
  typedef vector<double> Vector;

  range<>::exe(12.,13.,5);

  return EXIT_SUCCESS;
}
