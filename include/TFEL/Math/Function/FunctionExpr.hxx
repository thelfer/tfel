/*!
 * \file   include/TFEL/Math/Function/FunctionExpr.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   27 déc 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_FUNCTIONEXPR_HXX_
#define _LIB_TFEL_FUNCTIONEXPR_HXX_ 

#include<string>

namespace tfel{

  namespace math{

    /*
     * Forward declaration 
     * (needed since FunctionExpr is used in FunctionConcept declaration to define
     * function composition).
     */
    template<typename F>
    struct FunctionConcept; 

    template<typename Expr>
    class FunctionExpr
      : public FunctionConcept<FunctionExpr<Expr> >,
	public Expr
    {

      FunctionExpr& operator=(const FunctionExpr&);

    public:

      explicit TFEL_MATH_INLINE FunctionExpr()
	: Expr()
      {}

      TFEL_MATH_INLINE FunctionExpr(const FunctionExpr& src)
	: Expr(src)
      {}

      explicit TFEL_MATH_INLINE 
      FunctionExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE 
      FunctionExpr(typename Expr::first_arg&  a_, 
		   typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}
      
      using Expr::operator();
      
      ~FunctionExpr()
      {}

    }; // end of FunctionExpr

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_FUNCTIONEXPR_HXX */

