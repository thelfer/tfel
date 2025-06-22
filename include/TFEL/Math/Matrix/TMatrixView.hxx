/*!
 * \file   include/TFEL/Math/Matrix/TMatrixView.hxx
 * \brief  TMatrixFromTMatrixView
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

#ifndef LIB_TFEL_MATH_TMATRIXVIEW_HXX
#define LIB_TFEL_MATH_TMATRIXVIEW_HXX

#include "TFEL/Metaprogramming/StaticAssert.hxx"

#include "TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include "TFEL/Math/Matrix/MatrixUtilities.hxx"
#include "TFEL/Math/Matrix/MatrixConcept.hxx"
#include "TFEL/Math/tmatrix.hxx"

namespace tfel {

  namespace math {

    /*!
     * \param N  : number of rows
     * \param M  : number of columns
     * \param T  : value type
     */
    template <unsigned short N, unsigned short M, typename T>
    struct TMatrixViewExpr {
    };  // end of struct TMatrixFromTinyMatrixColumnViewExpr

    /*!
     * \param N  : number of rows
     * \param M  : number of columns
     * \param T  : value type
     */
    template <unsigned short N, unsigned short M, typename T>
    struct Expr<tmatrix<N, M, T>, TMatrixViewExpr<N, M, T>>
        : public MatrixConcept<
              Expr<tmatrix<N, M, T>, TMatrixViewExpr<N, M, T>>>,
          public tmatrix_base<Expr<tmatrix<N, M, T>, TMatrixViewExpr<N, M, T>>,
                              N,
                              M,
                              T> {
      typedef EmptyRunTimeProperties RunTimeProperties;
      typedef typename tmatrix<N, M, T>::value_type value_type;
      typedef typename tmatrix<N, M, T>::pointer pointer;
      typedef typename tmatrix<N, M, T>::const_pointer const_pointer;
      typedef typename tmatrix<N, M, T>::reference reference;
      typedef typename tmatrix<N, M, T>::const_reference const_reference;
      typedef typename tmatrix<N, M, T>::size_type size_type;
      typedef typename tmatrix<N, M, T>::difference_type difference_type;

      RunTimeProperties getRunTimeProperties() const {
        return RunTimeProperties();
      }

      Expr(T* const v_) noexcept : v(v_) {}  // end of Expr

      Expr(Expr&&) noexcept = default;

      Expr(const Expr&) noexcept = default;

      const T& operator()(const unsigned short i,
                          const unsigned short j) const noexcept {
        return this->v[i * M + j];
      }  // end of operator() const

      T& operator()(const unsigned short i, const unsigned short j) noexcept {
        return this->v[i * M + j];
      }  // end of operator()

      //! using tmatrix_base::operator=
      using tmatrix_base<Expr, N, M, T>::operator=;
      //! assignement operator
      Expr& operator=(const Expr& src) {
        tmatrix_base<Expr, N, M, T>::operator=(src);
        return *this;
      }
      /*!
       * size of the tiny matrix
       * (compatibility with matrix)
       */
      constexpr TFEL_MATH_INLINE size_type size() const noexcept {
        return N * M;
      }

     protected:
      T* const v;

     private:
      Expr() = delete;

    };  // end of struct TMatrixExpr

    template <unsigned short N, unsigned short M, typename T = double>
    using TMatrixView = Expr<tmatrix<N, M, T>, TMatrixViewExpr<N, M, T>>;

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TMATRIXVIEW_HXX */
