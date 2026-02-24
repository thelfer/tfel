/*!
 * \file   include/TFEL/Math/Stensor/StensorProduct.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   07 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_STENSORPRODUCTEXPR_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  template <StensorConcept A, StensorConcept B>
  class TFEL_VISIBILITY_LOCAL StensorProductExprBase : public ExprBase {
    typedef typename ComputeBinaryResult<std::decay_t<A>,
                                         std::decay_t<B>,
                                         OpMult>::Result Result;

   public:
    typedef EmptyRunTimeProperties RunTimeProperties;
    typedef index_type<Result> IndexType;
    typedef numeric_type<Result> NumType;
    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;

   protected:
    TFEL_MATH_INLINE StensorProductExprBase(A l, B r) : a(l), b(r) {}

    StensorProductExprBase() = delete;

    ArgumentStorage<A> a;
    ArgumentStorage<B> b;

   public:
    TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
      return EmptyRunTimeProperties();
    }
  };

  template <typename A, typename B>
  struct TFEL_VISIBILITY_LOCAL StensorProductExpr1D
      : public StensorProductExprBase<A, B> {
    static_assert(getSpaceDimension<A>() == 1u);
    static_assert(getSpaceDimension<B>() == 1u);

    using size_type = typename StensorProductExprBase<A, B>::size_type;

    TFEL_MATH_INLINE StensorProductExpr1D(A l, B r)
        : StensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {
    }

    TFEL_MATH_INLINE
    BinaryOperationResult<std::invoke_result_t<A, size_type>,
                          std::invoke_result_t<B, size_type>,
                          OpMult>
    operator()(const size_type i) const {
      return (this->a(i)) * (this->b(i));
    }  // end of operator()
  };

  template <typename A, typename B>
  struct TFEL_VISIBILITY_LOCAL StensorProductExpr2D
      : public StensorProductExprBase<A, B> {
    static_assert(getSpaceDimension<A>() == 2u);
    static_assert(getSpaceDimension<B>() == 2u);

    using size_type = typename StensorProductExprBase<A, B>::size_type;

    TFEL_MATH_INLINE StensorProductExpr2D(A l, B r)
        : StensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {
    }

    TFEL_MATH_INLINE
    BinaryOperationResult<std::invoke_result_t<A, size_type>,
                          std::invoke_result_t<B, size_type>,
                          OpMult>
    operator()(const size_type i) const {
      typedef typename StensorProductExprBase<A, B>::NumType T;
      constexpr auto icste = Cste<T>::isqrt2;
      switch (i) {
        case 0:
          return (this->a(3) * this->b(3)) / 2 + this->a(0) * this->b(0);
        case 1:
          return (this->a(3) * this->b(3)) / 2 + this->a(1) * this->b(1);
        case 2:
          return this->a(2) * this->b(2);
        case 3:
          return ((this->a(0) * this->b(3)) + (this->a(3) * this->b(1))) *
                 icste;
        case 4:
          return ((this->a(1) * this->b(3)) + (this->a(3) * this->b(0))) *
                 icste;
      }
      return T(0);
    }  // end of operator()
  };

  template <typename A, typename B>
  struct TFEL_VISIBILITY_LOCAL StensorProductExpr3D
      : public StensorProductExprBase<A, B> {
    static_assert(getSpaceDimension<A>() == 3u);
    static_assert(getSpaceDimension<B>() == 3u);

    using size_type = typename StensorProductExprBase<A, B>::size_type;

    TFEL_MATH_INLINE StensorProductExpr3D(A l, B r)
        : StensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {
    }

    TFEL_MATH_INLINE
    BinaryOperationResult<std::invoke_result_t<A, size_type>,
                          std::invoke_result_t<B, size_type>,
                          OpMult>
    operator()(const size_type i) const {
      typedef typename StensorProductExprBase<A, B>::NumType T;
      constexpr auto icste = Cste<T>::isqrt2;
      switch (i) {
        case 0:
          return (this->a(4) * this->b(4)) / 2 + (this->a(3) * this->b(3)) / 2 +
                 this->a(0) * this->b(0);
        case 1:
          return (this->a(5) * this->b(5)) / 2 + (this->a(3) * this->b(3)) / 2 +
                 this->a(1) * this->b(1);
        case 2:
          return (this->a(5) * this->b(5)) / 2 + (this->a(4) * this->b(4)) / 2 +
                 this->a(2) * this->b(2);
        case 3:
          return (this->a(4) * this->b(5)) / 2 +
                 (this->a(0) * this->b(3) + this->a(3) * this->b(1)) * icste;
        case 4:
          return (this->a(5) * this->b(4)) / 2 +
                 (this->a(1) * this->b(3) + this->a(3) * this->b(0)) * icste;
        case 5:
          return (this->a(3) * this->b(5)) / 2 +
                 (this->a(0) * this->b(4) + this->a(4) * this->b(2)) * icste;
        case 6:
          return (this->a(5) * this->b(3)) / 2 +
                 (this->a(2) * this->b(4) + this->a(4) * this->b(0)) * icste;
        case 7:
          return (this->a(3) * this->b(4)) / 2 +
                 (this->a(1) * this->b(5) + this->a(5) * this->b(2)) * icste;
        case 8:
          return (this->a(4) * this->b(3)) / 2 +
                 (this->a(2) * this->b(5) + this->a(5) * this->b(1)) * icste;
      }
      return T(0);
    }  // end of operator()
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_STENSORPRODUCTEXPR_HXX */
