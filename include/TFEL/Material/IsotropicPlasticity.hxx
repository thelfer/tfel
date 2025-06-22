/*!
 * \file   IsotropicPlasticity.hxx
 * \brief
 * \author Thomas Helfer
 * \date   14/01/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_HXX
#define LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_HXX

#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  /*!
   * \return the derivative of the determinant of the deviator of
   * symmetric tensor.
   *
   * Let \f$\underline{s}\f$ be a symmetric tensor and \f$J_{3}\f$
   * be the determinant of \f$\underline{s}'\f$ the deviator of
   * \f$\underline{s}\f$:
   * \f[
   * J_{3} = \mathrm{det}\left(\underline{s}'\right)
   *       =
   * \mathrm{det}\left(\underline{s}-\mathrm{tr}\left(\underline{s}'\right)\,\underline{I}\right)
   * \f]
   *
   * This function computes \f$\displaystyle\frac{\partial J_{3}}{\partial
   * \underline{\sigma}}\f$.
   *
   * \param[in] s: tensor
   *
   * \note this function is a simple wrapper around the
   * `tfel::math::computeDeviatorDeterminantDerivative` function.
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond,
      tfel::math::stensor<tfel::math::getSpaceDimension<StensorType>(),
                          typename tfel::math::ComputeUnaryResult<
                              tfel::math::numeric_type<StensorType>,
                              tfel::math::Power<2>>::Result>>::type
  computeJ3Derivative(const StensorType&);
  /*!
   * \brief Let \f$\underline{s}\f$ be a symmetric tensor and
   * \f$J_{3}\f$ be the determinant of \f$\underline{s}'\f$ the
   * deviator of \f$\underline{s}\f$:
   *
   * \f[
   * J_{3} = \mathrm{det}\left(\underline{s}'\right)
   *       =
   * \mathrm{det}\left(\underline{s}-\mathrm{tr}\left(\underline{s}'\right)\,\underline{I}\right)
   * \f]
   *
   * This function computes \f$\displaystyle\frac{\partial^{2} J_{3}}{\partial
   * \underline{\sigma}^{2}}\f$.
   *
   * \param[in] s: tensor
   * \return the second derivative of determinant of the
   * deviator of a symmetric tensor with respect to this tensor.
   *
   * \note this function is a simple wrapper around the
   * `tfel::math::computeDeviatorDeterminantSecondDerivative`
   * function.
   */
  template <typename StensorType>
  typename std::enable_if<
      tfel::meta::Implements<StensorType, tfel::math::StensorConcept>::cond &&
          tfel::typetraits::IsScalar<
              tfel::math::numeric_type<StensorType>>::cond,
      tfel::math::st2tost2<tfel::math::getSpaceDimension<StensorType>(),
                           tfel::math::numeric_type<StensorType>>>::type
  computeJ3SecondDerivative(const StensorType&);

}  // end of namespace tfel::material

#include "TFEL/Material/IsotropicPlasticity.ixx"

#endif /* LIB_TFEL_MATERIAL_ISOTROPICPLASTICITY_HXX */
