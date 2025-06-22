/*!
 * \file   tests/Math/RungeKutta/rk54.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   27 sep 2007
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
#include "TFEL/Math/RungeKutta54.hxx"

struct VanDerPol : public tfel::math::RungeKutta54<2, VanDerPol> {
  typedef tfel::math::tvector<2, double> vector;

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

  ofstream out("rk54.txt");

  VanDerPol rk;
  tvector<2> y;
  const double end = 100.;

  y(0) = 1.0;
  y(1) = 0.0;

  rk.setInitialValue(y);
  rk.setCriterium(1.e-9);
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

  return EXIT_SUCCESS;
}
