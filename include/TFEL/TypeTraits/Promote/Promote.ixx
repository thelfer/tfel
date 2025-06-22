/*!
 *\file   Promote.ixx
 *\brief  This file implement the Promote class.
 *\author Helfer Thomas
 *\date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 *linking exception or the CECILL-A licence. A copy of thoses licences are
 *delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PROMOTE_IXX_
#define LIB_TFEL_PROMOTE_IXX_

#include <type_traits>
#include "TFEL/Metaprogramming/StaticAssert.hxx"

/*!
 * \def    TFEL_MATH_CHOOSETYPE
 * \brief  An helper macro to specialise ChooseType.
 * \author Helfer Thomas
 * \date   01 Sept 2006
 */
#define TFEL_MATH_CHOOSETYPE(X)                    \
  /*!                                              \
   * \brief Partial specialisation for X           \
   * \see   ChooseType                             \
   */                                              \
  template <>                                      \
  struct ChooseType<long double, X, true, false> { \
    /*!                                            \
     * The result                                  \
     */                                            \
    typedef long double type;                      \
  };                                               \
                                                   \
  /*!                                              \
   * \brief Partial specialisation for X           \
   * \see   ChooseType                             \
   */                                              \
  template <>                                      \
  struct ChooseType<X, long double, false, true> { \
    /*!                                            \
     * The result                                  \
     */                                            \
    typedef long double type;                      \
  }

namespace tfel {

  namespace typetraits {

    namespace internals {

      /*!
       * \class  PromoteNumericsGreater_
       * \brief  An helper class which return the greatest type, ie the type
       * which has the maximal positive value. \param  A, first  type \param  B,
       * second type \return type, the result. \pre    std::numeric_limits<A>
       * and std::numeric_limits<B> must be defined \author Helfer Thomas \date
       * 02 jun 2006
       */
      template <typename A, typename B>
      struct PromoteNumericsGreater_ {
        /*!
         * Requires std::numeric_limits<A> to be defined
         */
        TFEL_STATIC_ASSERT((std::numeric_limits<A>::is_specialized));
        /*!
         * Requires std::numeric_limits<B> to be defined
         */
        TFEL_STATIC_ASSERT((std::numeric_limits<B>::is_specialized));

        /*!
         * The result
         */
        typedef typename std::conditional<
            ((std::numeric_limits<A>::max_exponent10 <
              std::numeric_limits<B>::max_exponent10) ||
             ((std::numeric_limits<A>::max_exponent10 ==
               std::numeric_limits<B>::max_exponent10) &&
              (std::numeric_limits<A>::digits <=
               std::numeric_limits<B>::digits))),
            B,
            A>::type type;
      };

      /*!
       * \brief Partial specialisation for unsigned int and long unsigned int.
       * They can be the same type on some plateform, so the algorithm used in
       * PromoteNumericsGreater_ can't tell which is the greatest one. \see
       * PromoteNumericsGreater_
       */
      template <>
      struct PromoteNumericsGreater_<unsigned int, long unsigned int> {
        /*!
         * The result
         */
        typedef long unsigned int type;
      };

      /*!
       * \brief Partial specialisation for long unsigned int and unsigned int.
       * They can be the same type on some plateform, so the algorithm used in
       * PromoteNumericsGreater_ can't tell which is the greatest one. \see
       * PromoteNumericsGreater_
       */
      template <>
      struct PromoteNumericsGreater_<long unsigned int, unsigned int> {
        /*!
         * The result
         */
        typedef long unsigned int type;
      };

      /*!
       * \brief Partial specialisation for int and long int.
       * They can be the same type on some plateform, so the algorithm used in
       * PromoteNumericsGreater_ can't tell which is the greatest one. \see
       * PromoteNumericsGreater_
       */
      template <>
      struct PromoteNumericsGreater_<int, long int> {
        /*!
         * The result
         */
        typedef long int type;
      };

      /*!
       * \brief Partial specialisation for long int and int.
       * They can be the same type on some plateform, so the algorithm used in
       * PromoteNumericsGreater_ can't tell which is the greatest one. \see
       * PromoteNumericsGreater_
       */
      template <>
      struct PromoteNumericsGreater_<long int, int> {
        /*!
         * The result
         */
        typedef long int type;
      };

      /*!
       * \brief  An helper class which associates to a type a type that is
       * greater. \param  T, a type \return type, the result \author Helfer
       * Thomas \date   02 jun 2006
       */
      template <typename>
      struct Promote_;

      /*!
       * \brief Partial specialisation for short.
       * \see Promote_
       */
      template <>
      struct Promote_<short> {
        /*!
         * Check if int is greater than short (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<PromoteNumericsGreater_<short, int>::type,
                          int>::value));
        /*!
         * The result
         */
        typedef int type;
      };

      /*!
       * \brief Partial specialisation for unsigned short.
       * \see Promote_
       */
      template <>
      struct Promote_<unsigned short> {
        /*!
         * Check if unsigned int is greater than unsigned short (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<
                PromoteNumericsGreater_<unsigned short, unsigned int>::type,
                unsigned int>::value));
        /*!
         * The result
         */
        typedef unsigned int type;
      };

      /*!
       * \brief Partial specialisation for int.
       * \see Promote_
       */
      template <>
      struct Promote_<int> {
        /*!
         * Check if long int is greater than int (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<PromoteNumericsGreater_<int, long int>::type,
                          long int>::value));
        /*!
         * The result
         */
        typedef long int type;
      };

      /*!
       * \brief Partial specialisation for unsigned int.
       * \see Promote_
       */
      template <>
      struct Promote_<unsigned int> {
        /*!
         * Check if long unsigned int is greater than unsigned int (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<
                PromoteNumericsGreater_<unsigned int, long unsigned int>::type,
                long unsigned int>::value));
        /*!
         * The result
         */
        typedef long unsigned int type;
      };

      /*!
       * \brief Partial specialisation for long int.
       * \see Promote_
       */
      template <>
      struct Promote_<long int> {
        /*!
         * Check if float is greater than long int
         * \warning this was false under cygwin
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<PromoteNumericsGreater_<long int, float>::type,
                          float>::value));
        /*!
         * The result
         */
        typedef float type;
      };

      /*!
       * \brief Partial specialisation for long unsigned int.
       * \see Promote_
       */
      template <>
      struct Promote_<long unsigned int> {
        /*!
         * Check if float is greater than long unsigned int
         * \warning this was false under cygwin
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<
                PromoteNumericsGreater_<long unsigned int, float>::type,
                float>::value));
        /*!
         * The result
         */
        typedef float type;
      };

      /*!
       * \brief Partial specialisation for float
       * \see Promote_
       */
      template <>
      struct Promote_<float> {
        /*!
         * Check if double is greater than float (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<PromoteNumericsGreater_<float, double>::type,
                          double>::value));
        /*!
         * The result
         */
        typedef double type;
      };

      /*!
       * \brief Partial specialisation for double
       * \see Promote_
       */
      template <>
      struct Promote_<double> {
        /*!
         * Check if long double is greater than double (!)
         */
        TFEL_STATIC_ASSERT(
            (std::is_same<PromoteNumericsGreater_<double, long double>::type,
                          long double>::value));
        /*!
         * The result
         */
        typedef long double type;
      };

      /*!
       * \brief Partial specialisation for long double.
       * long double is supposed to be the greatest numerical type.
       * \see Promote_
       */
      template <>
      struct Promote_<long double> {
        /*!
         * The result
         */
        typedef long double type;
      };

      /*!
       * \class  ChooseType
       * \brief  Choose the largest of the two types, taking their signess into
       * account \author Helfer Thomas \date   01 Sept 2006
       */
      template <typename A, typename B, bool A_is_signed, bool B_is_signed>
      struct ChooseType;

      /*!
       * \brief Partial specialisation.
       * \see ChooseType
       */
      template <typename A, typename B>
      struct ChooseType<A, B, true, true> {
        /*!
         * The result
         */
        typedef typename tfel::typetraits::internals::
            PromoteNumericsGreater_<A, B>::type type;
      };

      /*!
       * \brief Partial specialisation.
       * \see ChooseType
       */
      template <typename A, typename B>
      struct ChooseType<A, B, false, false> {
        /*!
         * The result
         */
        typedef typename tfel::typetraits::internals::
            PromoteNumericsGreater_<A, B>::type type;
      };

      /*!
       * \brief Partial specialisation.
       * \see ChooseType
       */
      template <typename A, typename B>
      struct ChooseType<A, B, true, false> {
        /*!
         * The result
         */
        typedef typename std::conditional<
            std::is_same<typename tfel::typetraits::internals::
                             PromoteNumericsGreater_<A, B>::type,
                         A>::value,
            A,
            typename ChooseType<typename Promote_<A>::type, B, true, false>::
                type>::type type;
      };

      /*!
       * \brief Partial specialisation.
       * \see ChooseType
       */
      template <typename A, typename B>
      struct ChooseType<A, B, false, true> {
        /*!
         * The result
         */
        typedef typename std::conditional<
            std::is_same<typename tfel::typetraits::internals::
                             PromoteNumericsGreater_<A, B>::type,
                         B>::value,
            B,
            typename ChooseType<A, typename Promote_<B>::type, false, true>::
                type>::type type;
      };

      TFEL_MATH_CHOOSETYPE(unsigned short);
      TFEL_MATH_CHOOSETYPE(unsigned int);
      TFEL_MATH_CHOOSETYPE(long unsigned int);
      TFEL_MATH_CHOOSETYPE(short);
      TFEL_MATH_CHOOSETYPE(int);
      TFEL_MATH_CHOOSETYPE(long int);
      TFEL_MATH_CHOOSETYPE(float);
      TFEL_MATH_CHOOSETYPE(double);
      TFEL_MATH_CHOOSETYPE(long double);

    }  // end of namespace internals

    /*!
     * \brief Partial specialisation: each type is promoted to itself
     * This is not necessary but may hopefully decrease compile time.
     */
    template <typename A>
    struct Promote<A, A> {
      /*!
       * The result
       */
      typedef A type;
    };

    template <typename A, typename B>
    struct Promote {
      /*!
       * The result
       */
      typedef typename tfel::typetraits::internals::ChooseType<
          A,
          B,
          std::numeric_limits<A>::is_signed,
          std::numeric_limits<B>::is_signed>::type type;
    };

    /*!
     * Specialisation for Complex type
     */
    template <typename A, typename B>
    struct Promote<tfel::math::Complex<A>, B> {
      /*!
       * The result
       */
      typedef tfel::math::Complex<typename Promote<A, B>::type> type;
    };

    /*!
     * Specialisation for Complex type
     */
    template <typename A, typename B>
    struct Promote<A, tfel::math::Complex<B>> {
      /*!
       * The result
       */
      typedef tfel::math::Complex<typename Promote<A, B>::type> type;
    };

    /*!
     * Specialisation for Complex type
     */
    template <typename A, typename B>
    struct Promote<tfel::math::Complex<A>, tfel::math::Complex<B>> {
      /*!
       * The result
       */
      typedef tfel::math::Complex<typename Promote<A, B>::type> type;
    };

    /*!
     * \brief Specialisation for Complex type.
     * This specialisation was necessary to disambiguate things.
     */
    template <typename A>
    struct Promote<tfel::math::Complex<A>, tfel::math::Complex<A>> {
      /*!
       * The result
       */
      typedef tfel::math::Complex<A> type;
    };

  }  // end of namespace typetraits

}  // end of namespace tfel

#endif /* LIB_TFEL_PROMOTE_IXX_ */
