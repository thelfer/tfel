/*!
 * \file   include/TFEL/Math/Tensor/TensorProduct.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   07 avr 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX_
#define LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX_

#include <cmath>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel {

  namespace math {

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExprBase : public ExprBase {
      typedef EmptyRunTimeProperties RunTimeProperties;

      TFEL_MATH_INLINE RunTimeProperties getRunTimeProperties(void) const {
        return EmptyRunTimeProperties();
      }

     protected:
      typedef typename ComputeBinaryResult<A, B, OpMult>::Result Result;
      typedef typename TensorTraits<Result>::NumType NumType;
      typedef typename TensorTraits<Result>::IndexType IndexType;

      typedef NumType value_type;
      typedef NumType* pointer;
      typedef const NumType* const_pointer;
      typedef NumType& reference;
      typedef const NumType& const_reference;
      typedef IndexType size_type;
      typedef ptrdiff_t difference_type;

      TensorProductExprBase() = delete;

      TFEL_MATH_INLINE TensorProductExprBase(A l, B r) : a(l), b(r) {}

      ArgumentStorage<A> a;
      ArgumentStorage<B> b;

      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<A>::type,
                                                 TensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<typename std::decay<B>::type,
                                                 TensorConcept>::cond));
    };

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr1D
        : public TensorProductExprBase<A, B> {
      TFEL_MATH_INLINE TensorProductExpr1D(A l, B r)
          : TensorProductExprBase<A, B>(std::forward<A>(l),
                                        std::forward<B>(r)) {}

      TFEL_MATH_INLINE typename TensorProductExprBase<A, B>::NumType operator()(
          const typename TensorProductExprBase<A, B>::IndexType i) const {
        return (this->a(i)) * (this->b(i));
      }  // end of operator()

     private:
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<A>::type>::dime == 1u));
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<B>::type>::dime == 1u));
    };

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr2D
        : public TensorProductExprBase<A, B> {
      TFEL_MATH_INLINE TensorProductExpr2D(A l, B r)
          : TensorProductExprBase<A, B>(std::forward<A>(l),
                                        std::forward<B>(r)) {}

      TFEL_MATH_INLINE typename TensorProductExprBase<A, B>::NumType operator()(
          const typename TensorProductExprBase<A, B>::IndexType i) const {
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
        }
        return T(0);
      }  // end of operator()

     private:
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<A>::type>::dime == 2u));
      TFEL_STATIC_ASSERT(
          (tfel::math::TensorTraits<typename std::decay<B>::type>::dime == 2u));
    };

    template <typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr3D
        : public TensorProductExprBase<A, B> {
      TFEL_MATH_INLINE TensorProductExpr3D(A l, B r)
          : TensorProductExprBase<A, B>(std::forward<A>(l),
                                        std::forward<B>(r)) {}

      TFEL_MATH_INLINE typename TensorProductExprBase<A, B>::NumType operator()(
          const typename TensorProductExprBase<A, B>::IndexType i) const {
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
        }
        return T(0);
      }  // end of operator()

     private:
      using ttypeA = typename std::decay<A>::type;
      using ttypeB = typename std::decay<A>::type;
      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<ttypeA>::dime == 3u));
      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<ttypeB>::dime == 3u));
    };

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORPRODUCTEXPR_HXX_ */
