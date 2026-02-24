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

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/Forward/tensor.hxx"

namespace tfel::math {

  //! \brief tensor view from a symmetric tensor
  template <StensorConcept>
  struct TensorViewFromStensorExpr {
  };  // end of struct TensorViewFromStensorExpr

  template <unsigned short N, typename ValueType, StensorConcept T>
  struct Expr<tensor<N, ValueType>, TensorViewFromStensorExpr<T>>
      : public TensorConceptBase<
            Expr<tensor<N, ValueType>, TensorViewFromStensorExpr<T>>>,
        public ExprBase {
    typedef EmptyRunTimeProperties RunTimeProperties;

    constexpr auto getRunTimeProperties() const noexcept {
      return RunTimeProperties();
    }

    TFEL_HOST_DEVICE constexpr Expr(T s_) noexcept : s(s_) {}  // end of Expr

    TFEL_HOST_DEVICE constexpr Expr(const Expr&) = default;
    TFEL_HOST_DEVICE constexpr Expr(Expr&&) = default;
    TFEL_HOST_DEVICE constexpr Expr& operator=(const Expr&) = default;
    TFEL_HOST_DEVICE constexpr Expr& operator=(Expr&&) = default;

    TFEL_HOST ValueType operator[](const unsigned short i) const {
      return this->operator()(i);
    }

    TFEL_HOST ValueType operator()(const unsigned short i) const {
      constexpr auto icste = Cste<ValueType>::isqrt2;
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
        default:
          break;
      }
      throw(TensorInvalidIndexException());
    }  // end of operator() const

   protected:
    //! The stensor object
    ArgumentStorage<T> s;

   private:
    using stype = std::decay_t<T>;
    using traits = MathObjectTraits<stype>;

    static_assert((N == 1u) || (N == 2u) || (N == 3u));
    static_assert(traits::dime == N);
    static_assert(std::is_same<ValueType, typename traits::NumType>::value);
  };  // end of struct Expr

  template <typename T>
  struct TensorViewFromStensor {
    using stype = std::decay_t<T>;
    using traits = MathObjectTraits<stype>;
    static const unsigned short N = traits::dime;
    typedef Expr<tensor<N, typename traits::NumType>,
                 TensorViewFromStensorExpr<T>>
        type;
  };  // end of struct TensorViewFromStensor

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_HXX */
