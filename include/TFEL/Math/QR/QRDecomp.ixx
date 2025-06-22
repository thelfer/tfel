/*!
 * \file  include/TFEL/Math/QR/QRDecomp.ixx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QRDECOMPIXX
#define LIB_TFEL_MATH_QRDECOMPIXX

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
      using real = typename MatrixTraits<MatrixType>::NumType;
      using size_type = typename MatrixTraits<MatrixType>::IndexType;
      const auto m = a.getNbRows();
      const auto n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != rdiag.size()) || (n != beta.size())) {
        throw(QRUnmatchedSize());
      }
      for (size_type k = 0; k != m; ++k) {
        real alpha;
        if (a(k, k) > real(0)) {
          alpha = -QRDecomp::enorm(a, k, k, m);
        } else {
          alpha = QRDecomp::enorm(a, k, k, m);
        }
        beta[k] = alpha * alpha - alpha * a(k, k);
        rdiag(k) = alpha;
        a(k, k) -= alpha;
        size_type nk = k + 1;
        if (nk != m) {
          for (size_type j = nk; j != n; ++j) {
            auto gamma = real(0);
            for (size_type i = k; i != m; ++i) {
              gamma += a(i, k) * a(i, j);
            }
            gamma /= beta[k];
            for (size_type i = k; i != m; ++i) {
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
      using real = typename MatrixTraits<MatrixType>::NumType;
      using size_type = typename MatrixTraits<MatrixType>::IndexType;
      const auto m = a.getNbRows();
      const auto n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != beta.size()) || (m != v.size())) {
        throw(QRUnmatchedSize());
      }
      auto gamma = real(0);
      for (size_type i = c; i != m; ++i) {
        gamma += a(i, c) * v(i);
      }
      gamma /= beta(c);
      for (size_type i = c; i != m; ++i) {
        v(i) -= gamma * a(i, c);
      }
    }  // end of QRDecomp::householder_product

    template <typename VectorType, typename MatrixType>
    void QRDecomp::tq_product(VectorType& v,
                              const MatrixType& a,
                              const VectorType& beta) {
      using size_type = typename MatrixTraits<MatrixType>::IndexType;
      const auto m = a.getNbCols();
      for (size_type i = 0; i != m; ++i) {
        QRDecomp::householder_product(v, a, beta, i);
      }
    }  // end of QRDecomp::tq_product

    template <typename VectorType, typename MatrixType>
    void QRDecomp::back_substitute(
        VectorType& v,
        const MatrixType& a,
        const VectorType& d,
        const typename MatrixTraits<MatrixType>::NumType e) {
      using size_type = typename MatrixTraits<MatrixType>::IndexType;
      const auto m = a.getNbRows();
      const auto n = a.getNbCols();
      if ((m == 0) || (n == 0)) {
        throw(QRInvalidMatrixSize());
      }
      if ((n != v.size()) || (n != d.size())) {
        throw(QRUnmatchedSize());
      }
      for (size_type i = m; i != 0; --i) {
        const size_type l = i - 1;
        if (std::abs(d(l)) < e) {
          throw(QRNullPivot());
        }
        auto t = v(l);
        for (size_type j = i; j != n; ++j) {
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
      TFEL_CONSTEXPR const auto eps = 100 * std::numeric_limits<real>::min();
      QRDecomp::back_substitute(v, a, d, eps);
    }

    template <typename MatrixType>
    typename MatrixTraits<MatrixType>::NumType QRDecomp::enorm(
        const MatrixType& m,
        const typename MatrixTraits<MatrixType>::IndexType c,
        const typename MatrixTraits<MatrixType>::IndexType b,
        const typename MatrixTraits<MatrixType>::IndexType e) {
      using real = typename MatrixTraits<MatrixType>::NumType;
      using size_type = typename MatrixTraits<MatrixType>::IndexType;
      auto n = real(0);
      for (size_type i = b; i != e; ++i) {
        const auto v = m(i, c);
        n += v * v;
      }
      return std::sqrt(n);
    }

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_QRDECOMPIXX */
