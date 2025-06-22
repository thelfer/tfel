/*!
 * \file   include/TFEL/Math/RungeKutta/RungeKutta2.ixx
 * \brief
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_RUNGE_KUTTA2_IXX
#define TFEL_MATH_RUNGE_KUTTA2_IXX 1

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace math {

    template <unsigned int N, typename T, typename Func>
    struct RungeKutta2 {
      RungeKutta2() = default;

      TFEL_MATH_INLINE2 void set_y(const tvector<N, T>& y_) { this->y = y_; }

      TFEL_MATH_INLINE2 void set_t(const T t_) { this->t = t_; }

      TFEL_MATH_INLINE2 void set_h(const T h_) { this->h = h_; }

      TFEL_MATH_INLINE2 void increm() {
        TFEL_CONSTEXPR const auto one_half = T{1} / T{2};
        static_cast<Func&>(*this).computeF(t, y);
        k1 = y + one_half * h * f;
        static_cast<Func&>(*this).computeF(t + one_half * h, k1);
        t += h;
        y += h * f;
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

     protected:
      tvector<N, T> f;

     private:
      T h;
      T t;
      tvector<N, T> y;
      tvector<N, T> k1;

      RungeKutta2(const RungeKutta2&) = delete;
      RungeKutta2& operator=(const RungeKutta2&) = delete;
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* TFEL_MATH_RUNGE_KUTTA2_IXX */
