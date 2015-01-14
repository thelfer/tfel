/*!
 * \file   tests/Math/ExpressionTemplates/test_expr10.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   17 oct 2006
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

#include"test10-header.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using tfel::math::vector;
  USING_TFEL_FUNCTIONS;
  vector<double> v1(4);
  vector<double> v2(4);
  vector<double> v3(4);
  v1(0) = 12.12;
  v1(1) = 12.234;
  v1(2) = 22.4322;
  v1(3) = 2.2112;
  v2(0) = 2.;
  v2(1) = 4.;
  v2(2) = 5.;
  v2(3) = 6.;
  ::function(1.25*v1);
  ::function(cos(v2));
  ::function(v2*v1);
  assert(abs((1.25*v1)(0)-1.25*12.12)<1.e-14);
  assert(abs((1.25*v1)(1)-1.25*12.234)<1.e-14);
  assert(abs((1.25*v1)(2)-1.25*22.4322)<1.e-14);
  assert(abs((1.25*v1)(3)-1.25*2.2112)<1.e-14);
  assert(abs((cos(v2))(0)-cos(v2(0)))<1.e-14);
  assert(abs((cos(v2))(1)-cos(v2(1)))<1.e-14);
  assert(abs((cos(v2))(2)-cos(v2(2)))<1.e-14);
  assert(abs((cos(v2))(3)-cos(v2(3)))<1.e-14);
  assert(abs((v1*v2)(0)-(v1(0)*v2(0)))<1.e-14);
  assert(abs((v1*v2)(1)-(v1(1)*v2(1)))<1.e-14);
  assert(abs((v1*v2)(2)-(v1(2)*v2(2)))<1.e-14);
  assert(abs((v1*v2)(3)-(v1(3)*v2(3)))<1.e-14);
  return EXIT_SUCCESS;
}
