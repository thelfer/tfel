/*!
 * \file   tests/Math/complex_operations.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <cstdlib>

#include"TFEL/Math/General/Complex.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
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
