/*!
 * \file   LambdaConceptOperations.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#ifndef _LIB_TFEL_LAMBDACONCEPTOPERATIONS_HXX_
#define _LIB_TFEL_LAMBDACONCEPTOPERATIONS_HXX_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Math/LambdaExpression/LambdaLambdaExpr.hxx"
#include"TFEL/Math/LambdaExpression/ScalarLambdaExpr.hxx"
#include"TFEL/Math/LambdaExpression/LambdaScalarExpr.hxx"
#include"TFEL/Math/LambdaExpression/FctLambdaExpr.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpPlus>
      >::type
    operator +(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,LambdaTag,T1,T2,OpPlus>
    {
      typedef LambdaLambdaExpr<T1,T2,OpPlus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaLambdaExpr<T1,T2,OpPlus> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  typename UnaryResultType<T,T2>::type,
				  OpPlus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,LambdaTag,T1,T2,OpMinus>
    {
      typedef LambdaLambdaExpr<T1,T2,OpMinus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaLambdaExpr<T1,T2,OpMinus> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  typename UnaryResultType<T,T2>::type,
				  OpMinus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpMult>
      >::type
    operator * (const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,LambdaTag,T1,T2,OpMult>
    {
      typedef LambdaLambdaExpr<T1,T2,OpMult> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaLambdaExpr<T1,T2,OpMult> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  typename UnaryResultType<T,T2>::type,
				  OpMult>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      LambdaLambdaExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,LambdaTag,T1,T2,OpDiv>
    {
      typedef LambdaLambdaExpr<T1,T2,OpDiv> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaLambdaExpr<T1,T2,OpDiv> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  typename UnaryResultType<T,T2>::type,
				  OpDiv>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpPlus>
      >::type
    operator + (const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<ScalarTag,LambdaTag,T1,T2,OpPlus>
    {
      typedef ScalarLambdaExpr<T1,T2,OpPlus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,ScalarLambdaExpr<T1,T2,OpPlus> >
    {
      typedef typename ResultType<T1,typename UnaryResultType<T,T2>::type,
				  OpPlus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<ScalarTag,LambdaTag,T1,T2,OpMinus>
    {
      typedef ScalarLambdaExpr<T1,T2,OpMinus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,ScalarLambdaExpr<T1,T2,OpMinus> >
    {
      typedef typename ResultType<T1,typename UnaryResultType<T,T2>::type,
				  OpMinus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpMult>
      >::type
    operator * (const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<ScalarTag,LambdaTag,T1,T2,OpMult>
    {
      typedef ScalarLambdaExpr<T1,T2,OpMult> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,ScalarLambdaExpr<T1,T2,OpMult> >
    {
      typedef typename ResultType<T1,typename UnaryResultType<T,T2>::type,
				  OpMult>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond && 
      tfel::meta::Implements<T2,LambdaConcept>::cond,
      ScalarLambdaExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<ScalarTag,LambdaTag,T1,T2,OpDiv>
    {
      typedef ScalarLambdaExpr<T1,T2,OpDiv> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,ScalarLambdaExpr<T1,T2,OpDiv> >
    {
      typedef typename ResultType<T1,typename UnaryResultType<T,T2>::type,
				  OpDiv>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpPlus>
      >::type
    operator +(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,ScalarTag,T1,T2,OpPlus>
    {
      typedef LambdaScalarExpr<T1,T2,OpPlus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaScalarExpr<T1,T2,OpPlus> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  T2,OpPlus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpMinus>
      >::type
    operator -(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,ScalarTag,T1,T2,OpMinus>
    {
      typedef LambdaScalarExpr<T1,T2,OpMinus> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaScalarExpr<T1,T2,OpMinus> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  T2,OpMinus>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpMult>
      >::type
    operator *(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,ScalarTag,T1,T2,OpMult>
    {
      typedef LambdaScalarExpr<T1,T2,OpMult> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaScalarExpr<T1,T2,OpMult> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  T2,OpMult>::type type;
    };

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,LambdaConcept>::cond && 
      tfel::typetraits::IsScalar<T2>::cond,
      LambdaScalarExpr<T1,T2,OpDiv>
      >::type
    operator /(const T1 a, const T2 b);

    template<typename T1,typename T2>
    struct ResultType_<LambdaTag,ScalarTag,T1,T2,OpDiv>
    {
      typedef LambdaScalarExpr<T1,T2,OpDiv> type;
    };

    template<typename T,typename T1,typename T2>
    struct UnaryResultType<T,LambdaScalarExpr<T1,T2,OpDiv> >
    {
      typedef typename ResultType<typename UnaryResultType<T,T1>::type,
				  T2,OpDiv>::type type;
    };

  } // end of namespace math

} // end of namespace tfel 

#include"TFEL/Math/LambdaExpression/LambdaConceptOperations.ixx"

#endif /* _LIB_TFEL_LAMBDACONCEPTOPERATIONS_HXX */

