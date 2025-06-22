/*!
 *\file   qtSpecific.hxx
 *\brief  This file pecialises some class traits for quantities.
 *\author Helfer Thomas
 *\date   06 Jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 *linking exception or the CECILL-A licence. A copy of thoses licences are
 *delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_QTSPECIFIC_H_
#define LIB_TFEL_QTSPECIFIC_H_

/*!
 * \def TFEL_MATH_QT_BASE_TYPE
 * \brief  An helper macro used to define BaseType for quantity.
 * \author Helfer Thomas
 * \date   26 Jul 2006
 */
#define TFEL_MATH_QT_BASE_TYPE(X)                          \
  /*!                                                      \
   * \brief Partial specialisation for X                   \
   * \see   BaseType                                       \
   */                                                      \
  template <typename unit>                                 \
  struct BaseType<tfel::math::qt<unit, X>> {               \
    /*!                                                    \
     *  Result                                             \
     */                                                    \
    typedef tfel::math::qt<tfel::math::NoUnit, X> type;    \
  };                                                       \
  /*!                                                      \
   * \brief Partial specialisation for NoUnit              \
   * \see   BaseType                                       \
   */                                                      \
  template <>                                              \
  struct BaseType<tfel::math::qt<tfel::math::NoUnit, X>> { \
    /*!                                                    \
     *  Result                                             \
     */                                                    \
    typedef tfel::math::qt<tfel::math::NoUnit, X> type;    \
  }

#define TFEL_MATH_QTNOUNIT_ISASSIGNABLETO(X)                        \
  /*!                                                               \
   * \brief Partial specialisation for qt                           \
   * \see   IsAssignableTo                                          \
   */                                                               \
  template <typename T>                                             \
  struct IsAssignableTo<tfel::math::qt<tfel::math::NoUnit, T>, X> { \
    /*!                                                             \
     *  Result                                                      \
     */                                                             \
    static constexpr bool cond = IsAssignableTo<T, X>::cond;        \
  };                                                                \
                                                                    \
  /*!                                                               \
   * \brief Partial specialisation for qt                           \
   * \see   IsAssignableTo                                          \
   */                                                               \
  template <typename T>                                             \
  struct IsAssignableTo<X, tfel::math::qt<tfel::math::NoUnit, T>> { \
    /*!                                                             \
     *  Result                                                      \
     */                                                             \
    static constexpr bool cond = IsAssignableTo<X, T>::cond;        \
  }

#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/RealPartType.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/Math/qt.hxx"

namespace tfel {

  namespace typetraits {

    /*!
     * \brief Partial specialisation for qt
     * \see   IsScalar
     */
    template <typename unit, typename T>
    struct IsScalar<tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsScalar
     */
    template <typename unit, typename T>
    struct IsScalar<const tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsReal
     */
    template <typename unit, typename T>
    struct IsReal<tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsReal<T>::cond;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsReal
     */
    template <typename unit, typename T>
    struct IsReal<const tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsReal<T>::cond;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsComplex
     */
    template <typename unit, typename T>
    struct IsComplex<tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsComplex<T>::cond;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsComplex
     */
    template <typename unit, typename T>
    struct IsComplex<const tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = IsComplex<T>::cond;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsFundamentalNumericType
     */
    template <typename T>
    struct IsFundamentalNumericType<tfel::math::qt<tfel::math::NoUnit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsFundamentalNumericType
     */
    template <typename T>
    struct IsFundamentalNumericType<
        const tfel::math::qt<tfel::math::NoUnit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   RealPartType
     */
    template <typename unit, typename T>
    struct RealPartType<tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      typedef tfel::math::qt<unit, typename RealPartType<T>::type> type;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   RealPartType
     */
    template <typename unit, typename T>
    struct RealPartType<const tfel::math::qt<unit, T>> {
      /*!
       *  Result
       */
      typedef const tfel::math::qt<unit, typename RealPartType<T>::type> type;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsAssignableTo
     */
    template <typename unit, typename T, typename T2>
    struct IsAssignableTo<tfel::math::qt<unit, T>, tfel::math::qt<unit, T2>> {
      /*!
       *  Result
       */
      static constexpr bool cond =
          std::is_same<typename tfel::typetraits::Promote<T, T2>::type,
                       T2>::value;
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

    /*!
     * \brief Partial specialisation for qt
     * \see   IsSafelyReinterpretCastableTo
     */
    template <typename unit, typename T>
    struct IsSafelyReinterpretCastableTo<tfel::math::qt<unit, T>, T> {
      /*!
       *  Check if qt<unit,T> and T have the same size
       */
      TFEL_STATIC_ASSERT(sizeof(tfel::math::qt<unit, T>) == sizeof(T));
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsSafelyReinterpretCastableTo
     */
    template <typename unit, typename T>
    struct IsSafelyReinterpretCastableTo<T, tfel::math::qt<unit, T>> {
      /*!
       *  Check if qt<unit,T> and T have the same size
       */
      TFEL_STATIC_ASSERT(sizeof(tfel::math::qt<unit, T>) == sizeof(T));
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsSafelyReinterpretCastableTo
     */
    template <typename unit, typename T>
    struct IsSafelyReinterpretCastableTo<
        tfel::math::qt<unit, T>,
        tfel::math::qt<tfel::math::NoUnit, T>> {
      /*!
       *  Check if qt<unit,T> and T have the same size
       */
      TFEL_STATIC_ASSERT(sizeof(tfel::math::qt<unit, T>) ==
                         sizeof(tfel::math::qt<tfel::math::NoUnit, T>));
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsSafelyReinterpretCastableTo
     */
    template <typename unit, typename T>
    struct IsSafelyReinterpretCastableTo<tfel::math::qt<tfel::math::NoUnit, T>,
                                         tfel::math::qt<unit, T>> {
      /*!
       *  Check if qt<unit,T> and T have the same size
       */
      TFEL_STATIC_ASSERT(sizeof(tfel::math::qt<unit, T>) ==
                         sizeof(tfel::math::qt<tfel::math::NoUnit, T>));
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

    /*!
     * \brief Partial specialisation for qt
     * \see   IsSafelyReinterpretCastableTo
     */
    template <typename T>
    struct IsSafelyReinterpretCastableTo<
        tfel::math::qt<tfel::math::NoUnit, T>,
        tfel::math::qt<tfel::math::NoUnit, T>> {
      /*!
       *  Result
       */
      static constexpr bool cond = true;
    };

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

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_QTSPECIFIC_H_ */
