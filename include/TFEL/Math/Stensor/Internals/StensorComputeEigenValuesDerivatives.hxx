/*!
 * \file   StensorComputeEigenValuesDerivatives.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVALUESDERIVATIVES_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVALUESDERIVATIVES_HXX

namespace tfel::math::internals {

  template <unsigned short N>
  struct StensorComputeEigenValuesDerivatives;

  template <>
  struct StensorComputeEigenValuesDerivatives<1u> {
    template <typename StensorType>
    static TFEL_MATH_INLINE2
        typename std::enable_if<((implementsStensorConcept<StensorType>()) &&
                                 (getSpaceDimension<StensorType>() == 1u)),
                                void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const rotation_matrix<numeric_type<StensorType>>&) {
      using real = base_type<numeric_type<StensorType>>;
      constexpr auto zero = real(0);
      constexpr auto one = real(1);
      n0 = {one, zero, zero};
      n1 = {zero, one, zero};
      n2 = {zero, zero, one};
    }
  };

  template <>
  struct StensorComputeEigenValuesDerivatives<2u> {
    template <typename StensorType>
    static TFEL_MATH_INLINE2
        typename std::enable_if<((implementsStensorConcept<StensorType>()) &&
                                 (getSpaceDimension<StensorType>() == 2u)),
                                void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const rotation_matrix<numeric_type<StensorType>>& m) {
      using real = base_type<numeric_type<StensorType>>;
      constexpr auto zero = real(0);
      constexpr auto one = real(1);
      const tvector<3u, real> v0 = m.template column_view<0u>();
      const tvector<3u, real> v1 = m.template column_view<1u>();
      n0 = StensorType::buildFromVectorDiadicProduct(v0);
      n1 = StensorType::buildFromVectorDiadicProduct(v1);
      n2 = {zero, zero, one, zero};
    }
  };

  template <>
  struct StensorComputeEigenValuesDerivatives<3u> {
    template <typename StensorType>
    static TFEL_MATH_INLINE2
        typename std::enable_if<((implementsStensorConcept<StensorType>()) &&
                                 (getSpaceDimension<StensorType>() == 3u)),
                                void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const rotation_matrix<numeric_type<StensorType>>& m) {
      using real = base_type<numeric_type<StensorType>>;
      const tvector<3u, real> v0 = m.template column_view<0u>();
      const tvector<3u, real> v1 = m.template column_view<1u>();
      const tvector<3u, real> v2 = m.template column_view<2u>();
      n0 = StensorType::buildFromVectorDiadicProduct(v0);
      n1 = StensorType::buildFromVectorDiadicProduct(v1);
      n2 = StensorType::buildFromVectorDiadicProduct(v2);
    }
  };

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVALUESDERIVATIVES_HXX */
