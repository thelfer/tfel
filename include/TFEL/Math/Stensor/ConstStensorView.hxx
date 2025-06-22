/*!
 * \file   include/TFEL/Math/Stensor/ConstStensorView.hxx
 * \author Helfer Thomas
 * \date   16 July 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_CONSTSTENSORVIEW_HXX_
#define LIB_TFEL_MATH_CONSTSTENSORVIEW_HXX_

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     *  Stensor From Tiny Vector
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct ConstStensorViewExpr {
    };  // end of struct StensorFromTinyMatrixColumnViewExpr

    /*!
     *  Stensor From Tiny Vector expression
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct Expr<stensor<N, T>, ConstStensorViewExpr<N, T>>
        : public StensorConcept<
              Expr<stensor<N, T>, ConstStensorViewExpr<N, T>>> {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename stensor<N, T>::value_type value_type;
      typedef typename stensor<N, T>::pointer pointer;
      typedef typename stensor<N, T>::const_pointer const_pointer;
      typedef typename stensor<N, T>::reference reference;
      typedef typename stensor<N, T>::const_reference const_reference;
      typedef typename stensor<N, T>::size_type size_type;
      typedef typename stensor<N, T>::difference_type difference_type;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      /*!
       * \param[in] v : v_ values
       */
      explicit Expr(const T* const v_) noexcept : v(v_) {}  // end of Expr

      Expr(Expr&&) noexcept = default;

      Expr(const Expr&) noexcept = default;

      const T& operator()(const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator() const

      const T& operator[](const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator[] const

      /*!
       * size of the symmetric tensor
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size(void) const noexcept {
        return StensorDimeToSize<N>::value;
      }

     protected:
      const T* const v;

     private:
      Expr() = delete;
      Expr& operator=(Expr&&) = delete;
      Expr& operator=(const Expr&) = delete;
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N, typename T = double>
    using ConstStensorView = Expr<stensor<N, T>, ConstStensorViewExpr<N, T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_CONSTSTENSORVIEW_HXX_ */
