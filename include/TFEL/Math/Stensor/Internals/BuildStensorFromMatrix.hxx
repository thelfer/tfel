/*!
 * \file   BuildStensorFromMatrix.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 janv. 2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMMATRIX_HXX
#define LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMMATRIX_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct BuildStensorFromMatrix;

      template <>
      struct BuildStensorFromMatrix<1u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<MatrixNumType<MatrixType>,
                                             T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const MatrixType& m) {
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
        }
      };

      template <>
      struct BuildStensorFromMatrix<2u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<MatrixNumType<MatrixType>,
                                             T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const MatrixType& m) {
          constexpr const auto cste = Cste<T>::isqrt2;
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
          s[3] = (m(0, 1) + m(1, 0)) * cste;
        }
      };

      template <>
      struct BuildStensorFromMatrix<3u> {
        template <typename T, typename MatrixType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<MatrixNumType<MatrixType>,
                                             T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const MatrixType& m) {
          constexpr const auto cste = Cste<T>::isqrt2;
          s[0] = m(0, 0);
          s[1] = m(1, 1);
          s[2] = m(2, 2);
          s[3] = (m(0, 1) + m(1, 0)) * cste;
          s[4] = (m(0, 2) + m(2, 0)) * cste;
          s[5] = (m(2, 1) + m(1, 2)) * cste;
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMMATRIX_HXX */
