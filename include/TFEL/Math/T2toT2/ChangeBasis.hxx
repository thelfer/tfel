/*!
 * \file   include/TFEL/Math/T2toT2/ChangeBasis.hxx
 * \brief
 * \author Thomas Helfer
 * \date   13 oct. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2_CHANGEBASIS_HXX
#define LIB_TFEL_MATH_T2TOT2_CHANGEBASIS_HXX

#include <type_traits>

namespace tfel {

  namespace math {

    namespace t2tot2_internals {

      /*!
       * \brief an auxiliary structure used to dispatch over the
       * dimension
       */
      template <unsigned short>
      struct ChangeBasis;

      /*!
       * \brief partial specialisation in 1D
       */
      template <>
      struct ChangeBasis<1u> {
        /*!
         */
        template <typename T2toT2Type>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond &&
             tfel::math::T2toT2Traits<T2toT2Type>::dime == 1u),
            tfel::math::t2tot2<
                1u,
                typename tfel::math::T2toT2Traits<T2toT2Type>::NumType>>::type
        exe(const T2toT2Type& s,
            const tfel::math::tmatrix<3,
                                      3,
                                      typename tfel::typetraits::BaseType<
                                          typename tfel::math::T2toT2Traits<
                                              T2toT2Type>::NumType>::type>&) {
          return s;
        }
      };  // end of ChangeBasis<1u>
      /*!
       * \brief partial specialisation in 2D
       */
      template <>
      struct ChangeBasis<2u> {
        /*!
         * \param[in] r : rotation matrix
         */
        template <typename T2toT2Type>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond &&
             tfel::math::T2toT2Traits<T2toT2Type>::dime == 2u),
            tfel::math::t2tot2<
                2u,
                typename tfel::math::T2toT2Traits<T2toT2Type>::NumType>>::type
        exe(const T2toT2Type& s,
            const tfel::math::tmatrix<3,
                                      3,
                                      typename tfel::typetraits::BaseType<
                                          typename tfel::math::T2toT2Traits<
                                              T2toT2Type>::NumType>::type>& r) {
          using t2tot2 = tfel::math::t2tot2<
              2u, typename tfel::math::T2toT2Traits<T2toT2Type>::NumType>;
          const auto sr = t2tot2::fromRotationMatrix(r);
          const auto sir = t2tot2::fromRotationMatrix(transpose(r));
          return sr * s * sir;
        }  // end of ChangeBasis<2u>::exe
      };   // end of ChangeBasis<2u>
      /*!
       * \brief partial specialisation in 3D
       */
      template <>
      struct ChangeBasis<3u> {
        /*!
         * \return the t2tot2 that has the same effect as applying
         * the rotation
         * \param[in] r : rotation matrix
         */
        template <typename T2toT2Type>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<T2toT2Type,
                                    tfel::math::T2toT2Concept>::cond &&
             tfel::math::T2toT2Traits<T2toT2Type>::dime == 3u),
            tfel::math::t2tot2<
                3u,
                typename tfel::math::T2toT2Traits<T2toT2Type>::NumType>>::type
        exe(const T2toT2Type& s,
            const tfel::math::tmatrix<3,
                                      3,
                                      typename tfel::typetraits::BaseType<
                                          typename tfel::math::T2toT2Traits<
                                              T2toT2Type>::NumType>::type>& r) {
          using t2tot2 = tfel::math::t2tot2<
              3u, typename tfel::math::T2toT2Traits<T2toT2Type>::NumType>;
          const auto sr = t2tot2::fromRotationMatrix(r);
          const auto sir = t2tot2::fromRotationMatrix(transpose(r));
          return sr * s * sir;
        }  // end of ChangeBasis<3u>::exe
      };   // end of ChangeBasis<2u>

    }  // end of namespace t2tot2_internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2_CHANGEBASIS_HXX */
