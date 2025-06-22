/*!
 * \file   BuildStensorFromVectorsSymmetricDiadicProduct.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORSSYMMETRICDIADICPRODUCT_HXX
#define LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORSSYMMETRICDIADICPRODUCT_HXX

namespace tfel {

  namespace math {

    namespace internals {

      /*!
       * \brief The `BuildStensorFromVectorsSymmetricDiadicProduct` is
       * used to build a tensor resulting from the symmetrisation of
       * the diadic product of two vectors \f$\vec{a}\f$ and
       * \f$\vec{b}\f$, defined by:
       * \f[
       * s_{ij}=a_{i}\,b_{j}+b_{i}\,a_{j}
       * \f]
       * \tparam N: space dimension
       */
      template <unsigned short N>
      struct BuildStensorFromVectorsSymmetricDiadicProduct;
      /*!
       * \brief Partial specialisation of the
       * `BuildStensorFromVectorsSymmetricDiadicProduct` structure for
       * 2D.
       */
      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<1u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<VectorNumType<VectorType>,
                                             VectorNumType<VectorType2>,
                                             OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const VectorType& v1, const VectorType2& v2) {
          s = {2 * v1(0) * v2(0), 2 * v1(1) * v2(1), 2 * v1(2) * v2(2)};
        }
      };
      /*!
       * \brief Partial specialisation of the
       * `BuildStensorFromVectorsSymmetricDiadicProduct` structure for
       * 2D.
       */
      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<2u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<VectorNumType<VectorType>,
                                             VectorNumType<VectorType2>,
                                             OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const VectorType& v1, const VectorType2& v2) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {2 * v1(0) * v2(0), 2 * v1(1) * v2(1), 2 * v1(2) * v2(2),
               cste * (v1(0) * v2(1) + v2(0) * v1(1))};
        }
      };
      /*!
       * \brief Partial specialisation of the
       * `BuildStensorFromVectorsSymmetricDiadicProduct` structure for
       * 3D.
       */
      template <>
      struct BuildStensorFromVectorsSymmetricDiadicProduct<3u> {
        template <typename T, typename VectorType, typename VectorType2>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeBinaryResult<VectorNumType<VectorType>,
                                             VectorNumType<VectorType2>,
                                             OpMult>::Result,
                T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const VectorType& v1, const VectorType2& v2) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {2 * v1(0) * v2(0),
               2 * v1(1) * v2(1),
               2 * v1(2) * v2(2),
               (v1(0) * v2(1) + v2(0) * v1(1)) * cste,
               (v1(0) * v2(2) + v2(0) * v1(2)) * cste,
               (v1(1) * v2(2) + v2(1) * v1(2)) * cste};
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORSSYMMETRICDIADICPRODUCT_HXX \
        */
