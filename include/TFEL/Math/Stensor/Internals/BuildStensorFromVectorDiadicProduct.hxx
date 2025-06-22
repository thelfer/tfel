/*!
 * \file   BuildStensorFromVectorDiadicProduct.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORDIADICPRODUCT_HXX
#define LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORDIADICPRODUCT_HXX

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct BuildStensorFromVectorDiadicProduct;

      template <>
      struct BuildStensorFromVectorDiadicProduct<1u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<VectorNumType<VectorType>,
                                            Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<1u, T>& s, const VectorType& v) {
          s = {v(0) * v(0), v(1) * v(1), v(2) * v(2)};
        }
      };

      template <>
      struct BuildStensorFromVectorDiadicProduct<2u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<VectorNumType<VectorType>,
                                            Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<2u, T>& s, const VectorType& v) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {v(0) * v(0), v(1) * v(1), v(2) * v(2), v(0) * v(1) * cste};
        }
      };

      template <>
      struct BuildStensorFromVectorDiadicProduct<3u> {
        template <typename T, typename VectorType>
        static TFEL_MATH_INLINE2 TFEL_VISIBILITY_LOCAL typename std::enable_if<
            tfel::typetraits::IsAssignableTo<
                typename ComputeUnaryResult<VectorNumType<VectorType>,
                                            Power<2>>::Result,
                T>::cond,
            void>::type
        exe(stensor<3u, T>& s, const VectorType& v) {
          constexpr const auto cste = Cste<T>::sqrt2;
          s = {v(0) * v(0),        v(1) * v(1),        v(2) * v(2),
               v(0) * v(1) * cste, v(0) * v(2) * cste, v(1) * v(2) * cste};
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_BUILDSTENSORFROMVECTORDIADICPRODUCT_HXX */
