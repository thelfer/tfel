/*!
 * \file   include/TFEL/Math/RungeKutta/RungeKutta54.ixx
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

#ifndef LIB_TFEL_MATH_RUNGEKUTTA54_IXX
#define LIB_TFEL_MATH_RUNGEKUTTA54_IXX

#include <cassert>
#include "TFEL/Raise.hxx"
#include "TFEL/FSAlgorithm/accumulate.hxx"
#include "TFEL/Math/General/Abs.hxx"

namespace tfel::math {

  template <unsigned short N, typename Child, typename Scalar>
  void RungeKutta54<N, Child, Scalar>::iterate() {
    constexpr auto cste_12_13 = Scalar(12) / Scalar(13);
    constexpr auto cste_1932_2197 = Scalar(1932) / Scalar(2197);
    constexpr auto cste_7200_2197 = Scalar(7200) / Scalar(2197);
    constexpr auto cste_7296_2197 = Scalar(7296) / Scalar(2197);
    constexpr auto cste_439_216 = Scalar(439) / Scalar(216);
    constexpr auto cste_3680_513 = Scalar(3680) / Scalar(513);
    constexpr auto cste_845_4104 = Scalar(845) / Scalar(4104);
    constexpr auto cste_8_27 = Scalar(8) / Scalar(27);
    constexpr auto cste_3544_2565 = Scalar(3544) / Scalar(2565);
    constexpr auto cste_1859_4104 = Scalar(1859) / Scalar(4104);
    constexpr auto cste_11_40 = Scalar(11) / Scalar(40);
    constexpr auto cste_16_135 = Scalar(16) / Scalar(135);
    constexpr auto cste_6656_12825 = Scalar(6656) / Scalar(12825);
    constexpr auto cste_28561_56430 = Scalar(28561) / Scalar(56430);
    constexpr auto cste_9_50 = Scalar(9) / Scalar(50);
    constexpr auto cste_2_55 = Scalar(2) / Scalar(55);
    constexpr auto cste_1_360 = Scalar(1) / Scalar(360);
    constexpr auto cste_128_4275 = Scalar(128) / Scalar(4275);
    constexpr auto cste_2197_75240 = Scalar(2197) / Scalar(75240);
    constexpr auto cste_1_50 = Scalar(1) / Scalar(50);
    const auto& child = static_cast<const Child&>(*this);
    Scalar t = this->ti;  // current time
    this->dt = std::min(this->dt, this->tf - this->ti);
    if (this->dt < 0) {
      tfel::raise<InvalidTimeStepException>();
    }
    while (t < this->tf - (this->dt) / 2) {
      const auto k1 = this->dt * child.computeF(t, this->y);
      auto t_ = t + (this->dt) / 4;
      auto y_ = eval(this->y + k1 / 4);
      const auto k2 = this->dt * child.computeF(t_, y_);
      t_ = t + 0.375 * (this->dt);
      y_ = this->y + 0.09375 * (k1 + 3 * k2);
      const auto k3 = this->dt * child.computeF(t_, y_);
      t_ = t + cste_12_13 * (this->dt);
      y_ = this->y + cste_1932_2197 * k1 - cste_7200_2197 * k2 +
           cste_7296_2197 * k3;
      const auto k4 = this->dt * child.computeF(t_, y_);
      t_ = t + this->dt;
      y_ = this->y + cste_439_216 * k1 - 8 * k2 + cste_3680_513 * k3 -
           cste_845_4104 * k4;
      const auto k5 = this->dt * child.computeF(t_, y_);
      t_ = t + (this->dt) / 2;
      y_ = this->y - cste_8_27 * k1 + 2 * k2 - cste_3544_2565 * k3 +
           cste_1859_4104 * k4 - cste_11_40 * k5;
      const auto k6 = this->dt * child.computeF(t_, y_);
      // error
      const auto ve = cste_1_360 * k1 - cste_128_4275 * k3 -
                      cste_2197_75240 * k4 + cste_1_50 * k5 + cste_2_55 * k6;
      // error norm
      auto e = [&ve] {
        if constexpr (N == 1) {
          return tfel::math::abs(ve);
        } else {
          return tfel::fsalgo::accumulate<N>::exe(
              eval(ve).begin(), Scalar(0), [](const auto& a, const auto& b) {
                return tfel::math::abs(a) + b;
              });
        }
      }();
      assert(e >= 0.);
      if (e < 0.01 * (this->epsilon)) {
        e = 0.01 * (this->epsilon);
      }
      if (e > 100 * (this->epsilon)) {
        e = 100 * (this->epsilon);
      }
      // test for convergence
      if (e < this->epsilon) {
        this->y += cste_16_135 * k1 + cste_6656_12825 * k3 +
                   cste_28561_56430 * k4 - cste_9_50 * k5 + cste_2_55 * k6;
        t += this->dt;
      }
      // time multiplier
      if (t < this->tf - (this->dt) / 2) {
        this->dt *= 0.8 * pow(this->epsilon / e, 0.2);
        if (this->dt > this->tf - t) {
          this->dt = this->tf - t;
        }
      }
    }
  }  // end of RungeKutta54<N,C,S>::iterate

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RUNGEKUTTA54_IXX */
