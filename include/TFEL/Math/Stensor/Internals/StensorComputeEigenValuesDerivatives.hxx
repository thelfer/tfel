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

namespace tfel {

  namespace math {

    namespace internals {

      template <unsigned short N>
      struct StensorComputeEigenValuesDerivatives;

      template <>
      struct StensorComputeEigenValuesDerivatives<1u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 1u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    StensorNumType<StensorType>>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>&) {
          constexpr const auto zero = BaseType(0);
          constexpr const auto one = BaseType(1);
          n0 = {one, zero, zero};
          n1 = {zero, one, zero};
          n2 = {zero, zero, one};
        }
      };

      template <>
      struct StensorComputeEigenValuesDerivatives<2u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 2u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    StensorNumType<StensorType>>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>& m) {
          using namespace tfel::math;
          const tvector<3u, BaseType> v0 = m.template column_view<0u>();
          const tvector<3u, BaseType> v1 = m.template column_view<1u>();
          constexpr const auto zero = BaseType(0);
          constexpr const auto one = BaseType(1);
          n0 = StensorType::buildFromVectorDiadicProduct(v0);
          n1 = StensorType::buildFromVectorDiadicProduct(v1);
          n2 = {zero, zero, one, zero};
        }
      };

      template <>
      struct StensorComputeEigenValuesDerivatives<3u> {
        template <typename StensorType, typename BaseType>
        static TFEL_MATH_INLINE2 typename std::enable_if<
            (tfel::meta::Implements<StensorType, StensorConcept>::cond) &&
                (StensorTraits<StensorType>::dime == 3u) &&
                (tfel::typetraits::IsAssignableTo<
                    BaseType,
                    StensorNumType<StensorType>>::cond),
            void>::type
        exe(StensorType& n0,
            StensorType& n1,
            StensorType& n2,
            const tfel::math::tmatrix<3u, 3u, BaseType>& m) {
          using namespace tfel::math;
          const tvector<3u, BaseType> v0 = m.template column_view<0u>();
          const tvector<3u, BaseType> v1 = m.template column_view<1u>();
          const tvector<3u, BaseType> v2 = m.template column_view<2u>();
          n0 = StensorType::buildFromVectorDiadicProduct(v0);
          n1 = StensorType::buildFromVectorDiadicProduct(v1);
          n2 = StensorType::buildFromVectorDiadicProduct(v2);
        }
      };

    }  // end of namespace internals

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVALUESDERIVATIVES_HXX */
