/*!
 * \file   include/TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.hxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   24/03/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_GURSON_1975_HXX
#define LIB_TFEL_MATERIAL_GURSON_1975_HXX

#include <iosfwd>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982StressType =
      tfel::math::numeric_type<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982BaseType =
      tfel::typetraits::base_type<tfel::math::numeric_type<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982PorosityType =
      GursonTvergaardNeedleman1982BaseType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982InvertStressType = tfel::math::result_type<
      GursonTvergaardNeedleman1982BaseType<StressStensor>,
      GursonTvergaardNeedleman1982StressType<StressStensor>,
      tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982StressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          GursonTvergaardNeedleman1982BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982StressDerivativeWithRespectToPorosityType =
      GursonTvergaardNeedleman1982StressType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982StressEigenTensorType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          GursonTvergaardNeedleman1982BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982StressSecondDerivativeType =
      tfel::math::st2tost2<
          tfel::math::getSpaceDimension<StressStensor>(),
          GursonTvergaardNeedleman1982InvertStressType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using GursonTvergaardNeedleman1982NormalDerivativeWithRespectToPorosityType =
      GursonTvergaardNeedleman1982StressNormalType<StressStensor>;

  /*!
   * \brief parameters of the GursonTvergaardNeedleman1982 criterion
   */
  template <typename StressStensor>
  struct GursonTvergaardNeedleman1982StressCriterionParameters {
    //! a simple alias
    using real = GursonTvergaardNeedleman1982BaseType<StressStensor>;
    //! \brief \f$f_c\f$ material property
    real f_c;
    //! \brief \f$f_r\f$ material property
    real f_r;
    //! \brief \f$q_1\f$ material property
    real q_1;
    //! \brief \f$q_2\f$ material property
    real q_2;
    //! \brief \f$q_3\f$ material property
    real q_3;
    /*!
     * \brief convergence criterion for the determination of the equilvalent
     * stress.
     */
    real eps = real(1e-14);
  };  // end of struct GursonTvergaardNeedleman1982StressCriterionParameters

  /*!
   * \brief output operator
   * \param[out] os: output stream
   * \param[in] p: parameters
   */
  template <typename StressStensor>
  std::ostream& operator<<(
      std::ostream&,
      const GursonTvergaardNeedleman1982StressCriterionParameters<
          StressStensor>&);

  /*!
   * \brief compute the GursonTvergaardNeedleman1982 yield stress
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  TFEL_HOST_DEVICE GursonTvergaardNeedleman1982StressType<StressStensor>
  computeGursonTvergaardNeedleman1982Stress(
      const StressStensor&,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GursonTvergaardNeedleman1982StressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);
  /*!
   * \brief compute the GursonTvergaardNeedleman1982 yield stress and the its
   * first derivative
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
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
  computeGursonTvergaardNeedleman1982StressNormal(
      const StressStensor&,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GursonTvergaardNeedleman1982StressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);
  /*!
   * \brief compute the GursonTvergaardNeedleman1982 yield stress and its
   * first and second
   * derivatives
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
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
  computeGursonTvergaardNeedleman1982StressSecondDerivative(
      const StressStensor&,
      const GursonTvergaardNeedleman1982PorosityType<StressStensor>,
      const GursonTvergaardNeedleman1982StressCriterionParameters<
          StressStensor>&,
      const GursonTvergaardNeedleman1982StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/GursonTvergaardNeedleman1982StressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_GURSON_1975_HXX */
