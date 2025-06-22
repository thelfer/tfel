/*!
 * \file   include/TFEL/Math/ST2toST2/ChangeBasis.hxx
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

#ifndef LIB_TFEL_MATH_ST2TOST2_CHANGEBASIS_HXX
#define LIB_TFEL_MATH_ST2TOST2_CHANGEBASIS_HXX

#include <type_traits>

namespace tfel::math::st2tost2_internals {

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
    template <typename ST2toST2Type>
    static TFEL_MATH_INLINE2 typename std::enable_if<
        (tfel::math::implementsST2toST2Concept<ST2toST2Type>() &&
         tfel::math::getSpaceDimension<ST2toST2Type>() == 1u),
        tfel::math::st2tost2<1u, numeric_type<ST2toST2Type>>>::type
    exe(const ST2toST2Type& s,
        const tfel::math::rotation_matrix<
            tfel::math::numeric_type<ST2toST2Type>>&) {
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
    template <typename ST2toST2Type>
    static TFEL_MATH_INLINE2 typename std::enable_if<
        (tfel::math::implementsST2toST2Concept<ST2toST2Type>() &&
         tfel::math::getSpaceDimension<ST2toST2Type>() == 2u),
        tfel::math::st2tost2<2u, tfel::math::numeric_type<ST2toST2Type>>>::type
    exe(const ST2toST2Type& s,
        const tfel::math::rotation_matrix<
            tfel::math::numeric_type<ST2toST2Type>>& r) {
      using st2tost2 =
          tfel::math::st2tost2<2u, tfel::math::numeric_type<ST2toST2Type>>;
      const auto sr = st2tost2::fromRotationMatrix(r);
      const auto sir = st2tost2::fromRotationMatrix(transpose(r));
      return sr * s * sir;
    }  // end of ChangeBasis<2u>::exe
  };   // end of ChangeBasis<2u>
  /*!
   * \brief partial specialisation in 3D
   */
  template <>
  struct ChangeBasis<3u> {
    /*!
     * \return the st2tost2 that has the same effect as applying
     * the rotation
     * \param[in] r : rotation matrix
     */
    template <typename ST2toST2Type>
    static TFEL_MATH_INLINE2 typename std::enable_if<
        (tfel::math::implementsST2toST2Concept<ST2toST2Type>() &&
         tfel::math::getSpaceDimension<ST2toST2Type>() == 3u),
        tfel::math::st2tost2<3u, tfel::math::numeric_type<ST2toST2Type>>>::type
    exe(const ST2toST2Type& s,
        const tfel::math::rotation_matrix<
            tfel::math::numeric_type<ST2toST2Type>>& r) {
      using st2tost2 =
          tfel::math::st2tost2<3u, tfel::math::numeric_type<ST2toST2Type>>;
      const auto sr = st2tost2::fromRotationMatrix(r);
      const auto sir = st2tost2::fromRotationMatrix(transpose(r));
      return sr * s * sir;
    }  // end of ChangeBasis<3u>::exe
  };   // end of ChangeBasis<2u>

}  // end of namespace tfel::math::st2tost2_internals

#endif /* LIB_TFEL_MATH_ST2TOST2_CHANGEBASIS_HXX */
