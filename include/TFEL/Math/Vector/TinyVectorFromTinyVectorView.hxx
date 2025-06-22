/*!
 * \file   include/TFEL/Math/Vector/TinyVectorFromTinyVectorView.hxx
 * \brief  TinyVectorFromTVectorView
 * \author Helfer Thomas
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX_
#define LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX_

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Forward/tvector.hxx"
#include "TFEL/Math/tvector.hxx"

namespace tfel {

  namespace math {

    /*!
     *  Tiny vector from Tiny Vector view expression
     * \param[in] N  : number of object holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T,
              bool b>
    struct TinyVectorFromTinyVectorViewExpr {
      /*!
       * numerical type holded by the expression
       */
      typedef T NumType;
    };  // end of struct TinyVectorFromTinyVectorViewExpr

    /*!
     * \param[in] N : size of the tvector holed
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the beginning in the underlying vector
     * \param[in] T  : underlying type
     * \param[in] b  : if true the underlying tvector is const
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T,
              bool b>
    struct Expr<tvector<N, T>,
                TinyVectorFromTinyVectorViewExpr<N, Mn, In, T, b>>
        : public VectorConcept<
              Expr<tvector<N, T>,
                   TinyVectorFromTinyVectorViewExpr<N, Mn, In, T, b>>>,
          public tvector_base<
              Expr<tvector<N, T>,
                   TinyVectorFromTinyVectorViewExpr<N, Mn, In, T, b>>,
              N,
              T> {
      typedef typename std::
          conditional<b, const tvector<Mn, T>&, tvector<Mn, T>&>::type ref_type;

      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      /*!
       * constructor
       */
      Expr(ref_type v_) : v(v_) {}  // end of Expr
      /*!
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      RunTimeProperties getRunTimeProperties(void) const {
        return RunTimeProperties();
      }

      const T& operator[](const unsigned short i) const {
        return this->v[static_cast<unsigned short>(In + i)];
      }  // end of operator[] const

      T& operator[](const unsigned short i) {
        return this->v[static_cast<unsigned short>(In + i)];
      }  // end of operator[]

      const T& operator()(const unsigned short i) const {
        return this->v[static_cast<unsigned short>(In + i)];
      }  // end of operator() const

      T& operator()(const unsigned short i) {
        return this->v[static_cast<unsigned short>(In + i)];
      }  // end of operator()

      using tvector_base<Expr, N, T>::operator=;

     protected:
      //! underlying vector
      ref_type v;

     private:
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((In < Mn));
      TFEL_STATIC_ASSERT((N <= Mn - In));
    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T,
              bool b>
    struct TinyVectorFromTinyVectorView {
      typedef Expr<tvector<N, T>,
                   TinyVectorFromTinyVectorViewExpr<N, Mn, In, T, b>>
          type;
    };  // end of struct TinyVectorFromTinyVectorView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYVECTORFROMTINYVECTORVIEW_HXX_ */
