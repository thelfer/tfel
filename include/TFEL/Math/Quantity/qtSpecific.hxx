/*!
 *\file   qtSpecific.hxx
 *\brief  This file pecialises some class traits for quantities.
 *\author Thomas Helfer
 *\date   06 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 *linking exception or the CECILL-A licence. A copy of thoses licences are
 *delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_QTSPECIFIC_HXX
#define LIB_TFEL_MATH_QTSPECIFIC_HXX

#include "TFEL/TypeTraits/AbsType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"

/*!
 * \def TFEL_MATH_QT_BASE_TYPE
 * \brief  An helper macro used to define BaseType for quantity.
 * \author Thomas Helfer
 * \date   26 Jul 2006
 */
#define TFEL_MATH_QT_BASE_TYPE(X)                                       \
  /*!                                                                   \
   * \brief Partial specialisation for X                                \
   * \see   BaseType                                                    \
   */                                                                   \
  template <typename UnitType, typename OwnershipPolicy>                \
  struct BaseType<tfel::math::Quantity<UnitType, X, OwnershipPolicy>> { \
    /*!                                                                 \
     * \brief result of the metafunction                                \
     */                                                                 \
    using type = X;                                                     \
  }

#define TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(X)                             \
  /*!                                                                    \
   * \brief Partial specialisation for quantity                          \
   * \see   IsAssignableTo                                               \
   */                                                                    \
  template <typename T, typename OwnershipPolicy>                        \
  struct IsAssignableTo<                                                 \
      tfel::math::Quantity<tfel::math::NoUnit, T, OwnershipPolicy>, X> { \
    /*!                                                                  \
     * \brief result of the metafunction                                 \
     */                                                                  \
    static constexpr bool cond = IsAssignableTo<T, X>::cond;             \
  };                                                                     \
                                                                         \
  /*!                                                                    \
   * \brief Partial specialisation for quantity                          \
   * \see   IsAssignableTo                                               \
   */                                                                    \
  template <typename T, typename OwnershipPolicy>                        \
  struct IsAssignableTo<                                                 \
      X, tfel::math::Quantity<tfel::math::NoUnit, T, OwnershipPolicy>> { \
    /*!                                                                  \
     * \brief result of the metafunction                                 \
     */                                                                  \
    static constexpr bool cond = IsAssignableTo<X, T>::cond;             \
  }

#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/qt.hxx"

namespace tfel::typetraits {

  /*!
   * \brief Partial specialisation for qt
   * \see   IsScalar
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsScalar<tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsScalar
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsScalar<
      const tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsReal
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsReal<tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsReal<ValueType>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsReal
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsReal<
      const tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsReal<ValueType>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsComplex
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsComplex<tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsComplex<ValueType>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsComplex
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct IsComplex<
      const tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    static constexpr bool cond = IsComplex<ValueType>::cond;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsFundamentalNumericType
   */
  template <typename T>
  struct IsFundamentalNumericType<tfel::math::qt<tfel::math::NoUnit, T>> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsFundamentalNumericType
   */
  template <typename T>
  struct IsFundamentalNumericType<const tfel::math::qt<tfel::math::NoUnit, T>> {
    //! \brief result of the metafunction
    static constexpr bool cond = true;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   RealPartType
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct RealPartType<
      tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    typedef tfel::math::qt<UnitType, typename RealPartType<ValueType>::type>
        type;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   RealPartType
   */
  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct RealPartType<
      const tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    //! \brief result of the metafunction
    typedef const tfel::math::qt<UnitType,
                                 typename RealPartType<ValueType>::type>
        type;
  };

  /*!
   * \brief Partial specialisation for qt
   * \see   IsAssignableTo
   */
  template <typename UnitType,
            typename ValueType,
            typename OwnershipPolicy,
            typename ValueType2,
            typename OwnershipPolicy2>
  struct IsAssignableTo<
      tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>,
      tfel::math::Quantity<UnitType, ValueType2, OwnershipPolicy2>> {
    //! \brief result of the metafunction
    static constexpr bool cond = std::is_same_v<
        typename tfel::typetraits::Promote<ValueType, ValueType2>::type,
        ValueType2>;
  };

  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(unsigned short);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(unsigned int);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(long unsigned int);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(short);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(int);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(long int);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(float);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(double);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(long double);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<unsigned short>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<unsigned int>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<long unsigned int>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<short>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<int>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<long int>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<float>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<double>);
  TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(tfel::math::Complex<long double>);

  TFEL_MATH_QT_BASE_TYPE(unsigned short);
  TFEL_MATH_QT_BASE_TYPE(unsigned int);
  TFEL_MATH_QT_BASE_TYPE(long unsigned int);
  TFEL_MATH_QT_BASE_TYPE(short);
  TFEL_MATH_QT_BASE_TYPE(int);
  TFEL_MATH_QT_BASE_TYPE(long int);
  TFEL_MATH_QT_BASE_TYPE(float);
  TFEL_MATH_QT_BASE_TYPE(double);
  TFEL_MATH_QT_BASE_TYPE(long double);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<unsigned short>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<unsigned int>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<long unsigned int>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<short>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<int>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<long int>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<float>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<double>);
  TFEL_MATH_QT_BASE_TYPE(tfel::math::Complex<long double>);

  template <typename UnitType, typename ValueType, typename OwnershipPolicy>
  struct AbsType<tfel::math::Quantity<UnitType, ValueType, OwnershipPolicy>> {
    using type = tfel::math::qt<UnitType, typename AbsType<ValueType>::type>;
  };  // end of struct AbsType

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_QTSPECIFIC_HXX */
