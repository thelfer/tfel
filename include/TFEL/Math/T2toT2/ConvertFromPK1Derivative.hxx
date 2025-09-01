/*!
 * \file   include/TFEL/Math/T2toT2/ConvertFromPK1Derivative.hxx
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

#ifndef LIB_TFEL_MATH_CONVERTFROMPK1DERIVATIVE_HXX
#define LIB_TFEL_MATH_CONVERTFROMPK1DERIVATIVE_HXX

#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Math/t2tot2.hxx"

namespace tfel ::math {

  /*!
   * \brief convert the derivative of the first Piola-Kirchhoff stress with
   * first Piola-Kirchhoff deformation gradient to the derivative of the
   * Kirchhoff stress with respect to the deformation gradient.
   *
   * \tparam N: spatial dimension
   * \tparam stress: stress type
   * \tparam real: numeric type
   *
   * \param[in] dP: derivative of first Piola-Kirchhoff stress the with
   *                respect to the deformation gradient
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   *
   * \return the derivative of the Kirchhoff stress with respect to
   * the deformation gradient.
   */
  template <unsigned short N, typename stress, typename real>
  t2tost2<N, stress>
  convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative(
      const t2tot2<N, stress>&,
      const tensor<N, real>&,
      const stensor<N, stress>&);

  /*!
   * \brief convert the derivative of the first Piola-Kirchhoff stress with
   * first Piola-Kirchhoff deformation gradient to the derivative of the
   * Kirchhoff stress with respect to the deformation gradient.
   *
   * \tparam N: spatial dimension
   * \tparam stress: stress type
   * \tparam real: numeric type
   *
   * \param[out] ds: derivative of the Kirchhoff stress with respect to the
   *                 deformation gradient
   * \param[in] dP: derivative of first Piola-Kirchhoff stress the with
   *                respect to the deformation gradient
   * \param[in] F: deformation gradient
   * \param[in] s: Cauchy stress
   */
  template <unsigned short N, typename stress, typename real>
  void convertFirstPiolaKirchoffStressDerivativeToKirchhoffStressDerivative(
      t2tost2<N, stress>&,
      const t2tot2<N, stress>&,
      const tensor<N, real>&,
      const stensor<N, stress>&);

}  // end of namespace tfel::math

#include "TFEL/Math/T2toT2/ConvertFromPK1Derivative.ixx"

#endif /* LIB_TFEL_MATH_CONVERTFROMPK1DERIVATIVE_HXX */
