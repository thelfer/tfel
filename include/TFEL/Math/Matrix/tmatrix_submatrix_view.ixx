/*! 
 * \file  include/TFEL/Math/Matrix/tmatrix_submatrix_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 */

#ifndef _LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_IXX_
#define _LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_IXX_ 

namespace tfel
{

  namespace math
  {

    /*!
     * partial specialisation for the tmatrix_submatrix_view_expr
     * (normally the constructors of MatrixExpr only takes constant arguments)
     */
    template<typename T_type,unsigned short N,
	     unsigned short M,unsigned short I,
	     unsigned short J,unsigned short R,
	     unsigned short C,typename T>
    class MatrixExpr<T_type,tmatrix_submatrix_view_expr<N,M,I,J,R,C,T> >
      : public MatrixConcept<MatrixExpr<T_type,tmatrix_submatrix_view_expr<N,M,I,J,R,C,T> > >,
	public tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>
    {

      MatrixExpr& operator=(const MatrixExpr&);

    public:
      
      typedef tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>   Expr;
      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      TFEL_MATH_INLINE MatrixExpr(const MatrixExpr& src)
	: Expr(src)	
      {}

      explicit TFEL_MATH_INLINE MatrixExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}
      
      using Expr::operator();

      ~MatrixExpr()
      {}

    };

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_submatrix_view<N,M,I,J,R,C,T>::tmatrix_submatrix_view(tmatrix<N,M,T>& m_)
      : MatrixExpr<tmatrix<R,C,T>, tmatrix_submatrix_view_expr<N,M,I,J,R,C,T> >(m_)
    {}

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::tmatrix_submatrix_view_expr(tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_submatrix_view_expr
    
    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    typename tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::RunTimeProperties
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::getRunTimeProperties(void) const
    {
      return RunTimeProperties();
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    const T& 
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::operator()(const unsigned short i,
							 const unsigned short j) const
    {
      return m(static_cast<unsigned short>(I+i),
	       static_cast<unsigned short>(J+j));
    }

    template<unsigned short N,unsigned short M,
	     unsigned short I,unsigned short J,
	     unsigned short R,unsigned short C,
	     typename T>
    T&
    tmatrix_submatrix_view_expr<N,M,I,J,R,C,T>::operator()(const unsigned short i,
							 const unsigned short j)
    {
      return m(static_cast<unsigned short>(I+i),
	       static_cast<unsigned short>(J+j));
    }

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_SUBMATRIX_VIEW_IXX */
