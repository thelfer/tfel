/*!
 * \file  include/TFEL/Math/Matrix/tmatrix_const_column_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_TMATRIX_CONST_COLUMN_VIEW_IXX_
#define LIB_TFEL_MATH_TMATRIX_CONST_COLUMN_VIEW_IXX_

namespace tfel {

  namespace math {

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short K,
              typename T>
    TMatrixConstColumnView<N, M, I, J, K, T>::TMatrixConstColumnView(
        const tmatrix<N, M, T>& m_)
        : m(m_) {}  // end of TMatrixConstColumnView

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short K,
              typename T>
    typename TMatrixConstColumnView<N, M, I, J, K, T>::RunTimeProperties
    TMatrixConstColumnView<N, M, I, J, K, T>::getRunTimeProperties(void) const {
      return RunTimeProperties();
    }

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short K,
              typename T>
    const T& TMatrixConstColumnView<N, M, I, J, K, T>::operator()(
        const unsigned short i) const {
      return m(static_cast<unsigned short>(J + i), I);
    }

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short K,
              typename T>
    const T& TMatrixConstColumnView<N, M, I, J, K, T>::operator[](
        const unsigned short i) const {
      return m(static_cast<unsigned short>(J + i), I);
    }

    template <unsigned short N,
              unsigned short M,
              unsigned short I,
              unsigned short J,
              unsigned short K,
              typename T>
    tmatrix_const_column_view<N, M, I, J, K, T>::tmatrix_const_column_view(
        const tmatrix<N, M, T>& m_)
        : Expr<tvector<K, T>, TMatrixConstColumnView<N, M, I, J, K, T>>(m_) {
    }  // end of tmatrix_const_column_view

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_TMATRIX_CONST_COLUMN_VIEW_IXX_ */
