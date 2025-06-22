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

#ifndef LIB_TFEL_RUNGEKUTTA54IXX
#define LIB_TFEL_RUNGEKUTTA54IXX

namespace tfel {
  namespace math {

    template <unsigned short N, typename Child, typename Scalar>
    struct RungeKutta54<N, Child, Scalar>::AbsoluteSommation {
      const Scalar operator()(const Scalar& a, const Scalar& b) {
        return std::abs(a) + b;
      }
    };  // end of struct AbsoluteSommation

    template <unsigned short N, typename C, typename S>
    void RungeKutta54<N, C, S>::setInitialValue(
        const typename RungeKutta54<N, C, S>::VariableType& y_) {
      this->y = y_;
    }  // end of RungeKutta54<N,C,S>::setInitialValue

    template <unsigned short N, typename C, typename S>
    void RungeKutta54<N, C, S>::setInitialTime(const S ti_) {
      this->ti = ti_;
    }  // end of RungeKutta54<N,C,S>::setInitialTime

    template <unsigned short N, typename C, typename S>
    void RungeKutta54<N, C, S>::setFinalTime(const S tf_) {
      this->tf = tf_;
    }  // end of RungeKutta54<N,C,S>::setFinalTime

    template <unsigned short N, typename C, typename S>
    void RungeKutta54<N, C, S>::setInitialTimeIncrement(const S dt_) {
      this->dt = dt_;
    }  // end of RungeKutta54<N,C,S>::setInitialTimeIncrement

    template <unsigned short N, typename C, typename S>
    void RungeKutta54<N, C, S>::setCriterium(const S e_) {
      this->epsilon = e_;
    }  // end of RungeKutta54<N,C,S>::setInitialTimeIncrement

    template <unsigned short N, typename C, typename S>
    const typename RungeKutta54<N, C, S>::VariableType&
    RungeKutta54<N, C, S>::getValue() const {
      return this->y;
    }  // end of RungeKutta54<N,C,S>::getValue

    template <unsigned short N, typename C, typename S>
    const S& RungeKutta54<N, C, S>::getTimeIncrement() const {
      return this->dt;
    }  // end of RungeKutta54<N,C,S>::getTimeIncrement

    template <unsigned short N, typename Child, typename Scalar>
    void RungeKutta54<N, Child, Scalar>::iterate() {
      TFEL_CONSTEXPR const Scalar cste12_13 = Scalar(12) / Scalar(13);
      TFEL_CONSTEXPR const Scalar cste1932_2197 = Scalar(1932) / Scalar(2197);
      TFEL_CONSTEXPR const Scalar cste7200_2197 = Scalar(7200) / Scalar(2197);
      TFEL_CONSTEXPR const Scalar cste7296_2197 = Scalar(7296) / Scalar(2197);
      TFEL_CONSTEXPR const Scalar cste439_216 = Scalar(439) / Scalar(216);
      TFEL_CONSTEXPR const Scalar cste3680_513 = Scalar(3680) / Scalar(513);
      TFEL_CONSTEXPR const Scalar cste845_4104 = Scalar(845) / Scalar(4104);
      TFEL_CONSTEXPR const Scalar cste8_27 = Scalar(8) / Scalar(27);
      TFEL_CONSTEXPR const Scalar cste3544_2565 = Scalar(3544) / Scalar(2565);
      TFEL_CONSTEXPR const Scalar cste1859_4104 = Scalar(1859) / Scalar(4104);
      TFEL_CONSTEXPR const Scalar cste11_40 = Scalar(11) / Scalar(40);
      TFEL_CONSTEXPR const Scalar cste16_135 = Scalar(16) / Scalar(135);
      TFEL_CONSTEXPR const Scalar cste6656_12825 = Scalar(6656) / Scalar(12825);
      TFEL_CONSTEXPR const Scalar cste28561_56430 =
          Scalar(28561) / Scalar(56430);
      TFEL_CONSTEXPR const Scalar cste9_50 = Scalar(9) / Scalar(50);
      TFEL_CONSTEXPR const Scalar cste2_55 = Scalar(2) / Scalar(55);
      TFEL_CONSTEXPR const Scalar cste1_360 = Scalar(1) / Scalar(360);
      TFEL_CONSTEXPR const Scalar cste128_4275 = Scalar(128) / Scalar(4275);
      TFEL_CONSTEXPR const Scalar cste2197_75240 = Scalar(2197) / Scalar(75240);
      TFEL_CONSTEXPR const Scalar cste1_50 = Scalar(1) / Scalar(50);
      Scalar t = this->ti;  // current time
      Scalar t_;            // intermediate time
      Scalar e = 0;         // error norm
      VariableType y_;      // unknown estimation
      VariableType ve;      // error
      VariableType k1;
      VariableType k2;
      VariableType k3;
      VariableType k4;
      VariableType k5;
      VariableType k6;
      this->dt = std::min(this->dt, this->tf - this->ti);
      if (this->dt < 0) {
        throw(InvalidTimeStepException());
      }
      while (t < this->tf - 0.5 * (this->dt)) {
        k1 = this->dt * (static_cast<const Child*>(this)->computeF(t, this->y));
        t_ = t + 0.25 * (this->dt);
        y_ = this->y + 0.25 * k1;
        k2 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        t_ = t + 0.375 * (this->dt);
        y_ = this->y + 0.09375 * (k1 + 3 * k2);
        k3 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        t_ = t + cste12_13 * (this->dt);
        y_ = this->y + cste1932_2197 * k1 - cste7200_2197 * k2 +
             cste7296_2197 * k3;
        k4 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        t_ = t + this->dt;
        y_ = this->y + cste439_216 * k1 - 8 * k2 + cste3680_513 * k3 -
             cste845_4104 * k4;
        k5 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        t_ = t + 0.5 * (this->dt);
        y_ = this->y - cste8_27 * k1 + 2 * k2 - cste3544_2565 * k3 +
             cste1859_4104 * k4 - cste11_40 * k5;
        k6 = this->dt * (static_cast<const Child*>(this)->computeF(t_, y_));
        // error
        ve = cste1_360 * k1 - cste128_4275 * k3 - cste2197_75240 * k4 +
             cste1_50 * k5 + cste2_55 * k6;
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
          this->y += cste16_135 * k1 + cste6656_12825 * k3 +
                     cste28561_56430 * k4 - cste9_50 * k5 + cste2_55 * k6;
          t += this->dt;
        }
        // time multiplier
        if (t < this->tf - 0.5 * (this->dt)) {
          this->dt *= 0.8 * pow(this->epsilon / e, 0.2);
          if (this->dt > this->tf - t) {
            this->dt = this->tf - t;
          }
        }
      }
      this->dt *= 0.8 * pow(this->epsilon / e, 0.2);
    }  // end of RungeKutta54<N,C,S>::iterate

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_RUNGEKUTTA54IXX */
