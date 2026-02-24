/*!
 * \file   include/TFEL/Math/Tensor/TensorProduct.hxx
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

#ifndef LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX
#define LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX

#include <cmath>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel::math {

  template <TensorConcept A, TensorConcept B>
  struct TFEL_VISIBILITY_LOCAL TensorProductExprBase : public ExprBase {
    typedef EmptyRunTimeProperties RunTimeProperties;

    TFEL_HOST_DEVICE constexpr auto getRunTimeProperties() const noexcept {
      return EmptyRunTimeProperties();
    }

   protected:
    typedef BinaryOperationResult<A, B, OpMult> Result;
    typedef numeric_type<Result> NumType;
    typedef index_type<Result> IndexType;

    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;

    TensorProductExprBase() = delete;

    TFEL_HOST_DEVICE constexpr TensorProductExprBase(A l, B r) noexcept
        : a(std::forward<A>(l)), b(std::forward<B>(r)) {}

    ArgumentStorage<A> a;
    ArgumentStorage<B> b;
  };

  template <TensorConcept A, TensorConcept B>
  struct TFEL_VISIBILITY_LOCAL TensorProductExpr1D
      : public TensorProductExprBase<A, B> {
    static_assert(tfel::math::MathObjectTraits<std::decay_t<A>>::dime == 1u);
    static_assert(tfel::math::MathObjectTraits<std::decay_t<B>>::dime == 1u);

    TFEL_HOST_DEVICE constexpr TensorProductExpr1D(A l, B r) noexcept
        : TensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {}

    TFEL_HOST_DEVICE constexpr typename TensorProductExprBase<A, B>::NumType
    operator()(const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      return (this->a(i)) * (this->b(i));
    }  // end of operator()

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      return this->operator()(i);
    }  // end of operator[]
  };

  template <TensorConcept A, TensorConcept B>
  struct TFEL_VISIBILITY_LOCAL TensorProductExpr2D
      : public TensorProductExprBase<A, B> {
    static_assert(tfel::math::MathObjectTraits<std::decay_t<A>>::dime == 2u);
    static_assert(tfel::math::MathObjectTraits<std::decay_t<B>>::dime == 2u);

    TFEL_HOST_DEVICE constexpr TensorProductExpr2D(A l, B r) noexcept
        : TensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {}

    TFEL_HOST_DEVICE constexpr typename TensorProductExprBase<A, B>::NumType
    operator()(const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      typedef typename TensorProductExprBase<A, B>::NumType T;
      switch (i) {
        case 0:
          return (this->a(3)) * (this->b(4)) + (this->a(0)) * (this->b(0));
        case 1:
          return (this->a(4)) * (this->b(3)) + (this->a(1)) * (this->b(1));
        case 2:
          return (this->a(2)) * (this->b(2));
        case 3:
          return (this->a(0)) * (this->b(3)) + (this->a(3)) * (this->b(1));
        case 4:
          return (this->a(1)) * (this->b(4)) + (this->a(4)) * (this->b(0));
        default:
          break;
      }
      return T(0);
    }  // end of operator()

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      return this->operator()(i);
    }  // end of operator[]
  };

  template <TensorConcept A, TensorConcept B>
  struct TFEL_VISIBILITY_LOCAL TensorProductExpr3D
      : public TensorProductExprBase<A, B> {
    TFEL_HOST_DEVICE constexpr TensorProductExpr3D(A l, B r) noexcept
        : TensorProductExprBase<A, B>(std::forward<A>(l), std::forward<B>(r)) {}

    TFEL_HOST_DEVICE constexpr typename TensorProductExprBase<A, B>::NumType
    operator()(const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      using namespace std;
      typedef typename TensorProductExprBase<A, B>::NumType T;
      switch (i) {
        case 0: /* a5*b6+a3*b4+a0*b0 */
          return (this->a(5)) * (this->b(6)) + (this->a(3)) * (this->b(4)) +
                 (this->a(0)) * (this->b(0));
        case 1: /* a7*b8+a4*b3+a1*b1 */
          return (this->a(7)) * (this->b(8)) + (this->a(4)) * (this->b(3)) +
                 (this->a(1)) * (this->b(1));
        case 2: /* a8*b7+a6*b5+a2*b2 */
          return (this->a(8)) * (this->b(7)) + (this->a(6)) * (this->b(5)) +
                 (this->a(2)) * (this->b(2));
        case 3: /* a5*b8+a0*b3+a3*b1 */
          return (this->a(5)) * (this->b(8)) + (this->a(0)) * (this->b(3)) +
                 (this->a(3)) * (this->b(1));
        case 4: /* a7*b6+a1*b4+a4*b0 */
          return (this->a(7)) * (this->b(6)) + (this->a(1)) * (this->b(4)) +
                 (this->a(4)) * (this->b(0));
        case 5: /* a3*b7+a0*b5+a5*b2 */
          return (this->a(3)) * (this->b(7)) + (this->a(0)) * (this->b(5)) +
                 (this->a(5)) * (this->b(2));
        case 6: /* a2*b6+a8*b4+a6*b0 */
          return (this->a(2)) * (this->b(6)) + (this->a(8)) * (this->b(4)) +
                 (this->a(6)) * (this->b(0));
        case 7: /* a1*b7+a4*b5+a7*b2 */
          return (this->a(1)) * (this->b(7)) + (this->a(4)) * (this->b(5)) +
                 (this->a(7)) * (this->b(2));
        case 8: /* a2*b8+a6*b3+a8*b1 */
          return (this->a(2)) * (this->b(8)) + (this->a(6)) * (this->b(3)) +
                 (this->a(8)) * (this->b(1));
        default:
          break;
      }
      return T(0);
    }  // end of operator()

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorProductExprBase<A, B>::IndexType i)
        const noexcept {
      return this->operator()(i);
    }  // end of operator[]

   private:
    using ttypeA = std::decay_t<A>;
    using ttypeB = std::decay_t<A>;
    static_assert(tfel::math::getSpaceDimension<ttypeA>() == 3u);
    static_assert(tfel::math::getSpaceDimension<ttypeB>() == 3u);
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX */
