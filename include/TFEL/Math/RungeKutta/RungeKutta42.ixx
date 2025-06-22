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

#ifndef LIB_TFEL_RUNGEKUTTA42IXX
#define LIB_TFEL_RUNGEKUTTA42IXX

namespace tfel {
  namespace math {

    template <unsigned short N, typename Child, typename Scalar>
    struct RungeKutta42<N, Child, Scalar>::AbsoluteSommation {
      const Scalar operator()(const Scalar& a, const Scalar& b) {
        return std::abs(a) + b;
      }
    };  // end of struct AbsoluteSommation

    template <unsigned short N, typename C, typename S>
    void RungeKutta42<N, C, S>::setInitialValue(
        const typename RungeKutta42<N, C, S>::VariableType& y_) {
      this->y = y_;
    }  // end of RungeKutta42<N,C,S>::setInitialValue

    template <unsigned short N, typename C, typename S>
    void RungeKutta42<N, C, S>::setInitialTime(const S ti_) {
      this->ti = ti_;
    }  // end of RungeKutta42<N,C,S>::setInitialTime

    template <unsigned short N, typename C, typename S>
    void RungeKutta42<N, C, S>::setFinalTime(const S tf_) {
      this->tf = tf_;
    }  // end of RungeKutta42<N,C,S>::setFinalTime

    template <unsigned short N, typename C, typename S>
    void RungeKutta42<N, C, S>::setInitialTimeIncrement(const S dt_) {
      this->dt = dt_;
    }  // end of RungeKutta42<N,C,S>::setInitialTimeIncrement

    template <unsigned short N, typename C, typename S>
    void RungeKutta42<N, C, S>::setCriterium(const S e_) {
      this->epsilon = e_;
    }  // end of RungeKutta42<N,C,S>::setInitialTimeIncrement

    template <unsigned short N, typename C, typename S>
    const typename RungeKutta42<N, C, S>::VariableType&
    RungeKutta42<N, C, S>::getValue() const {
      return this->y;
    }  // end of RungeKutta42<N,C,S>::getValue

    template <unsigned short N, typename C, typename S>
    const S& RungeKutta42<N, C, S>::getTimeIncrement() const {
      return this->dt;
    }  // end of RungeKutta42<N,C,S>::getTimeIncrement

    template <unsigned short N, typename Child, typename Scalar>
    void RungeKutta42<N, Child, Scalar>::iterate() {
      TFEL_CONSTEXPR const Scalar cste1_6 = Scalar(1) / Scalar(6);
      TFEL_CONSTEXPR const Scalar cste1_3 = Scalar(1) / Scalar(3);
      Scalar t = this->ti;  // current time
      Scalar t_;            // intermediate time
      Scalar e = 0;         // error norm
      VariableType y_;      // unknown estimation
      VariableType ve;      // error
      VariableType k1;
      VariableType k2;
      VariableType k3;
      VariableType k4;
      this->dt = std::min(this->dt, this->tf - this->ti);
      if (this->dt < 0) {
        throw(InvalidTimeStepException());
      }
      while (t < this->tf - 0.5 * (this->dt)) {
        // compute k1
        k1 = this->dt * (static_cast<const Child*>(this)->computeF(t, this->y));
        // compute k2
        t_ = t + 0.5 * (this->dt);
        y_ = this->y + 0.5 * k1;
        k2 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        // compute k3
        y_ = this->y + 0.5 * k2;
        k3 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        // compute k4
        t_ = t + this->dt;
        y_ = this->y + k3;
        k4 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        // error
        ve = cste1_6 * (k1 + k4 - k2 - k3);
        // error norm
        e = tfel::fsalgo::accumulate<N>::exe(ve.begin(), Scalar(0),
                                             AbsoluteSommation());
        assert(e >= 0.);
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
      this->dt *= 0.8 * pow(this->epsilon / e, cste1_3);
    }  // end of RungeKutta42<N,C,S>::iterate

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_RUNGEKUTTA42IXX */
