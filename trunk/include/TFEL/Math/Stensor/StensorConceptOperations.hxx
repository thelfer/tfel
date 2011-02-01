/*!
 * \file   StensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to stensor.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX_ 

#include <cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/HasRandomAccessConstIterator.hxx"

#include"TFEL/TypeTraits/IsScalar.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/ComputeObjectTag.hxx"
#include"TFEL/Math/General/ComputeBinaryResult.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"

#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/Stensor/ScalarStensorExpr.hxx"
#include"TFEL/Math/Stensor/StensorStensorExpr.hxx"
#include"TFEL/Math/Stensor/StensorNegExpr.hxx"
#include"TFEL/Math/Stensor/StensorStensorDiadicProductExpr.hxx"
#include"TFEL/Math/Stensor/ScalarStensorExprWithoutConstIterator.hxx"
#include"TFEL/Math/Stensor/StensorStensorExprWithoutConstIterator.hxx"
#include"TFEL/Math/Stensor/StensorNegExprWithoutConstIterator.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2Expr.hxx"

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    class StensorDotProductHandle
    {
      struct DummyHandle
      {};
    public:
      typedef DummyHandle type;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<StensorTag,StensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type StensA;
      typedef typename StensorType<B>::type StensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      StensorStensorExpr<A,B,Op>,
				      StensorStensorExprWithoutConstIterator<A,B,Op>
				      >::type Expr;			    
    public:
      typedef typename ResultType<StensA,StensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,StensorTag,A,B,OpDiadicProduct>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type StensA;
      typedef typename StensorType<B>::type StensB;
      typedef StensorStensorDiadicProductExpr<A,B> Expr;
    public:
      typedef typename ResultType<StensA,StensB,OpDiadicProduct>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      ST2toST2Expr<Result,Expr> >::type Handle;
    };
    
    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-stensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,StensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename StensorType<B>::type StensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarStensorExpr<A,B,Op>,
				      ScalarStensorExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<A,StensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<StensorTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type      StensA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      StensorScalarExpr<A,B,Op>,
				      StensorScalarExprWithoutConstIterator<A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<StensA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for stensors
     */
    template<typename A>
    struct ComputeUnaryResult_<StensorTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type                    StensA;
      typedef typename StensorTraits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      StensorNegExpr<A>,
				      StensorNegExprWithoutConstIterator<A>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<StensA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      StensorExpr<Result,Expr> >::type Handle;
    };

    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,StensorTag,A,B,OpDotProduct>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type StensorA;
      typedef typename StensorType<B>::type StensorB;
    public:
      typedef typename ResultType<StensorA,StensorB,OpDotProduct>::type Result;
      typedef typename StensorDotProductHandle<StensorA,StensorB>::type Handle;
    };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2);

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1&);

    /*!
     * \return the inner product of a stensor
     * \param const T1&, the left  stensor.
     * \param const T2&, the right stensor.
     * \return const typename ResultType<T,T2,OpMult>::type, the
     * result.
     * \warning the operator| has not the priority expected for such
     * an operation : use of parenthesis is required.
     */
    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1&, const T2&);
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Stensor/StensorConceptOperations.ixx"

#endif /* _LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX */

