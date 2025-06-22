/*!
 * \file   include/TFEL/Material/Cazacu2001YieldCriterion.hxx
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

#ifndef LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_HXX
#define LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Material/OrthotropicPlasticity.hxx"

namespace tfel {

  namespace material {

    //! a simple alias
    template <typename StressStensor>
    using Cazacu2001StressType = tfel::math::StensorNumType<StressStensor>;
    //! a simple alias
    template <typename StressStensor>
    using Cazacu2001BaseType =
        tfel::typetraits::base_type<tfel::math::StensorNumType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using Cazacu2001InvertStressType =
        tfel::math::result_type<Cazacu2001BaseType<StressStensor>,
                                Cazacu2001StressType<StressStensor>,
                                tfel::math::OpDiv>;
    //! a simple alias
    template <typename StressStensor>
    using Cazacu2001StressNormalType =
        tfel::math::stensor<tfel::math::StensorTraits<StressStensor>::dime,
                            Cazacu2001BaseType<StressStensor>>;
    //! a simple alias
    template <typename StressStensor>
    using Cazacu2001StressSecondDerivativeType =
        tfel::math::st2tost2<tfel::math::StensorTraits<StressStensor>::dime,
                             Cazacu2001InvertStressType<StressStensor>>;

    /*!
     * \brief compute the Cazacu2001 yield stress defined by:
     * \f[
     * \sigma_{\mathrm{eq}}^{C}=
     * \sqrt{3}\,\sqrt[6]{\left(J_{2}^{O}\right)^{3} -
     * c\,\left(J_{3}^{O}\right)^{2}} \f] where \(J_{2}\) and \(J_{3}\) are the
     * first and second invariants of the deviatoric part of the stress tensor
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
     * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
     * \param[in] c: c coefficient
     */
    template <typename StressStensor>
    Cazacu2001StressType<StressStensor> computeCazacu2001StressCriterion(
        const StressStensor&,
        const J2OCoefficients<StressStensor>&,
        const J3OCoefficients<StressStensor>&,
        const Cazacu2001BaseType<StressStensor>);
    /*!
     * \brief compute the Cazacu2001 yield stress and the its first derivative
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
     * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
     * \param[in] c: c coefficient
     * \param[in] seps: small stress value. This value is used to regularize the
     *                  computation of the stress normal.
     */
    template <typename StressStensor>
    std::tuple<Cazacu2001StressType<StressStensor>,
               Cazacu2001StressNormalType<StressStensor>>
    computeCazacu2001StressCriterionNormal(
        const StressStensor&,
        const J2OCoefficients<StressStensor>&,
        const J3OCoefficients<StressStensor>&,
        const Cazacu2001BaseType<StressStensor>,
        const Cazacu2001StressType<StressStensor>);
    /*!
     * \brief compute the Cazacu2001 yield stress and its first and second
     * derivatives
     * \tparam StressStensor: type of the stress tensor
     * \param[in] sig: stress tensor
     * \param[in] a: coefficients for the \f$J_{2}^{O}\f$ invariant
     * \param[in] b: coefficients for the \f$J_{3}^{O}\f$ invariant
     * \param[in] c: c coefficient
     * \param[in] seps: small stress value. This value is used to regularize the
     *                  computation of the stress normal.
     */
    template <typename StressStensor>
    std::tuple<Cazacu2001StressType<StressStensor>,
               Cazacu2001StressNormalType<StressStensor>,
               Cazacu2001StressSecondDerivativeType<StressStensor>>
    computeCazacu2001StressCriterionSecondDerivative(
        const StressStensor&,
        const J2OCoefficients<StressStensor>&,
        const J3OCoefficients<StressStensor>&,
        const Cazacu2001BaseType<StressStensor>,
        const Cazacu2001StressType<StressStensor>);

  }  // end of namespace material

}  // end of namespace tfel

#include "TFEL/Material/Cazacu2001YieldCriterion.ixx"

#endif /* LIB_TFEL_MATERIAL_CAZACU2001YIELDCRITERION_HXX */
