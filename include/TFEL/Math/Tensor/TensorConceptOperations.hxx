/*!
 * \file   TensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to tensor.
 * \author Helfer Thomas
 * \date   01 jui 2006
 */

#ifndef _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX_ 

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

#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/ScalarMathObjectExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExpr.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectNegExprWithoutConstIterator.hxx"
#include"TFEL/Math/ExpressionTemplates/MathObjectMathObjectDiadicProductExpr.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"
#include"TFEL/Math/Tensor/TensorProduct.hxx"
#include"TFEL/Math/Tensor/TensorViewFromStensor.hxx"

namespace tfel{

  namespace math{

    // template<typename T1,typename T2>
    // class TensorDotProductHandle
    // {
    //   struct DummyHandle
    //   {};
    // public:
    //   typedef DummyHandle type;
    // };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type TensA;
      typedef typename TensorType<B>::type TensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MathObjectMathObjectExpr<TensorConcept,
							       TensorTraits,A,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<TensorConcept,
										   TensorTraits,
										   A,B,Op>
				      >::type Expr;			    
    public:
      typedef typename ResultType<TensA,TensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,TensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type tensA;
      typedef typename TensorType<B>::type tensB;
      typedef typename tfel::meta::IF<TensorTraits<A>::dime==1u,
				      TensorProductExpr1D<A,B>,
				      typename tfel::meta::IF<TensorTraits<A>::dime==2u,
							      TensorProductExpr2D<A,B>,
							      TensorProductExpr3D<A,B> >::type
				      >::type Expr;
    public:
      typedef typename ResultType<tensA,tensB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor vs stensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,StensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type  TensA;
      typedef typename StensorType<B>::type STensB;
      typedef typename TensorViewFromStensor<B>::type        TensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<TensB>::cond,
				      MathObjectMathObjectExpr<TensorConcept,
							       TensorTraits,A,TensB,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<TensorConcept,
										   TensorTraits,
										   A,TensB,Op>
				      >::type Expr;
    public:
      typedef typename ResultType<TensA,STensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor vs tensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<StensorTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type           STensA;
      typedef typename TensorViewFromStensor<A>::type TensA;
      typedef typename TensorType<B>::type            TensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<TensA>::cond&&
                                      tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      MathObjectMathObjectExpr<TensorConcept,
							       TensorTraits,TensA,B,Op>,
				      MathObjectMathObjectExprWithoutConstIterator<TensorConcept,
										   TensorTraits,
										   TensA,B,Op>
				      >::type Expr;
    public:
      typedef typename ResultType<STensA,TensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,StensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type TensA;
      typedef typename StensorType<B>::type STensB;
      typedef typename TensorViewFromStensor<B>::type TensB;
      typedef typename tfel::meta::IF<TensorTraits<A>::dime==1u,
				      TensorProductExpr1D<A,TensB>,
				      typename tfel::meta::IF<TensorTraits<A>::dime==2u,
							      TensorProductExpr2D<A,TensB>,
							      TensorProductExpr3D<A,TensB> >::type
				      >::type Expr;
    public:
      typedef typename ResultType<TensA,STensB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,TensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename StensorType<A>::type STensA;
      typedef typename TensorViewFromStensor<A>::type TensA;
      typedef typename TensorType<B>::type TensB;
      typedef typename tfel::meta::IF<TensorTraits<A>::dime==1u,
				      TensorProductExpr1D<TensA,B>,
				      typename tfel::meta::IF<TensorTraits<B>::dime==2u,
							      TensorProductExpr2D<TensA,B>,
							      TensorProductExpr3D<TensA,B> >::type
				      >::type Expr;
    public:
      typedef typename ResultType<STensA,TensB,OpMult>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    // /*
    //  * Partial Specialisation of ComputeBinaryResult_ for tensor's operation
    //  */
    // template<typename A, typename B>
    // class ComputeBinaryResult_<TensorTag,TensorTag,A,B,OpDiadicProduct>
    // {
    //   struct DummyHandle{};
    //   typedef typename TensorType<A>::type TensA;
    //   typedef typename TensorType<B>::type TensB;
    //   typedef MathObjectMathObjectDiadicProductExpr<TensorConcept,TensorTraits,A,B> Expr;
    // public:
    //   typedef typename ResultType<TensA,TensB,OpDiadicProduct>::type Result;
    //   typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
    // 				      DummyHandle,
    // 				      ST2toST2Expr<Result,Expr> >::type Handle;
    // };
   
    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-tensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<B>::type TensB;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<B>::cond,
				      ScalarMathObjectExpr<TensorConcept,
							   TensorTraits,
							   A,B,Op>,
				      ScalarMathObjectExprWithoutConstIterator<TensorConcept,
									       TensorTraits,
									       A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<A,TensB,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type      TensA;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectScalarExpr<TensorConcept,TensorTraits,A,B,Op>,
				      MathObjectScalarExprWithoutConstIterator<TensorConcept,TensorTraits,A,B,Op>
                                     >::type Expr;
    public:
      typedef typename ResultType<TensA,B,Op>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for tensors
     */
    template<typename A>
    struct ComputeUnaryResult_<TensorTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename TensorType<A>::type                    TensA;
      typedef typename TensorTraits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
      typedef typename tfel::meta::IF<tfel::meta::HasRandomAccessConstIterator<A>::cond,
				      MathObjectNegExpr<TensorConcept,TensorTraits,A>,
				      MathObjectNegExprWithoutConstIterator<TensorConcept,TensorTraits,A>
				      >::type Expr;
    public:
      typedef typename UnaryResultType<TensA,OpNeg>::type Result;
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      TensorExpr<Result,Expr> >::type Handle;
    };

    // template<typename A, typename B>
    // class ComputeBinaryResult_<TensorTag,TensorTag,A,B,OpDotProduct>
    // {
    //   struct DummyHandle{};
    //   typedef typename TensorType<A>::type TensorA;
    //   typedef typename TensorType<B>::type TensorB;
    // public:
    //   typedef typename ResultType<TensorA,TensorB,OpDotProduct>::type Result;
    //   typedef typename TensorDotProductHandle<TensorA,TensorB>::type Handle;
    // };

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1&,const T2);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1&,const T2);

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1&);

    // /*!
    //  * \return the inner product of a tensor
    //  * \param const T1&, the left  tensor.
    //  * \param const T2&, the right tensor.
    //  * \return const typename ResultType<T,T2,OpMult>::type, the
    //  * result.
    //  * \warning the operator| has not the priority expected for such
    //  * an operation : use of parenthesis is required.
    //  */
    // template<typename T1,typename T2>
    // typename tfel::meta::EnableIf<
    //   tfel::meta::Implements<T1,TensorConcept>::cond&&
    //   tfel::meta::Implements<T2,TensorConcept>::cond&&
    //   !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
    //   typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    // >::type
    // operator | (const T1&, const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,TensorConcept>::cond&&
      tfel::meta::Implements<T2,StensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,StensorConcept>::cond&&
      tfel::meta::Implements<T2,TensorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1&,const T2&);
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Tensor/TensorConceptOperations.ixx"

#endif /* _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX */

