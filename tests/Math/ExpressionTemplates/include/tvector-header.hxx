/*!
 * \file   tvector-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_TVECTOR_HEADER_HXX_
#define _LIB_TFEL_TVECTOR_HEADER_HXX_ 

#include<cstdlib>
#include<cassert>
#include<cmath>
#include<limits>
#include"Math/functions.hxx"
#include"Math/tvector.hxx"

template<typename T>
void test1(void);

template<typename T>
void test1(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;

  tvector<3,float>    v;
  tvector<3,float>  v2;
  tvector<3,T> v3;

  v(0) = 2.f;
  v(1) = 7.f;
  v(2) = 9.f;

  v2(0) = 2.34f;
  v2(1) = 54.2f;
  v2(2) = 412.232f;

  v3 = v+v2;

  v3 = exp(sin(2.f*v+3.5f*cos(v2))); 

  assert(abs(v3(0)-exp(sin(2.f*v(0)+3.5f*cos(v2(0)))))<1.e-14);
  assert(abs(v3(1)-exp(sin(2.f*v(1)+3.5f*cos(v2(1)))))<1.e-14);
  assert(abs(v3(2)-exp(sin(2.f*v(2)+3.5f*cos(v2(2)))))<1.e-14);

}

void test2(void);

void test3(void);

#endif /* _LIB_TFEL_TVECTOR_HEADER_HXX */

