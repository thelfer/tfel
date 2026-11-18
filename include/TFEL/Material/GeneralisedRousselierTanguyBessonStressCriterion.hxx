/*!
 * \file
 * include/TFEL/Material/GeneralisedRousselierTanguyBessonStressCriterion.hxx
 * \brief  This file declares the generalised Rousselier-Tanguy-Besson stress
 * criterion.
 * \author Tristan Chenaille
 * \date   14/11/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_HXX
#define LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_HXX

#include <tuple>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/RousselierTanguyBesson2002StressCriterion.hxx"

namespace tfel::material {

  /*!
   * \brief parameters of the GeneralisedRousselierTanguyBesson criterion, which
   * are the parameters of the RousselierTanguyBesson2002 criterion.
   */
  template <typename StressStensor>
  using GeneralisedRousselierTanguyBessonStressCriterionParameters =
      RousselierTanguyBesson2002StressCriterionParameters<StressStensor>;

  /*!
   * \brief compute the GeneralisedRousselierTanguyBesson stress.
   *
   * The stress \f$\sigma_{\star}\f$ is the solution of:
   *
   * \f[
   * \frac{\sigma_{\mathrm{eq}}}{(1-f)\,\sigma_{\star}}
   * + \frac{2}{3}\,f\,D_R\,\exp\left(\frac{3\,q_R\,\sigma_m}
   * {2\,(1-f)\,\sigma_{\star}}\right) - 1 = 0
   * \f]
   *
   * where \f$\sigma_{\mathrm{eq}}\f$ is an equivalent stress given by the
   * caller and \f$\sigma_m\f$ is the mean stress. The
   * RousselierTanguyBesson2002 criterion is recovered if
   * \f$\sigma_{\mathrm{eq}}\f$ is the von Mises stress.
   *
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] seq: equivalent stress
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE RousselierTanguyBesson2002StressType<StressStensor>
  computeGeneralisedRousselierTanguyBessonStress(
      const StressStensor&,
      const RousselierTanguyBesson2002StressType<StressStensor>,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);
  /*!
   * \brief compute the GeneralisedRousselierTanguyBesson stress and its first
   * derivatives
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] seq: equivalent stress
   * \param[in] dseq: derivative of the equivalent stress with respect to the
   * stress
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE std::tuple<
      RousselierTanguyBesson2002StressType<StressStensor>,
      RousselierTanguyBesson2002StressNormalType<StressStensor>,
      RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedRousselierTanguyBessonStressNormal(
      const StressStensor&,
      const RousselierTanguyBesson2002StressType<StressStensor>,
      const RousselierTanguyBesson2002StressNormalType<StressStensor>&,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);
  /*!
   * \brief compute the GeneralisedRousselierTanguyBesson stress and its first
   * and second derivatives
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] seq: equivalent stress
   * \param[in] dseq: derivative of the equivalent stress with respect to the
   * stress
   * \param[in] d2seq: second derivative of the equivalent stress with respect
   * to the stress
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE std::tuple<
      RousselierTanguyBesson2002StressType<StressStensor>,
      RousselierTanguyBesson2002StressNormalType<StressStensor>,
      RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType<
          StressStensor>,
      RousselierTanguyBesson2002StressSecondDerivativeType<StressStensor>,
      RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedRousselierTanguyBessonStressSecondDerivative(
      const StressStensor&,
      const RousselierTanguyBesson2002StressType<StressStensor>,
      const RousselierTanguyBesson2002StressNormalType<StressStensor>&,
      const RousselierTanguyBesson2002StressSecondDerivativeType<
          StressStensor>&,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const GeneralisedRousselierTanguyBessonStressCriterionParameters<
          StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/GeneralisedRousselierTanguyBessonStressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_GENERALISED_ROUSSELIER_TANGUY_BESSON_STRESS_CRITERION_HXX \
        */
