/*!
 * \file   tfel-check/include/TFEL/Check/Curve.hxx
 * \brief  functions to build and evaluate the curves read in files,
 * used internally by the TFELCheck library
 * \date   20/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFELCHECK_CURVE_HXX
#define LIB_TFELCHECK_CURVE_HXX

#include <string>
#include <vector>

namespace tfel::check {

  // forward declaration
  struct Interpolation;

  /*!
   * \brief a curve, i.e. a set of points sorted by increasing abscissa,
   * with no duplicated abscissa.
   */
  struct Curve {
    //! \brief abscissas
    std::vector<double> abscissas;
    //! \brief values
    std::vector<double> values;
  };

  /*!
   * \return the curve defined by the given abscissas and values
   * \param[in] f: file from which the abscissas and values were read,
   * used in error messages
   * \param[in] x: abscissas
   * \param[in] y: values
   *
   * The points are sorted by increasing abscissa. If an abscissa
   * appears more than once, only the first point is kept, as in the
   * `Linearization` class. An exception is thrown if no point is given.
   */
  Curve buildCurve(const std::string&,
                   const std::vector<double>&,
                   const std::vector<double>&);
  /*!
   * \return true if the given abscissas are in the range of the curve
   * \param[in] c: curve
   * \param[in] x: abscissas
   *
   * A small tolerance, relative to the magnitude of the abscissas of the
   * curve, absorbs the rounding errors on the bounds of the range.
   */
  bool covers(const Curve&, const std::vector<double>&);
  /*!
   * \return the values of a curve at the given abscissas
   * \param[in] i: interpolation
   * \param[in] c: curve
   * \param[in] x: abscissas
   *
   * The values of the curve are used at its own abscissas. Between
   * them, the given interpolation is used. Outside the range of the
   * curve, the first or last value is used. The interpolation is only
   * built if needed.
   */
  std::vector<double> evaluateCurve(Interpolation&,
                                    const Curve&,
                                    const std::vector<double>&);

}  // end of namespace tfel::check

#endif /* LIB_TFELCHECK_CURVE_HXX */
