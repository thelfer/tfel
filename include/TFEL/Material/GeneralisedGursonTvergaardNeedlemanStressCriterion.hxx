/*!
 * \file
 * include/TFEL/Material/GeneralisedGursonTvergaardNeedlemanStressCriterion.hxx
 * \brief  This file declares the generalised Gurson-Tvergaard-Needleman stress
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

#ifndef LIB_TFEL_MATERIAL_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESS_CRITERION_HXX
#define LIB_TFEL_MATERIAL_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESS_CRITERION_HXX

#include <tuple>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.hxx"

namespace tfel::material {

  /*!
   * \brief parameters of the GeneralisedGursonTvergaardNeedleman criterion,
   * which are the parameters of the GursonTvergaardNeedleman1982 criterion.
   */
  template <typename StressStensor>
  using GeneralisedGursonTvergaardNeedlemanStressCriterionParameters =
      GursonTvergaardNeedleman1982StressCriterionParameters<StressStensor>;

  /*!
   * \brief compute the GeneralisedGursonTvergaardNeedleman stress.
   *
   * The stress \f$\sigma_{\star}\f$ is the solution of:
   *
   * \f[
   * \left(\frac{\sigma_{\mathrm{eq}}}{\sigma_{\star}}\right)^2
   * + 2\,q_1\,f_{\star}\,\cosh\left(\frac{3\,q_2\,\sigma_m}
   * {2\,\sigma_{\star}}\right) - 1 - q_3\,f_{\star}^2 = 0
   * \f]
   *
   * where \f$\sigma_{\mathrm{eq}}\f$ is an equivalent stress given by the
   * caller and \f$\sigma_m\f$ is the mean stress. The
   * GursonTvergaardNeedleman1982 criterion is recovered if
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
  TFEL_HOST_DEVICE GursonTvergaardNeedleman1982StressType<StressStensor>
  computeGeneralisedGursonTvergaardNeedlemanStress(
      const StressStensor&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GeneralisedGursonTvergaardNeedlemanStressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);
  /*!
   * \brief compute the GeneralisedGursonTvergaardNeedleman stress and its first
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
      GursonTvergaardNeedleman1982StressType<StressStensor>,
      GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
      GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedGursonTvergaardNeedlemanStressNormal(
      const StressStensor&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>,
      const GursonTvergaardNeedleman1982StressNormalType<StressStensor>&,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GeneralisedGursonTvergaardNeedlemanStressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);
  /*!
   * \brief compute the GeneralisedGursonTvergaardNeedleman stress and its first
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
      GursonTvergaardNeedleman1982StressType<StressStensor>,
      GursonTvergaardNeedleman1982StressNormalType<StressStensor>,
      GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType<
          StressStensor>,
      GursonTvergaardNeedleman1982StressSecondDerivativeType<StressStensor>,
      GursonTvergaardNeedleman1982NormalDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeGeneralisedGursonTvergaardNeedlemanStressSecondDerivative(
      const StressStensor&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>,
      const GursonTvergaardNeedleman1982StressNormalType<StressStensor>&,
      const GursonTvergaardNeedleman1982StressSecondDerivativeType<
          StressStensor>&,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GeneralisedGursonTvergaardNeedlemanStressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/GeneralisedGursonTvergaardNeedlemanStressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_GENERALISED_GURSON_TVERGAARD_NEEDLEMAN_STRESS_CRITERION_HXX \
        */
