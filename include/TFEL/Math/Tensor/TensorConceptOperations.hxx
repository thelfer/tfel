/*!
 * \file   include/TFEL/Math/Tensor/TensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to tensor.
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX_
#define _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX_ 

#include <cmath>
#include <type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/ExpressionTemplates/Expr.hxx"
#include"TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include"TFEL/Math/Tensor/TensorProduct.hxx"
#include"TFEL/Math/Tensor/TensorViewFromStensor.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Operation>
    struct TensorType<Expr<T_type,Operation> >
    {
      typedef T_type type;
    };

    template<typename T_type, typename Operation>
    struct TensorTraits<Expr<T_type,Operation>>
    {
      typedef typename TensorTraits<T_type>::NumType   NumType;
      typedef typename TensorTraits<T_type>::IndexType IndexType;
      static constexpr unsigned short dime = TensorTraits<T_type>::dime;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's operation
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
    public:
      typedef typename ResultType<TensA,TensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,BinaryOperation<A,B,Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,TensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
      typedef typename std::conditional<TensorTraits<TensA>::dime==1u,
					TensorProductExpr1D<A,B>,
					typename std::conditional<TensorTraits<TensA>::dime==2u,
								  TensorProductExpr2D<A,B>,
								  TensorProductExpr3D<A,B> >::type
					>::type Handler;
    public:
      typedef typename ResultType<TensA,TensB,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,Handler> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor vs stensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,StensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType <typename std::decay<A>::type>::type TensA;
      typedef typename StensorType<typename std::decay<B>::type>::type STensB;
      typedef typename TensorViewFromStensor<B>::type        TensB;
    public:
      typedef typename ResultType<TensA,STensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,BinaryOperation<A,TensB&&,Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for stensor vs tensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<StensorTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename StensorType<typename std::decay<A>::type>::type STensA;
      typedef typename TensorViewFromStensor<A>::type TensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
    public:
      typedef typename ResultType<STensA,TensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,BinaryOperation<TensA&&,B,Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,StensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename TensorType <typename std::decay<A>::type>::type TensA;
      typedef typename StensorType<typename std::decay<B>::type>::type STensB;
      typedef typename TensorViewFromStensor<B>::type TensB;
      typedef typename std::conditional<TensorTraits<TensA>::dime==1u,
					TensorProductExpr1D<A,TensB&&>,
					typename std::conditional<TensorTraits<TensA>::dime==2u,
								  TensorProductExpr2D<A,TensB&&>,
								  TensorProductExpr3D<A,TensB&&> >::type
				      >::type TensorProductOperation;
    public:
      typedef typename ResultType<TensA,STensB,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      Expr<Result,TensorProductOperation> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's multiplication
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<StensorTag,TensorTag,A,B,OpMult>
    {
      struct DummyHandle{};
      typedef typename StensorType<typename std::decay<A>::type>::type STensA;
      typedef typename TensorViewFromStensor<A>::type TensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
      typedef typename std::conditional<TensorTraits<TensB>::dime==1u,
					TensorProductExpr1D<TensA&&,B>,
					typename std::conditional<TensorTraits<TensB>::dime==2u,
								  TensorProductExpr2D<TensA&&,B>,
								  TensorProductExpr3D<TensA&&,B> >::type
				      >::type TensorProductOperation;
    public:
      typedef typename ResultType<STensA,TensB,OpMult>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
				      DummyHandle,
				      Expr<Result,TensorProductOperation> >::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor's operation
     */
    template<typename A, typename B>
    class ComputeBinaryResult_<TensorTag,TensorTag,A,B,OpDiadicProduct>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<A>::type>::type TensA;
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
    public:
      typedef typename ResultType<TensA,TensB,OpDiadicProduct>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,DiadicProductOperation<A,B>>>::type Handle;
    };
      
    /*
     * Partial Specialisation of ComputeBinaryResult_ for scalar-tensor operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<ScalarTag,TensorTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<B>::type>::type TensB;
    public:
      typedef typename ResultType<A,TensB,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,ScalarObjectOperation<A,B,Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeBinaryResult_ for tensor-scalar operations
     */
    template<typename A, typename B,typename Op>
    class ComputeBinaryResult_<TensorTag,ScalarTag,A,B,Op>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<A>::type>::type      TensA;
    public:
      typedef typename ResultType<TensA,B,Op>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,ObjectScalarOperation<A,B,Op>>>::type Handle;
    };

    /*
     * Partial Specialisation of ComputeUnaryResult_ for tensors
     */
    template<typename A>
    struct ComputeUnaryResult_<TensorTag,UnaryOperatorTag,A,OpNeg>
    {
      struct DummyHandle{};
      typedef typename TensorType<typename std::decay<A>::type>::type                    TensA;
      typedef typename TensorTraits<A>::NumType               NumA;
      typedef typename ComputeUnaryResult<NumA,OpNeg>::Result  NumResult;
    public:
      typedef typename UnaryResultType<TensA,OpNeg>::type Result;
      typedef typename std::conditional<tfel::typetraits::IsInvalid<Result>::cond,
					DummyHandle,
					Expr<Result,UnaryOperation<A,OpNeg>>>::type Handle;
    };
    
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Tensor/TensorConceptOperations.ixx"

#endif /* _LIB_TFEL_TENSOR_CONCEPT_OPERATIONS_HXX */

