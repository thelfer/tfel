/*!
 * \file   include/TFEL/Math/LUSolve.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   06 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LUSOLVE_HXX
#define LIB_TFEL_MATH_LUSOLVE_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/LU/LUException.hxx"
#include "TFEL/Math/LU/Permutation.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"

namespace tfel {

  namespace math {

    /*!
     * This structure contains two static methods for solving linear
     * systems by LU decomposition.
     */
    struct LUSolve {
      template <typename MatrixType, typename VectorType>
      static TFEL_MATH_INLINE2 void exe(MatrixType& m, VectorType& b) {
        typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
        Permutation<IndexType> p(m.getNbRows());
        VectorType x(b.size());
        LUSolve::exe(m, b, x, p);
      }  // end of exe

      template <typename MatrixType, typename VectorType>
      static void exe(
          MatrixType& m,
          VectorType& b,
          VectorType& x,
          Permutation<typename MatrixTraits<MatrixType>::IndexType>& p) {
        if (m.getNbRows() != m.getNbCols()) {
          throw(LUMatrixNotSquare());
        }
        if ((m.getNbRows() != b.size()) || (m.getNbRows() != p.size()) ||
            (m.getNbRows() != p.size())) {
          throw(LUUnmatchedSize());
        }
        if (m.getNbRows() == 0) {
          throw(LUInvalidMatrixSize());
        }
        p.reset();
        LUDecomp::exe(m, p);
        LUSolve::back_substitute(m, b, x, p);
      }  // end of LUSolve::exe

      template <typename MatrixType, typename VectorType>
      static void back_substitute(
          const MatrixType& m,
          VectorType& b,
          VectorType& x,
          Permutation<typename MatrixTraits<MatrixType>::IndexType>& p) {
        using size_type = typename MatrixTraits<MatrixType>::IndexType;
        if (m.getNbRows() != m.getNbCols()) {
          throw(LUMatrixNotSquare());
        }
        if ((m.getNbRows() != b.size()) || (m.getNbRows() != p.size()) ||
            (m.getNbRows() != p.size())) {
          throw(LUUnmatchedSize());
        }
        if (m.getNbRows() == 0) {
          throw(LUInvalidMatrixSize());
        }
        std::copy(b.begin(), b.end(), x.begin());
        const size_type n = m.getNbRows();
        for (size_type i = 0; i != n; ++i) {
          size_type pi = p(i);
          for (size_type j = 0; j != i; ++j) {
            x(pi) -= m(pi, j) * x(p(j));
          }
          x(pi) /= m(pi, i);
        }
        b(n - 1) = x(p(n - 1));
        for (size_type i = n - 1; i != 0; --i) {
          const size_type pi2 = i - 1;
          const size_type pi = p(pi2);
          b(pi2) = x(pi);
          for (size_type j = i; j != n; ++j) {
            b(pi2) -= m(pi, j) * b(j);
          }
        }
      }  // end of LUSolve::back_substitute

    };  // end of struct LUSolve

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_LUSOLVE_HXX */
