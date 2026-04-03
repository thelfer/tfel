/*!
 * \file   include/TFEL/Material/RousselierTanguyBesson2002StressCriterion.hxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   14/04/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_HXX
#define LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_HXX

#include <tuple>
#include <iosfwd>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002StressType =
      tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002BaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002PorosityType =
      RousselierTanguyBesson2002BaseType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002InvertStressType = tfel::math::result_type<
      RousselierTanguyBesson2002BaseType<StressStensor>,
      RousselierTanguyBesson2002StressType<StressStensor>,
      tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002StressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          RousselierTanguyBesson2002BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002StressDerivativeWithRespectToPorosityType =
      RousselierTanguyBesson2002StressType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002StressEigenTensorType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          RousselierTanguyBesson2002BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002StressSecondDerivativeType =
      tfel::math::st2tost2<
          tfel::math::getSpaceDimension<StressStensor>(),
          RousselierTanguyBesson2002InvertStressType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using RousselierTanguyBesson2002NormalDerivativeWithRespectToPorosityType =
      RousselierTanguyBesson2002StressNormalType<StressStensor>;

  /*!
   * \brief parameters of the RousselierTanguyBesson2002 criterion
   */
  template <typename StressStensor>
  struct RousselierTanguyBesson2002StressCriterionParameters {
    //! a simple alias
    using stress = RousselierTanguyBesson2002StressType<StressStensor>;
    //! a simple alias
    using real = RousselierTanguyBesson2002BaseType<StressStensor>;
    //! \brief \f$Dr\f$ material property
    real DR;
    //! \brief \f$qr\f$ material property
    real qR;
  };  // end of struct RousselierTanguyBesson2002StressCriterionParameters

  /*!
   * \brief output operator
   * \param[out] os: output stream
   * \param[in] p: parameters
   */
  template <typename StressStensor>
  std::ostream& operator<<(
      std::ostream&,
      const RousselierTanguyBesson2002StressCriterionParameters<
          StressStensor>&);

  /*!
   * \brief compute the RousselierTanguyBesson2002 yield stress
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE RousselierTanguyBesson2002StressType<StressStensor>
  computeRousselierTanguyBesson2002Stress(
      const StressStensor&,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const RousselierTanguyBesson2002StressCriterionParameters<StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);
  /*!
   * \brief compute the RousselierTanguyBesson2002 yield stress and the its
   * first derivative
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
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
  computeRousselierTanguyBesson2002StressNormal(
      const StressStensor&,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const RousselierTanguyBesson2002StressCriterionParameters<StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);
  /*!
   * \brief compute the RousselierTanguyBesson2002 yield stress and its first
   * and second
   * derivatives
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
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
  computeRousselierTanguyBesson2002StressSecondDerivative(
      const StressStensor&,
      const RousselierTanguyBesson2002PorosityType<StressStensor>,
      const RousselierTanguyBesson2002StressCriterionParameters<StressStensor>&,
      const RousselierTanguyBesson2002StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/RousselierTanguyBesson2002StressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_ROUSSELIER_TANGUY_BESSON_2002_HXX */
