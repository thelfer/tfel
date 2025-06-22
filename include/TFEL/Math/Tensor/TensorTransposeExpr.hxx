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

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/Forward/TensorConcept.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"

namespace tfel {

  namespace math {

    template <typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExprBase : public ExprBase {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename TensorTraits<typename std::decay<A>::type>::IndexType
          IndexType;
      typedef
          typename TensorTraits<typename std::decay<A>::type>::NumType NumType;

      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties() const {
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

      TFEL_MATH_INLINE TensorTransposeExprBase(A l) : a(l) {}

      ArgumentStorage<A> a;

     private:
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<A>::type,
                                                 TensorConcept>::cond));
    };

    template <typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr1D
        : public TensorTransposeExprBase<A> {
      TFEL_MATH_INLINE TensorTransposeExpr1D(A l)
          : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType operator()(
          const typename TensorTransposeExprBase<A>::IndexType i) const {
        return this->a(i);
      }  // end of operator()

     private:
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<A>::type>::dime == 1u));
    };

    template <typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr2D
        : public TensorTransposeExprBase<A> {
      TFEL_MATH_INLINE TensorTransposeExpr2D(A l)
          : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType operator()(
          const typename TensorTransposeExprBase<A>::IndexType i) const {
        constexpr const typename TensorTransposeExprBase<A>::NumType zero{0};
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

     private:
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<A>::type>::dime == 2u));
    };

    template <typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr3D
        : public TensorTransposeExprBase<A> {
      TFEL_MATH_INLINE TensorTransposeExpr3D(A l)
          : TensorTransposeExprBase<A>(std::forward<A>(l)) {}

      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType operator()(
          const typename TensorTransposeExprBase<A>::IndexType i) const {
        constexpr const typename TensorTransposeExprBase<A>::NumType zero{0};
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

     private:
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<A>::type>::dime == 3u));
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX */
