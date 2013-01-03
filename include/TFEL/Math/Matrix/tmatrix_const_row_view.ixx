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
	     typename T>
    tmatrix_const_row_view_expr<N,M,I,T>::tmatrix_const_row_view_expr(const tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_const_row_view_expr
    
    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     typename T>
    typename tmatrix_const_row_view_expr<N,M,I,T>::RunTimeProperties
    tmatrix_const_row_view_expr<N,M,I,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     typename T>
    const T& 
    tmatrix_const_row_view_expr<N,M,I,T>::operator()(const unsigned short i) const
    {
      return m(I,i);
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     typename T>
    const T& 
    tmatrix_const_row_view_expr<N,M,I,T>::operator[](const unsigned short i) const
    {
      return m(I,i);
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     typename T>
    tmatrix_const_row_view<N,M,I,T>::tmatrix_const_row_view(const tmatrix<N,M,T>& m_)
      : VectorExpr<tvector<M,T>, tmatrix_const_row_view_expr<N,M,I,T> >(m_)
    {} // end of tmatrix_const_row_view

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_CONST_ROW_VIEW_H */
