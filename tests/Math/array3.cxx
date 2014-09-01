/*!
 * \file   tests/Math/array3.cxx
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

#ifdef NO_RUNTIME_CHECK_BOUNDS
#undef NO_RUNTIME_CHECK_BOUNDS
#endif /* NO_RUNTIME_CHECK_BOUNDS */

#include<cstdlib>
#include<cassert>
#include<algorithm>

#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"
#include"TFEL/Math/array.hxx"

int main(void)
{
  using namespace tfel::math;

  array<1u>::RunTimeProperties RT(4);
  array<1u>::RunTimeProperties RT2(3);

  array<1u> a(RT,2.);
  array<1u> b(RT2,4.);
  array<1u> c(RT2,4.);

  bool catched = false;
  try{
    c = a+b;
  }
  catch(MathRunTimeException&){
    catched = true;
  }
  assert(catched);

  catched = false;
  try{
    c = 2*a;
  }
  catch(MathRunTimeException&){
    catched = true;
  }
  assert(catched);

  return EXIT_SUCCESS;
}
