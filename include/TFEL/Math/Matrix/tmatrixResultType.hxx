/*!
 * \file   include/TFEL/Math/Matrix/tmatrixResultType.hxx
 * \brief  this file implements partial specialisations of the ResultType and
 * GenerateMatrix traits class for tmatrix. \author Helfer Thomas \date   12 Jul
 * 2006 \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TINY_MATRIX_RESULT_TYPE_H_
#define LIB_TFEL_TINY_MATRIX_RESULT_TYPE_H_

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"
#include "TFEL/Math/Matrix/TVectorTMatrixExpr.hxx"
#include "TFEL/Math/Matrix/TMatrixTVectorExpr.hxx"

namespace tfel {

  namespace math {

    /*
     * Partial specialisation for negation.
     */
    template <unsigned short N, unsigned short M, typename T>
    class UnaryResultType<tmatrix<N, M, T>, OpNeg> {
      typedef typename UnaryResultType<T, OpNeg>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tmatrix and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, unsigned short M, typename T, typename Scal>
    class ResultType_<MatrixTag, ScalarTag, tmatrix<N, M, T>, Scal, OpMult> {
      typedef typename ResultType<T, Scal, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tmatrix and
     * scalars operations
     * \see   ResultType_
     */
    template <unsigned short N, unsigned short M, typename T, typename Scal>
    class ResultType_<MatrixTag, ScalarTag, tmatrix<N, M, T>, Scal, OpDiv> {
      typedef typename ResultType<T, Scal, OpDiv>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tmatrix and
     * scalars operations
     * \see   ResultType_
     */
    template <typename Scal, unsigned short N, unsigned short M, typename T>
    class ResultType_<ScalarTag, MatrixTag, Scal, tmatrix<N, M, T>, OpMult> {
      typedef typename ResultType<Scal, T, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tmatrix
     * \see   ResultType
     */
    template <unsigned short N, unsigned short M, typename T, typename T2>
    class ResultType<tmatrix<N, M, T>, tmatrix<N, M, T2>, OpPlus> {
      typedef typename ResultType<T, T2, OpPlus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    /*!
     * \brief Partial specialisation for tmatrix
     * \see   ResultType
     */
    template <unsigned short N, unsigned short M, typename T, typename T2>
    class ResultType<tmatrix<N, M, T>, tmatrix<N, M, T2>, OpMinus> {
      typedef typename ResultType<T, T2, OpMinus>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tmatrix<N, M, ResBase_>>::type type;
    };

    template <unsigned short N, unsigned short M, typename T, typename T2>
    class ResultType<tmatrix<N, M, T>, tvector<M, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<N, ResBase_>>::type type;
    };  // end of ComputeBinaryResult.

    template <unsigned short N, unsigned short M, typename T, typename T2>
    class ResultType<tvector<N, T>, tmatrix<N, M, T2>, OpMult> {
      typedef typename ResultType<T, T2, OpMult>::type ResBase_;

     public:
      typedef
          typename std::conditional<tfel::typetraits::IsInvalid<ResBase_>::cond,
                                    tfel::meta::InvalidType,
                                    tvector<M, ResBase_>>::type type;
    };  // end of ComputeBinaryResult.

    template <unsigned short N,
              unsigned short M,
              typename T,
              typename T2,
              typename A,
              typename B>
    class MatrixVectorHandle<tmatrix<N, M, T>, tvector<M, T2>, A, B> {
      struct DummyHandle {};

     public:
      typedef Expr<
          typename ResultType<tmatrix<N, M, T>, tvector<M, T2>, OpMult>::type,
          TMatrixTVectorExpr<N, M, A, B>>
          type;
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
      typedef Expr<
          typename ResultType<tvector<N, T>, tmatrix<N, M, T2>, OpMult>::type,
          TVectorTMatrixExpr<N, M, A, B>>
          type;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_TINY_MATRIX_RESULT_TYPE_H_ */
