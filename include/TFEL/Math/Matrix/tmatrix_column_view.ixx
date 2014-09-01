/*! 
 * \file  include/TFEL/Math/Matrix/tmatrix_column_view.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 27 déc. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_IXX_
#define _LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_IXX_ 

namespace tfel
{

  namespace math
  {

    /*!
     * partial specialisation for the tmatrix_column_view_expr
     * (normally the constructors of VectorExpr only takes constant arguments)
     */
    template<typename T_type,
	     unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    class VectorExpr<T_type,tmatrix_column_view_expr<N,M,I,J,K,T> >
      : public VectorConcept<VectorExpr<T_type,tmatrix_column_view_expr<N,M,I,J,K,T> > >,
	public tmatrix_column_view_expr<N,M,I,J,K,T>
    {

      VectorExpr& operator=(const VectorExpr&);

    public:
      
      typedef tmatrix_column_view_expr<N,M,I,J,K,T>   Expr;
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
    T& tmatrix_column_view_expr<N,M,I,J,K,T>::operator[](const unsigned short i)
    {
      return m(static_cast<unsigned short>(J+i),I);
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_column_view<N,M,I,J,K,T>::tmatrix_column_view(tmatrix<N,M,T>& m_)
      : VectorExpr<tvector<K,T>, tmatrix_column_view_expr<N,M,I,J,K,T> >(m_)
    {}

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    tmatrix_column_view_expr<N,M,I,J,K,T>::tmatrix_column_view_expr(tmatrix<N,M,T>& m_)
      : m(m_)
    {} // end of tmatrix_column_view_expr
    
    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    typename tmatrix_column_view_expr<N,M,I,J,K,T>::RunTimeProperties
    tmatrix_column_view_expr<N,M,I,J,K,T>::getRunTimeProperties(void) const
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
    tmatrix_column_view_expr<N,M,I,J,K,T>::operator()(const unsigned short i) const
    {
      return m(static_cast<unsigned short>(J+i),I);
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    T&
    tmatrix_column_view_expr<N,M,I,J,K,T>::operator()(const unsigned short i)
    {
      return m(static_cast<unsigned short>(J+i),I);
    }

    template<unsigned short N,
	     unsigned short M,
	     unsigned short I,
	     unsigned short J,
	     unsigned short K,
	     typename T>
    const T& 
    tmatrix_column_view_expr<N,M,I,J,K,T>::operator[](const unsigned short i) const
    {
      return m(static_cast<unsigned short>(J+i),I);
    }

  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TMATRIX_COLUMN_VIEW_IXX */
