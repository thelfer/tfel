/*!
 * \file   include/TFEL/Math/LU/TinyMatrixSolve.ixx
 *
 * \brief
 * \author Thomas Helfer
 * \date   02 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYMATRIXSOLVE_IXX
#define LIB_TFEL_MATH_TINYMATRIXSOLVE_IXX

#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/LU/LUDecomp.hxx"
#include "TFEL/Math/LU/TinyPermutation.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N, typename T>
    void TinyMatrixSolveBase<N, T>::back_substitute(const tmatrix<N, N, T>& m,
                                                    const TinyPermutation<N>& p,
                                                    tvector<N, T>& b,
                                                    const T eps) {
      using size_type = typename MatrixTraits<tmatrix<N, N, T>>::IndexType;
      auto x = b;
      if (p.isIdentity()) {
        for (size_type i = 0; i != N; ++i) {
          auto v = T(0);
          for (size_type j = 0; j != i; ++j) {
            v += m(i, j) * x(j);
          }
          if (std::abs(m(i, i)) < eps) {
            tfel::raise<LUNullPivot>();
          }
          auto& xv = x(i);
          xv -= v;
          xv /= m(i, i);
        }
        b(N - 1) = x(N - 1);
        for (size_type i = N - 1, pi = N - 2; i != 0; --i, --pi) {
          auto v = T(0);
          for (size_type j = i; j != N; ++j) {
            v += m(pi, j) * b(j);
          }
          b(pi) = x(pi) - v;
        }
      } else {
        for (size_type i = 0; i != N; ++i) {
          const size_type pi = p(i);
          auto v = T(0);
          for (size_type j = 0; j != i; ++j) {
            v += m(pi, j) * x(p(j));
          }
          if (std::abs(m(pi, i)) < eps) {
            tfel::raise<LUNullPivot>();
          }
          auto& xv = x(pi);
          xv -= v;
          xv /= m(pi, i);
        }
        b(N - 1) = x(p(N - 1));
        for (size_type i = N - 1, pi2 = N - 2; i != 0; --i, --pi2) {
          const size_type pi = p(pi2);
          auto v = T(0);
          for (size_type j = i; j != N; ++j) {
            v += m(pi, j) * b(j);
          }
          b(pi2) = x(pi) - v;
        }
      }
    }  // end of TinyMatrixSolve<N,T>::exe

    template <unsigned short N, typename T>
    template <unsigned short M>
    void TinyMatrixSolveBase<N, T>::back_substitute(
        const tmatrix<N, N, T>& m,
        const TinyPermutation<N>& p,
        tfel::math::tmatrix<N, M, T>& b,
        const T eps) {
      using size_type = typename MatrixTraits<tmatrix<N, N, T>>::IndexType;
      auto x = b;
      if (p.isIdentity()) {
        for (size_type i = 0; i != N; ++i) {
          auto v = tvector<M, T>(T(0));
          for (size_type j = 0; j != i; ++j) {
            for (size_type k = 0; k != M; ++k) {
              v(k) += m(i, j) * x(j, k);
            }
          }
          if (std::abs(m(i, i)) < eps) {
            tfel::raise<LUNullPivot>();
          }
          for (size_type k = 0; k != M; ++k) {
            auto& xv = x(i, k);
            xv -= v(k);
            xv /= m(i, i);
          }
        }
        for (size_type k = 0; k != M; ++k) {
          b(N - 1, k) = x(N - 1, k);
        }
        for (size_type i = N - 1, pi = N - 2; i != 0; --i, --pi) {
          auto v = tvector<M, T>(T(0));
          for (size_type j = i; j != N; ++j) {
            for (size_type k = 0; k != M; ++k) {
              v(k) += m(pi, j) * b(j, k);
            }
          }
          for (size_type k = 0; k != M; ++k) {
            b(pi, k) = x(pi, k) - v(k);
          }
        }
      } else {
        for (size_type i = 0; i != N; ++i) {
          const size_type pi = p(i);
          auto v = tvector<M, T>(T(0));
          for (size_type j = 0; j != i; ++j) {
            const auto pj = p(j);
            for (size_type k = 0; k != M; ++k) {
              v(k) += m(pi, j) * x(pj, k);
            }
          }
          if (std::abs(m(pi, i)) < eps) {
            tfel::raise<LUNullPivot>();
          }
          for (size_type k = 0; k != M; ++k) {
            auto& xv = x(pi, k);
            xv -= v(k);
            xv /= m(pi, i);
          }
        }
        const auto pnmo = p(N - 1);
        for (size_type k = 0; k != M; ++k) {
          b(N - 1, k) = x(pnmo, k);
        }
        for (size_type i = N - 1, pi2 = N - 2; i != 0; --i, --pi2) {
          const size_type pi = p(pi2);
          auto v = tvector<M, T>(T(0));
          for (size_type j = i; j != N; ++j) {
            for (size_type k = 0; k != M; ++k) {
              v(k) += m(pi, j) * b(j, k);
            }
          }
          for (size_type k = 0; k != M; ++k) {
            b(pi2, k) = x(pi, k) - v(k);
          }
        }
      }
    }  // end of TinyMatrixSolve<N,T>::exe

    template <unsigned short N, typename T>
    void TinyMatrixSolveBase<N, T>::decomp(tmatrix<N, N, T>& m,
                                           TinyPermutation<N>& p,
                                           const T eps) {
      LUDecomp::exe(m, p, eps);
    }  // end of TinyMatrixSolve<N,T>::exe

    template <unsigned short N, typename T>
    void TinyMatrixSolve<N, T>::exe(tmatrix<N, N, T>& m,
                                    tvector<N, T>& b,
                                    const T eps) {
      TinyPermutation<N> p;
      TinyMatrixSolve<N, T>::decomp(m, p, eps);
      TinyMatrixSolve<N, T>::back_substitute(m, p, b, eps);
    }  // end of TinyMatrixSolve<N,T>::exe

    template <unsigned short N, typename T>
    template <unsigned short M>
    void TinyMatrixSolve<N, T>::exe(tmatrix<N, N, T>& m,
                                    tmatrix<N, M, T>& b,
                                    const T eps) {
      TinyPermutation<N> p;
      TinyMatrixSolve<N, T>::decomp(m, p, eps);
      TinyMatrixSolve<N, T>::back_substitute(m, p, b, eps);
    }  // end of TinyMatrixSolve<N,T>::exe

    // Partial specialisation for 1*1 matrix
    template <typename T>
    void TinyMatrixSolve<1u, T>::exe(const tmatrix<1u, 1u, T>& m,
                                     tvector<1u, T>& b,
                                     const T eps) {
      if (std::abs(m(0, 0)) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      b(0) /= m(0, 0);
    }  // end of TinyMatrixSolve<2u,T>::exe

    template <typename T>
    template <unsigned short M>
    void TinyMatrixSolve<1u, T>::exe(const tmatrix<1u, 1u, T>& m,
                                     tmatrix<1u, M, T>& b,
                                     const T eps) {
      if (std::abs(m(0, 0)) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      b /= m(0, 0);
    }  // end of TinyMatrixSolve<2u,T>::exe

    // Partial specialisation for 2*2 matrix
    template <typename T>
    void TinyMatrixSolve<2u, T>::exe(const tmatrix<2u, 2u, T>& m,
                                     tvector<2u, T>& b,
                                     const T eps) {
      const auto det = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
      if (std::abs(det) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      const auto b0 = b(0);
      const auto b1 = b(1);
      b(0) = (m(1, 1) * b0 - m(0, 1) * b1) / det;
      b(1) = (-m(1, 0) * b0 + m(0, 0) * b1) / det;
    }  // end of TinyMatrixSolve<2u,T>::exe

    template <typename T>
    template <unsigned short M>
    void TinyMatrixSolve<2u, T>::exe(const tmatrix<2u, 2u, T>& m,
                                     tmatrix<2u, M, T>& b,
                                     const T eps) {
      const auto det = m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0);
      if (std::abs(det) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      for (unsigned short k = 0; k != M; ++k) {
        const auto b0 = b(0, k);
        const auto b1 = b(1, k);
        b(0, k) = (m(1, 1) * b0 - m(0, 1) * b1) / det;
        b(1, k) = (-m(1, 0) * b0 + m(0, 0) * b1) / det;
      }
    }  // end of TinyMatrixSolve<2u,T>::exe

    // Partial specialisation for 3*3 matrix
    template <typename T>
    void TinyMatrixSolve<3u, T>::exe(const tmatrix<3u, 3u, T>& m,
                                     tvector<3u, T>& b,
                                     const T eps) {
      const auto det = m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
                       m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                       m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
      if (std::abs(det) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      const auto b0 = b(0);
      const auto b1 = b(1);
      const auto b2 = b(2);
      b(0) = ((m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) * b0 -
              (m(0, 1) * m(2, 2) - m(0, 2) * m(2, 1)) * b1 +
              (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * b2) /
             det;
      b(1) = ((m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * b0 +
              (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * b1 -
              (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * b2) /
             det;
      b(2) = ((m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)) * b0 -
              (m(0, 0) * m(2, 1) - m(0, 1) * m(2, 0)) * b1 +
              (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * b2) /
             det;
    }  // end of TinyMatrixSolve<2u,T>::exe

    template <typename T>
    template <unsigned short M>
    void TinyMatrixSolve<3u, T>::exe(const tmatrix<3u, 3u, T>& m,
                                     tmatrix<3u, M, T>& b,
                                     const T eps) {
      const auto det = m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
                       m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
                       m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
      if (std::abs(det) < eps) {
        tfel::raise<LUNullDeterminant>();
      }
      for (unsigned short k = 0; k != M; ++k) {
        const auto b0 = b(0, k);
        const auto b1 = b(1, k);
        const auto b2 = b(2, k);
        b(0, k) = ((m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) * b0 -
                   (m(0, 1) * m(2, 2) - m(0, 2) * m(2, 1)) * b1 +
                   (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * b2) /
                  det;
        b(1, k) = ((m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) * b0 +
                   (m(0, 0) * m(2, 2) - m(0, 2) * m(2, 0)) * b1 -
                   (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * b2) /
                  det;
        b(2, k) = ((m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)) * b0 -
                   (m(0, 0) * m(2, 1) - m(0, 1) * m(2, 0)) * b1 +
                   (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * b2) /
                  det;
      }
    }  // end of TinyMatrixSolve<2u,T>::exe

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYMATRIXSOLVE_IXX */
