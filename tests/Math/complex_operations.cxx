/*!
 * \file   complex_operations.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>

#include "Math/General/Complex.hxx"
#include "Math/General/BasicOperations.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  Complex<double> a(12.);
  Complex<int>    b(2,3);
  Complex<double> c(0.);
  Complex<double> d(0.);
  
  c += b;

  return EXIT_SUCCESS;
}
