/*!
 * \file   include/TFEL/Math/Matrix/MatrixExpr.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _TFEL_MATH_MATRIX_EXPR_LIB_
#define _TFEL_MATH_MATRIX_EXPR_LIB_ 1

#include <string>

#include"TFEL/Config/TFELConfig.hxx"
#include<type_traits>
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct MatrixExpr;

    template<typename T_type, typename Expr>
    struct MatrixTraits<MatrixExpr<T_type,Expr> >{
      typedef typename MatrixTraits<T_type>::NumType   NumType;
      typedef typename MatrixTraits<T_type>::IndexType IndexType;
      typedef typename MatrixTraits<T_type>::RunTimeProperties RunTimeProperties;
    };

    template<typename T_type, typename Expr>
    struct  MatrixExpr
      : public MatrixConcept<MatrixExpr<T_type,Expr> >,
	public Expr
    {

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type        value_type;      
      typedef typename Expr::pointer	       pointer;	    
      typedef typename Expr::const_pointer     const_pointer; 
      typedef typename Expr::reference	       reference;	    
      typedef typename Expr::const_reference   const_reference;
      typedef typename Expr::size_type 	       size_type;	    
      typedef typename Expr::difference_type   difference_type;

      explicit TFEL_MATH_INLINE MatrixExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE MatrixExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE MatrixExpr(typename Expr::first_arg& a_, 
					   typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}
      
      using Expr::operator();

      using Expr::operator=;
    };
    
    template<typename T_type, typename Expr>
    struct MatrixType<MatrixExpr<T_type,Expr> >
    {
      typedef T_type type;
    };
      
  } // end of namespace math

  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::MatrixExpr<T_type,Expr> >
    {
      static constexpr bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _TFEL_MATH_MATRIX_EXPR_LIB_ */
