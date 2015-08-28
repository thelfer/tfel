/*!
 * \file   tests/Math/qt2.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   18 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cmath>
#include<cstdlib>
#include<cassert>

#include"TFEL/Math/qt.hxx"

/* coverity [UNCAUGHT_EXCEPT]*/
int main(void){
#ifndef _MSC_VER
  using namespace std;
  using namespace tfel::math;
  typedef PowUnit_<1,2,Mass>::type SquareMass;
  constexpr qt<SquareMass> v1(1.5);
  constexpr qt<Mass>       v2 = v1 * v1;
  assert(std::abs(v2.getValue()-2.25)<1.e-14);
#endif
  return EXIT_SUCCESS;
}
