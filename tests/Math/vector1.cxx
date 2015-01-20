/*!
 * \file   tests/Math/vector1.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 Oct 2006
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
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/functions.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;
  using tfel::math::vector;
  vector<double> v = {0.,1.,2.,3.,4.};
  vector<double> v2(5,3.);
  vector<double> v3(5);
  v3 = v+2.5*v2;
  assert(abs(v(0))<1.e-14);
  assert(abs(v(1)-1.)<1.e-14);
  assert(abs(v(2)-2.)<1.e-14);
  assert(abs(v(3)-3.)<1.e-14);
  assert(abs(v(4)-4.)<1.e-14);
  assert(abs(v2(0)-3.)<1.e-14);
  assert(abs(v2(1)-3.)<1.e-14);
  assert(abs(v2(2)-3.)<1.e-14);
  assert(abs(v2(3)-3.)<1.e-14);
  assert(abs(v2(4)-3.)<1.e-14);
  assert(abs(v3(0)-7.5)<1.e-14);
  assert(abs(v3(1)-8.5)<1.e-14);
  assert(abs(v3(2)-9.5)<1.e-14);
  assert(abs(v3(3)-10.5)<1.e-14);
  assert(abs(v3(4)-11.5)<1.e-14);
  v3 = sin(v);
  assert(abs(v3(0)-0.)<1.e-14);
  assert(abs(v3(1)-sin(1.))<1.e-14);
  assert(abs(v3(2)-sin(2.))<1.e-14);
  assert(abs(v3(3)-sin(3.))<1.e-14);
  assert(abs(v3(4)-sin(4.))<1.e-14);

  return EXIT_SUCCESS;
}
