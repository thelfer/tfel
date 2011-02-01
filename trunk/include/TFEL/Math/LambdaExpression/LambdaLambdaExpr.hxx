/*!
 * \file   LambdaLambdaExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

#ifndef _LIB_TFEL_LAMBDALAMBDAEXPR_HXX_
#define _LIB_TFEL_LAMBDALAMBDAEXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Lambda1,typename Lambda2,typename Op>
    struct LambdaLambdaExpr;

    template<typename Lambda1,typename Lambda2,typename Op>
    class LambdaTraits<LambdaLambdaExpr<Lambda1,Lambda2,Op> >{
      static const unsigned short dime1 = LambdaTraits<Lambda1>::dimension; 
      static const unsigned short dime2 = LambdaTraits<Lambda2>::dimension;
      typedef typename LambdaTraits<Lambda1>::Variables Vars1;
      typedef typename LambdaTraits<Lambda2>::Variables Vars2;
      typedef typename tfel::meta::TLConcatenate<Vars1,Vars2>::type Cons;
    public:
      static const unsigned short dimension = (dime1>dime2) ? dime1 : dime2;
      typedef typename tfel::meta::TLUnique<Cons>::type  Variables;
    };

    template<unsigned short N,typename Lambda1,typename Lambda2,typename Op>
    struct LambdaLambdaExprImpl;

    template<typename Lambda1,typename Lambda2,typename Op>
    class LambdaLambdaExpr
      : public LambdaConcept<LambdaLambdaExpr<Lambda1,Lambda2,Op> >,
	public LambdaLambdaExprImpl<LambdaTraits<LambdaLambdaExpr<Lambda1,Lambda2,Op> >::dimension,
				    Lambda1,Lambda2,Op>
    {
      LambdaLambdaExpr();

      static const unsigned short 
      dimension =LambdaTraits<LambdaLambdaExpr<Lambda1,Lambda2,Op> >::dimension;
	
      typedef LambdaLambdaExprImpl<dimension,Lambda1,Lambda2,Op> Impl;


    public:


      LambdaLambdaExpr(const Lambda1 a_, const Lambda2 b_)
	: Impl(a_,b_)
      {}
	
      using Impl::operator();
    };

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/LambdaLambdaExpr.ixx"  

#endif /* _LIB_TFEL_LAMBDALAMBDAEXPR_HXX */

