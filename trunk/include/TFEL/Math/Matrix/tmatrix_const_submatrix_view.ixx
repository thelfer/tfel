/*! 
 * \file  tmatrix_const_submatrix_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 */

#ifndef _LIB_TFEL_MATH_TMATRIX_CONST_SUBMATRIX_VIEW_IXX_
#define _LIB_TFEL_MATH_TMATRIX_CONST_SUBMATRIX_VIEW_IXX_ 

namespace tfel
{

  namespace math
  {

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_const_submatrix_view_expr<N,M,I,J,R,C,T>::tmatrix_const_submatrix_view_expr(const tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_const_submatrix_view_expr
    
    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    typename tmatrix_const_submatrix_view_expr<N,M,I,J,R,C,T>::RunTimeProperties
    tmatrix_const_submatrix_view_expr<N,M,I,J,R,C,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    const T& 
    tmatrix_const_submatrix_view_expr<N,M,I,J,R,C,T>::operator()(const unsigned short i,
								 const unsigned short j) const
    {
      return m(static_cast<unsigned short>(I+i),
	       static_cast<unsigned short>(J+j));
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_const_submatrix_view<N,M,I,J,R,C,T>::tmatrix_const_submatrix_view(const tmatrix<N,M,T>& m_)
      : MatrixExpr<tmatrix<R,C,T>, tmatrix_const_submatrix_view_expr<N,M,I,J,R,C,T> >(m_)
    {} // end of tmatrix_const_submatrix_view

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_CONST_SUBMATRIX_VIEW_H */
