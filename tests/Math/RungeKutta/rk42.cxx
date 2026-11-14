/*!
 * \file   tests/Math/RungeKutta/rk42.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 27/09/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <cassert>

#include "TFEL/Math/Vector/tvectorIO.hxx"
#include "TFEL/Math/RungeKutta42.hxx"

struct VanDerPol : public tfel::math::RungeKutta42<2, VanDerPol> {
  typedef tfel::math::tvector<2, double> vector;

  VanDerPol() = default;

  void setMu(const double mu_) { this->mu = mu_; }

  const vector computeF(const double, const vector& x) const {
    vector f;
    f(0) = x(1);
    f(1) = -x(0) + (this->mu) * x(1) * (1. - x(0) * x(0));
    return f;
  }

 private:
  double mu;
};

/* coverity [UNCAUGHT_EXCEPT]*/
int main() {
  using namespace std;
  using namespace tfel::math;

  ofstream out("rk42.txt");

  const double end = 100.;
  VanDerPol rk;
  tvector<2> y;

  y(0) = 1.0;
  y(1) = 0.0;

  rk.setInitialValue(y);
  rk.setCriterionValue(1.e-9);
  rk.setMu(10.);

  double t = 0.;
  double h = 1.;
  rk.setInitialTimeIncrement(h);
  out << t << " " << rk.getValue() << endl;
  for (; t < end - 0.5 * h; t += h) {
    h = std::min(end - t, rk.getTimeIncrement());
    rk.setInitialTime(t);
    rk.setFinalTime(t + h);
    rk.iterate();
    out << t + h << " " << rk.getValue() << endl;
  }

  // values at t = 100 computed with mpmath (odefun, Taylor series, 35
  // digits); the tolerance is ten times the criterion value, as in
  // rk_final_time.cxx
  VanDerPol rk2;
  rk2.setInitialValue(y);
  rk2.setCriterionValue(1.e-9);
  rk2.setMu(10.);
  rk2.setInitialTime(0.);
  rk2.setFinalTime(end);
  rk2.setInitialTimeIncrement(1.);
  rk2.iterate();
  auto success = (std::abs(rk2.getValue()(0) + 1.7588880803915539) < 1.e-8) &&
                 (std::abs(rk2.getValue()(1) - 0.083643606665915065) < 1.e-8);
  // without damping (mu = 0), the solution is (cos(t), -sin(t))
  VanDerPol rk3;
  rk3.setInitialValue(y);
  rk3.setCriterionValue(1.e-9);
  rk3.setMu(0.);
  rk3.setInitialTimeIncrement(1.);
  for (int i = 0; i != 10; ++i) {
    rk3.setInitialTime(i);
    rk3.setFinalTime(i + 1);
    rk3.iterate();
    success = success &&
              (std::abs(rk3.getValue()(0) - std::cos(i + 1.)) < 1.e-8) &&
              (std::abs(rk3.getValue()(1) + std::sin(i + 1.)) < 1.e-8);
  }
  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
