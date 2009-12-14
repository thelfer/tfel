/*!
 * \file   FctLambdaExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 */

#ifndef _LIB_TFEL_FCTLAMBDAEXPR_HXX_
#define _LIB_TFEL_FCTLAMBDAEXPR_HXX_ 

#include"Math/LambdaExpression/LambdaConcept.hxx"

namespace tfel{

  namespace math{

    template<typename Lambda,typename Fct>
    struct FctLambdaExpr;

    template<typename Lambda,typename Fct>
    struct LambdaTraits<FctLambdaExpr<Lambda,Fct> >{
      /*
       * minimal dimension of the argument.
       */
      static const unsigned short dimension = LambdaTraits<Lambda>::dimension; 
      /*
       * variables list
       */
      typedef typename LambdaTraits<Lambda>::Variables Variables; 
    }; // end of LambdaTraits

    /*									                
     * Partial Specialisation of ComputeUnaryResult_ for lambdas	                
     */									                
    template<typename A,typename Fct>
    struct ComputeUnaryResult_<LambdaTag,FunctionTag,A,Fct>
    {
      typedef FctLambdaExpr<A,Fct> Result;
      typedef FctLambdaExpr<A,Fct> Handle;
    };

    template<unsigned short N,typename Lambda,typename Fct>
    struct FctLambdaExprImpl;

    template<typename Lambda,typename Fct>
    class FctLambdaExpr
      : public LambdaConcept<FctLambdaExpr<Lambda,Fct> >,
	public FctLambdaExprImpl<LambdaTraits<FctLambdaExpr<Lambda,Fct> >::dimension,Lambda,Fct>
    {

      FctLambdaExpr();

      static const unsigned short 
      dimension =LambdaTraits<FctLambdaExpr<Lambda,Fct> >::dimension;
	
      typedef FctLambdaExprImpl<dimension,Lambda,Fct> Impl;

    public:

      FctLambdaExpr(const Lambda a_,const Fct b_)
	: Impl(a_,b_)
      {}
	
      using Impl::operator();
    };

  } // end of namespace tfel

} // end of namespace math

#include"Math/LambdaExpression/FctLambdaExpr.ixx"

#endif /* _LIB_TFEL_FCTLAMBDAEXPR_HXX */
