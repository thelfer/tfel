/*!
 * \file   include/TFEL/TypeTraits/IsScalar.hxx
 * \brief  This file declares the IsScalar traits class
 * and specialises it for standard numeric types.
 * \author Thomas Helfer
 * \date   07/07/2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_ISSCALAR_HXX
#define LIB_TFEL_TYPETRAITS_ISSCALAR_HXX

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Forward/Complex.hxx"

/*!
 * \def    TFEL_META_IS_SCALAR
 * \brief An helper macro to specialise IsScalar for each standard
 * "real" numeric types.
 *
 * \author Thomas Helfer
 * \date   28/06/2006
 */
#define TFEL_META_IS_SCALAR(X)           \
  /*!                                    \
   * \brief Partial specialisation for X \
   * \see   IsScalar                     \
   */                                    \
  template <>                            \
  struct IsScalar<X> {                   \
    /*!                                  \
     *  Result                           \
     */                                  \
    static constexpr bool cond = true;   \
  };                                     \
  /*!                                    \
   * \brief Partial specialisation for X \
   * \see   IsScalar                     \
   */                                    \
  template <>                            \
  struct IsScalar<const X> {             \
    /*!                                  \
     *  Result                           \
     */                                  \
    static constexpr bool cond = true;   \
  }

namespace tfel::typetraits {

  /*!
   * \brief  Traits class which says if its argument stands for a scalar
   *
   * \param  T, type to be tested.
   * \return bool cond, true if T stands for a scalar, false otherwise.
   *
   * \see IsReal, IsComplex
   *
   * \author Thomas Helfer
   * \date   04/07/2006
   */
  template <typename T>
  struct IsScalar {
    //! result of the metafunction
    static constexpr bool cond = false;
  };

  TFEL_META_IS_SCALAR(unsigned short);
  TFEL_META_IS_SCALAR(unsigned int);
  TFEL_META_IS_SCALAR(long unsigned int);
  TFEL_META_IS_SCALAR(short);
  TFEL_META_IS_SCALAR(int);
  TFEL_META_IS_SCALAR(long int);
  TFEL_META_IS_SCALAR(float);
  TFEL_META_IS_SCALAR(double);
  TFEL_META_IS_SCALAR(long double);
  TFEL_META_IS_SCALAR(tfel::math::Complex<unsigned short>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<unsigned int>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<long unsigned int>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<short>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<int>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<long int>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<float>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<double>);
  TFEL_META_IS_SCALAR(tfel::math::Complex<long double>);

  /*!
   * \brief a simple helper function
   * \tparam T: type tested
   */
  template <typename T>
  TFEL_HOST_DEVICE constexpr bool isScalar() {
    return IsScalar<std::decay_t<T>>::cond;
  }

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_ISSCALAR_HXX */
