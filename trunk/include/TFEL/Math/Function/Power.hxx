/*!
 * \file   Power.hxx
 * \brief  This file declares the power function.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_MATH_FUNCTION_POWER_H_
#define _LIB_TFEL_MATH_FUNCTION_POWER_H_ 

#include<cmath>
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
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
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N,unsigned int D,typename T>
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
	!tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N,typename F>
      typename tfel::meta::EnableIf<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<FunctionsCompositionExpr<tfel::math::Power<N,1u>,F> >
	>::type                                                                             
      power(const F f);

      template<int N,unsigned int D,typename F>
      typename tfel::meta::EnableIf<                                                      
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<tfel::math::FunctionsCompositionExpr<tfel::math::Power<N,D>,F> >
      >::type                                                                             
      power(const F f);

    } // end of namespace stdfunctions

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/Power.ixx"

#endif /* _LIB_TFEL_MATH_FUNCTION_POWER_H */

