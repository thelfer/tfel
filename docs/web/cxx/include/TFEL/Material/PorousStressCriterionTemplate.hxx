/*!
 * \file   include/TFEL/Material/__StressCriterionName__StressCriterion.hxx
 * \brief
 * \author __Author__
 * \date   __Date__
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___HXX
#define LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___HXX

#include <tuple>
#include <iosfwd>
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::material {

  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__StressType =
      tfel::math::StensorNumType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__BaseType =
      tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__PorosityType =
      __StressCriterionName__BaseType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__InvertStressType =
      tfel::math::result_type<__StressCriterionName__BaseType<StressStensor>,
                              __StressCriterionName__StressType<StressStensor>,
                              tfel::math::OpDiv>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__StressNormalType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          __StressCriterionName__BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__StressDerivativeWithRespectToPorosityType =
      __StressCriterionName__StressType<StressStensor>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__StressEigenTensorType =
      tfel::math::stensor<tfel::math::getSpaceDimension<StressStensor>(),
                          __StressCriterionName__BaseType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__StressSecondDerivativeType =
      tfel::math::st2tost2<
          tfel::math::getSpaceDimension<StressStensor>(),
          __StressCriterionName__InvertStressType<StressStensor>>;
  //! a simple alias
  template <typename StressStensor>
  using __StressCriterionName__NormalDerivativeWithRespectToPorosityType =
      __StressCriterionName__StressNormalType<StressStensor>;

  /*!
   * \brief parameters of the __StressCriterionName__ criterion
   */
  template <typename StressStensor>
  struct __StressCriterionName__StressCriterionParameters {
    //! a simple alias
    using stress = __StressCriterionName__StressType<StressStensor>;
    //! a simple alias
    using real = __StressCriterionName__BaseType<StressStensor>;

  };  // end of struct __StressCriterionName__StressCriterionParameters

  /*!
   * \brief output operator
   * \param[out] os: output stream
   * \param[in] p: parameters
   */
  template <typename StressStensor>
  std::ostream& operator<<(
      std::ostream&,
      const __StressCriterionName__StressCriterionParameters<StressStensor>&);

  /*!
   * \brief compute the __StressCriterionName__ yield stress
   * \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  __StressCriterionName__StressType<StressStensor>
  compute__StressCriterionName__Stress(
      const StressStensor&,
      const __StressCriterionName__PorosityType<StressStensor>,
      const __StressCriterionName__StressCriterionParameters<StressStensor>&,
      const __StressCriterionName__StressType<StressStensor>);
  /*!
   * \brief compute the __StressCriterionName__ yield stress and the its first
   * derivative \tparam StressStensor: type of the stress tensor \param[in]
   * sig: stress tensor \param[in] f: porosity \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  std::tuple<__StressCriterionName__StressType<StressStensor>,
             __StressCriterionName__StressNormalType<StressStensor>,
             __StressCriterionName__StressDerivativeWithRespectToPorosityType<
                 StressStensor>>
  compute__StressCriterionName__StressNormal(
      const StressStensor&,
      const __StressCriterionName__PorosityType<StressStensor>,
      const __StressCriterionName__StressCriterionParameters<StressStensor>&,
      const __StressCriterionName__StressType<StressStensor>);
  /*!
   * \brief compute the __StressCriterionName__ yield stress and its first and
   * second derivatives \tparam StressStensor: type of the stress tensor
   * \param[in] sig: stress tensor
   * \param[in] f: porosity
   * \param[in] p: parameters
   * \param[in] seps: threshold for the equivalent stress.
   */
  template <typename StressStensor>
  std::tuple<__StressCriterionName__StressType<StressStensor>,
             __StressCriterionName__StressNormalType<StressStensor>,
             __StressCriterionName__StressDerivativeWithRespectToPorosityType<
                 StressStensor>,
             __StressCriterionName__StressSecondDerivativeType<StressStensor>,
             __StressCriterionName__NormalDerivativeWithRespectToPorosityType<
                 StressStensor>>
  compute__StressCriterionName__StressSecondDerivative(
      const StressStensor&,
      const __StressCriterionName__PorosityType<StressStensor>,
      const __StressCriterionName__StressCriterionParameters<StressStensor>&,
      const __StressCriterionName__StressType<StressStensor>);

}  // end of namespace tfel::material

#include "TFEL/Material/__StressCriterionName__StressCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL___STRESS_CRITERION_NAME___HXX */
