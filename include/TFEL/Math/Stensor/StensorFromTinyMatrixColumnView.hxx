/*!
 * \file   include/TFEL/Math/Stensor/StensorFromTinyMatrixColumnView.hxx
 * \brief
 * Stensor From Matric Column View
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX
#define LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/ExpressionTemplates/Expr.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Stensor/StensorConcept.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T>
    struct StensorFromTinyMatrixColumnViewExpr {
    };  // end of struct StensorFromTinyMatrixColumnViewExpr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T>
    struct Expr<stensor<N, T>,
                StensorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>
        : public StensorConcept<
              Expr<stensor<N, T>,
                   StensorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>>,
          public stensor_base<
              Expr<stensor<N, T>,
                   StensorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      Expr(tmatrix<Mn, Mm, T>& m_) : m(m_) {}  // end of Expr

      const T& operator()(const unsigned short i) const {
        return this->m(In + i, Im);
      }  // end of operator() const

      T& operator()(const unsigned short i) {
        return this->m(In + i, Im);
      }  // end of operator()

      const T& operator[](const unsigned short i) const {
        return this->m(In + i, Im);
      }  // end of operator[] const

      T& operator[](const unsigned short i) {
        return this->m(In + i, Im);
      }  // end of operator[]

      //! assignement operator
      using stensor_base<Expr>::operator=;

     protected:
      //! the underlying matrix
      tmatrix<Mn, Mm, T>& m;

     private:
      //! a simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      //! a simple check
      TFEL_STATIC_ASSERT((Im < Mm));
      //! a simple check
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value <= Mn - In));

    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T = double>
    struct StensorFromTinyMatrixColumnView {
      typedef Expr<stensor<N, T>,
                   StensorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>
          type;
    };  // end of struct StensorFromTinyMatrixColumnView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_STENSORFROMTINYMATRIXCOLUMNVIEW_HXX */
