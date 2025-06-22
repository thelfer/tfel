/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2View.hxx
 * \author Thomas Helfer
 * \date   16 July 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_T2TOT2VIEW_HXX
#define LIB_TFEL_MATH_T2TOT2VIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include "TFEL/Math/t2tot2.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     *  T2toT2 From Tiny Vector
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct T2toT2ViewExpr {
    };  // end of struct T2toT2FromTinyMatrixColumnViewExpr

    /*!
     *  T2toT2 From Tiny Vector expression
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct Expr<t2tot2<N, T>, T2toT2ViewExpr<N, T>>
        : public T2toT2Concept<Expr<t2tot2<N, T>, T2toT2ViewExpr<N, T>>>,
          public t2tot2_base<Expr<t2tot2<N, T>, T2toT2ViewExpr<N, T>>> {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename t2tot2<N, T>::value_type value_type;
      typedef typename t2tot2<N, T>::pointer pointer;
      typedef typename t2tot2<N, T>::const_pointer const_pointer;
      typedef typename t2tot2<N, T>::reference reference;
      typedef typename t2tot2<N, T>::const_reference const_reference;
      typedef typename t2tot2<N, T>::size_type size_type;
      typedef typename t2tot2<N, T>::difference_type difference_type;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      /*!
       * \param[in] v : v_ values
       */
      explicit Expr(T* const v_) noexcept : v(v_) {}  // end of Expr

      Expr(Expr&&) noexcept = default;

      Expr(const Expr&) noexcept = default;

      const T& operator()(const unsigned short i,
                          const unsigned short j) const noexcept {
        return this->v[TensorDimeToSize<N>::value * i + j];
      }  // end of operator() const

      T& operator()(const unsigned short i, const unsigned short j) noexcept {
        return this->v[TensorDimeToSize<N>::value * i + j];
      }  // end of operator()

      //! using t2tot2_base::operator=
      using t2tot2_base<Expr>::operator=;

      Expr& operator=(const Expr& src) {
        t2tot2_base<Expr>::operator=(src);
        return *this;
      }

      /*!
       * size of the symmetric tensor
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept {
        return TensorDimeToSize<N>::value * TensorDimeToSize<N>::value;
      }

     protected:
      T* const v;

     private:
      Expr() = delete;
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N, typename T = double>
    using T2toT2View = Expr<t2tot2<N, T>, T2toT2ViewExpr<N, T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_T2TOT2VIEW_HXX */
