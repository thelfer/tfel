/*!
 * \file   tests/Math/array2.cxx
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
#include<limits>

#include"TFEL/Math/Array/ArrayRunTimeProperties.hxx"
#include"TFEL/Math/array.hxx"

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

int main(void)
{
  using tfel::math::array;
  array<2>::RunTimeProperties RT(4,3);
  array<2> a(RT,2.);
  array<2> b(RT,4.);
  return EXIT_SUCCESS;
}
