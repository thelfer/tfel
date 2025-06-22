/*!
 * \file   include/TFEL/Material/IsotropicPlasticity.ixx
 * \brief This files implements the function declared in
 * `include/TFEL/Material/IsotropicPlasticity.ixx`.
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX
#define LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX

namespace tfel::material {

  template <typename StensorType>
  constexpr typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          typename tfel::math::ComputeUnaryResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>::Result>>::type
  computeJ3Derivative(const StensorType& s) {
    return tfel::math::computeDeviatorDeterminantDerivative(s);
  }  // end of computeJ3Derivative

  template <typename StensorType>
  constexpr typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::typetraits::IsScalar<
              tfel::math::numeric_type<StensorType>>::cond,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3SecondDerivative(const StensorType& s) {
    return tfel::math::computeDeviatorDeterminantSecondDerivative(s);
  }  // end of computeJ3SecondDerivative

}  // end of namespace tfel::material

#endif /* LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_IXX */
