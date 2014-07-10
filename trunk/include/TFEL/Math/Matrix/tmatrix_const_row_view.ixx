/*! 
 * \file  tmatrix_const_row_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 */

#ifndef _LIB_TFEL_MATH_TMATRIX_CONST_ROW_VIEW_IXX_
#define _LIB_TFEL_MATH_TMATRIX_CONST_ROW_VIEW_IXX_ 

namespace tfel
{

  namespace math
  {

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_const_row_view_expr<N,M,I,J,K,T>::tmatrix_const_row_view_expr(const tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_const_row_view_expr
    
    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    typename tmatrix_const_row_view_expr<N,M,I,J,K,T>::RunTimeProperties
    tmatrix_const_row_view_expr<N,M,I,J,K,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    const T& 
    tmatrix_const_row_view_expr<N,M,I,J,K,T>::operator()(const unsigned short i) const
    {
      return m(I,static_cast<unsigned short>(J+i));
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    const T& 
    tmatrix_const_row_view_expr<N,M,I,J,K,T>::operator[](const unsigned short i) const
    {
      return m(I,static_cast<unsigned short>(J+i));
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_const_row_view<N,M,I,J,K,T>::tmatrix_const_row_view(const tmatrix<N,M,T>& m_)
      : VectorExpr<tvector<K,T>, tmatrix_const_row_view_expr<N,M,I,J,K,T> >(m_)
    {} // end of tmatrix_const_row_view

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_CONST_ROW_VIEW_H */
