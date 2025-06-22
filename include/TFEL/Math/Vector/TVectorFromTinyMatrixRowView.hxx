/*!
 * \file   include/TFEL/Math/Vector/TVectorFromTinyMatrixRowView.hxx
 * \brief
 * TVector From Tensor Matrix Row View
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW_HXX
#define LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW_HXX

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
    struct TVectorFromTinyMatrixRowViewExpr {
    };  // end of struct TVectorFromTinyMatrixRowViewExpr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T>
    struct TvectorExpr<tvector<N, T>,
                       TVectorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>
        : public TvectorConcept<TvectorExpr<
              tvector<N, T>,
              TVectorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>>,
          public tvector_base<
              TvectorExpr<
                  tvector<N, T>,
                  TVectorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>,
              N,
              T> {
      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      TvectorExpr(tmatrix<Mn, Mm, T>& m_) : m(m_) {}  // end of TvectorExpr

      const T& operator()(const unsigned short i) const {
        return this->m(In, Im + i);
      }  // end of operator() const

      T& operator()(const unsigned short i) {
        return this->m(In, Im + i);
      }  // end of operator()

      const T& operator[](const unsigned short i) const {
        return this->m(In, Im + i);
      }  // end of operator[] const

      T& operator[](const unsigned short i) {
        return this->m(In, Im + i);
      }  // end of operator[]

      //! assignement operator
      using tvector_base<TvectorExpr, N, T>::operator=;

     protected:
      tmatrix<Mn, Mm, T>& m;

     private:
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      TFEL_STATIC_ASSERT((In < Mn));
      TFEL_STATIC_ASSERT((N <= Mm - Im));

    };  // end of struct TvectorExpr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short Mm,
              unsigned short In,
              unsigned short Im,
              typename T = double>
    struct TVectorFromTinyMatrixRowView {
      typedef TvectorExpr<
          tvector<N, T>,
          TVectorFromTinyMatrixRowViewExpr<N, Mn, Mm, In, Im, T>>
          type;
    };  // end of struct TVectorFromTinyMatrixRowView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TVECTORFROMTINYMATRIXROWVIEW_HXX */
