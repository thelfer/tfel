/*!
 * \file   include/TFEL/Math/RungeKutta42.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_RUNGEKUTTA42_HXX
#define LIB_TFEL_MATH_RUNGEKUTTA42_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/RungeKutta/TinyRungeKuttaSolverBase.hxx"

namespace tfel::math {

  /*!
   * \brief a simple solver for ordinary differential equations
   */
  template <unsigned short N, typename Child, typename ScalarType = double>
  struct RungeKutta42 : TinyRungeKuttaSolverBase<N, ScalarType> {
    /*!
     * \brief solve the ordinary differential equation for the prescribed
     * time range
     */
    void iterate();
  };  // end of struct RungeKutta42

}  // end of namespace tfel::math

#include "TFEL/Math/RungeKutta/RungeKutta42.ixx"

#endif /* LIB_TFEL_MATH_RUNGEKUTTA42_HXX */
