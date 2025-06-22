/*!
 * \file   include/TFEL/Math/T2toT2/ConvertToPK1Derivative.hxx
 * \brief
 * \author Thomas Heler
 * \date   25/04/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX
#define LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"

namespace tfel {

  namespace math {

    /*!
     * \brief convert the derivative of the Cauchy stress with respect to the
     * deformation gradient to the derivative of the first Piola-Kirchhoff
     * stress with respect to the deformation gradient.
     *
     * \tparam N: spatial dimension
     * \tparam stress: stress type
     * \tparam real: numeric type
     *
     * \param[in] ds: derivative of the Cauchy stress with respect to the
     *                deformation gradient
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     *
     * \return the derivative of the first Piola-Kirchhoff
     * stress with respect to the deformation gradient.
     */
    template <unsigned short N, typename stress, typename real>
    t2tot2<N, stress>
    convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
        const t2tost2<N, stress>&,
        const tensor<N, real>&,
        const stensor<N, stress>&);

    /*!
     * \brief convert the derivative of the Cauchy stress with respect to the
     * deformation gradient to the derivative of the first Piola-Kirchhoff
     * stress with respect to the deformation gradient.
     *
     * \tparam N: spatial dimension
     * \tparam stress: stress type
     * \tparam real: numeric type
     *
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] ds: derivative of the Cauchy stress with respect to the
     *                deformation gradient
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     */
    template <unsigned short N, typename stress, typename real>
    void convertCauchyStressDerivativeToFirstPiolaKirchoffStressDerivative(
        t2tot2<N, stress>&,
        const t2tost2<N, stress>&,
        const tensor<N, real>&,
        const stensor<N, stress>&);

    /*!
     * \brief convert the derivative of the second Piola-Kirchoff stress with
     * respect to the Green-Lagrange strain to the derivative of the
     * first Piola-Kirchhoff stress with respect to the deformation gradient.
     *
     * \tparam N: spatial dimension
     * \tparam stress: stress type
     * \tparam real: numeric type
     *
     * \param[in] dS: derivative of the second Piola-Kirchoff stress with
     *                respect to the Green-Lagrange strain
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     *
     * \return the derivative of the first Piola-Kirchhoff
     * stress with respect to the deformation gradient.
     */
    template <unsigned short N, typename stress, typename real>
    t2tot2<N, stress>
    convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
        const st2tost2<N, stress>&,
        const tensor<N, real>&,
        const stensor<N, stress>&);

    /*!
     * \brief convert the derivative of the second Piola-Kirchoff stress with
     * respect to the Green-Lagrange strain to the derivative of the
     * first Piola-Kirchhoff stress with respect to the deformation gradient.
     *
     * \tparam N: spatial dimension
     * \tparam stress: stress type
     * \tparam real: numeric type
     *
     * \param[out] dP: derivative of the first Piola-Kirchhoff stress with
     *                 respect to the deformation gradient
     * \param[in] dS: derivative of the second Piola-Kirchoff stress with
     *                respect to the Green-Lagrange strain
     * \param[in] F: deformation gradient
     * \param[in] s: Cauchy stress
     */
    template <unsigned short N, typename stress, typename real>
    void
    convertSecondPiolaKirchhoffStressDerivativeToFirstPiolaKirchoffStressDerivative(
        t2tot2<N, stress>&,
        const st2tost2<N, stress>&,
        const tensor<N, real>&,
        const stensor<N, stress>&);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/T2toT2/ConvertToPK1Derivative.ixx"

#endif /* LIB_TFEL_MATH_CONVERTTOPK1DERIVATIVE_HXX */
