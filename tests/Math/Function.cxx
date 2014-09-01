/*!
 * \file   tests/Math/Function.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   05 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/functions.hxx"

int main(void){

  using namespace std;
  USING_TFEL_FUNCTIONS;

  assert(abs(cos(12.)-std::cos(12.))<1.e-14);
  assert(abs(sin(12.)-std::sin(12.))<1.e-14);
  assert(abs((cos+sin)(12.)-std::cos(12.)-std::sin(12.))<1.e-14);
  assert(abs((exp[sin])(12.)-std::exp(std::sin(12.)))<1.e-14);
  assert(abs((2.321*sin)(12.)-2.321*std::sin(12.))<1.e-14);
  assert(abs((sin*3.421)(12.)-3.421*std::sin(12.))<1.e-14);

  return EXIT_SUCCESS;
}
