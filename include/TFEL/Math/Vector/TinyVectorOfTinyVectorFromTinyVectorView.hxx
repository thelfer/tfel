/*!
 * \file   include/TFEL/Math/Vector/TinyVectorOfTinyVectorFromTinyVectorView.hxx
 * \brief  TinyVectorOfTinyVectorFromTVectorView
 * This class creates an object that acts like a tiny vector of
 * tvectors.
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYVECTOROFTVECTORFROMTINYVECTORVIEW_HXX_
#define LIB_TFEL_MATH_TINYVECTOROFTVECTORFROMTINYVECTORVIEW_HXX_

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/Vector/TVectorView.hxx"
#include "TFEL/Math/Vector/ConstTVectorView.hxx"

namespace tfel {

  namespace math {

    /*!
     *  TVector of tvectors  from Tiny Vector view expression
     * \param[in] N  : dimension used to define the tvector
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first tvector in the underlying vector
     * \param[in] Nn : number of tvector holed
     * \param[in] T  : underlying type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T>
    struct TinyVectorOfTinyVectorFromTinyVectorViewExpr {
      /*!
       * numerical type holded by the expression
       */
      typedef tvector<N, T> NumType;
    };  // end of struct TinyVectorOfTinyVectorFromTinyVectorViewExpr

    /*!
     * \param[in] N  : dimension used to define the tvector (1,2 or 3)
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first tvector in the underlying vector
     * \param[in] Nn : number of tvector holed
     * \param[in] T  : underlying type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T>
    struct Expr<tvector<Nn, tvector<N, T>>,
                TinyVectorOfTinyVectorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>
        : public VectorConcept<Expr<
              tvector<Nn, tvector<N, T>>,
              TinyVectorOfTinyVectorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>>,
          public tvector_base<
              Expr<tvector<Nn, tvector<N, T>>,
                   TinyVectorOfTinyVectorFromTinyVectorViewExpr<N,
                                                                Mn,
                                                                In,
                                                                Nn,
                                                                T>>,
              N,
              T> {
      typedef EmptyRunTimeProperties RunTimeProperties;

      Expr(tvector<Mn, T>& v_) : v(v_) {}  // end of Expr

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      RunTimeProperties getRunTimeProperties(void) const {
        return RunTimeProperties();
      }

      ConstTVectorView<N, T> operator[](const unsigned short i) const {
        return ConstTVectorView<N, T>{this->v.begin() + In + i * N};
      }  // end of operator[] const

      TVectorView<N, T> operator[](const unsigned short i) {
        return TVectorView<N, T>{this->v.begin() + In + i * N};
      }  // end of operator[]

      ConstTVectorView<N, T> operator()(const unsigned short i) const {
        return ConstTVectorView<N, T>{this->v.begin() + In + i * N};
      }  // end of operator() const

      TVectorView<N, T> operator()(const unsigned short i) {
        return TVectorView<N, T>{this->v.begin() + In + i * N};
      }  // end of operator()

      using tvector_base<Expr, N, T>::operator=;

     protected:
      tvector<Mn, T>& v;

     private:
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      TFEL_STATIC_ASSERT((In < Mn));
      TFEL_STATIC_ASSERT((Nn * N <= Mn - In));

    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T = double>
    struct TinyVectorOfTinyVectorFromTinyVectorView {
      typedef Expr<
          tvector<Nn, tvector<N, T>>,
          TinyVectorOfTinyVectorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>
          type;
    };  // end of struct TinyVectorOfTinyVectorFromTinyVectorView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYVECTOROFTVECTORFROMTINYVECTORVIEW_HXX_ */
