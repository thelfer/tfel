/*!
 * \file   cadna.hxx
 * \brief
 * \author Thomas Helfer
 * \date   29 sept. 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CADNA_HXX
#define LIB_TFEL_MATH_CADNA_HXX

#include "cadna/numeric_type.hxx"

#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/TypeTraits/IsReal.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/IsAssignableTo.hxx"
#include "TFEL/TypeTraits/IsFundamentalNumericType.hxx"
#include "TFEL/TypeTraits/BaseType.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/General/UnaryResultType.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/power.hxx"

namespace tfel::math {

  namespace internals {

    struct InvalidResult {
      using type = tfel::meta::InvalidType;
    };

    template <typename T1, typename T2, typename Op>
    struct ResultType1 : public InvalidResult {};
    template <typename T1, typename T2, typename Op>
    struct ResultType2 : public InvalidResult {};
    template <typename T1, typename T2, typename Op>
    struct ResultType3 : public InvalidResult {};

    template <typename T1, typename T2>
    struct ResultType1<T1, T2, tfel::math::OpPlus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType1<T1, T2, tfel::math::OpMinus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType1<T1, T2, tfel::math::OpMult> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType1<T1, T2, tfel::math::OpDiv> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };

    template <typename T1, typename T2>
    struct ResultType2<T1, T2, tfel::math::OpPlus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType2<T1, T2, tfel::math::OpMinus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType2<T1, T2, tfel::math::OpMult> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType2<T1, T2, tfel::math::OpDiv> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };

    template <typename T1, typename T2>
    struct ResultType3<T1, T2, tfel::math::OpPlus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType3<T1, T2, tfel::math::OpMinus> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType3<T1, T2, tfel::math::OpMult> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };
    template <typename T1, typename T2>
    struct ResultType3<T1, T2, tfel::math::OpDiv> {
      using type = cadna::numeric_type<cadna::promote_t<T1, T2>>;
    };

  }  // end of namespace internals

  template <typename T1, typename T2, typename Op>
  struct ResultType<cadna::numeric_type<T1>, cadna::numeric_type<T2>, Op> {
    static constexpr bool b = (std::is_floating_point<T1>::value &&
                               std::is_floating_point<T2>::value);
    using dispatch =
        typename std::conditional<b,
                                  internals::ResultType1<T1, T2, Op>,
                                  internals::InvalidResult>::type;
    using type = typename dispatch::type;
  };

  template <typename T1, typename T2, typename Op>
  struct ResultType<cadna::numeric_type<T1>, T2, Op> {
    static constexpr bool b = (std::is_floating_point<T1>::value &&
                               cadna::is_cxx_fundamental_type<T2>::value);
    using dispatch =
        typename std::conditional<b,
                                  internals::ResultType2<T1, T2, Op>,
                                  internals::InvalidResult>::type;
    using type = typename dispatch::type;
  };

  template <typename T1, typename T2, typename Op>
  struct ResultType<T1, cadna::numeric_type<T2>, Op> {
    static constexpr bool b = (cadna::is_cxx_fundamental_type<T1>::value &&
                               std::is_floating_point<T2>::value);
    using dispatch =
        typename std::conditional<b,
                                  internals::ResultType3<T1, T2, Op>,
                                  internals::InvalidResult>::type;
    using type = typename dispatch::type;
  };

  template <typename T, int N, unsigned int D>
  struct UnaryResultType<cadna::numeric_type<T>, Power<N, D>> {
    using type =
        cadna::numeric_type<typename UnaryResultType<T, Power<N, D>>::type>;
  };

}  // namespace tfel::math

namespace tfel::typetraits {

  namespace internals {
    template <typename T1, typename T2>
    struct IsAssignableTo {
      static constexpr bool value =
          std::is_same<typename tfel::typetraits::Promote<T1, T2>::type,
                       T2>::value;
    };
  }  // namespace internals

  template <typename T1>
  struct Promote<cadna::numeric_type<T1>, cadna::numeric_type<T1>> {
    using type = cadna::numeric_type<T1>;
  };

  template <typename T1, typename T2>
  struct Promote<cadna::numeric_type<T1>, cadna::numeric_type<T2>> {
    using type = cadna::numeric_type<typename Promote<T1, T2>::type>;
  };

  template <typename T1, typename T2>
  struct Promote<cadna::numeric_type<T1>, T2> {
    using type = cadna::numeric_type<typename Promote<T1, T2>::type>;
  };

  template <typename T1, typename T2>
  struct Promote<T1, cadna::numeric_type<T2>> {
    using type = cadna::numeric_type<typename Promote<T1, T2>::type>;
  };

  template <typename T>
  struct IsScalar<cadna::numeric_type<T>> {
    static const bool cond = std::is_floating_point<T>::value;
  };
  template <typename T>
  struct IsReal<cadna::numeric_type<T>> {
    static const bool cond = std::is_floating_point<T>::value;
  };
  template <typename T>
  struct IsComplex<cadna::numeric_type<T>> {
    static const bool cond = false;
  };
  template <typename T1, typename T2>
  struct IsAssignableTo<T1, cadna::numeric_type<T2>> {
    static constexpr bool b = (cadna::is_cxx_fundamental_type<T1>::value &&
                               std::is_floating_point<T2>::value);
    using dispatch =
        typename std::conditional<b,
                                  internals::IsAssignableTo<T1, T2>,
                                  std::false_type>::type;
    static const bool cond = dispatch::value;
  };
  template <typename T1, typename T2>
  struct IsAssignableTo<cadna::numeric_type<T1>, cadna::numeric_type<T2>> {
    static constexpr bool b = (std::is_floating_point<T1>::value &&
                               std::is_floating_point<T2>::value);
    using dispatch =
        typename std::conditional<b,
                                  internals::IsAssignableTo<T1, T2>,
                                  std::false_type>::type;
    static const bool cond = dispatch::value;
  };
  template <typename T>
  struct IsFundamentalNumericType<cadna::numeric_type<T>> {
    static const constexpr bool cond = std::is_floating_point<T>::value;
  };
  template <typename T>
  struct BaseType<cadna::numeric_type<T>> {
    using type = cadna::numeric_type<T>;
  };

}  // end of namespace tfel::typetraits

#endif /* LIB_TFEL_MATH_CADNA_HXX */
