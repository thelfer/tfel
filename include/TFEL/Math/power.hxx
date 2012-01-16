/*! 
 * \file  power.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 janv. 2012
 */

#ifndef _LIB_TFEL_MATH_POWER_H_
#define _LIB_TFEL_MATH_POWER_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"

namespace tfel{

  namespace math{

    namespace stdfunctions{
      
      template<int N,typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T>::cond,
	T>::type
      power(const T);

      template<int N,unsigned int D,
	       typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T>::cond,
	T>::type
      power(const T);

      float
      pow(const float,const float);

      double
      pow(const double,const double);
      
      long double
      pow(const long double,const long double);
      
    } // end of namespace stdfunctions

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/power.ixx"

#endif /* _LIB_TFEL_MATH_POWER_H */

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,float>
      // power(const qt<Unit,float>);

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,double>
      // power(const qt<Unit,double>);

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,long double>
      // power(const qt<Unit,long double>);

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,Complex<float> >
      // power(const qt<Unit,Complex<float> >);

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,Complex<double> >
      // power(const qt<Unit,Complex<double> >);

      // template<int N,unsigned int D,typename Unit>
      // TFEL_MATH_INLINE
      // qt<typename PowUnit_<N,D,Unit>::type,Complex<long double> >
      // power(const qt<Unit,Complex<long double> >);

      // template<int N,unsigned int D,typename T>
      // typename tfel::meta::EnableIf<                                                      
      // 	!tfel::typetraits::IsInvalid<
      // 	typename ComputeUnaryResult<T,Power<N,D> >::Result>::cond&&
      // !tfel::typetraits::IsScalar<T>::cond,
      // 	typename ComputeUnaryResult<T,Power<N,D> >::Handle                               
      // >::type                                                                             
      // power(const T& v);

      // template<int N,unsigned int D>
      // TFEL_MATH_INLINE
      // Power<N,D>
      // power(void);
