/*!
 * \file   include/TFEL/Math/Tensor/TensorTransposeExpr.hxx
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

#ifndef LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX
#define LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX

#include <cmath>
#include <cstddef>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel::math {

  template <TensorConcept A>
  struct TFEL_VISIBILITY_LOCAL TensorTransposeExprBase : public ExprBase {
    typedef EmptyRunTimeProperties RunTimeProperties;
    using IndexType = index_type<A>;
    using NumType = numeric_type<A>;

    TFEL_HOST_DEVICE constexpr RunTimeProperties getRunTimeProperties()
        const noexcept {
      return EmptyRunTimeProperties();
    }

   protected:
    typedef NumType value_type;
    typedef NumType* pointer;
    typedef const NumType* const_pointer;
    typedef NumType& reference;
    typedef const NumType& const_reference;
    typedef IndexType size_type;
    typedef ptrdiff_t difference_type;

    TFEL_HOST_DEVICE constexpr TensorTransposeExprBase(A l) : a(l) {}

    ArgumentStorage<A> a;
  };

  template <typename A>
  struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr1D
      : public TensorTransposeExprBase<A> {
    static_assert(getSpaceDimension<A>() == 1u);

    TFEL_HOST_DEVICE constexpr TensorTransposeExpr1D(A l) noexcept
        : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      return this->operator()(i);
    }  // end of operator[]

    TFEL_HOST_DEVICE constexpr typename TensorTransposeExprBase<A>::NumType
    operator()(
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      return this->a(i);
    }  // end of operator()
  };

  template <typename A>
  struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr2D
      : public TensorTransposeExprBase<A> {
    static_assert(getSpaceDimension<A>() == 2u);

    TFEL_HOST_DEVICE constexpr TensorTransposeExpr2D(A l) noexcept
        : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      return this->operator()(i);
    }  // end of operator[]

    TFEL_HOST_DEVICE constexpr typename TensorTransposeExprBase<A>::NumType
    operator()(
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      constexpr typename TensorTransposeExprBase<A>::NumType zero{0};
      switch (i) {
        case 0:
          return this->a(0);
          break;
        case 1:
          return this->a(1);
          break;
        case 2:
          return this->a(2);
          break;
        case 3:
          return this->a(4);
          break;
        case 4:
          return this->a(3);
          break;
        default:
          break;
      }
      return zero;
    }  // end of operator()
  };

  template <typename A>
  struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr3D
      : public TensorTransposeExprBase<A> {
    static_assert(getSpaceDimension<A>() == 3u);

    TFEL_HOST_DEVICE constexpr TensorTransposeExpr3D(A l) noexcept
        : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

    TFEL_HOST_DEVICE constexpr auto operator[](
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      return this->operator()(i);
    }  // end of operator[]

    TFEL_HOST_DEVICE constexpr typename TensorTransposeExprBase<A>::NumType
    operator()(
        const typename TensorTransposeExprBase<A>::IndexType i) const noexcept {
      constexpr typename TensorTransposeExprBase<A>::NumType zero{0};
      switch (i) {
        case 0:
          return this->a(0);
          break;
        case 1:
          return this->a(1);
          break;
        case 2:
          return this->a(2);
          break;
        case 3:
          return this->a(4);
          break;
        case 4:
          return this->a(3);
          break;
        case 5:
          return this->a(6);
          break;
        case 6:
          return this->a(5);
          break;
        case 7:
          return this->a(8);
          break;
        case 8:
          return this->a(7);
          break;
        default:
          break;
      }
      return zero;
    }  // end of operator()
  };

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX */
