/*!
 * \file   include/TFEL/Material/Hosford1972YieldCriterion.hxx
 * \brief
 * \author Thomas Helfer
 * \date   15/11/2017
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_HOSFORD_HXX
#define LIB_TFEL_MATERIAL_HOSFORD_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel {

  namespace material {

    //! a simple alias
    template <typename StressStensor>
    using HosfordStressType = tfel::math::StensorNumType<StressStensor>;
    //! a simple alias
    template <typename StressStensor>
    using HosfordBaseType =
        tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using HosfordInvertStressType =
        tfel::math::result_type<HosfordBaseType<StressStensor>,
                                HosfordStressType<StressStensor>,
                                tfel::math::OpDiv>;
    //! a simple alias
    template <typename StressStensor>
    using HosfordStressNormalType =
        tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
                            HosfordBaseType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using HosfordStressEigenTensorType =
        tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
                            HosfordBaseType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using HosfordStressSecondDerivativeType =
        tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
                             HosfordInvertStressType<StressStensor>>;
    /*!
     * \brief compute the Hosford yield stress
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] a: Hosford exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template <typename StressStensor,
              typename HosfordExponentType,
              tfel::math::stensor_common::EigenSolver =
                  tfel::math::stensor_common::TFELEIGENSOLVER>
    HosfordStressType<StressStensor> computeHosfordStress(
        const StressStensor&,
        const HosfordExponentType,
        const HosfordStressType<StressStensor>);
    /*!
     * \brief compute the Hosford yield stress and the its first derivative
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] a: Hosford exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template <typename StressStensor,
              typename HosfordExponentType,
              tfel::math::stensor_common::EigenSolver =
                  tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<HosfordStressType<StressStensor>,
               HosfordStressNormalType<StressStensor>>
    computeHosfordStressNormal(const StressStensor&,
                               const HosfordExponentType,
                               const HosfordStressType<StressStensor>);
    /*!
     * \brief compute the Hosford yield stress and its first and second
     * derivatives
     * \tparam StressStensor: type of the stress tensor
     * \tparam HosfordExponentType: type of the hosford exponent
     * (could be a numeric type or an integer type)
     * \tparam es: eigen solver to be used
     * \param[in] s: stress tensor
     * \param[in] a: Hosford exponent
     * \param[in] e: criterion used to check if the stress are null
     */
    template <typename StressStensor,
              typename HosfordExponentType,
              tfel::math::stensor_common::EigenSolver =
                  tfel::math::stensor_common::TFELEIGENSOLVER>
    std::tuple<HosfordStressType<StressStensor>,
               HosfordStressNormalType<StressStensor>,
               HosfordStressSecondDerivativeType<StressStensor>>
    computeHosfordStressSecondDerivative(
        const StressStensor&,
        const HosfordExponentType,
        const HosfordStressType<StressStensor>);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/Hosford1972YieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_HOSFORD_HXX */
