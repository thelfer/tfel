/*!
 * \file  include/TFEL/Math/Tensor/TensorViewFromStensor.hxx
 * \brief The TensorViewFromStensor describes how a stensor can be interpreted
 * as a tensor \author Thomas Helfer \brief 04 juil. 2013 \copyright Copyright
 * (C) 2006-2018 CEA/DEN, EDF R&D. All rights reserved. This project is publicly
 * released under either the GNU GPL Licence with linking exception or the
 * CECILL-A licence. A copy of thoses licences are delivered with the sources of
 * TFEL. CEA or EDF may also distribute this project under specific licensing
 * conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_HXX
#define LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel {

  namespace math {

    /*!
     * Tensor View From Stensor
     */
    template <typename T>
    struct TensorViewFromStensorExpr {
    };  // end of struct TensorViewFromStensorExpr

    template <unsigned short N, typename ValueType, typename T>
    struct Expr<tensor<N, ValueType>, TensorViewFromStensorExpr<T>>
        : public TensorConcept<
              Expr<tensor<N, ValueType>, TensorViewFromStensorExpr<T>>>,
          public ExprBase {
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      TFEL_MATH_INLINE Expr(T s_) : s(s_) {}  // end of Expr

      TFEL_MATH_INLINE Expr(const Expr&) = default;
      TFEL_MATH_INLINE Expr(Expr&&) = default;
      TFEL_MATH_INLINE Expr& operator=(const Expr&) = default;
      TFEL_MATH_INLINE Expr& operator=(Expr&&) = default;

      ValueType operator()(const unsigned short i) const {
        constexpr const auto icste = Cste<ValueType>::isqrt2;
        switch (i) {
          case 0:
            return this->s(0);
          case 1:
            return this->s(1);
          case 2:
            return this->s(2);
          case 3:
          case 4:
            if (traits::dime < 2) {
              throw(TensorInvalidIndexException());
            }
            return this->s(3) * icste;
          case 5:
          case 6:
            if (traits::dime < 3) {
              throw(TensorInvalidIndexException());
            }
            return this->s(4) * icste;
          case 7:
          case 8:
            if (traits::dime < 3) {
              throw(TensorInvalidIndexException());
            }
            return this->s(5) * icste;
        }
        throw(TensorInvalidIndexException());
      }  // end of operator() const

     protected:
      //! The stensor object
      ArgumentStorage<T> s;

     private:
      using stype = typename std::decay<T>::type;
      using traits = StensorTraits<stype>;

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      TFEL_STATIC_ASSERT((traits::dime == N));
      TFEL_STATIC_ASSERT(
          (std::is_same<ValueType, typename traits::NumType>::value));
    };  // end of struct Expr

    template <typename T>
    struct TensorViewFromStensor {
      using stype = typename std::decay<T>::type;
      using traits = StensorTraits<stype>;
      static const unsigned short N = traits::dime;
      typedef Expr<tensor<N, typename traits::NumType>,
                   TensorViewFromStensorExpr<T>>
          type;
    };  // end of struct TensorViewFromStensor

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_HXX */
