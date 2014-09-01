/*!
 * \file   include/TFEL/Math/LambdaExpression/FctLambdaExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_FCTLAMBDAEXPR_HXX_
#define _LIB_TFEL_FCTLAMBDAEXPR_HXX_ 

#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

namespace tfel{

  namespace math{

    template<typename Lambda,typename Fct>
    class FctLambdaExpr;

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
    class FctLambdaExprImpl;

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

#include"TFEL/Math/LambdaExpression/FctLambdaExpr.ixx"

#endif /* _LIB_TFEL_FCTLAMBDAEXPR_HXX */
