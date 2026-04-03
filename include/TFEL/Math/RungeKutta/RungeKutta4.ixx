/*!
 * \file   include/TFEL/Math/RungeKutta/RungeKutta4.ixx
 * \brief  This file implements the RungeKutta4 class.
 * \author Thomas Helfer
 * \date   01 Sep 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_RUNGE_KUTTA4_IXX
#define TFEL_MATH_RUNGE_KUTTA4_IXX 1

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel::math {

  template <unsigned int N, typename T, typename Func>
  class RungeKutta4 {
    T h;
    T t;
    tvector<N, T> y;
    tvector<N, T> k1;
    tvector<N, T> k2;
    tvector<N, T> k3;
    tvector<N, T> k4;
    tvector<N, T> tmp;

    RungeKutta4(const RungeKutta4&);
    RungeKutta4& operator=(const RungeKutta4&);

   protected:
    tvector<N, T> f;

   public:
    RungeKutta4() {}

    TFEL_MATH_INLINE2 void set_y(const tvector<N, T>& y_) { this->y = y_; }

    TFEL_MATH_INLINE2 void set_t(const T t_) { this->t = t_; }

    TFEL_MATH_INLINE2 void set_h(const T h_) { this->h = h_; }

    void increm() {
      constexpr const auto Cste_1_2 = T{1} / T{2};
      constexpr const auto Cste_1_3 = T{1} / T{3};
      constexpr const auto Cste_1_6 = T{1} / T{6};
      static_cast<Func&>(*this).computeF(t, y);
      k1 = h * f;
      tmp = y + Cste_1_2 * k1;
      t += Cste_1_2 * h;
      static_cast<Func&>(*this).computeF(t, tmp);
      k2 = h * f;
      tmp = y + Cste_1_2 * k2;
      static_cast<Func&>(*this).computeF(t, tmp);
      k3 = h * f;
      tmp = y + k3;
      t += Cste_1_2 * h;
      static_cast<Func&>(*this).computeF(t, tmp);
      k4 = h * f;
      y += Cste_1_6 * (k1 + k4) + Cste_1_3 * (k2 + k3);
    }

    TFEL_MATH_INLINE2 void exe(const T begin, const T end) {
      this->t = begin;
      while (this->t < end) {
        this->increm();
      }
    }

    TFEL_MATH_INLINE2 const tvector<N, T>& get_y() const { return this->y; }

    TFEL_MATH_INLINE2 T get_t() const { return this->t; }

    TFEL_MATH_INLINE2 T get_h() const { return this->h; }
  };

}  // end of namespace tfel::math

#endif /* TFEL_MATH_RUNGE_KUTTA4_IXX */
