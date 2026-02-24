/*!
 * \file   include/TFEL/Math/NonLinearSolvers/BissectionAlgorithmBase.ixx
 * \brief
 * \author Thomas Helfer
 * \date   10/10/2020
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_IXX
#define LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_IXX

#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel::math {

  template <typename NumericType>
  void BissectionAlgorithmBase<NumericType>::updateBounds(
      const NumericType& x, const NumericType& f) {
    if ((!tfel::math::ieee754::isfinite(x)) ||
        (!tfel::math::ieee754::isfinite(f))) {
      return;
    }
    auto update_range = [this](const NumericType x1, const NumericType f1,
                               const NumericType x2, const NumericType f2) {
      if (x1 < x2) {
        this->xmin = x1;
        this->fmin = f1;
        this->xmax = x2;
        this->fmax = f2;
      } else {
        this->xmin = x2;
        this->fmin = f2;
        this->xmax = x1;
        this->fmax = f1;
      }
    };
    if (tfel::math::ieee754::isnan(this->xmin)) {
      // the first bound has not been set yet
      this->xmin = x;
      this->fmin = f;
      return;
    }
    if (tfel::math::ieee754::isnan(this->xmax)) {
      // the second bound has not been set yet
      update_range(this->xmin, this->fmin, x, f);
      return;
    }
    if (BissectionAlgorithmBase::haveSameSign(this->fmin, this->fmax)) {
      // still have not found an interval around a root
      if (BissectionAlgorithmBase::haveSameSign(this->fmin, f)) {
        if (x < this->xmin) {
          update_range(this->xmax, this->fmax, x, f);
        }
        if (x > this->xmax) {
          update_range(this->xmin, this->fmin, x, f);
        }
      } else {
        // an interval has been found, now we choose the closest point
        if (tfel::math::abs(x - this->xmin) < tfel::math::abs(x - this->xmax)) {
          update_range(this->xmin, this->fmin, x, f);
        } else {
          update_range(this->xmax, this->fmax, x, f);
        }
      }
      return;
    }
    // a interval around a root has been found
    if (BissectionAlgorithmBase::haveSameSign(this->fmin, f)) {
      if ((x > this->xmin) && (x < this->xmax)) {
        this->xmin = x;
        this->fmin = f;
      }
    } else {
      if ((x < this->xmax) && (x > this->xmin)) {
        this->xmax = x;
        this->fmax = f;
      }
    }
  }  // end of BissectionAlgorithm<NumericType>::updateBounds

  template <typename NumericType>
  void BissectionAlgorithmBase<NumericType>::iterate(NumericType& x) {
    if ((!tfel::math::ieee754::isfinite(this->xmin)) ||
        (!tfel::math::ieee754::isfinite(this->xmax)) ||
        (BissectionAlgorithmBase::haveSameSign(this->fmin, this->fmax))) {
      return;
    }
    if ((!tfel::math::ieee754::isfinite(x)) ||  //
        ((x < this->xmin) || (x > this->xmax))) {
      this->getNextRootEstimate(x);
    }
  }  // end of BissectionAlgorithm<NumericType>::iterate

  template <typename NumericType>
  bool BissectionAlgorithmBase<NumericType>::getNextRootEstimate(
      NumericType& x) const {
    if ((!tfel::math::ieee754::isfinite(this->xmin)) ||
        (!tfel::math::ieee754::isfinite(this->xmax)) ||
        (BissectionAlgorithmBase::haveSameSign(this->fmin, this->fmax))) {
      return false;
    }
    if (tfel::math::ieee754::fpclassify(this->fmax - this->fmin) != FP_ZERO) {
      const auto islope = (this->xmax - this->xmin) / (this->fmax - this->fmin);
      x = this->xmin - islope * (this->fmin);
      if ((x < this->xmin) || (x > this->xmax)) {
        x = (this->xmin + this->xmax) / 2;
      }
    } else {
      x = (this->xmin + this->xmax) / 2;
    }
    return true;
  }  // end of BissectionAlgorithmBase<NumericType>::getNextRootEstimate

  template <typename NumericType>
  bool BissectionAlgorithmBase<NumericType>::haveSameSign(const NumericType a,
                                                          const NumericType b) {
    auto sgn = [](const NumericType value) {
      constexpr auto zero = NumericType{};
      return (zero < value) - (value < zero);
    };
    return sgn(a) == sgn(b);
  }  // end of BissectionAlgorithmBase<NumericType>::haveSameSign

  template <typename NumericType>
  const BissectionAlgorithmData<NumericType>&
  BissectionAlgorithmBase<NumericType>::getData() const {
    return *this;
  }  // end of BissectionAlgorithm<NumericType>::getData

  template <typename NumericType>
  std::ostream& operator<<(std::ostream& os,
                           const BissectionAlgorithmData<NumericType>& a) {
    os << "{{" << a.xmin << "," << a.fmin << "}, {" << a.xmax << "," << a.fmax
       << "}}";
    return os;
  }  // end of operator <<

  template <typename NumericType>
  std::ostream& operator<<(std::ostream& os,
                           const BissectionAlgorithmBase<NumericType>& a) {
    os << a.getData();
    return os;
  }  // end of operator<<

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_BISSECTIONALGORITHMBASE_IXX */
