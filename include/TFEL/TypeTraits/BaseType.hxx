/*!
 * \file   include/TFEL/TypeTraits/BaseType.hxx
 * \brief  This file declares the BaseType traits class.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TYPETRAITS_BASETYPE_HXX
#define LIB_TFEL_TYPETRAITS_BASETYPE_HXX

#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/Forward/Complex.hxx"

/*!
 * \def TFEL_MATH_BASETYPE
 * \brief An helper macro used to define BaseType for the standard
 * types.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 */
#define TFEL_MATH_BASETYPE(X)            \
  /*!                                    \
   * \brief Partial specialisation for X \
   * \see   BaseType                     \
   */                                    \
  template <>                            \
  struct BaseType<X> {                   \
    /*! \brief result */                 \
    using type = X;                      \
  }

namespace tfel::typetraits {

  /*!
   * \class BaseType
   * \brief Most mathematical objects are build upon a fundamental
   * numeric type. This traits gives access to this type.
   *
   * \param  T, type for which we want to know the base type.
   * \return type, the fundamental numeric type upon which T is build.
   *
   * \author Thomas Helfer
   * \date   26/07/2006
   */
  template <typename T>
  struct BaseType {
    //! \brief result of the metafunction
    using type = tfel::meta::InvalidType;
  };
  /*!
   * \brief Partial specialisation for unsigned short
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(unsigned short);
  /*!
   * \brief Partial specialisation for unsigned int
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(unsigned int);
  /*!
   * \brief Partial specialisation for long unsigned int
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(long unsigned int);
  /*!
   * \brief Partial specialisation for short
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(short);
  /*!
   * \brief Partial specialisation for int
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(int);
  /*!
   * \brief Partial specialisation for long int
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(long int);
  /*!
   * \brief Partial specialisation for float
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(float);
  /*!
   * \brief Partial specialisation for double
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(double);
  /*!
   * \brief Partial specialisation for long double
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(long double);
  /*!
   * \brief Partial specialisation for Complex<unsigned short>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<unsigned short>);
  /*!
   * \brief Partial specialisation for Complex<unsigned int>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<unsigned int>);
  /*!
   * \brief Partial specialisation for Complex<long unsigned int>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<long unsigned int>);
  /*!
   * \brief Partial specialisation for Complex<short>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<short>);
  /*!
   * \brief Partial specialisation for Complex<int>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<int>);
  /*!
   * \brief Partial specialisation for Complex<long int>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<long int>);
  /*!
   * \brief Partial specialisation for Complex<float>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<float>);
  /*!
   * \brief Partial specialisation for Complex<double>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<double>);
  /*!
   * \brief Partial specialisation for Complex<long double>
   * \see   BaseType
   */
  TFEL_MATH_BASETYPE(tfel::math::Complex<long double>);
  //! a convenient alias
  template <typename T>
  using base_type = typename BaseType<T>::type;

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_TYPETRAITS_BASETYPE_HXX */
