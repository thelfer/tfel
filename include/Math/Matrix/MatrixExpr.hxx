#ifndef _TFEL_MATH_MATRIX_EXPR_LIB_
#define _TFEL_MATH_MATRIX_EXPR_LIB_ 1

#include <string>

#include "Config/TFELConfig.hxx"
#include "Utilities/Name.hxx"
#include "Metaprogramming/IF.hxx"
#include "Metaprogramming/EmptyClass.hxx"
#include "Metaprogramming/HasIterator.hxx"
#include "Metaprogramming/HasConstIterator.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "Math/General/BasicOperations.hxx"
#include "Math/Matrix/MatrixConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    class MatrixExpr;

    template<typename T_type, typename Expr>
    struct MatrixTraits<MatrixExpr<T_type,Expr> >{
      typedef typename Expr::NumType NumType;
      typedef typename MatrixTraits<T_type>::IndexType IndexType;
    };

    template<typename T_type, typename Expr>
    struct  MatrixExpr
      : public MatrixConcept<MatrixExpr<T_type,Expr> >,
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

      static 
      const std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("MatrixExpr<")+Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }

      explicit TFEL_MATH_INLINE MatrixExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE MatrixExpr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE MatrixExpr(const typename Expr::first_arg& a_, 
					   const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}
      
      using Expr::operator();

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
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _TFEL_MATH_MATRIX_EXPR_LIB_ */
