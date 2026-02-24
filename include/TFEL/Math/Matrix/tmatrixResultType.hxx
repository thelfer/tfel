/*!
 * \file   include/TFEL/Math/Matrix/tmatrixResultType.hxx
 * \brief  this file implements partial specialisations of the ResultType and
 * GenerateMatrix traits class for tmatrix. \author Thomas Helfer \date   12 Jul
 * 2006 \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINY_MATRIX_RESULT_TYPE_HXX
#define LIB_TFEL_MATH_TINY_MATRIX_RESULT_TYPE_HXX

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/TVectorTMatrixExpr.hxx"
#include "TFEL/Math/Matrix/TMatrixTVectorExpr.hxx"
#include "TFEL/Math/Matrix/TMatrixTMatrixExpr.hxx"

namespace tfel::math {

  /*
   * Partial specialisation for negation.
   */
  template <unsigned short N, unsigned short M, typename T>
  class UnaryResultType<tmatrix<N, M, T>, OpNeg> {
    typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tmatrix and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, unsigned short M, typename T, typename Scal>
  class ComputeBinaryOperationResult<MatrixTag,
                                     ScalarTag,
                                     tmatrix<N, M, T>,
                                     Scal,
                                     OpMult> {
    typedef result_type<T, Scal, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tmatrix and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <unsigned short N, unsigned short M, typename T, typename Scal>
  class ComputeBinaryOperationResult<MatrixTag,
                                     ScalarTag,
                                     tmatrix<N, M, T>,
                                     Scal,
                                     OpDiv> {
    typedef result_type<T, Scal, OpDiv> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tmatrix and
   * scalars operations
   * \see   ComputeBinaryOperationResult
   */
  template <typename Scal, unsigned short N, unsigned short M, typename T>
  class ComputeBinaryOperationResult<ScalarTag,
                                     MatrixTag,
                                     Scal,
                                     tmatrix<N, M, T>,
                                     OpMult> {
    typedef result_type<Scal, T, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tmatrix
   * \see   ResultType
   */
  template <unsigned short N, unsigned short M, typename T, typename T2>
  class ResultType<tmatrix<N, M, T>, tmatrix<N, M, T2>, OpPlus> {
    typedef result_type<T, T2, OpPlus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  /*!
   * \brief Partial specialisation for tmatrix
   * \see   ResultType
   */
  template <unsigned short N, unsigned short M, typename T, typename T2>
  class ResultType<tmatrix<N, M, T>, tmatrix<N, M, T2>, OpMinus> {
    typedef result_type<T, T2, OpMinus> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };

  template <unsigned short N, unsigned short M, typename T, typename T2>
  class ResultType<tmatrix<N, M, T>, tvector<M, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>;
  };  // end of ResultType.

  template <unsigned short N, unsigned short M, typename T, typename T2>
  class ResultType<tvector<N, T>, tmatrix<N, M, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tvector<M, ResBase_>>;
  };  // end of ResultType.

  template <unsigned short N,
            unsigned short M,
            unsigned short K,
            typename T,
            typename T2>
  class ResultType<tmatrix<N, K, T>, tmatrix<K, M, T2>, OpMult> {
    typedef result_type<T, T2, OpMult> ResBase_;

   public:
    using type = std::conditional_t<isInvalid<ResBase_>(),
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>;
  };  // end of ResultType.

  template <unsigned short N,
            unsigned short M,
            typename T,
            typename T2,
            typename A,
            typename B>
  class MatrixVectorHandle<tmatrix<N, M, T>, tvector<M, T2>, A, B> {
    struct DummyHandle {};

   public:
    using type = Expr<result_type<tmatrix<N, M, T>, tvector<M, T2>, OpMult>,
                      TMatrixTVectorExpr<N, M, A, B>>;
  };

  template <unsigned short N,
            unsigned short M,
            typename T,
            typename T2,
            typename A,
            typename B>
  class MatrixVectorHandle<tvector<N, T>, tmatrix<N, M, T2>, A, B> {
    struct DummyHandle {};

   public:
    using type = Expr<result_type<tvector<N, T>, tmatrix<N, M, T2>, OpMult>,
                      TVectorTMatrixExpr<N, M, A, B>>;
  };

  template <unsigned short N,
            unsigned short M,
            unsigned short K,
            typename T,
            typename T2,
            typename A,
            typename B>
  class MatrixMatrixHandle<tmatrix<N, K, T>, tmatrix<K, M, T2>, A, B> {
    struct DummyHandle {};

   public:
    using type = Expr<result_type<tmatrix<N, K, T>, tmatrix<K, M, T2>, OpMult>,
                      TMatrixTMatrixExpr<N, M, K, A, B>>;
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINY_MATRIX_RESULT_TYPE_HXX */
