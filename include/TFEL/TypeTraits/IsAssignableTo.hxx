/*!
 * \file   include/TFEL/TypeTraits/IsAssignableTo.hxx
 * \brief  This file implements the IsAssignableTo class traits and
 * its partial specialisation for each standard numeric types.
 *
 * \author Thomas Helfer
 * \date   28 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_IS_ASSIGNABLETO_HXX
#define LIB_TFEL_IS_ASSIGNABLETO_HXX

#include <type_traits>
#include "TFEL/TypeTraits/Promote.hxx"
#include "TFEL/Math/Forward/Complex.hxx"

/*!
 * \def TFEL_MATH_IS_ASSIGNABLE_
 * \brief An helper macro to specialise IsAssignableTo for each
 * standard numeric types.
 * \author Thomas Helfer
 * \date   28 jun 2006
 */
#define TFEL_MATH_IS_ASSIGNABLE_(X, Y)                                        \
  /*!                                                                         \
   * \brief Partial specialisation for X and Y                                \
   * \see   IsAssignableTo                                                    \
   */                                                                         \
  template <>                                                                 \
  struct IsAssignableTo<X, Y> {                                               \
    /*!                                                                       \
     *  Result                                                                \
     */                                                                       \
    static constexpr bool cond = std::is_same<Promote<X, Y>::type, Y>::value; \
  }

/*!
 * \def TFEL_MATH_IS_ASSIGNABLE
 * \brief An helper macro to specialise IsAssignableTo for each
 * standard numeric types.
 * \author Thomas Helfer
 * \date   28 jun 2006
 */
#define TFEL_MATH_IS_ASSIGNABLE(X)                                     \
  TFEL_MATH_IS_ASSIGNABLE_(X, unsigned short);                         \
  TFEL_MATH_IS_ASSIGNABLE_(X, unsigned int);                           \
  TFEL_MATH_IS_ASSIGNABLE_(X, long unsigned int);                      \
  TFEL_MATH_IS_ASSIGNABLE_(X, short);                                  \
  TFEL_MATH_IS_ASSIGNABLE_(X, int);                                    \
  TFEL_MATH_IS_ASSIGNABLE_(X, long int);                               \
  TFEL_MATH_IS_ASSIGNABLE_(X, float);                                  \
  TFEL_MATH_IS_ASSIGNABLE_(X, double);                                 \
  TFEL_MATH_IS_ASSIGNABLE_(X, long double);                            \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<unsigned short>);    \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<unsigned int>);      \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<long unsigned int>); \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<short>);             \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<int>);               \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<long int>);          \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<float>);             \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<double>);            \
  TFEL_MATH_IS_ASSIGNABLE_(X, tfel::math::Complex<long double>)

namespace tfel {

  namespace typetraits {

    /*!
     * \class IsAssignableTo
     * \brief Traits class which says if an object a of type A is
     * assignable to an object b of type B, ie if b = a is valid.
     *
     * \param  A, first type
     * \param  B, second type
     * \return bool cond, true if b = a is valid, false otherwise.
     *
     * \see Promote
     * \author Thomas Helfer
     * \date   28 jun 2006
     */
    template <typename A, typename B>
    struct IsAssignableTo {
      //! result
      static constexpr bool cond = false;
    };

    TFEL_MATH_IS_ASSIGNABLE(unsigned short);
    TFEL_MATH_IS_ASSIGNABLE(unsigned int);
    TFEL_MATH_IS_ASSIGNABLE(long unsigned int);
    TFEL_MATH_IS_ASSIGNABLE(short);
    TFEL_MATH_IS_ASSIGNABLE(int);
    TFEL_MATH_IS_ASSIGNABLE(long int);
    TFEL_MATH_IS_ASSIGNABLE(float);
    TFEL_MATH_IS_ASSIGNABLE(double);
    TFEL_MATH_IS_ASSIGNABLE(long double);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<unsigned short>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<unsigned int>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<long unsigned int>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<short>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<int>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<long int>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<float>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<double>);
    TFEL_MATH_IS_ASSIGNABLE(tfel::math::Complex<long double>);

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_IS_ASSIGNABLETO_HXX */
