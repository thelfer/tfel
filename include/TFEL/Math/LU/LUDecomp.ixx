/*!
 * \file   include/TFEL/Math/LU/LUDecomp.ixx
 * \brief
 * \author Thomas Helfer
 * \date   25/03/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LUDECOMP_IXX
#define LIB_TFEL_MATH_LUDECOMP_IXX

#include "TFEL/Raise.hxx"

namespace tfel::math {

  template <bool use_exceptions, bool perfom_runtime_checks>
  template <typename MatrixType, typename PermutationType>
  TFEL_HOST_DEVICE std::pair<bool, int>
  LUDecomp<use_exceptions, perfom_runtime_checks>::exe(
      MatrixType& m, PermutationType& p, const numeric_type<MatrixType> eps) {
    using size_type = index_type<MatrixType>;
    using real = numeric_type<MatrixType>;
    constexpr const auto c = real(1) / 10;
    const auto nr = m.getIndexingPolicy().size(0);
    if constexpr (perfom_runtime_checks) {
      const auto nc = m.getIndexingPolicy().size(1);
      if (nr != nc) {
        if constexpr (use_exceptions) {
          tfel::raise<LUMatrixNotSquare>();
        } else {
          return {false, 0};
        }
      }
      if (nr != p.size()) {
        if constexpr (use_exceptions) {
          tfel::raise<LUUnmatchedSize>();
        } else {
          return {false, 0};
        }
      }
      if (nr == 0) {
        if constexpr (use_exceptions) {
          tfel::raise<LUInvalidMatrixSize>();
        } else {
          return {false, 0};
        }
      }
    }
    int d = 1;
    for (size_type i = 0; i != nr; ++i) {
      // L update (column)
      if (p.isIdentity()) {
        for (size_type j = i; j != nr; ++j) {
          real v = real(0);
          for (size_type k = 0; k != i; ++k) {
            v += m(j, k) * m(k, i);
          }
          m(j, i) -= v;
        }
      } else {
        for (size_type j = i; j != nr; ++j) {
          size_type pj = p(j);
          real v = real(0);
          for (size_type k = 0; k != i; ++k) {
            v += m(pj, k) * m(p(k), i);
          }
          m(pj, i) -= v;
        }
      }
      // search for pivot
      size_type piv = i;
      if (p.isIdentity()) {
        auto cmax = tfel::math::abs(m(i, i));
        for (size_type j = static_cast<size_type>(i + 1u); j != nr; ++j) {
          if (tfel::math::abs(m(j, i)) > cmax) {
            cmax = tfel::math::abs(m(j, i));
            piv = j;
          }
        }
        if (piv != i) {
          if (!((tfel::math::abs(m(i, i)) > c * cmax) &&
                (tfel::math::abs(m(i, i)) > eps))) {
            d *= -1;
            p.swap(piv, i);
          }
        }
      } else {
        auto cmax = tfel::math::abs(m(p(i), i));
        for (size_type j = static_cast<size_type>(i + 1u); j != nr; ++j) {
          size_type pj = p(j);
          if (tfel::math::abs(m(pj, i)) > cmax) {
            cmax = tfel::math::abs(m(pj, i));
            piv = j;
          }
        }
        if (piv != i) {
          if (!((tfel::math::abs(m(p(i), i)) > c * cmax) &&
                (tfel::math::abs(m(p(i), i)) > eps))) {
            d *= -1;
            p.swap(piv, i);
          }
        }
      }
      if (tfel::math::abs(m(p(i), i)) < eps) {
        if constexpr (use_exceptions) {
          tfel::raise<LUNullPivot>();
        } else {
          return {false, 0};
        }
      }
      if (p.isIdentity()) {
        auto j = i;
        for (++j; j != nr; ++j) {
          auto v = real(0);
          for (size_type k = 0; k != i; ++k) {
            v += m(i, k) * m(k, j);
          }
          m(i, j) -= v;
          m(i, j) /= m(i, i);
        }
      } else {
        size_type pi = p(i);
        // U update
        auto j = i;
        for (++j; j != nr; ++j) {
          for (size_type k = 0; k != i; ++k) {
            m(pi, j) -= m(pi, k) * m(p(k), j);
          }
          m(pi, j) /= m(pi, i);
        }
      }
    }
    return {true, d};
  }  // end of LUDecomp::exe

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_LUDECOMP_IXX */
