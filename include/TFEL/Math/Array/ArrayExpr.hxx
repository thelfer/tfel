/*!
 * \file   include/TFEL/Math/Array/ArrayExpr.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _TFEL_MATH_MATH_ARRAY_EXPR_LIB_
#define _TFEL_MATH_MATH_ARRAY_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"

#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    class ArrayExpr;

    template<typename T_type, typename Expr>
    struct ArrayTraits<ArrayExpr<T_type,Expr> >{
      typedef typename Expr::NumType   NumType;
      typedef typename Expr::IndexType IndexType;
      static const unsigned short dimension = ArrayTraits<T_type>::dimension; 
    };

    template<typename T_type, typename Expr>
    class ArrayExpr
      : public ArrayConcept<ArrayExpr<T_type,Expr> >,
	public Expr
    {

      typedef ArrayTraits<T_type> traits;
 
    public:

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      explicit TFEL_MATH_INLINE ArrayExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE ArrayExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE ArrayExpr(typename Expr::first_arg& a_, 
					  typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

      using Expr::operator=;
    };
        
    template<typename T_type, typename Expr>
    struct ArrayType<ArrayExpr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math
  
  namespace typetraits{
    
    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::ArrayExpr<T_type,Expr> >
    {
      static const bool cond = true;
    };
    
  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _TFEL_MATH_ARRAY_EXPR_LIB_ */
