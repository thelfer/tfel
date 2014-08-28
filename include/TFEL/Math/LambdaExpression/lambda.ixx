/*!
 * \file   include/TFEL/Math/LambdaExpression/lambda.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 déc 2006
 */

#ifndef _LIB_TFEL_LAMBDA_IXX_
#define _LIB_TFEL_LAMBDA_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N>
    template<unsigned short M,typename T>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      (M>=N),
	T>::type
    Lambda<N>::operator()(const tvector<M,T>& v) const
    {
      return v(N-1);
    }
    
//     template<unsigned short M,typename T>
//     TFEL_MATH_INLINE
//     typename tfel::meta::EnableIf<
//       (M>=1u),
//       const T>::type
//     Lambda<1u>::operator()(const tvector<M,T>& v) const
//     {
//       return v(0u);
//     }

//     template<typename T>
//     TFEL_MATH_INLINE
//     typename tfel::meta::EnableIf<
//       tfel::typetraits::IsScalar<T>::cond,
//       const T>::type
//     Lambda<1u>::operator()(const T s) const
//     {
//       return s;
//     }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_LAMBDA_IXX */

