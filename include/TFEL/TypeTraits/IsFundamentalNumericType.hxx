/*!
 * \file   include/TFEL/TypeTraits/IsFundamentalNumericType.hxx
 * \brief  This file declares the IsFundamentalNumericType traits
 * class and specialises it for standard numerical types.
 * \author Thomas Helfer
 * \date   05 jui 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX
#define LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX

#include <type_traits>
#include "TFEL/Math/Forward/Complex.hxx"

/*!
 * \def    TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE
 * \brief An helper macro to specialise IsFundamentalNumericType for
 * each standard numeric types.
 * \author Thomas Helfer
 * \date   28 jun 2006
 */
#define TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(X) \
  /*!                                            \
   * \brief Partial specialisation for X         \
   * \see   IsFundamentalNumericType             \
   */                                            \
  template <>                                    \
  struct IsFundamentalNumericType<X> {           \
    /*!                                          \
     *  Result                                   \
     */                                          \
    static constexpr bool cond = true;           \
  };                                             \
  /*!                                            \
   * \brief Partial specialisation for X         \
   * \see   IsFundamentalNumericType             \
   */                                            \
  template <>                                    \
  struct IsFundamentalNumericType<const X> {     \
    /*!                                          \
     *  Result                                   \
     */                                          \
    static constexpr bool cond = true;           \
  }

namespace tfel::typetraits {

  /*!
   * \brief Traits class which says if its argument is one of the
   * standard numerical types
   *
   * \param  T, type to be tested.
   * \return bool cond, true if T is one of the standard numerical
   * types, false otherwise.
   *
   * \see IsReal, IsComplex
   *
   * \author Thomas Helfer
   * \date   04 Jul 2006
   */
  template <typename T>
  struct IsFundamentalNumericType {
    //! \brief result
    static constexpr bool cond = false;
  };

  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(unsigned short);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(unsigned int);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(long unsigned int);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(short);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(int);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(long int);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(float);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(double);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(long double);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<unsigned short>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<unsigned int>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<long unsigned int>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<short>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<int>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<long int>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<float>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<double>);
  TFEL_META_IS_FUNDAMENTAL_NUMERIC_TYPE(tfel::math::Complex<long double>);

  /*!
   * \brief a simple helper function
   * \tparam T: type tested
   */
  template <typename T>
  constexpr bool isFundamentalNumericType() {
    return IsFundamentalNumericType<std::decay_t<T>>::cond;
  }

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISFUNDAMENTALNUMERICTYPE_HXX */
