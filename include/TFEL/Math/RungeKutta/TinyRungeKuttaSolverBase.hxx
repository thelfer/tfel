/*!
 * \file   include/TFEL/Math/RungeKutta/TinyRungeKuttaSolverBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/06/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYRUNGEKUTTASOLVERBASE_HXX
#define LIB_TFEL_MATH_TINYRUNGEKUTTASOLVERBASE_HXX

#include <type_traits>
#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  /*!
   * \brief a base class for a system of ordinary differential equations based
   * on explicit methods of the Runge-Kutta family where the size of the
   * solution is known at compile-time.
   * \tparam N: size of the system of ordinary differential equations
   * \tparam ScalarType: scalar type
   */
  template <unsigned short N, typename ScalarType = double>
  struct TinyRungeKuttaSolverBase {
    /*!
     * \brief the type of solution determined by the ordinary differential
     * equation.
     */
    using SolutionType =
        std::conditional_t<N == 1, ScalarType, tvector<N, ScalarType>>;
    /*!
     * \brief set the initial values of the solution
     * \param[in] y0: initial values
     */
    void setInitialValue(const SolutionType& y0) { this->y = y0; }
    /*!
     * \brief set the initial time
     * \param[in] t0: initial time
     */
    void setInitialTime(const ScalarType t0) { this->ti = t0; }
    /*!
     * \brief set the final time
     * \param[in] t1: final time
     */
    void setFinalTime(const ScalarType t1) { this->tf = t1; }
    /*!
     * \brief set the initial time increment
     * \param[in] time_increment
     */
    void setInitialTimeIncrement(const ScalarType time_increment) {
      this->dt = time_increment;
    }
    /*!
     * \brief set the value of the stopping criterion
     * \param[in] criterion_value: new criterion value
     */
    void setCriterionValue(const ScalarType criterion_value) {
      this->epsilon = criterion_value;
    }
    //! \return the solution at the end of the time step after the resolution.
    const SolutionType& getValue() const { return this->y; }
    //! \return the last time increment used by the solver.
    const ScalarType& getTimeIncrement() const { return this->dt; }

   protected:
    //! \brief current estimate of the solution
    SolutionType y;
    //! \brief initial time
    ScalarType ti;
    //! \brief final time
    ScalarType tf;
    //! \brief current time increment
    ScalarType dt;
    //! \brief criterion value
    ScalarType epsilon;

  };  // end of struct TinyRungeKuttaSolverBase

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TINYRUNGEKUTTASOLVERBASE_HXX */
