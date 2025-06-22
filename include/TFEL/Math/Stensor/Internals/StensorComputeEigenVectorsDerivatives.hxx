/*!
 * \file   StensorComputeEigenVectorsDerivatives.hxx
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

#ifndef LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX

namespace tfel::math::internals {

  template <unsigned short N>
  struct StensorComputeEigenTensorsDerivatives;

  struct StensorComputeEigenTensorsDerivativesBase {
    template <typename NumType>
    TFEL_MATH_INLINE static
        typename ComputeBinaryResult<base_type<NumType>, NumType, OpDiv>::Result
        regularized_inverse(const NumType x, const NumType eps) {
      if (tfel::math::abs(x) < 100 * std::numeric_limits<NumType>::min()) {
        return NumType(0);
      }
      return regularization_function(x / eps) / x;
    }  // end of regularization
   protected:
    template <typename NumType>
    TFEL_MATH_INLINE static NumType regularization_function(const NumType x) {
      if (tfel::math::abs(x) > 1) {
        return NumType(1);
      }
      return x * x * (4 - x * x) / 3;
    }
  };

  template <>
  struct StensorComputeEigenTensorsDerivatives<1u> {
    template <ST2toST2Concept ST2toST2Type, typename NumType>
    static TFEL_MATH_INLINE2 void exe(
        ST2toST2Type& dn0_ds,
        ST2toST2Type& dn1_ds,
        ST2toST2Type& dn2_ds,
        const tfel::math::tvector<3u, NumType>&,
        const tfel::math::rotation_matrix<NumType>&,
        const NumType)  //
        requires(
            (getSpaceDimension<ST2toST2Type>() == 1u) &&
            (isAssignableTo<typename ComputeBinaryResult<base_type<NumType>,
                                                         NumType,
                                                         OpDiv>::Result,
                            numeric_type<ST2toST2Type>>())) {
      using namespace tfel::math;
      using namespace tfel::typetraits;
      typedef typename ComputeBinaryResult<base_type<NumType>, NumType,
                                           OpDiv>::Result InvNumType;
      dn0_ds = st2tost2<1u, InvNumType>(InvNumType(0));
      dn1_ds = dn0_ds;
      dn2_ds = dn0_ds;
    }  // end of exe
  };

  template <>
  struct StensorComputeEigenTensorsDerivatives<2u>
      : public StensorComputeEigenTensorsDerivativesBase {
    template <ST2toST2Concept ST2toST2Type, typename NumType>
    static TFEL_MATH_INLINE2 void exe(
        ST2toST2Type& dn0_ds,
        ST2toST2Type& dn1_ds,
        ST2toST2Type& dn2_ds,
        const tfel::math::tvector<3u, NumType>& vp,
        const tfel::math::rotation_matrix<NumType>& m,
        const NumType eps)  //
        requires(
            (getSpaceDimension<ST2toST2Type>() == 2u) &&
            (isAssignableTo<typename ComputeBinaryResult<base_type<NumType>,
                                                         NumType,
                                                         OpDiv>::Result,
                            numeric_type<ST2toST2Type>>())) {
      typedef base_type<NumType> base;
      typedef BinaryOperationResult<base, NumType, OpDiv> InvNumType;
      constexpr auto icste = Cste<base>::isqrt2;
      const tvector<3u, base> v0 = m.template column_view<0u>();
      const tvector<3u, base> v1 = m.template column_view<1u>();
      const stensor<2u, base> n01 =
          stensor<2u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
          icste;
      dn0_ds = regularized_inverse(vp(0) - vp(1), eps) * (n01 ^ n01);
      dn1_ds = -dn0_ds;
      dn2_ds = st2tost2<2u, InvNumType>(InvNumType(0));
    }  // end of exe
  };

  template <>
  struct StensorComputeEigenTensorsDerivatives<3u>
      : public StensorComputeEigenTensorsDerivativesBase {
    template <ST2toST2Concept ST2toST2Type, typename NumType>
    static TFEL_MATH_INLINE2 void exe(
        ST2toST2Type& dn0_ds,
        ST2toST2Type& dn1_ds,
        ST2toST2Type& dn2_ds,
        const tfel::math::tvector<3u, NumType>& vp,
        const tfel::math::rotation_matrix<NumType>& m,
        const NumType eps)  //
        requires(
            (getSpaceDimension<ST2toST2Type>() == 3u) &&
            (isAssignableTo<typename ComputeBinaryResult<base_type<NumType>,
                                                         NumType,
                                                         OpDiv>::Result,
                            numeric_type<ST2toST2Type>>())) {
      using namespace tfel::math;
      typedef base_type<NumType> base;
      constexpr auto cste = Cste<base>::isqrt2;
      const tvector<3u, base> v0 = m.template column_view<0u>();
      const tvector<3u, base> v1 = m.template column_view<1u>();
      const tvector<3u, base> v2 = m.template column_view<2u>();
      const stensor<3u, base> n01 =
          stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v1) *
          cste;
      const stensor<3u, base> n02 =
          stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v0, v2) *
          cste;
      const stensor<3u, base> n12 =
          stensor<3u, base>::buildFromVectorsSymmetricDiadicProduct(v1, v2) *
          cste;
      dn0_ds = regularized_inverse(vp(0) - vp(1), eps) * (n01 ^ n01) +
               regularized_inverse(vp(0) - vp(2), eps) * (n02 ^ n02);
      dn1_ds = regularized_inverse(vp(1) - vp(0), eps) * (n01 ^ n01) +
               regularized_inverse(vp(1) - vp(2), eps) * (n12 ^ n12);
      dn2_ds = regularized_inverse(vp(2) - vp(0), eps) * (n02 ^ n02) +
               regularized_inverse(vp(2) - vp(1), eps) * (n12 ^ n12);
    }  // end of exe
  };

}  // end of namespace tfel::math::internals

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX */
