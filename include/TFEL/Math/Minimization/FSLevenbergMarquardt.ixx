/*!
 * \file   include/TFEL/Math/LevenbergMarquardt/FSLevenbergMarquardt.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   21 nov 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_FSLEVENBERGMARQUARDTIXX
#define LIB_TFEL_MATH_FSLEVENBERGMARQUARDTIXX

#include <algorithm>
#include "TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/MathException.hxx"

namespace tfel::math {

  template <unsigned short N, unsigned short M, typename T>
  FSLevenbergMarquardt<N, M, T>::FSLevenbergMarquardt(
      const typename FSLevenbergMarquardt::PtrFun f_)
      : lambda0(T(1.e-3)),
        factor(T(2)),
        eps1(1.e-10),
        eps2(1.e-10),
        iterMax(100),
        f(f_) {}  // end of FSLevenbergMarquardt::FSLevenbergMarquardt

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setInitialDampingParameter(
      const T value) {
    this->lambda0 = value;
  }  // end of FSLevenbergMarquardt<N,M,T>::setInitialDampingParameter

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setFirstCriterium(const T value) {
    this->eps1 = value;
  }  // end of FSLevenbergMarquardt<N,M,T>::setFirstCriterium

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setSecondCriterium(const T value) {
    this->eps2 = value;
  }  // end of FSLevenbergMarquardt<N,M,T>::setSecondCriterium

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setMultiplicationFactor(const T value) {
    this->factor = value;
  }  // end of FSLevenbergMarquardt<N,M,T>::setMultiplicationFactor

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::addData(
      const typename FSLevenbergMarquardt<N, M, T>::Variable& x, const T y) {
    using namespace std;
    this->data.push_back(pair<Variable, T>(x, y));
  }  // end of FSLevenbergMarquardt::addData

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setInitialGuess(
      const typename FSLevenbergMarquardt<N, M, T>::Parameter& p_) {
    this->p = p_;
  }  // end of FSLevenbergMarquardt::setInitialGuess

  template <unsigned short N, unsigned short M, typename T>
  void FSLevenbergMarquardt<N, M, T>::setMaximumIteration(const T nb) {
    this->iterMax = nb;
  }  // end of FSLevenbergMarquardt::setInitialGuess

  template <unsigned short N, unsigned short M, typename T>
  const typename FSLevenbergMarquardt<N, M, T>::Parameter&
  FSLevenbergMarquardt<N, M, T>::execute() {
    using namespace std;
    using tfel::fsalgo::fill;
    using tfel::math::stdfunctions::power;
    typename vector<pair<Variable, T>>::const_iterator it;
    // grad is the opposite of the gradient
    tmatrix<M, M, T> J(T(0));
    Parameter g(T(0));
    T s(T(0));
    T lambda = this->lambda0;
    unsigned short i;
    unsigned short iter;
    bool success;
    s = T(0);
    for (it = this->data.begin(); it != this->data.end(); ++it) {
      const pair<T, Parameter>& o = (*f)(it->first, this->p);
      g += (o.first - it->second) * o.second;
      J += (o.second) ^ (o.second);
      s += (o.first - it->second) * (o.first - it->second);
    }
    lambda *= *(max_element(J.begin(), J.end()));
    for (i = 0; i != M; ++i) {
      J(i, i) += lambda;
    }
    this->factor = 2;
    success = false;
    for (iter = 0; (iter != this->iterMax) && (!success); ++iter) {
      tmatrix<M, M, T> Jn(J);
      Parameter gn(T(0));
      Parameter h(-g);
      T sn(T(0));
      TinyMatrixSolve<M, T>::exe(Jn, h);
      fill<M * M>::exe(Jn.begin(), T(0));
      for (it = this->data.begin(); it != this->data.end(); ++it) {
        const pair<T, Parameter>& o = (*f)(it->first, this->p + h);
        gn += (o.first - it->second) * o.second;
        Jn += (o.second) ^ (o.second);
        sn += power<2>(o.first - it->second);
      }
      T rho = (s - sn) / (0.5 * (h | (lambda * h - g)));
      if (rho > 0) {
        lambda *= max(T(0.3333), T(1) - power<3>(2 * rho - 1));
        this->factor = 2;
      } else {
        lambda *= this->factor;
        this->factor *= 2;
      }
      for (i = 0; i != M; ++i) {
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

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_FSLEVENBERGMARQUARDTIXX */
