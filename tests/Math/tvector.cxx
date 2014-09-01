/*! 
 * \file   tests/Math/tvector.cxx
 * \brief
 * \author Helfer Thomas
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

#include"TFEL/Math/qt.hxx"
#include"TFEL/Math/tvector.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;

  typedef qt<Acceleration,int> acceleration;
  typedef qt<Mass,int>  mass;
  typedef qt<Force>     force;

  typedef tvector<5,acceleration> avector;
  typedef tvector<5,acceleration> avector;
  typedef tvector<5,force>        fvector;

  mass m = mass(2);
  avector a1;
  avector a2;
  fvector f;

  a2(0)=acceleration(2);
  a2(1)=acceleration(1);
  a2(2)=acceleration(4);
  a2(3)=acceleration(2);
  a2(4)=acceleration(3);

  a1=2*(a2+a2);
  assert(abs(a1(0).getValue()-8)==0);
  assert(abs(a1(1).getValue()-4)==0);
  assert(abs(a1(2).getValue()-16)==0);
  assert(abs(a1(3).getValue()-8)==0);
  assert(abs(a1(4).getValue()-12)==0);

  f = m*a1;
  assert(abs(f(0).getValue()-16)<1.e-14);
  assert(abs(f(1).getValue()-8)<1.e-14);
  assert(abs(f(2).getValue()-32)<1.e-14);
  assert(abs(f(3).getValue()-16)<1.e-14);
  assert(abs(f(4).getValue()-24)<1.e-14);

  return EXIT_SUCCESS;

}
