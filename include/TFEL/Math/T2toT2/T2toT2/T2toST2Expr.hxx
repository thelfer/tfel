/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/T2toST2Expr.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_T2TOST2_EXPR_LIB_
#define _LIB_TFEL_MATH_T2TOST2_EXPR_LIB_ 1

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/T2toST2/T2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct T2toST2Expr;

    template<typename T_type, typename Expr>
    struct T2toST2Traits<T2toST2Expr<T_type,Expr> >{
      typedef typename T2toST2Traits<T_type>::NumType   NumType;
      typedef unsigned short IndexType;
      static constexpr unsigned short dime = T2toST2Traits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct T2toST2Expr
      : public T2toST2Concept<T2toST2Expr<T_type,Expr> >,
	public Expr
    {

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      explicit T2toST2Expr() = default;

      explicit TFEL_MATH_INLINE T2toST2Expr(typename Expr::first_arg a_)
	: Expr(std::forward<typename Expr::first_arg>(a_))
      {}

      using Expr::operator();

      using Expr::operator=;
    };

    template<typename T_type, typename Expr>
    struct T2toST2Type<T2toST2Expr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math

} // end of TFEL namespace

#endif /* _LIB_TFEL_MATH_T2TOST2_EXPR_LIB_ */
