/*!
 * \file  include/TFEL/Math/power.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 janv. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_POWER_H_
#define LIB_TFEL_MATH_POWER_H_

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"

namespace tfel {

  namespace math {

    template <int N, unsigned int D = 1>
    struct Power;

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<float, Power<N, D>> {
     public:
      typedef float type;
    };

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<double, Power<N, D>> {
     public:
      typedef double type;
    };

    template <int N, unsigned int D>
    class TFEL_VISIBILITY_LOCAL UnaryResultType<long double, Power<N, D>> {
     public:
      typedef long double type;
    };

    template <int N, typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, T>::type
        power(const T);

    template <int N, unsigned int D, typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond, T>::type
        power(const T);

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/power.ixx"

#endif /* LIB_TFEL_MATH_POWER_H_ */
