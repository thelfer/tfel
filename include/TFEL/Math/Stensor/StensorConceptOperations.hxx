/*!
 * \file   include/TFEL/Math/Stensor/StensorConceptOperations.hxx
 * \brief  This file implements operations that can be applied to stensor.
 * \author Thomas Helfer
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX
#define LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/ExpressionTemplates/StandardOperations.hxx"
#include "TFEL/Math/Stensor/StensorProduct.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  template <typename T_type, typename Operation>
  struct StensorTraits<Expr<T_type, Operation>> {
    typedef typename StensorTraits<T_type>::NumType NumType;
    typedef typename StensorTraits<T_type>::IndexType IndexType;
    static constexpr unsigned short dime = StensorTraits<T_type>::dime;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<StensorTag, StensorTag, A, B, Op> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<StensA, StensB, Op>::type;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, BinaryOperation<A, B, Op>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for stensor's multiplication
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<StensorTag, StensorTag, A, B, OpMult> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;
    //! \brief a simple alias
    using Operation =
        std::conditional_t<StensorTraits<StensA>::dime == 1u,
                           StensorProductExpr1D<A, B>,
                           std::conditional_t<StensorTraits<StensA>::dime == 2u,
                                              StensorProductExpr2D<A, B>,
                                              StensorProductExpr3D<A, B>>>;

   public:
    using Result = typename ResultType<StensA, StensB, OpMult>::type;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, Operation>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for stensor's operation
   */
  template <typename A, typename B>
  class ComputeBinaryResult_<StensorTag, StensorTag, A, B, OpDiadicProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;

   public:
    using Result = typename ResultType<StensA, StensB, OpDiadicProduct>::type;
    using Handle =
        std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                           DummyHandle,
                           Expr<Result, DiadicProductOperation<A, B>>>;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for scalar-stensor
   * operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<ScalarTag, StensorTag, A, B, Op> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensB = EvaluationResult<B>;

   public:
    typedef typename ResultType<A, StensB, Op>::type Result;
    typedef typename std::conditional<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        Expr<Result, ScalarObjectOperation<A, B, Op>>>::type Handle;
  };

  /*
   * Partial Specialisation of ComputeBinaryResult_ for stensor-scalar
   * operations
   */
  template <typename A, typename B, typename Op>
  class ComputeBinaryResult_<StensorTag, ScalarTag, A, B, Op> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;

   public:
    typedef typename ResultType<StensA, B, Op>::type Result;
    typedef typename std::conditional<
        tfel::typetraits::IsInvalid<Result>::cond,
        DummyHandle,
        Expr<Result, ObjectScalarOperation<A, B, Op>>>::type Handle;
  };

  /*
   * Partial Specialisation of ComputeUnaryResult_ for stensors
   */
  template <typename A>
  struct ComputeUnaryResult_<StensorTag, UnaryOperatorTag, A, OpNeg> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensA = EvaluationResult<A>;

   public:
    typedef typename UnaryResultType<StensA, OpNeg>::type Result;
    using Handle = std::conditional_t<tfel::typetraits::IsInvalid<Result>::cond,
                                      DummyHandle,
                                      Expr<Result, UnaryOperation<A, OpNeg>>>;
  };

  template <typename A, typename B>
  class ComputeBinaryResult_<StensorTag, StensorTag, A, B, OpDotProduct> {
    struct DummyHandle {};
    //! \brief a simple alias
    using StensorA = EvaluationResult<A>;
    //! \brief a simple alias
    using StensorB = EvaluationResult<B>;

   public:
    typedef typename ResultType<StensorA, StensorB, OpDotProduct>::type Result;
    typedef typename ResultType<StensorA, StensorB, OpDotProduct>::type Handle;
  };

  /*!
   * \return the inner product of a stensor
   * \param const T1&, the left  stensor.
   * \param const T2&, the right stensor.
   * \return const typename ResultType<T,T2,OpMult>::type, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <typename T1, typename T2>
  typename std::enable_if<
      tfel::meta::Implements<T1, StensorConcept>::cond &&
          tfel::meta::Implements<T2, StensorConcept>::cond &&
          StensorTraits<T1>::dime == 1u && StensorTraits<T2>::dime == 1u &&
          !tfel::typetraits::IsInvalid<
              typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
  operator|(const T1&, const T2&);
  /*!
   * \return the inner product of a stensor
   * \param const T1&, the left  stensor.
   * \param const T2&, the right stensor.
   * \return const typename ResultType<T,T2,OpMult>::type, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <typename T1, typename T2>
  typename std::enable_if<
      tfel::meta::Implements<T1, StensorConcept>::cond &&
          tfel::meta::Implements<T2, StensorConcept>::cond &&
          StensorTraits<T1>::dime == 2u && StensorTraits<T2>::dime == 2u &&
          !tfel::typetraits::IsInvalid<
              typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
  operator|(const T1&, const T2&);
  /*!
   * \return the inner product of a stensor
   * \param const T1&, the left  stensor.
   * \param const T2&, the right stensor.
   * \return const typename ResultType<T,T2,OpMult>::type, the
   * result.
   * \warning the operator| has not the priority expected for such
   * an operation : use of parenthesis is required.
   */
  template <typename T1, typename T2>
  typename std::enable_if<
      tfel::meta::Implements<T1, StensorConcept>::cond &&
          tfel::meta::Implements<T2, StensorConcept>::cond &&
          StensorTraits<T1>::dime == 3u && StensorTraits<T2>::dime == 3u &&
          !tfel::typetraits::IsInvalid<
              typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1, T2, OpDotProduct>::Result>::type
  operator|(const T1&, const T2&);

}  // end of namespace tfel::math

#include "TFEL/Math/Stensor/StensorConceptOperations.ixx"

#endif /* LIB_TFEL_STENSOR_CONCEPT_OPERATIONS_HXX */
