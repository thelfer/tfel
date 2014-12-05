/*!
 * \file   include/TFEL/Math/Function/Power.hxx
 * \brief  This file declares the power function.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_FUNCTION_POWER_H_
#define _LIB_TFEL_MATH_FUNCTION_POWER_H_ 

#include<cmath>
#include<type_traits>
#include<type_traits>
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/power.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    template<int N,unsigned int D=1>
    struct Power;

    typedef Power<1,2> FctSqrt;

    namespace stdfunctions{

      template<int N>
      TFEL_MATH_INLINE
      Power<N>
      power(void);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Power<N,D>
      power(void);

      template<int N,typename T>      			                                  
      typename std::enable_if<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N,unsigned int D,typename T>
      typename std::enable_if<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
	!tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N,typename F>
      typename std::enable_if<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<FunctionsCompositionExpr<tfel::math::Power<N,1u>,F> >
	>::type                                                                             
      power(const F f);

      template<int N,unsigned int D,typename F>
      typename std::enable_if<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<tfel::math::FunctionsCompositionExpr<tfel::math::Power<N,D>,F> >
      >::type                                                                             
      power(const F f);

    } // end of namespace stdfunctions

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/Power.ixx"

#endif /* _LIB_TFEL_MATH_FUNCTION_POWER_H */

