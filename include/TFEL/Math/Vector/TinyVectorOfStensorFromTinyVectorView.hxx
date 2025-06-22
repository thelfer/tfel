/*!
 * \file   include/TFEL/Math/Vector/TinyVectorOfStensorFromTinyVectorView.hxx
 * \brief  TinyVectorOfStensorFromTVectorView
 * This class creates an object that acts like a tiny vector of
 * stensors.
 * \author Thomas Helfer
 * \date   16 oct 2008
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX
#define LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Vector/VectorConcept.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Math/stensor.hxx"
#include "TFEL/Math/Stensor/StensorView.hxx"
#include "TFEL/Math/Stensor/ConstStensorView.hxx"

namespace tfel {

  namespace math {

    /*!
     *  Tvector of stensors  from Tiny Vector view expression
     * \param[in] N  : dimension used to define the stensor (1,2 or 3)
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first stensor in the underlying vector
     * \param[in] Nn : number of stensor holed
     * \param[in] T  : underlying type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T>
    struct TinyVectorOfStensorFromTinyVectorViewExpr {
      /*!
       * numerical type holded by the expression
       */
      typedef stensor<N, T> NumType;
    };  // end of struct TinyVectorOfStensorFromTinyVectorViewExpr

    /*!
     * \param[in] N  : dimension used to define the stensor (1,2 or 3)
     * \param[in] Mn : dimension of the underlying vector
     * \param[in] In : Index of the first stensor in the underlying vector
     * \param[in] Nn : number of stensor holed
     * \param[in] T  : underlying type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T>
    struct Expr<tvector<Nn, stensor<N, T>>,
                TinyVectorOfStensorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>
        : public VectorConcept<Expr<
              tvector<Nn, stensor<N, T>>,
              TinyVectorOfStensorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>>,
          public tvector_base<
              Expr<tvector<Nn, stensor<N, T>>,
                   TinyVectorOfStensorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>,
              N,
              T> {
      typedef EmptyRunTimeProperties RunTimeProperties;

      Expr(tvector<Mn, T>& v_) : v(v_) {}  // end of Expr

      /*
       * Return the RunTimeProperties of the tvector
       * \return tvector::RunTimeProperties
       */
      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      ConstStensorView<N, T> operator[](const unsigned short i) const {
        return ConstStensorView<N, T>(this->v.begin() + In +
                                      i * StensorDimeToSize<N>::value);
      }  // end of operator[] const

      StensorView<N, T> operator[](const unsigned short i) {
        return StensorView<N, T>(this->v.begin() + In +
                                 i * StensorDimeToSize<N>::value);
      }  // end of operator[]

      ConstStensorView<N, T> operator()(const unsigned short i) const {
        return ConstStensorView<N, T>(this->v.begin() + In +
                                      i * StensorDimeToSize<N>::value);
      }  // end of operator() const

      StensorView<N, T> operator()(const unsigned short i) {
        return StensorView<N, T>(this->v.begin() + In +
                                 i * StensorDimeToSize<N>::value);
      }  // end of operator()

      using tvector_base<Expr, N, T>::operator=;

     protected:
      tvector<Mn, T>& v;

     private:
      //! simple check
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));

    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              unsigned short Nn,
              typename T = double>
    struct TinyVectorOfStensorFromTinyVectorView {
      typedef Expr<tvector<Nn, stensor<N, T>>,
                   TinyVectorOfStensorFromTinyVectorViewExpr<N, Mn, In, Nn, T>>
          type;
    };  // end of struct TinyVectorOfStensorFromTinyVectorView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TINYVECTOROFSTENSORFROMTINYVECTORVIEW_HXX */
