/*!
 * \file   include/TFEL/Math/Stensor/StensorFromTinyVectorView.hxx
 * \brief  StensorFromTVectorView
 *
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX_
#define LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX_

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
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T>
    struct StensorFromTinyVectorViewExpr {
    };  // end of struct StensorFromTinyMatrixColumnViewExpr

    /*!
     *  Stensor From Tiny Vector expression
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T>
    struct Expr<stensor<N, T>, StensorFromTinyVectorViewExpr<N, Mn, In, T>>
        : public StensorConcept<
              Expr<stensor<N, T>, StensorFromTinyVectorViewExpr<N, Mn, In, T>>>,
          public stensor_base<
              Expr<stensor<N, T>,
                   StensorFromTinyVectorViewExpr<N, Mn, In, T>>> {
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

      Expr(tvector<Mn, T>& v_) : v(v_) {}  // end of Expr

      const T& operator()(const unsigned short i) const {
        return this->v(In + i);
      }  // end of operator() const

      T& operator()(const unsigned short i) {
        return this->v(In + i);
      }  // end of operator()

      const T& operator[](const unsigned short i) const {
        return this->v(In + i);
      }  // end of operator[] const

      T& operator[](const unsigned short i) {
        return this->v(In + i);
      }  // end of operator[]

      //! using stensor_base::operator=
      using stensor_base<Expr>::operator=;

      Expr& operator=(const Expr& src) {
        stensor_base<Expr>::operator=(src);
        return *this;
      }

     protected:
      tvector<Mn, T>& v;

     private:
      // simple checks
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      TFEL_STATIC_ASSERT((In < Mn));
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value <= Mn - In));

    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T = double>
    struct StensorFromTinyVectorView {
      typedef Expr<stensor<N, T>, StensorFromTinyVectorViewExpr<N, Mn, In, T>>
          type;
    };  // end of struct StensorFromTinyVectorView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSORFROMTINYVECTORVIEW_HXX_ */
