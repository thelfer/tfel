/*!
 * \file   include/TFEL/Math/RungeKutta/RungeKutta42.ixx
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

#ifndef LIB_TFEL_MATH_RUNGEKUTTA42_IXX
#define LIB_TFEL_MATH_RUNGEKUTTA42_IXX

#include "TFEL/Math/General/Abs.hxx"

namespace tfel::math {

  template <unsigned short N, typename Child, typename Scalar>
  void RungeKutta42<N, Child, Scalar>::iterate() {
    constexpr auto cste1_6 = Scalar(1) / Scalar(6);
    constexpr auto cste1_3 = Scalar(1) / Scalar(3);
    const auto& child = static_cast<const Child&>(*this);
    auto t = this->ti;  // current time
    this->dt = std::min(this->dt, this->tf - this->ti);
    if (this->dt < 0) {
      throw(InvalidTimeStepException());
    }
    while (t < this->tf - 0.5 * (this->dt)) {
      // compute k1
      const auto k1 = this->dt * child.computeF(t, this->y);
      // compute k2
      const auto k2 = this->dt * child.computeF(t + (this->dt) / 2,  //
                                                this->y + k1 / 2);
      // compute k3
      const auto k3 = this->dt * child.computeF(t + (this->dt) / 2,  //
                                                this->y + k2 / 2);
      // compute k4
      const auto k4 = this->dt * child.computeF(t + this->dt,  //
                                                this->y + k3);
      // error
      const auto ve = cste1_6 * (k1 + k4 - k2 - k3);
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
      if (e < 0.01 * (this->epsilon)) {
        e = 0.01 * (this->epsilon);
      }
      if (e > 100 * (this->epsilon)) {
        e = 100 * (this->epsilon);
      }
      // test for convergence
      if (e < this->epsilon) {
        this->y += cste1_6 * (k1 + k4) + cste1_3 * (k2 + k3);
        t += this->dt;
      }
      // time multiplier
      if (t < this->tf - 0.5 * (this->dt)) {
        this->dt *= 0.8 * pow(this->epsilon / e, cste1_3);
        if (this->dt > this->tf - t) {
          this->dt = this->tf - t;
        }
      }
    }
  }  // end of RungeKutta42<N,C,S>::iterate

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_RUNGEKUTTA42_IXX */
