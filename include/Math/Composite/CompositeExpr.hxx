/*!
 * \file   CompositeExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITEEXPR_HXX_
#define _LIB_TFEL_COMPOSITEEXPR_HXX_ 

#include<string>
#include"Config/TFELConfig.hxx"
#include"Utilities/Name.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include"Math/Composite/CompositeConcept.hxx"

namespace tfel{

  namespace math{

    template<typename Res,typename Expr>
    struct CompositeExpr;
    
    template<typename Res,typename Expr>
    struct CompositeTraits<CompositeExpr<Res,Expr> >
    {
      typedef typename Expr::Items Items;
    };

    
    template<typename Res,typename Expr>
    struct CompositeExpr
      : public CompositeConcept<CompositeExpr<Res,Expr> >,
	public Expr
    {
      typedef typename Expr::Items Items;
      
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("CompositeExpr<")
	  +Name<Res>::getName()+string(",")
	  +Name<Expr>::getName()+string(">");
      }

      explicit TFEL_MATH_INLINE CompositeExpr()
	: Expr()
      {}

      TFEL_MATH_INLINE CompositeExpr(const CompositeExpr& src)
	: Expr(src)	
      {}

      explicit TFEL_MATH_INLINE CompositeExpr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE CompositeExpr(const typename Expr::first_arg&  a_, 
					      const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::getComponent;

    };

    template<typename Res, typename Expr>
    struct CompositeType<CompositeExpr<Res,Expr> >
    {
      typedef Res type;
    };
    
  } // end of namespace math

  namespace typetraits{

    template<typename Res, typename Expr>
    struct IsTemporary<tfel::math::CompositeExpr<Res,Expr> >
    {
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of namespace tfel  

#include"Math/Composite/CompositeCompositeExpr.hxx"
#include"Math/Composite/ScalarCompositeExpr.hxx"
#include"Math/Composite/CompositeScalarExpr.hxx"
#include"Math/Composite/FctCompositeExpr.hxx"
#include"Math/Composite/CompositeNegExpr.hxx"

#endif /* _LIB_TFEL_COMPOSITEEXPR_HXX */

