/*!
 * \file   Power.hxx
 * \brief  This file declares the power function.
 * \author Helfer Thomas
 * \date   04 Jan 2007
 */

#ifndef _LIB_TFEL_POWER_H_
#define _LIB_TFEL_POWER_H_ 

#include<cmath>
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/General/Complex.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"
#include"TFEL/Math/General/UnaryResultType.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"
#include"TFEL/Math/qt.hxx"

namespace tfel{

  namespace math{

    template<int N,unsigned int D=1>
    class Power;

    typedef Power<1,2> FctSqrt;

    namespace stdfunctions{
      
      template<int N>
      TFEL_MATH_INLINE
      float 
      power(const float);

      template<int N>
      TFEL_MATH_INLINE
      double 
      power(const double);

      template<int N>
      TFEL_MATH_INLINE
      long double 
      power(const long double);

      template<int N>
      TFEL_MATH_INLINE
      Complex<float> 
      power(const Complex<float>);

      template<int N>
      TFEL_MATH_INLINE
      Complex<double> 
      power(const Complex<double>);

      template<int N>
      TFEL_MATH_INLINE
      Complex<long double> 
      power(const Complex<long double>);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,float>
      power(const qt<Unit,float>);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,double>
      power(const qt<Unit,double>);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,long double>
      power(const qt<Unit,long double>);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,Complex<float> >
      power(const qt<Unit,Complex<float> >);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,Complex<double> >
      power(const qt<Unit,Complex<double> >);

      template<int N,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,1u,Unit>::type,Complex<long double> >
      power(const qt<Unit,Complex<long double> >);

      template<int N,typename T>      			                                  
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N>
      TFEL_MATH_INLINE
      Power<N>
      power(void);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      float 
      power(const float);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      double 
      power(const double);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      long double 
      power(const long double);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Complex<float> 
      power(const Complex<float>);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Complex<double> 
      power(const Complex<double>);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Complex<long double> 
      power(const Complex<long double>);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,float>
      power(const qt<Unit,float>);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,double>
      power(const qt<Unit,double>);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,long double>
      power(const qt<Unit,long double>);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<float> >
      power(const qt<Unit,Complex<float> >);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<double> >
      power(const qt<Unit,Complex<double> >);

      template<int N,unsigned int D,typename Unit>
      TFEL_MATH_INLINE
      qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> >
      power(const qt<Unit,Complex<long double> >);

      template<int N,unsigned int D,typename T>
      typename tfel::meta::EnableIf<                                                      
	!tfel::typetraits::IsInvalid<
	typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
      !tfel::typetraits::IsScalar<T>::cond,
	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      >::type                                                                             
      power(const T& v);

      template<int N,unsigned int D>
      TFEL_MATH_INLINE
      Power<N,D>
      power(void);

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

      float
      pow(const float,const float);

      double
      pow(const double,const double);
      
      long double
      pow(const long double,const long double);
      
    } // end of namespace stdfunctions

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Function/Power.ixx"

#endif /* _LIB_TFEL_POWER_H */

