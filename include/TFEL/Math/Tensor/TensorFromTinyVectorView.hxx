/*!
 * \file   include/TFEL/Math/Tensor/TensorFromTinyVectorView.hxx
 * \brief  TensorFromTVectorView
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

#ifndef LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX
#define LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Vector/VectorUtilities.hxx"
#include "TFEL/Math/Tensor/TensorConcept.hxx"
#include "TFEL/Math/tensor.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     *  Tensor From Tiny Vector
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T>
    struct TensorFromTinyVectorViewExpr {
    };  // end of struct TensorFromTinyMatrixColumnViewExpr

    /*!
     *  Tensor From Tiny Vector expression
     * \param N  : space dimension
     * \param Mn : underlying tiny vector size
     * \param In : starting index in the underlying tiny vector
     * \param T  : value type
     */
    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T>
    struct Expr<tensor<N, T>, TensorFromTinyVectorViewExpr<N, Mn, In, T>>
        : public TensorConcept<
              Expr<tensor<N, T>, TensorFromTinyVectorViewExpr<N, Mn, In, T>>>,
          public tensor_base<
              Expr<tensor<N, T>, TensorFromTinyVectorViewExpr<N, Mn, In, T>>> {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename tensor<N, T>::value_type value_type;
      typedef typename tensor<N, T>::pointer pointer;
      typedef typename tensor<N, T>::const_pointer const_pointer;
      typedef typename tensor<N, T>::reference reference;
      typedef typename tensor<N, T>::const_reference const_reference;
      typedef typename tensor<N, T>::size_type size_type;
      typedef typename tensor<N, T>::difference_type difference_type;

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

      //! using tensor_base::operator=
      using tensor_base<Expr>::operator=;

     protected:
      tvector<Mn, T>& v;

     private:
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N == 1u) || (N == 2u) || (N == 3u));
      TFEL_STATIC_ASSERT((In < Mn));
      TFEL_STATIC_ASSERT((TensorDimeToSize<N>::value <= Mn - In));

    };  // end of struct Expr

    template <unsigned short N,
              unsigned short Mn,
              unsigned short In,
              typename T = double>
    struct TensorFromTinyVectorView {
      typedef Expr<tensor<N, T>, TensorFromTinyVectorViewExpr<N, Mn, In, T>>
          type;
    };  // end of struct TensorFromTinyVectorView

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TENSORFROMTINYVECTORVIEW_HXX */
