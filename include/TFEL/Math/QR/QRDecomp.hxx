/*!
 * \file  include/TFEL/Math/QR/QRDecomp.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QRDECOMP_H_
#define LIB_TFEL_MATH_QRDECOMP_H_

#include <cmath>
#include <limits>
#include <algorithm>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/LU/Permutation.hxx"

namespace tfel {

  namespace math {

    /*!
     * structure in charge of computing the QR decomposition of a
     * matrix. Column permutation is performed.
     */
    struct QRDecomp {
      /*!
       * Compute the QR decomposition of a matrix.
       *
       * \param[in,out] m      : the matrix to be decomposed
       * \param[out]    rdiag  : diagonal values of the R matrix
       * \param[out]    beta   : euclidian norm of the columns of m
       * The decomposition is done in-place
       *
       * This function uses householder transformations to compute a
       * qr factorization of the m by n matrix a. that is, one
       * determines an orthogonal matrix q, and an upper trapezoidal
       * matrix r with diagonal elements of nonincreasing magnitude,
       * such that a = q*r. the householder transformation for column
       * k, k = 1,2,...,min(m,n), is of the form:
       * \[
       * I - (1/beta(k))*v*v
       * \]
       * where v has zeros in the first k-1 positions.
       */
      template <typename MatrixType, typename VectorType>
      static TFEL_MATH_INLINE2 void exe(MatrixType&, VectorType&, VectorType&);

      /*!
       * do the product of the transpose q matrix which is stored in a
       * decomposed matrix a to a vector.
       * \param[in] v    : vector to be multiplied
       * \param[in] a    : decomposed matrix which stores a bunch of
       * householder transformations in its lower diagonal part
       * \param[in] beta : square of norm of the householder vectors
       */
      template <typename VectorType, typename MatrixType>
      static TFEL_MATH_INLINE2 void tq_product(VectorType&,
                                               const MatrixType&,
                                               const VectorType&);

      /*!
       * \param[in] v : vector
       * \param[in] a : decomposed matrix
       * \param[in] d : diagonal element of the matrix
       * \param[in] e : precision
       */
      template <typename VectorType, typename MatrixType>
      static TFEL_MATH_INLINE2 void back_substitute(
          VectorType&,
          const MatrixType&,
          const VectorType&,
          const typename MatrixTraits<MatrixType>::NumType);
      /*!
       * \param[in] v : vector
       * \param[in] a : decomposed matrix
       * \param[in] d : diagonal element of the matrix
       */
      template <typename VectorType, typename MatrixType>
      static TFEL_MATH_INLINE2 void back_substitute(VectorType&,
                                                    const MatrixType&,
                                                    const VectorType&);

     protected:
      /*!
       * do the product of a householder matrix, stored in the kth
       * column of the matrix a, to a vector.
       * \param[in] v    : vector to be multiplied
       * \param[in] a : decomposed matrix which stores a bunch of
       * householder transformations in its lower diagonal part
       * \param[in] beta : square of norm of the householder vectors
       * \param[in] c    : column index
       */
      template <typename VectorType, typename MatrixType>
      static TFEL_MATH_INLINE2 void householder_product(
          VectorType&,
          const MatrixType&,
          const VectorType&,
          const typename MatrixTraits<MatrixType>::IndexType);

      /*!
       * Compute the norm of the cth column of matrix m,
       * starting from b to e.
       * \param[in] m : matrix
       * \param[in] c : column number
       * \param[in] b : starting row
       * \param[in] e : past-the end row
       */
      template <typename MatrixType>
      static typename MatrixTraits<MatrixType>::NumType enorm(
          const MatrixType&,
          const typename MatrixTraits<MatrixType>::IndexType,
          const typename MatrixTraits<MatrixType>::IndexType,
          const typename MatrixTraits<MatrixType>::IndexType);
    };

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/QR/QRDecomp.ixx"

#endif /* LIB_TFEL_MATH_QRDECOMP_H_ */
