/*!
 * \file   include/TFEL/Material/Cazacu2004OrthotropicYieldCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   10/05/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_HXX
#define LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/OrthotropicPlasticity.hxx"
#include "TFEL/Material/Cazacu2004YieldCriterionTypes.hxx"

namespace tfel::material {

  /*!
   * \brief compute the Cazacu 2004 orthotropic yield stress defined by:
   * \f[
   * \sigma_{\mathrm{eq}}^{C}=
   * \sqrt{3}\,\sqrt[6]{\left(J_{2}^{O}\right)^{3} -
   * c\,\left(J_{3}^{O}\right)^{2}}
   * \f]
   * where \(J_{2}\) and \(J_{3}\) are the first and second invariants
   * of the deviatoric part of the stress tensor
   *
   * Cazacu, Oano and Barlat, Frédéric. 2004.
   * “A Criterion for Description of Anisotropy and Yield Differential
   * Effects in Pressure-Insensitive Metals.”
   * International Journal of Plasticity,
   * Daniel c. Drucker memorial issue, 20 (11):2027–45.
   * https://doi.org/10.1016/j.ijplas.2003.11.021.
   *
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
   * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
   * \param[in] c: c coefficient
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE Cazacu2004StressType<StressStensor>
  computeCazacu2004OrthotropicStressCriterion(
      const StressStensor&,
      const J2OCoefficients<StressStensor>&,
      const J3OCoefficients<StressStensor>&,
      const Cazacu2004BaseType<StressStensor>);
  /*!
   * \brief compute the Cazacu 2004 orthotropic yield stress and the its first
   * derivative.
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
   * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
   * \param[in] c: c coefficient
   * \param[in] seps: small stress value. This value is used to regularize the
   *                  computation of the stress normal.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE std::tuple<Cazacu2004StressType<StressStensor>,
                              Cazacu2004StressNormalType<StressStensor>>
  computeCazacu2004OrthotropicStressCriterionNormal(
      const StressStensor&,
      const J2OCoefficients<StressStensor>&,
      const J3OCoefficients<StressStensor>&,
      const Cazacu2004BaseType<StressStensor>,
      const Cazacu2004StressType<StressStensor>);
  /*!
   * \brief compute the Cazacu 2004 orthotropic yield stress and its first and
   * second derivatives
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
   * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
   * \param[in] c: c coefficient
   * \param[in] seps: small stress value. This value is used to regularize the
   *                  computation of the stress normal.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE
      std::tuple<Cazacu2004StressType<StressStensor>,
                 Cazacu2004StressNormalType<StressStensor>,
                 Cazacu2004StressSecondDerivativeType<StressStensor>>
      computeCazacu2004OrthotropicStressCriterionSecondDerivative(
          const StressStensor&,
          const J2OCoefficients<StressStensor>&,
          const J3OCoefficients<StressStensor>&,
          const Cazacu2004BaseType<StressStensor>,
          const Cazacu2004StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/Cazacu2004OrthotropicYieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_CAZACU2004ORTHOTROPICYIELDCRITERION_HXX */
