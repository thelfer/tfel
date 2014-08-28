/*! 
 * \file  include/TFEL/Math/Matrix/tmatrix_row_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 */

#ifndef _LIB_TFEL_MATH_TMATRIX_ROW_VIEW_IXX_
#define _LIB_TFEL_MATH_TMATRIX_ROW_VIEW_IXX_ 

namespace tfel
{

  namespace math
  {

    /*!
     * partial specialisation for the tmatrix_row_view_expr
     * (normally the constructors of VectorExpr only takes constant arguments)
     */
    template<typename T_type,
	     unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    class VectorExpr<T_type,tmatrix_row_view_expr<N,M,I,J,K,T> >
      : public VectorConcept<VectorExpr<T_type,tmatrix_row_view_expr<N,M,I,J,K,T> > >,
	public tmatrix_row_view_expr<N,M,I,J,K,T>
    {

      VectorExpr& operator=(const VectorExpr&);

    public:
      
      typedef tmatrix_row_view_expr<N,M,I,J,K,T>   Expr;
      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      TFEL_MATH_INLINE VectorExpr(const VectorExpr& src)
	: Expr(src)	
      {}

      explicit TFEL_MATH_INLINE VectorExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}
      
      using Expr::operator();

      using Expr::operator[];

      ~VectorExpr()
      {}

    };

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    T& tmatrix_row_view_expr<N,M,I,J,K,T>::operator[](const unsigned short i)
    {
      return m(I,static_cast<unsigned short>(J+i));
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_row_view<N,M,I,J,K,T>::tmatrix_row_view(tmatrix<N,M,T>& m_)
      : VectorExpr<tvector<K,T>, tmatrix_row_view_expr<N,M,I,J,K,T> >(m_)
    {}

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_row_view_expr<N,M,I,J,K,T>::tmatrix_row_view_expr(tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_row_view_expr
    
    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    typename tmatrix_row_view_expr<N,M,I,J,K,T>::RunTimeProperties
    tmatrix_row_view_expr<N,M,I,J,K,T>::getRunTimeProperties(void) const
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
    tmatrix_row_view_expr<N,M,I,J,K,T>::operator()(const unsigned short i) const
    {
      return m(I,static_cast<unsigned short>(J+i));
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    T&
    tmatrix_row_view_expr<N,M,I,J,K,T>::operator()(const unsigned short i)
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
    tmatrix_row_view_expr<N,M,I,J,K,T>::operator[](const unsigned short i) const
    {
      return m(I,static_cast<unsigned short>(J+i));
    }

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_ROW_VIEW_IXX */
