/*!
 * \file   include/TFEL/Math/General/BasicOperations.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_BASICOPERATIONS_LIB_
#define TFEL_MATH_BASICOPERATIONS_LIB_ 1

#include <functional>
#include <type_traits>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/TypeTraits/IsScalar.hxx"
#include "TFEL/TypeTraits/IsComplex.hxx"
#include "TFEL/TypeTraits/IsUnaryOperator.hxx"
#include "TFEL/TypeTraits/Promote.hxx"
#include "TFEL/Math/Forward/Complex.hxx"
#include "TFEL/Math/General/ComputeBinaryResult.hxx"
#include "TFEL/Math/General/ComputeUnaryResult.hxx"

#define TFEL_MATH_RESULT_TYPE_(X, Y)                        \
  /*!                                                       \
   * \brief Partial specialisation for X and Y              \
   */                                                       \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpPlus> {  \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
  /*!                                                       \
   * \brief Partial specialisation for X and Y              \
   */                                                       \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpMinus> { \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
                                                            \
  /*!                                                       \
   * \brief Partial specialisation for X and Y              \
   */                                                       \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpMult> {  \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
                                                            \
  /*!                                                       \
   * \brief Partial specialisation for X and Y              \
   */                                                       \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpDiv> {   \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  }

#define TFEL_MATH_RESULT_TYPE(X)                                              \
  TFEL_MATH_RESULT_TYPE_(X, unsigned short);                                  \
  TFEL_MATH_RESULT_TYPE_(X, unsigned int);                                    \
  TFEL_MATH_RESULT_TYPE_(X, long unsigned int);                               \
  TFEL_MATH_RESULT_TYPE_(X, short);                                           \
  TFEL_MATH_RESULT_TYPE_(X, int);                                             \
  TFEL_MATH_RESULT_TYPE_(X, long int);                                        \
  TFEL_MATH_RESULT_TYPE_(X, float);                                           \
  TFEL_MATH_RESULT_TYPE_(X, double);                                          \
  TFEL_MATH_RESULT_TYPE_(X, long double);                                     \
  /*!                                                                         \
   * \brief Partial specialisation for X                                      \
   * \see   UnaryResultType                                                   \
   */                                                                         \
  template <>                                                                 \
  struct UnaryResultType_<ScalarTag, UnaryOperatorTag, X, OpNeg> {            \
    typedef tfel::typetraits::Promote<X, short>::type type;                   \
  };                                                                          \
  /*!                                                                         \
   * \brief Partial specialisation for X                                      \
   * \see   UnaryResultType                                                   \
   */                                                                         \
  template <>                                                                 \
  struct UnaryResultType_<ScalarTag, UnaryOperatorTag, Complex<X>, OpNeg> {   \
    typedef tfel::typetraits::Promote<Complex<X>, Complex<short>>::type type; \
  }

#define TFEL_MATH_RESULT_TYPE_COMPLEX_(X, Y)                \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpPlus> {  \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
                                                            \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpMinus> { \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
                                                            \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpMult> {  \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  };                                                        \
                                                            \
  template <>                                               \
  struct ResultType_<ScalarTag, ScalarTag, X, Y, OpDiv> {   \
    typedef tfel::typetraits::Promote<X, Y>::type type;     \
  }

#define TFEL_MATH_RESULT_TYPE_COMPLEX(X) \
  TFEL_MATH_RESULT_TYPE_(X, Complex<X>); \
  TFEL_MATH_RESULT_TYPE_(Complex<X>, X); \
  TFEL_MATH_RESULT_TYPE_(Complex<X>, Complex<X>)

namespace tfel {

  namespace math {

    struct OpPlus {
      /*
       * Add its two arguments.
       * \see ComputeBinaryResult.
       */
      template <typename T1, typename T2>
      static TFEL_MATH_INLINE auto apply(T1&& a, T2&& b)
          -> decltype(std::forward<T1>(a) + std::forward<T2>(b)) {
        return std::forward<T1>(a) + std::forward<T2>(b);
      }
    };

    struct OpMinus {
      /*
       * Substract its two arguments.
       * \see ComputeBinaryResult.
       */
      template <typename T1, typename T2>
      static TFEL_MATH_INLINE auto apply(T1&& a, T2&& b)
          -> decltype(std::forward<T1>(a) - std::forward<T2>(b)) {
        return std::forward<T1>(a) - std::forward<T2>(b);
      }

    };  // end of OpMinus

    struct OpMult {
      /*
       * Multiply its two arguments.
       * \see ComputeBinaryResult
       */
      template <typename T1, typename T2>
      static TFEL_MATH_INLINE auto apply(T1&& a, T2&& b)
          -> decltype(std::forward<T1>(a) * std::forward<T2>(b)) {
        return std::forward<T1>(a) * std::forward<T2>(b);
      }
    };  // end of OpMult

    struct OpDiv {
      /*!
       * Divide the two arguments
       * \see ComputeBinaryResult.
       */
      template <typename T1, typename T2>
      static TFEL_MATH_INLINE auto apply(T1&& a, T2&& b)
          -> decltype(std::forward<T1>(a) / std::forward<T2>(b)) {
        return std::forward<T1>(a) / std::forward<T2>(b);
      }
    };  // end of OpDiv

    struct OpNeg {
      template <typename T1>
      static TFEL_MATH_INLINE auto apply(T1&& a)
          -> decltype(-std::forward<T1>(a)) {
        return -std::forward<T1>(a);
      }
    };

    struct OpDotProduct {};  // end of struct OpDotProduct

    struct OpDiadicProduct {};

  }  // end of namespace math

  namespace typetraits {

    /*
     * Partial Specialisation for OpNeg
     */
    template <>
    struct IsUnaryOperator<tfel::math::OpNeg> {
      static constexpr bool cond = true;
    };

  }  // end of namespace typetraits

  namespace math {

    TFEL_MATH_RESULT_TYPE(unsigned short);
    TFEL_MATH_RESULT_TYPE(unsigned int);
    TFEL_MATH_RESULT_TYPE(long unsigned int);
    TFEL_MATH_RESULT_TYPE(short);
    TFEL_MATH_RESULT_TYPE(int);
    TFEL_MATH_RESULT_TYPE(long int);
    TFEL_MATH_RESULT_TYPE(float);
    TFEL_MATH_RESULT_TYPE(double);
    TFEL_MATH_RESULT_TYPE(long double);

    template <typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond &&
                                    (!tfel::typetraits::IsComplex<T>::cond),
                                T>::type
        conj(const T src) {
      return src;
    }

    template <typename T>
    TFEL_MATH_INLINE
        typename std::enable_if<tfel::typetraits::IsScalar<T>::cond &&
                                    (!tfel::typetraits::IsComplex<T>::cond),
                                T>::type
        real(const T src) {
      return src;
    }

    TFEL_MATH_RESULT_TYPE_COMPLEX(unsigned short);
    TFEL_MATH_RESULT_TYPE_COMPLEX(unsigned int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long unsigned int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(short);
    TFEL_MATH_RESULT_TYPE_COMPLEX(int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long int);
    TFEL_MATH_RESULT_TYPE_COMPLEX(float);
    TFEL_MATH_RESULT_TYPE_COMPLEX(double);
    TFEL_MATH_RESULT_TYPE_COMPLEX(long double);

  }  // end of namespace math

}  // end of namespace tfel

#endif /* TFEL_MATH_BASICOPERATIONS_LIB_ */
