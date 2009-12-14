#ifndef _MATH_ARRAY_EXPR_LIB_
#define _MATH_ARRAY_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include "Config/TFELConfig.hxx"
#include "Metaprogramming/IF.hxx"
#include "Metaprogramming/EmptyClass.hxx"
#include "Metaprogramming/HasIterator.hxx"
#include "Metaprogramming/HasConstIterator.hxx"
#include "Utilities/Name.hxx"

#include "TypeTraits/IsTemporary.hxx"

#include "Math/General/BasicOperations.hxx"
#include "Math/Array/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    class ArrayExpr;

    template<typename T_type, typename Expr>
    struct ArrayTraits<ArrayExpr<T_type,Expr> >{
      typedef typename Expr::NumType NumType;
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

      static 
      const std::string
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayExpr<")+Name<T_type>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }

      explicit TFEL_MATH_INLINE ArrayExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE ArrayExpr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE ArrayExpr(const typename Expr::first_arg& a_, 
					  const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

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

#endif /* _MATH_ARRAY_EXPR_LIB_ */
