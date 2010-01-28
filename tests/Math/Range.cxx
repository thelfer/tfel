/*!
 * \file   Range.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   23 oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/Vector/Range.hxx"
#include"TFEL/Math/vector.hxx"

int main(void){

  using namespace tfel::math;
  typedef vector<double> Vector;

  range<>::exe(12.,13.,5);

  return EXIT_SUCCESS;
}
