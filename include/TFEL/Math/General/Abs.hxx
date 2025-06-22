/*!
 * \file   include/TFEL/Math/General/Abs.hxx
 * \brief  This file declares the abs function
 * \author Helfer Thomas
 * \date   13 Feb 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ABS_H_
#define LIB_TFEL_MATH_ABS_H_

#include <cmath>
#include <functional>

#include "TFEL/Config/TFELConfig.hxx"
#include <type_traits>
#include "TFEL/TypeTraits/AbsType.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"

namespace tfel {

  namespace math {

    template <typename Scal>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::typetraits::IsFundamentalNumericType<Scal>::cond,
        Scal>::type
    abs(const Scal& s) {
      return std::abs(s);
    }

    template <typename Scal>
    TFEL_MATH_INLINE typename std::enable_if<
        tfel::typetraits::IsFundamentalNumericType<Scal>::cond,
        Scal>::type
    abs(const Complex<Scal>& s) {
      return s.norm();
    }

    /*!
     * a basic functor accumulating the absolute value of a container
     */
    template <typename T>
    struct AbsSum : public std::unary_function<T, void> {
      /*!
       * \param [in] v : initial value
       */
      AbsSum(const T& = T());

      /*!
       * add the absolute value of the result to the res member
       */
      void operator()(const T&);

      /*!
       * result
       */
      T result;
    };  // end of struct AbsSum

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/General/Abs.ixx"

#endif /* LIB_TFEL_MATH_ABS_H_ */
