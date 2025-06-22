/*!
 * \file  include/TFEL/Math/QR/QRDecomp.ixx
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

#ifndef LIB_TFEL_MATH_QRDECOMP_IXX_
#define LIB_TFEL_MATH_QRDECOMP_IXX_

#include <cmath>
#include <limits>
#include <algorithm>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/QR/QRException.hxx"

namespace tfel {

  namespace math {

    template <typename MatrixType, typename VectorType>
    void QRDecomp::exe(MatrixType& a, VectorType& rdiag, VectorType& beta) {
      using namespace std;
      typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
      typedef typename MatrixTraits<MatrixType>::NumType NumType;
      typedef IndexType integer;
      typedef NumType real;
      const integer m = a.getNbRows();
      const integer n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != rdiag.size()) || (n != beta.size())) {
        throw(QRUnmatchedSize());
      }
      for (integer k = 0; k != m; ++k) {
        real alpha;
        if (a(k, k) > 0.) {
          alpha = -QRDecomp::enorm(a, k, k, m);
        } else {
          alpha = QRDecomp::enorm(a, k, k, m);
        }
        beta[k] = alpha * alpha - alpha * a(k, k);
        rdiag(k) = alpha;
        a(k, k) -= alpha;
        integer nk = k + 1;
        if (nk != m) {
          for (integer j = nk; j != n; ++j) {
            real gamma = 0;
            for (integer i = k; i != m; ++i) {
              gamma += a(i, k) * a(i, j);
            }
            gamma /= beta[k];
            for (integer i = k; i != m; ++i) {
              a(i, j) -= gamma * a(i, k);
            }
          }
        }
      }
    }  // end of exe

    template <typename VectorType, typename MatrixType>
    void QRDecomp::householder_product(
        VectorType& v,
        const MatrixType& a,
        const VectorType& beta,
        const typename MatrixTraits<MatrixType>::IndexType c) {
      typedef typename MatrixTraits<MatrixType>::NumType real;
      typedef typename MatrixTraits<MatrixType>::IndexType integer;
      const integer m = a.getNbRows();
      const integer n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != beta.size()) || (m != v.size())) {
        throw(QRUnmatchedSize());
      }
      real gamma = 0;
      for (integer i = c; i != m; ++i) {
        gamma += a(i, c) * v(i);
      }
      gamma /= beta(c);
      for (integer i = c; i != m; ++i) {
        v(i) -= gamma * a(i, c);
      }
    }  // end of QRDecomp::householder_product

    template <typename VectorType, typename MatrixType>
    void QRDecomp::tq_product(VectorType& v,
                              const MatrixType& a,
                              const VectorType& beta) {
      typedef typename MatrixTraits<MatrixType>::IndexType integer;
      const integer m = a.getNbCols();
      for (integer i = 0; i != m; ++i) {
        QRDecomp::householder_product(v, a, beta, i);
      }
    }  // end of QRDecomp::tq_product

    template <typename VectorType, typename MatrixType>
    void QRDecomp::back_substitute(
        VectorType& v,
        const MatrixType& a,
        const VectorType& d,
        const typename MatrixTraits<MatrixType>::NumType e) {
      using namespace std;
      typedef typename MatrixTraits<MatrixType>::NumType real;
      typedef typename MatrixTraits<MatrixType>::IndexType integer;
      const integer m = a.getNbRows();
      const integer n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != v.size()) || (n != d.size())) {
        throw(QRUnmatchedSize());
      }
      for (integer i = m; i != 0; --i) {
        integer l = i - 1;
        real t = v(l);
        if (abs(d(l)) < e) {
          throw(QRNullPivot());
        }
        for (integer j = i; j != n; ++j) {
          t -= a(l, j) * v(j);
        }
        v(l) = t / d(l);
      }
    }

    template <typename VectorType, typename MatrixType>
    void QRDecomp::back_substitute(VectorType& v,
                                   const MatrixType& a,
                                   const VectorType& d) {
      using real = typename MatrixTraits<MatrixType>::NumType;
      TFEL_CONSTEXPR real eps = 100 * std::numeric_limits<real>::min();
      QRDecomp::back_substitute(v, a, d, eps);
    }

    template <typename MatrixType>
    typename MatrixTraits<MatrixType>::NumType QRDecomp::enorm(
        const MatrixType& m,
        const typename MatrixTraits<MatrixType>::IndexType c,
        const typename MatrixTraits<MatrixType>::IndexType b,
        const typename MatrixTraits<MatrixType>::IndexType e) {
      using namespace std;
      typedef typename MatrixTraits<MatrixType>::NumType real;
      typename MatrixTraits<MatrixType>::IndexType i;
      real n = real(0);
      for (i = b; i != e; ++i) {
        const real v = m(i, c);
        n += v * v;
      }
      return std::sqrt(n);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QRDECOMP_IXX_ */
