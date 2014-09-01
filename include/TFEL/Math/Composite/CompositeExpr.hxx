/*!
 * \file   include/TFEL/Math/Composite/CompositeExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_COMPOSITEEXPR_HXX_
#define _LIB_TFEL_COMPOSITEEXPR_HXX_ 

#include<string>
#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/Composite/CompositeConcept.hxx"

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

      explicit TFEL_MATH_INLINE CompositeExpr()
	: Expr()
      {}

      TFEL_MATH_INLINE CompositeExpr(const CompositeExpr& src)
	: Expr(src)	
      {}

      explicit TFEL_MATH_INLINE CompositeExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE CompositeExpr(typename Expr::first_arg&  a_, 
					      typename Expr::second_arg& b_)
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

#include"TFEL/Math/Composite/CompositeCompositeExpr.hxx"
#include"TFEL/Math/Composite/ScalarCompositeExpr.hxx"
#include"TFEL/Math/Composite/CompositeScalarExpr.hxx"
#include"TFEL/Math/Composite/FctCompositeExpr.hxx"
#include"TFEL/Math/Composite/CompositeNegExpr.hxx"

#endif /* _LIB_TFEL_COMPOSITEEXPR_HXX */

