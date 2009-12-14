/*!
 * \file   tvector-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 fév 2008
 */

#ifndef _LIB_TFEL_TVECTOR_HEADER_HXX_
#define _LIB_TFEL_TVECTOR_HEADER_HXX_ 

#include<iostream>
#include<cstdlib>
#include<cassert>
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

  cout << "test1<T>()" << endl;

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

  cout << v3 << endl;

  v3 = exp(sin(2.f*v+3.5f*cos(v2))); 
  cout << v3 << endl;
}

void test2(void);

void test3(void);

#endif /* _LIB_TFEL_TVECTOR_HEADER_HXX */

