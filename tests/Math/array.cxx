/*!
 * \file   tests/Math/array.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   01 jui 2006
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

#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"
#include"TFEL/Math/array.hxx"
#include"TFEL/Math/functions.hxx"

int main(void)
{
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  array<1u>::RunTimeProperties RT(4);
  array<1u> a(RT,2.);
  array<1u> b(RT,4.);
  array<1u> c;
  array<1u> d(RT,4.);

  c = d;
  std::fill(d.begin(),d.end(),12);
  c = sin(2*a + b);
  c*=2;
  c/=3;
  c*=(b+a);
  a=-c;

  return EXIT_SUCCESS;
}
