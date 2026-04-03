/*!
 *\file   Promote.ixx
 *\brief  This file implement the Promote class.
 *\author Thomas Helfer
 *\date   02 jun 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 *linking exception or the CECILL-A licence. A copy of thoses licences are
 *delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_PROMOTEIXX
#define LIB_TFEL_PROMOTEIXX

#include <type_traits>

/*!
 * \def    TFEL_MATH_CHOOSETYPE
 * \brief  An helper macro to specialise ChooseType.
 * \author Thomas Helfer
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
     * \brief the result                           \
     */                                            \
    using type = long double;                      \
  };                                               \
                                                   \
  /*!                                              \
   * \brief Partial specialisation for X           \
   * \see   ChooseType                             \
   */                                              \
  template <>                                      \
  struct ChooseType<X, long double, false, true> { \
    /*!                                            \
     * \brief the result                           \
     */                                            \
    using type = long double;                      \
  }

namespace tfel::typetraits {

  namespace internals {

    /*!
     * \class  PromoteNumericsGreater_
     * \brief  An helper class which return the greatest type, ie the type
     * which has the maximal positive value. \param  A, first  type \param  B,
     * second type \return type, the result. \pre    std::numeric_limits<A>
     * and std::numeric_limits<B> must be defined \author Thomas Helfer \date
     * 02 jun 2006
     */
    template <typename A, typename B>
    struct PromoteNumericsGreater_ {
      // requires std::numeric_limits<A> to be defined
      static_assert(std::numeric_limits<A>::is_specialized);
      // requires std::numeric_limits<B> to be defined
      static_assert(std::numeric_limits<B>::is_specialized);
      //! \brief the result
      using type =
          std::conditional_t<((std::numeric_limits<A>::max_exponent10 <
                               std::numeric_limits<B>::max_exponent10) ||
                              ((std::numeric_limits<A>::max_exponent10 ==
                                std::numeric_limits<B>::max_exponent10) &&
                               (std::numeric_limits<A>::digits <=
                                std::numeric_limits<B>::digits))),
                             B,
                             A>;
    };

    /*!
     * \brief Partial specialisation for unsigned int and long unsigned int.
     * They can be the same type on some plateform, so the algorithm used in
     * PromoteNumericsGreater_ can't tell which is the greatest one. \see
     * PromoteNumericsGreater_
     */
    template <>
    struct PromoteNumericsGreater_<unsigned int, long unsigned int> {
      //! \brief the result
      using type = long unsigned int;
    };

    /*!
     * \brief Partial specialisation for long unsigned int and unsigned int.
     * They can be the same type on some plateform, so the algorithm used in
     * PromoteNumericsGreater_ can't tell which is the greatest one. \see
     * PromoteNumericsGreater_
     */
    template <>
    struct PromoteNumericsGreater_<long unsigned int, unsigned int> {
      //! \brief the result
      using type = long unsigned int;
    };

    /*!
     * \brief Partial specialisation for int and long int.
     * They can be the same type on some plateform, so the algorithm used in
     * PromoteNumericsGreater_ can't tell which is the greatest one. \see
     * PromoteNumericsGreater_
     */
    template <>
    struct PromoteNumericsGreater_<int, long int> {
      //! \brief the result
      using type = long int;
    };

    /*!
     * \brief Partial specialisation for long int and int.
     * They can be the same type on some plateform, so the algorithm used in
     * PromoteNumericsGreater_ can't tell which is the greatest one. \see
     * PromoteNumericsGreater_
     */
    template <>
    struct PromoteNumericsGreater_<long int, int> {
      //! \brief the result
      using type = long int;
    };

    /*!
     * \brief  An helper class which associates to a type a type that is
     * greater. \param  T, a type \return type, the result \author Thomas
     * Helfer \date   02 jun 2006
     */
    template <typename>
    struct Promote_;

    /*!
     * \brief Partial specialisation for short.
     * \see Promote_
     */
    template <>
    struct Promote_<short> {
      // Check if int is greater than short (!)
      static_assert(
          std::is_same_v<PromoteNumericsGreater_<short, int>::type, int>);
      //! \brief the result
      using type = int;
    };

    /*!
     * \brief Partial specialisation for unsigned short.
     * \see Promote_
     */
    template <>
    struct Promote_<unsigned short> {
      // check if unsigned int is greater than unsigned short (!)
      static_assert(std::is_same_v<
                    PromoteNumericsGreater_<unsigned short, unsigned int>::type,
                    unsigned int>);
      //! \brief the result
      using type = unsigned int;
    };

    /*!
     * \brief Partial specialisation for int.
     * \see Promote_
     */
    template <>
    struct Promote_<int> {
      // check if long int is greater than int (!)
      static_assert(std::is_same_v<PromoteNumericsGreater_<int, long int>::type,
                                   long int>);
      //! \brief result
      using type = long int;
    };

    /*!
     * \brief Partial specialisation for unsigned int.
     * \see Promote_
     */
    template <>
    struct Promote_<unsigned int> {
      // check if long unsigned int is greater than unsigned int (!)
      static_assert(
          std::is_same_v<
              PromoteNumericsGreater_<unsigned int, long unsigned int>::type,
              long unsigned int>);
      //! \brief result
      using type = long unsigned int;
    };

    /*!
     * \brief Partial specialisation for long int.
     * \see Promote_
     */
    template <>
    struct Promote_<long int> {
      /*
       * Check if float is greater than long int
       * \warning this was false under cygwin
       */
      static_assert(
          std::is_same_v<PromoteNumericsGreater_<long int, float>::type,
                         float>);
      //! \brief result
      using type = float;
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
      static_assert(std::is_same_v<
                    PromoteNumericsGreater_<long unsigned int, float>::type,
                    float>);
      //! \brief result
      using type = float;
    };

    /*!
     * \brief Partial specialisation for float
     * \see Promote_
     */
    template <>
    struct Promote_<float> {
      // check if double is greater than float (!)
      static_assert(
          std::is_same_v<PromoteNumericsGreater_<float, double>::type, double>);
      //! \brief result
      using type = double;
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
      static_assert(
          std::is_same_v<PromoteNumericsGreater_<double, long double>::type,
                         long double>);
      //! \brief result
      using type = long double;
    };

    /*!
     * \brief Partial specialisation for long double.
     * long double is supposed to be the greatest numerical type.
     * \see Promote_
     */
    template <>
    struct Promote_<long double> {
      //! \brief result
      using type = long double;
    };

    /*!
     * \class  ChooseType
     * \brief  Choose the largest of the two types, taking their signess into
     * account \author Thomas Helfer \date   01 Sept 2006
     */
    template <typename A, typename B, bool A_is_signed, bool B_is_signed>
    struct ChooseType;

    /*!
     * \brief Partial specialisation.
     * \see ChooseType
     */
    template <typename A, typename B>
    struct ChooseType<A, B, true, true> {
      //! \brief result
      using type = typename tfel::typetraits::internals::
          PromoteNumericsGreater_<A, B>::type;
    };

    /*!
     * \brief Partial specialisation.
     * \see ChooseType
     */
    template <typename A, typename B>
    struct ChooseType<A, B, false, false> {
      //! \brief result
      using type = typename tfel::typetraits::internals::
          PromoteNumericsGreater_<A, B>::type;
    };

    /*!
     * \brief Partial specialisation.
     * \see ChooseType
     */
    template <typename A, typename B>
    struct ChooseType<A, B, true, false> {
      //! \brief result
      using type = std::conditional_t<
          std::is_same_v<typename tfel::typetraits::internals::
                             PromoteNumericsGreater_<A, B>::type,
                         A>,
          A,
          typename ChooseType<typename Promote_<A>::type, B, true, false>::
              type>;
    };

    /*!
     * \brief Partial specialisation.
     * \see ChooseType
     */
    template <typename A, typename B>
    struct ChooseType<A, B, false, true> {
      //! \brief result
      using type = std::conditional_t<
          std::is_same_v<typename tfel::typetraits::internals::
                             PromoteNumericsGreater_<A, B>::type,
                         B>,
          B,
          typename ChooseType<A, typename Promote_<B>::type, false, true>::
              type>;
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
    //! \brief result
    using type = A;
  };

  template <typename A, typename B>
  struct Promote {
    //! \brief result
    using type = typename tfel::typetraits::internals::ChooseType<
        A,
        B,
        std::numeric_limits<A>::is_signed,
        std::numeric_limits<B>::is_signed>::type;
  };

  /*!
   * Specialisation for Complex type
   */
  template <typename A, typename B>
  struct Promote<tfel::math::Complex<A>, B> {
    //! \brief result
    using type = tfel::math::Complex<typename Promote<A, B>::type>;
  };

  /*!
   * Specialisation for Complex type
   */
  template <typename A, typename B>
  struct Promote<A, tfel::math::Complex<B>> {
    //! \brief result
    using type = tfel::math::Complex<typename Promote<A, B>::type>;
  };

  /*!
   * Specialisation for Complex type
   */
  template <typename A, typename B>
  struct Promote<tfel::math::Complex<A>, tfel::math::Complex<B>> {
    //! \brief result
    using type = tfel::math::Complex<typename Promote<A, B>::type>;
  };

  /*!
   * \brief Specialisation for Complex type.
   * This specialisation was necessary to disambiguate things.
   */
  template <typename A>
  struct Promote<tfel::math::Complex<A>, tfel::math::Complex<A>> {
    //! \brief result
    using type = tfel::math::Complex<A>;
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_PROMOTEIXX */
