/*!
 * \file   array2.cxx
 * \brief  
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#include<iostream>
#include<cstdlib>
#include<cassert>
#include<limits>

#include"Math/Array/ArrayRunTimeProperties.hxx"
#include"Math/array.hxx"

template<typename T>
void function(const tfel::math::array<1u,T>& a,const T value)
{
  assert(std::abs(a(0)-value)<std::numeric_limits<T>::epsilon());
}

template<typename T,typename Expr>
void function(const tfel::math::ArrayExpr<T,Expr> a,
	      const typename tfel::math::ArrayTraits<T>::NumType value)
{
  assert(std::abs(a(0)-value)<std::numeric_limits<typename tfel::math::ArrayTraits<T>::NumType>::epsilon());
}

// function(a,2.);
// function(b,4.);
// function(a+b,6.);
// function(2.f*sin(a),2.*std::sin(2.));

int main(void)
{
  using namespace std;
  using namespace tfel::math;

  array<2>::RunTimeProperties RT(4,3);
  array<2> a(RT,2.);
  array<2> b(RT,4.);

  cout << a   << endl;
  cout << b   << endl;

  cout << "success" << endl;

  return EXIT_SUCCESS;
}
