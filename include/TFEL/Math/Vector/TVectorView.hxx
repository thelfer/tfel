/*!
 * \file   include/TFEL/Math/Vector/TVectorView.hxx
 * \brief  TVectorFromTVectorView
 *
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTORVIEW_HXX
#define LIB_TFEL_MATH_TVECTORVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     *  TVector From Tiny Vector
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct TVectorViewExpr {
    };  // end of struct TVectorFromTinyMatrixColumnViewExpr

    /*!
     *  TVector from array expression
     * \param N  : space dimension
     * \param T  : value type
     */
    template <unsigned short N, typename T>
    struct Expr<tvector<N, T>, TVectorViewExpr<N, T>>
        : public VectorConcept<Expr<tvector<N, T>, TVectorViewExpr<N, T>>>,
          public tvector_base<Expr<tvector<N, T>, TVectorViewExpr<N, T>>,
                              N,
                              T> {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename tvector<N, T>::value_type value_type;
      typedef typename tvector<N, T>::pointer pointer;
      typedef typename tvector<N, T>::const_pointer const_pointer;
      typedef typename tvector<N, T>::reference reference;
      typedef typename tvector<N, T>::const_reference const_reference;
      typedef typename tvector<N, T>::size_type size_type;
      typedef typename tvector<N, T>::difference_type difference_type;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      Expr(T* const v_) noexcept : v(v_) {}  // end of TvectorExpr

      Expr(Expr&&) noexcept = default;

      Expr(const Expr&) noexcept = default;

      const T& operator()(const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator() const

      T& operator()(const unsigned short i) noexcept {
        return this->v[i];
      }  // end of operator()

      const T& operator[](const unsigned short i) const noexcept {
        return this->v[i];
      }  // end of operator[] const

      T& operator[](const unsigned short i) noexcept {
        return this->v[i];
      }  // end of operator[]

      //! using tvector_base::operator=
      using tvector_base<Expr, N, T>::operator=;

      /*!
       * size of the tiny vector
       * (compatibility with vector)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept { return N; }

     protected:
      T* const v;

     private:
      Expr() = delete;
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
    };  // end of struct TVectorExpr

    template <unsigned short N, typename T = double>
    using TVectorView = Expr<tvector<N, T>, TVectorViewExpr<N, T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TVECTORVIEW_HXX */
