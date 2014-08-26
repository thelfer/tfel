/*!
 * \file   T2toT2Expr.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_T2TOT2_EXPR_LIB_
#define _LIB_TFEL_MATH_T2TOT2_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct T2toT2Expr;

    template<typename T_type, typename Expr>
    struct T2toT2Traits<T2toT2Expr<T_type,Expr> >{
      typedef typename T2toT2Traits<T_type>::NumType   NumType;
      typedef unsigned short IndexType;
      static const unsigned short dime = T2toT2Traits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct T2toT2Expr
      : public T2toT2Concept<T2toT2Expr<T_type,Expr> >,
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

      explicit TFEL_MATH_INLINE T2toT2Expr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE T2toT2Expr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE T2toT2Expr(const typename Expr::first_arg& a_, 
					    const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

      using Expr::operator=;
    };

    template<typename T_type, typename Expr>
    struct T2toT2Type<T2toT2Expr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math

  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::T2toT2Expr<T_type,Expr> >
    {
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _LIB_TFEL_MATH_T2TOT2_EXPR_LIB_ */
