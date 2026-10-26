/*!
 * \file   tests/Math/RungeKutta/rk_final_time.cxx
 * \brief  check that the adaptive Runge-Kutta solvers reach the final time
 * \author Tristan Chenaille
 * \date   26/10/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <cstdlib>
#include "TFEL/Math/RungeKutta42.hxx"
#include "TFEL/Math/RungeKutta54.hxx"

/*!
 * \brief y0' = 1 and y1' = y1: y0 is the time reached and y1 = exp(t)
 */
template <template <unsigned short, typename, typename> class RungeKutta>
struct Exponential : public RungeKutta<2, Exponential<RungeKutta>, double> {
  tfel::math::tvector<2, double> computeF(
      const double, const tfel::math::tvector<2, double>& y) const {
    return {1, y(1)};
  }
};

//! \brief y' = y with a scalar solution
template <template <unsigned short, typename, typename> class RungeKutta>
struct ScalarExponential
    : public RungeKutta<1, ScalarExponential<RungeKutta>, double> {
  double computeF(const double, const double y) const { return y; }
};

//! \brief integrate from 0 to 1
template <typename Solver>
static auto solve(const typename Solver::SolutionType& y0, const double dt0) {
  Solver rk;
  rk.setInitialValue(y0);
  rk.setInitialTime(0);
  rk.setFinalTime(1);
  rk.setInitialTimeIncrement(dt0);
  rk.setCriterionValue(1.e-8);
  rk.iterate();
  return rk.getValue();
}

template <template <unsigned short, typename, typename> class RungeKutta>
static bool test() {
  auto success = true;
  for (const auto dt0 : {0.1, 0.2, 0.3, 1.}) {
    const auto y = solve<Exponential<RungeKutta>>({0, 1}, dt0);
    success = success && (std::abs(y(0) - 1) < 1.e-12) &&
              (std::abs(y(1) - std::exp(1.)) < 1.e-7);
    const auto ys = solve<ScalarExponential<RungeKutta>>(1, dt0);
    success = success && (std::abs(ys - std::exp(1.)) < 1.e-7);
  }
  return success;
}

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  const auto success =
      test<tfel::math::RungeKutta42>() && test<tfel::math::RungeKutta54>();
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
