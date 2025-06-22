/*!
 * \file  mtest/include/MTest/UmatNormaliseTangentOperator.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 23 août 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATOR_HXX
#define LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/st2tost2.hxx"

namespace tfel::math {

  /*!
   * \brief normalise the tangent operator returned by:
   * - Cast3M behaviours
   * - Abaqus/Standard behaviours
   * \tparam N: dimension
   * \tparam stress: numeric type
   */
  template <unsigned short N, typename stress>
  struct UmatNormaliseTangentOperatorBase;
  /*!
   * \brief partial specialisation of the
   * `UmatNormaliseTangentOperatorBase` class in 1D.
   */
  template <typename stress>
  struct UmatNormaliseTangentOperatorBase<1u, stress> {
    /*!
     * \brief convert a tangent operator given in:
     * - fortran ordering convention
     * - Voigt convention
     * to an tangent operator using TFEL conventions.
     *
     * \param[out] K: normalised matrix
     * \param[in]  D: tangent operator
     */
    static TFEL_MATH_INLINE void exe(stress* const, const stress* const);
  };
  /*!
   * \brief partial specialisation of the
   * `UmatNormaliseTangentOperatorBase` class in 1D
   */
  template <typename stress>
  struct UmatNormaliseTangentOperatorBase<2u, stress> {
    /*!
     * \brief convert a tangent operator given in:
     * - fortran ordering convention
     * - Voigt convention
     * to an tangent operator using TFEL conventions.
     *
     * \param[out] K: normalised matrix
     * \param[in]  D: tangent operator
     */
    static TFEL_MATH_INLINE void exe(stress* const, const stress* const);
  };
  /*!
   * \brief partial specialisation of the
   * `UmatNormaliseTangentOperatorBase` class in 1D
   */
  template <typename stress>
  struct UmatNormaliseTangentOperatorBase<3u, stress> {
    /*!
     * \brief convert a tangent operator given in:
     * - fortran ordering convention
     * - Voigt convention
     * to an tangent operator using TFEL conventions.
     *
     * \param[out] K: normalised matrix
     * \param[in]  D: tangent operator
     */
    static TFEL_MATH_INLINE void exe(stress* const, const stress* const);
  };
  /*!
   * \brief normalise the tangent operator returned by:
   * - Cast3M behaviours
   * - Abaqus/Standard behaviours
   * \tparam N: dimension
   * \tparam stress: numeric type
   */
  template <unsigned short N, typename stress>
  struct UmatNormaliseTangentOperator
      : UmatNormaliseTangentOperatorBase<N, stress> {
    /*!
     * \brief convert a tangent operator given in:
     * - fortran ordering convention
     * - Voigt convention
     * to an tangent operator using TFEL conventions.
     *
     * \param[out] K: normalised matrix
     * \param[in]  D: tangent operator
     */
    static TFEL_MATH_INLINE st2tost2<N, stress> exe(const stress* const);
  };

}  // end of namespace tfel::math

#include "TFEL/Math/ST2toST2/UmatNormaliseTangentOperator.ixx"

#endif /* LIB_TFEL_MATH_UMATNORMALISETANGENTOPERATOR_HXX */
