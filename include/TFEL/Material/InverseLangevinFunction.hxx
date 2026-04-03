/*!
 * \file   include/TFEL/Material/InverseLangevinFunction.hxx
 * \brief
 * \author Thomas Helfer, Annie Morch
 * \date   08/09/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_HXX
#define LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_HXX

#include <utility>

namespace tfel::material {

  /*!
   * \brief list of supported approximations of the inverse Langevin function
   */
  enum struct InverseLangevinFunctionApproximations {
    KUHN_GRUN_1942, /*!< Approximation of the Langevin function by Taylor
                     * expansion. The Taylor expansion is expression as
                     * \f[
                     * y\,P\left(y^{2}\right)
                     * \f]
                     * where \(P\) is a \(9\)th order polynomial. Hence, the
                     * Taylor expression is of order \(19\).
                     *
                     * Kuhn, Werner and Franz Grün. Beziehungen zwischen
                     * elastischen Konstanten und Dehnungsdoppelbrechung
                     * hochelastischer Stoffe. Kolloid-Zeitschrift 101 (1942):
                     * 248-271.
                     */
    COHEN_1991,     /*!< Cohen, A. A Padé approximant to the inverse
                     * Langevin function.
                     * Rheol Acta 30, 270–273 (1991).
                     * https://doi.org/10.1007/BF00366640 */
    JEDYNAK_2015,   /*!< Jedynak, R. Approximation of the inverse Langevin
                     * function revisited.
                     * Rheol Acta 54, 29–39 (2015).
                     * https://doi.org/10.1007/s00397-014-0802-2 */
    MORCH_2022      /*!< Morch, A.
                     *
                     * Approximation of the Langevin function by Taylor exansion.
                     * Inspired by the following works:
                     * - Jedynak, R. Approximation of the inverse Langevin function
                     *   revisited. Rheol Acta 54, 29–39 (2015).
                     *   https://doi.org/10.1007/s00397-014-0802-2
                     * - Merckel, Y. & Diani, J. & Roux, S. & Brieu, M. A simple
                     *   framework for full-network hyperelasticity and anisotropic
                     *   damage. JJ Mech Phys Solids 59. 75-88
                     *   (2011). 10.1016/j.jmps.2010.09.010.
                     * - Kuhn, Werner and Franz Grün. “Beziehungen zwischen
                     *   elastischen Konstanten und Dehnungsdoppelbrechung
                     *   hochelastischer Stoffe.” Kolloid-Zeitschrift 101 (1942):
                     *   248-271.
                     *
                     * \note This choice is equivalent to KUHN_GRUN_1942
                     */
  };

  /*!
   * \brief compute an approximation of the inverse Langenvin function
   * \tparam Approximation: approximation selected
   * \tparam NumericType: numeric type to be used
   * \param[in] y: argument
   */
  template <InverseLangevinFunctionApproximations =
                InverseLangevinFunctionApproximations::JEDYNAK_2015,
            typename NumericType>
  TFEL_HOST_DEVICE constexpr NumericType
  computeApproximateInverseLangevinFunction(const NumericType);
  /*!
   * \brief compute an approximation of the inverse Langenvin function and its
   * derivative.
   * \tparam Approximation: approximation selected
   * \tparam NumericType: numeric type to be used
   * \param[in] y: argument
   */
  template <InverseLangevinFunctionApproximations =
                InverseLangevinFunctionApproximations::JEDYNAK_2015,
            typename NumericType>
  TFEL_HOST_DEVICE constexpr std::pair<NumericType, NumericType>
  computeApproximateInverseLangevinFunctionAndDerivative(const NumericType);

  /*!
   * \brief compute the approximation of the inverse Langenvin function
   * following Bergström an Boyce.
   *
   * J.S. Bergström, M.C. Boyce, Constitutive modeling
   * of the large strain time-dependent behavior of
   * elastomers,
   * Journal of the Mechanics and Physics of Solids,
   * Volume 46, Issue 5, 1998,
   * https://doi.org/10.1016/S0022-5096(97)00075-6.
   *
   * \note this function can't be `constexpr` as it relies on a special
   *function.
   *
   * \tparam Approximation: approximation selected
   * \tparam NumericType: numeric type to be used
   * \param[in] y: argument
   */
  template <typename NumericType>
  TFEL_HOST_DEVICE NumericType
  computeBergstromBoyce1998ApproximateInverseLangevinFunction(
      const NumericType);
  /*!
   * \brief compute the approximation of the inverse Langenvin function
   * following Bergström an Boyce and its derivative.
   *
   * J.S. Bergström, M.C. Boyce, Constitutive modeling
   * of the large strain time-dependent behavior of
   * elastomers,
   * Journal of the Mechanics and Physics of Solids,
   * Volume 46, Issue 5, 1998,
   * https://doi.org/10.1016/S0022-5096(97)00075-6.
   *
   * \note this function can't be `constexpr` as it relies on a special
   *function.
   *
   * \tparam Approximation: approximation selected
   * \tparam NumericType: numeric type to be used
   * \param[in] y: argument
   */
  template <typename NumericType>
  TFEL_HOST_DEVICE std::pair<NumericType, NumericType>
  computeBergstromBoyce1998ApproximateInverseLangevinFunctionAndDerivative(
      const NumericType);

}  // namespace tfel::material

#include "TFEL/Material/InverseLangevinFunction.ixx"

#endif /* LIB_TFEL_MATERIAL_INVERSELANGEVINFUNCTION_HXX */
