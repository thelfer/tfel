/*!
 * \file   include/TFEL/Math/Stensor/StensorExpr.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _MATH_STENSOR_EXPR_LIB_
#define _MATH_STENSOR_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct StensorExpr;

    template<typename T_type, typename Expr>
    struct StensorTraits<StensorExpr<T_type,Expr> >{
      typedef typename StensorTraits<T_type>::NumType   NumType;
      typedef typename StensorTraits<T_type>::IndexType IndexType;
      static const unsigned short dime = StensorTraits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct StensorExpr
      : public StensorConcept<StensorExpr<T_type,Expr> >,
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

      explicit TFEL_MATH_INLINE StensorExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE StensorExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE StensorExpr(typename Expr::first_arg& a_, 
					    typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

      using Expr::operator=;
    };

    template<typename T_type, typename Expr>
    struct StensorType<StensorExpr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math
  
  namespace typetraits{

    /*!
     * \brief partial specialisation of the IsTemporary traits class
     * for objects of type StensorExpr.
     */
    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::StensorExpr<T_type,Expr> >
    {
      //! the result of the metafunction
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _MATH_STENSOR_EXPR_LIB_ */
