/*!
 * \file   include/TFEL/Material/Drucker1949YieldCriterion.hxx
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

#ifndef LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_HXX
#define LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel {

  namespace material {

    //! a simple alias
    template <typename StressStensor>
    using Drucker1949StressType = tfel::math::StensorNumType<StressStensor>;
    //! a simple alias
    template <typename StressStensor>
    using Drucker1949BaseType =
        tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using Drucker1949InvertStressType =
        tfel::math::result_type<Drucker1949BaseType<StressStensor>,
                                Drucker1949StressType<StressStensor>,
                                tfel::math::OpDiv>;
    //! a simple alias
    template <typename StressStensor>
    using Drucker1949StressNormalType =
        tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
                            Drucker1949BaseType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using Drucker1949StressSecondDerivativeType =
        tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
                             Drucker1949InvertStressType<StressStensor>>;
    /*!
     * \brief compute the Drucker1949 yield stress defined by:
     * \f[
     * \sigma_{\mathrm{eq}}^{D}=
     * \sqrt{3}\,\sqrt[6]{J_{2}^{3} - c\,J_{3}^{2}}
     * \f]
     * where \(J_{2}\) and \(J_{3}\) are the first and second invariants
     * of the deviatoric part of the stress tensor
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] c: c coefficient
     */
    template <typename StressStensor>
    Drucker1949StressType<StressStensor> computeDrucker1949StressCriterion(
        const StressStensor&, const Drucker1949BaseType<StressStensor>);
    /*!
     * \brief compute the Drucker1949 yield stress and the its first derivative
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] c: c coefficient
     * \param[in] seps: small stress value. This value is used to regularize the
     *                  computation of the stress normal.
     */
    template <typename StressStensor>
    std::tuple<Drucker1949StressType<StressStensor>,
               Drucker1949StressNormalType<StressStensor>>
    computeDrucker1949StressCriterionNormal(
        const StressStensor&,
        const Drucker1949BaseType<StressStensor>,
        const Drucker1949StressType<StressStensor>);
    /*!
     * \brief compute the Drucker1949 yield stress and its first and second
     * derivatives
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] c: c coefficient
     * \param[in] seps: small stress value. This value is used to regularize the
     *                  computation of the stress normal.
     */
    template <typename StressStensor>
    std::tuple<Drucker1949StressType<StressStensor>,
               Drucker1949StressNormalType<StressStensor>,
               Drucker1949StressSecondDerivativeType<StressStensor>>
    computeDrucker1949StressCriterionSecondDerivative(
        const StressStensor&,
        const Drucker1949BaseType<StressStensor>,
        const Drucker1949StressType<StressStensor>);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/Drucker1949YieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_DRUCKER1949YIELDCRITERION_HXX */
