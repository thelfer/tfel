/*!
 * \file   include/TFEL/Math/lambda.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 déc 2006
 */

#ifndef _LIB_TFEL_LAMBDA_HXX_
#define _LIB_TFEL_LAMBDA_HXX_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/UnaryResultType.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"
#include"TFEL/Math/Forward/lambda.hxx"

namespace tfel{

  namespace math{


    template<unsigned short N>
    struct LambdaTraits<Lambda<N> >
    {
      /*
       * minimal dimension of the argument.
       */
      static const unsigned short dimension = N; 
      /*
       * variables list
       */
      typedef typename tfel::meta::GenerateTypeList<Lambda<N> >::type Variables; 
    };

    template<unsigned short N>
    struct Lambda
      : public LambdaConcept<Lambda<N> >
    {

       template<unsigned short M,typename T>
       TFEL_MATH_INLINE
       typename tfel::meta::EnableIf<
	 (M>=N),
	   T>::type
       operator()(const tvector<M,T>&) const;

    }; // end of Lamba struct

    template<>
    struct Lambda<1u>
      : public LambdaConcept<Lambda<1u> >
    {

      template<unsigned short M,typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	(M>=1u),
	 T>::type
      operator()(const tvector<M,T>& v) const
      {
	return v(0u);
      }
      
      template<typename T>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T>::cond,
	T>::type
      operator()(const T s) const
      {
	return s;
      }
      
    }; // end of Lamba struct

    template<typename T,unsigned short M,unsigned short N>
    class UnaryResultType_<VectorTag,LambdaTag,tvector<M,T>,Lambda<N> >
    {
      typedef tfel::meta::InvalidType InvalidType;
    public:
      typedef typename tfel::meta::IF<(M>=N),T,InvalidType>::type type;
    };

    template<typename Scal>
    struct UnaryResultType_<ScalarTag,LambdaTag,Scal,Lambda<1u> >
    {
      typedef Scal type;
    };
    
    extern TFELMATH_VISIBILITY_EXPORT Lambda<1u> x_;
    extern TFELMATH_VISIBILITY_EXPORT Lambda<2u> y_;
    extern TFELMATH_VISIBILITY_EXPORT Lambda<3u> z_;

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/lambda.ixx"

#endif /* _LIB_TFEL_LAMBDA_HXX */

