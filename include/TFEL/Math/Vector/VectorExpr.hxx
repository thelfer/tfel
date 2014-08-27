/*!
 * \file   VectorExpr.hxx
 * \author Helfer Thomas
 */

#ifndef _MATH_VECTOR_EXPR_LIB_
#define _MATH_VECTOR_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    class VectorExpr;

    template<typename T_type, typename Expr>
    struct VectorTraits<VectorExpr<T_type,Expr> >{
      typedef typename Expr::NumType NumType;
      typedef typename VectorTraits<T_type>::IndexType IndexType;
      typedef typename VectorTraits<T_type>::RunTimeProperties RunTimeProperties;
    };

    template<typename T_type, typename Expr>
    class VectorExpr
      : public VectorConcept<VectorExpr<T_type,Expr> >,
	public Expr
    {

      VectorExpr& operator=(const VectorExpr&);

    public:

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      explicit TFEL_MATH_INLINE VectorExpr()
	: Expr()
      {}

      TFEL_MATH_INLINE VectorExpr(const VectorExpr& src)
	: Expr(src)	
      {}

      explicit TFEL_MATH_INLINE VectorExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE VectorExpr(typename Expr::first_arg&  a_, 
					   typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}
      
      using Expr::operator();

      using Expr::operator=;

      ~VectorExpr()
      {}

    };

    template<typename T_type, typename Expr>
    struct VectorType<VectorExpr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of math namespace
  
  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::VectorExpr<T_type,Expr> >
    {
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _MATH_VECTOR_EXPR_LIB_ */
