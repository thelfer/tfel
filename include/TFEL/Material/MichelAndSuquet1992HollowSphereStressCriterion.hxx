/*!
 * \file
 * include/TFEL/Material/MichelAndSuquet1992HollowSphereStressCriterion.hxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_HXX
#define LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_HXX

#include <tuple>
#include <iosfwd>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereStressType =
      tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereBaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSpherePorosityType =
      MichelAndSuquet1992HollowSphereBaseType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereInvertStressType =
      tfel::math::result_type<
          MichelAndSuquet1992HollowSphereBaseType<StressStensor>,
          MichelAndSuquet1992HollowSphereStressType<StressStensor>,
          tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereStressNormalType = tfel::math::stensor<
      tfel::math::getSpaceDimension<StressStensor>(),
      MichelAndSuquet1992HollowSphereBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereStressDerivativeWithRespectToPorosityType =
      MichelAndSuquet1992HollowSphereStressType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereStressEigenTensorType =
      tfel::math::stensor<
          tfel::math::getSpaceDimension<StressStensor>(),
          MichelAndSuquet1992HollowSphereBaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereStressSecondDerivativeType =
      tfel::math::st2tost2<
          tfel::math::getSpaceDimension<StressStensor>(),
          MichelAndSuquet1992HollowSphereInvertStressType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using MichelAndSuquet1992HollowSphereNormalDerivativeWithRespectToPorosityType =
      MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>;

  /*!
   * \brief parameters of the MichelAndSuquet1992HollowSphere criterion
   */
  template <typename StressStensor>
  struct MichelAndSuquet1992HollowSphereStressCriterionParameters {
    //! a simple alias
    using real = MichelAndSuquet1992HollowSphereBaseType<StressStensor>;
    //! \brief \f$n\f$ is the Norton exponent of the matrix
    real n;
    //! \brief \f$feps\f$ is a small numerical parameter
    real feps = real(1e-16);
    //! \brief \f$feps\f$ is a small numerical parameter
    real feps2 = real(1e-15);
  };  // end of struct MichelAndSuquet1992HollowSphereStressCriterionParameters

  /*!
   * \brief output operator
   * \param[out] os: output stream
   * \param[in] p: parameters
   */
  template <typename StressStensor>
  TFEL_HOST std::ostream& operator<<(
      std::ostream&,
      const MichelAndSuquet1992HollowSphereStressCriterionParameters<
          StressStensor>&);

  /*!
   * \brief compute the MichelAndSuquet1992HollowSphere yield stress
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE MichelAndSuquet1992HollowSphereStressType<StressStensor>
  computeMichelAndSuquet1992HollowSphereStress(
      const StressStensor&,
      const MichelAndSuquet1992HollowSpherePorosityType<StressStensor>,
      const MichelAndSuquet1992HollowSphereStressCriterionParameters<
          StressStensor>&,
      const MichelAndSuquet1992HollowSphereStressType<StressStensor>);
  /*!
   * \brief compute the MichelAndSuquet1992HollowSphere yield stress and the its
   * first derivative \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE std::tuple<
      MichelAndSuquet1992HollowSphereStressType<StressStensor>,
      MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>,
      MichelAndSuquet1992HollowSphereStressDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeMichelAndSuquet1992HollowSphereStressNormal(
      const StressStensor&,
      const MichelAndSuquet1992HollowSpherePorosityType<StressStensor>,
      const MichelAndSuquet1992HollowSphereStressCriterionParameters<
          StressStensor>&,
      const MichelAndSuquet1992HollowSphereStressType<StressStensor>);
  /*!
   * \brief compute the MichelAndSuquet1992HollowSphere yield stress and its
   * first and second derivatives \tparam StressStensor: type of the stress
   * tensor \param[in] sig: stress tensor \param[in] f: porosity \param[in] p:
   * parameters \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE std::tuple<
      MichelAndSuquet1992HollowSphereStressType<StressStensor>,
      MichelAndSuquet1992HollowSphereStressNormalType<StressStensor>,
      MichelAndSuquet1992HollowSphereStressDerivativeWithRespectToPorosityType<
          StressStensor>,
      MichelAndSuquet1992HollowSphereStressSecondDerivativeType<StressStensor>,
      MichelAndSuquet1992HollowSphereNormalDerivativeWithRespectToPorosityType<
          StressStensor>>
  computeMichelAndSuquet1992HollowSphereStressSecondDerivative(
      const StressStensor&,
      const MichelAndSuquet1992HollowSpherePorosityType<StressStensor>,
      const MichelAndSuquet1992HollowSphereStressCriterionParameters<
          StressStensor>&,
      const MichelAndSuquet1992HollowSphereStressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/MichelAndSuquet1992HollowSphereStressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_MICHEL_SUQUET_1992_HOLLOW_SPHERE_HXX */
