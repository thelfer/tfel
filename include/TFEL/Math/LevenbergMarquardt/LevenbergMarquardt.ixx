/*!
 * \file   include/TFEL/Math/LevenbergMarquardt/LevenbergMarquardt.ixx
 * \brief
 *
 * \author Helfer Thomas
 * \date   21 nov 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX_
#define LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX_

#include <algorithm>

#include "TFEL/Math/power.hxx"
#include "TFEL/Math/MathException.hxx"

namespace tfel {

  namespace math {

    template <typename F>
    LevenbergMarquardt<F>::LevenbergMarquardt(const F f_)
        : f(f_),
          p(f_.getNumberOfParameters()) {
    }  // end of LevenbergMarquardt::LevenbergMarquardt

    template <typename F>
    void LevenbergMarquardt<F>::setInitialDampingParameter(const T value) {
      this->lambda0 = value;
    }  // end of LevenbergMarquardt<F>::setInitialDampingParameter

    template <typename F>
    void LevenbergMarquardt<F>::setFirstCriterium(const T value) {
      this->eps1 = value;
    }  // end of LevenbergMarquardt<F>::setFirstCriterium

    template <typename F>
    void LevenbergMarquardt<F>::setSecondCriterium(const T value) {
      this->eps2 = value;
    }  // end of LevenbergMarquardt<F>::setSecondCriterium

    template <typename F>
    void LevenbergMarquardt<F>::setMultiplicationFactor(const T value) {
      this->factor = value;
    }  // end of LevenbergMarquardt<F>::setMultiplicationFactor

    template <typename F>
    void LevenbergMarquardt<F>::addData(
        const typename LevenbergMarquardt<F>::Variable& x, const T y) {
      using namespace std;
      this->data.push_back(pair<Variable, T>(x, y));
    }  // end of LevenbergMarquardt::addData

    template <typename F>
    void LevenbergMarquardt<F>::setInitialGuess(
        const typename LevenbergMarquardt<F>::Parameter& p_) {
      this->p = p_;
    }  // end of LevenbergMarquardt::setInitialGuess

    template <typename F>
    void LevenbergMarquardt<F>::setMaximumIteration(const T nb) {
      this->iterMax = nb;
    }  // end of LevenbergMarquardt::setMaximumIteration

    template <typename F>
    unsigned short LevenbergMarquardt<F>::getNumberOfIterations(void) const {
      return this->iter;
    }  // end of LevenbergMarquardt::getNumberOfIterations

    template <typename F>
    const typename LevenbergMarquardt<F>::Parameter&
    LevenbergMarquardt<F>::execute(void) {
      using namespace std;
      using tfel::math::stdfunctions::power;
      typename vector<pair<Variable, T>>::const_iterator it;
      size_type m = this->f.getNumberOfParameters();
      // grad is the opposite of the gradient
      matrix<T> J(m, m, T(0));
      matrix<T> Jn(m, m, T(0));
      Parameter g(m, T(0));
      Parameter gn(m, T(0));
      Parameter gradient(m, T(0));
      Parameter h(m);
      Parameter p_(m);
      T s(T(0));
      T v(0);
      T lambda = this->lambda0;
      unsigned short i;
      bool success;
      s = T(0);
      for (it = this->data.begin(); it != this->data.end(); ++it) {
        p_ = this->p + h;
        (this->f)(v, gradient, it->first, p_);
        g += (v - it->second) * gradient;
        J += gradient ^ gradient;
        s += power<2>(v - it->second);
      }
      lambda *= *(max_element(J.begin(), J.end()));
      for (i = 0; i != m; ++i) {
        J(i, i) += lambda;
      }
      this->factor = 2;
      success = false;
      for (this->iter = 0; (this->iter != this->iterMax) && (!success);
           ++(this->iter)) {
        Jn = J;
        fill(gn.begin(), gn.end(), T(0));
        h = -g;
        T sn(T(0));
        LUSolve::exe(Jn, h);
        fill(Jn.begin(), Jn.end(), T(0));
        for (it = this->data.begin(); it != this->data.end(); ++it) {
          p_ = this->p + h;
          (this->f)(v, gradient, it->first, p_);
          gn += (v - it->second) * gradient;
          Jn += gradient ^ gradient;
          sn += power<2>(v - it->second);
        }
        T rho = (s - sn) / (0.5 * (h | (lambda * h - g)));
        if (rho > 0) {
          lambda *= max(T(0.3333), T(1) - power<3>(2 * rho - 1));
          this->factor = 2;
        } else {
          lambda *= this->factor;
          this->factor *= 2;
        }
        for (i = 0; i != m; ++i) {
          Jn(i, i) += lambda;
        }
        this->p += h;
        T ng = norm(gn);
        T nh = norm(h);
        T np = norm(p);
        if (nh < this->eps2 * (np + this->eps2)) {
          success = true;
        } else if (ng < this->eps1) {
          success = true;
        } else {
          // preparing next iteration
          J = Jn;
          g = gn;
          s = sn;
        }
      }
      if (!success) {
        throw(MaximumNumberOfIterationsReachedException());
      }

      return this->p;
    }  // end of execute

    template <typename F>
    LevenbergMarquardt<F>::~LevenbergMarquardt() = default;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_LEVENBERGMARQUARDT_IXX_ */
