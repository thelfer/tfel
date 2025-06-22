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

#ifndef LIB_STENSORPRODUCTEXPR_HXX
#define LIB_STENSORPRODUCTEXPR_HXX

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathConstants.hxx"
#include "TFEL/Math/General/ConstExprMathFunctions.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel {

  namespace math {

    template <typename A, typename B>
    class TFEL_VISIBILITY_LOCAL StensorProductExprBase : public ExprBase {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<A>::type,
                                                 StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<B>::type,
                                                 StensorConcept>::cond));

      typedef typename ComputeBinaryResult<typename std::decay<A>::type,
                                           typename std::decay<B>::type,
                                           OpMult>::Result Result;

     public:
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename TensorTraits<Result>::IndexType IndexType;
      typedef typename TensorTraits<Result>::NumType NumType;
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
      using size_type = typename StensorProductExprBase<A, B>::size_type;

      TFEL_MATH_INLINE StensorProductExpr1D(A l, B r)
          : StensorProductExprBase<A, B>(std::forward<A>(l),
                                         std::forward<B>(r)) {}

      TFEL_MATH_INLINE
      BinaryOperationResult<typename tfel::meta::ResultOf<A, size_type>::type,
                            typename tfel::meta::ResultOf<B, size_type>::type,
                            OpMult>
      operator()(const size_type i) const {
        return (this->a(i)) * (this->b(i));
      }  // end of operator()

     private:
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<A>::type>::dime == 1u));
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<B>::type>::dime == 1u));
    };

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL StensorProductExpr2D
        : public StensorProductExprBase<A, B> {
      using size_type = typename StensorProductExprBase<A, B>::size_type;

      TFEL_MATH_INLINE StensorProductExpr2D(A l, B r)
          : StensorProductExprBase<A, B>(std::forward<A>(l),
                                         std::forward<B>(r)) {}

      TFEL_MATH_INLINE
      BinaryOperationResult<typename tfel::meta::ResultOf<A, size_type>::type,
                            typename tfel::meta::ResultOf<B, size_type>::type,
                            OpMult>
      operator()(const size_type i) const {
        typedef typename StensorProductExprBase<A, B>::NumType T;
        constexpr const auto icste = Cste<T>::isqrt2;
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

     private:
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<A>::type>::dime == 2u));
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<B>::type>::dime == 2u));
    };

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL StensorProductExpr3D
        : public StensorProductExprBase<A, B> {
      using size_type = typename StensorProductExprBase<A, B>::size_type;

      TFEL_MATH_INLINE StensorProductExpr3D(A l, B r)
          : StensorProductExprBase<A, B>(std::forward<A>(l),
                                         std::forward<B>(r)) {}

      TFEL_MATH_INLINE
      BinaryOperationResult<typename tfel::meta::ResultOf<A, size_type>::type,
                            typename tfel::meta::ResultOf<B, size_type>::type,
                            OpMult>
      operator()(const size_type i) const {
        typedef typename StensorProductExprBase<A, B>::NumType T;
        constexpr const auto icste = Cste<T>::isqrt2;
        switch (i) {
          case 0:
            return (this->a(4) * this->b(4)) / 2 +
                   (this->a(3) * this->b(3)) / 2 + this->a(0) * this->b(0);
          case 1:
            return (this->a(5) * this->b(5)) / 2 +
                   (this->a(3) * this->b(3)) / 2 + this->a(1) * this->b(1);
          case 2:
            return (this->a(5) * this->b(5)) / 2 +
                   (this->a(4) * this->b(4)) / 2 + this->a(2) * this->b(2);
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

     private:
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<A>::type>::dime == 3u));
      TFEL_STATIC_ASSERT((
          tfel::math::StensorTraits<typename std::decay<B>::type>::dime == 3u));
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_STENSORPRODUCTEXPR_HXX */
