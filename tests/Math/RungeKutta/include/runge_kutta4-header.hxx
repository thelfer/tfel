/*!
 * \file   tests/Math/RungeKutta/include/runge_kutta4-header.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   05/02/2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_RUNGE_KUTTA4_HEADER_HXX
#define LIB_TFEL_RUNGE_KUTTA4_HEADER_HXX

#include <cstdlib>
#include <fstream>
#include <string>

#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/RungeKutta4.hxx"

template <typename T>
struct Name;

template <>
struct Name<long double> {
  static std::string getName() { return "long double"; }
};

template <>
struct Name<double> {
  static std::string getName() { return "double"; }
};

template <>
struct Name<float> {
  static std::string getName() { return "float"; }
};

template <typename T>
class VanDerPol : public tfel::math::RungeKutta4<2, T, VanDerPol<T>> {
  T mu;

  typedef tfel::math::tvector<2, T> vector;

 public:
  void set_mu(const T mu_) { this->mu = mu_; }

  TFEL_MATH_INLINE void computeF(const T, const vector& x) {
    (this->f)(0) = x(1);
    (this->f)(1) = -x(0) + mu * x(1) * (1 - x(0) * x(0));
  }
};

template <typename T>
void test();

template <typename T>
void test() {
  using namespace std;
  using namespace tfel::math;

  string name("rk4_");
  name += Name<T>::getName();
  name += ".txt";

  ofstream out(name.c_str());
  VanDerPol<T> rk;
  tvector<2, T> y;
  T begin;
  T end;

  y(0) = 1.f;
  y(1) = 0.f;

  rk.set_mu(10.f);
  rk.set_h(1.e-2f);
  rk.set_y(y);

  begin = 0.f;
  end = 100.f;

  rk.set_t(begin);

  out.precision(15);
  while (rk.get_t() < end) {
    rk.increm();
    out << rk.get_t() << "  " << (rk.get_y())(0) << endl;
  }
}

#endif /* LIB_TFEL_RUNGE_KUTTA4_HEADER_HXX */
