/*!
 * \file
 * include/TFEL/Math/AccelerationAlgorithms/AitkenAccelerationAlgorithm.ixx
 * \brief
 * \author Thomas Helfer
 * \date   28/05/2020
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_IXX
#define LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_IXX

#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/IEEE754.hxx"

namespace tfel {

  namespace math {

    template <typename NumericType, typename IndexType>
    void AitkenAccelerationAlgorithm<NumericType, IndexType>::initialize(
        const NumericType x) {
      this->i = false;
      this->x0 = x;
      this->x1 = NumericType{};
    }  // end of AitkenAccelerationAlgorithm::initialize

    template <typename NumericType, typename IndexType>
    void AitkenAccelerationAlgorithm<NumericType, IndexType>::accelerate(
        NumericType& x) {
      using real = tfel::typetraits::base_type<NumericType>;
      constexpr const auto one = real{1};
      if (!this->i) {
        this->x1 = x;
      } else {
        const auto x2 = x;
        const auto d0 = this->x1 - this->x0;
        const auto d1 = x2 - this->x1;
        if ((tfel::math::ieee754::fpclassify(d0) != FP_ZERO) &&
            (tfel::math::ieee754::fpclassify(d1) != FP_ZERO)) {
          const auto id0 = one / d0;
          const auto id1 = one / d1;
          const auto d2 = id1 - id0;
          if (tfel::math::ieee754::fpclassify(d2) != FP_ZERO) {
            x = this->x1 + one / d2;
          }
        }
        this->x0 = x;
        this->x1 = NumericType{};
      }
      this->i = !(this->i);
    }  // end of AitkenAccelerationAlgorithm::accelerate

    template <typename NumericType, typename IndexType>
    std::ostream& operator<<(
        std::ostream& os,
        const AitkenAccelerationAlgorithm<NumericType, IndexType>&) {
      os << "{}";
      return os;
    }  // end of operator<<

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_ACCELERATIONALGORITHMS_AITKENACCELERATIONALGORITHM_IXX \
        */
