/*!
 * \file   include/TFEL/Math/Vector/TVectorFromTinyMatrixColumnView.hxx
 * \brief
 * Tvector From Matric Column View
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTORFROMTINYMATRIXCOLUMNVIEW_HXX
#define LIB_TFEL_MATH_TVECTORFROMTINYMATRIXCOLUMNVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tvector/TvectorConcept.hxx"
#include "TFEL/Math/Tvector/TvectorExpr.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T>
    struct TVectorFromTinyMatrixColumnViewExpr {
    };  // end of struct TVectorFromTinyMatrixColumnViewExpr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T>
    struct TvectorExpr<
        tvector<N, T>,
        TVectorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>
        : public TvectorConcept<TvectorExpr<
              tvector<N, T>,
              TVectorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>>,
          public tvector_base<
              TvectorExpr<
                  tvector<N, T>,
                  TVectorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>,
              N,
              T> {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      TvectorExpr(tmatrix<Mn, Mm, T>& m_) : m(m_) {}  // end of TvectorExpr

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
      using tvector_base<TvectorExpr, N, T>::operator=;

     protected:
      //! the underlying matrix
      tmatrix<Mn, Mm, T>& m;

     private:
      //! a simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      //! a simple check
      TFEL_STATIC_ASSERT((Im < Mm));
      //! a simple check
      TFEL_STATIC_ASSERT((TvectorDimeToSize<N>::value <= Mn - In));

    };  // end of struct TvectorExpr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T = double>
    struct TVectorFromTinyMatrixColumnView {
      typedef TvectorExpr<
          tvector<N, T>,
          TVectorFromTinyMatrixColumnViewExpr<N, Mn, Mm, In, Im, T>>
          type;
    };  // end of struct TVectorFromTinyMatrixColumnView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TVECTORFROMTINYMATRIXCOLUMNVIEW_HXX */
