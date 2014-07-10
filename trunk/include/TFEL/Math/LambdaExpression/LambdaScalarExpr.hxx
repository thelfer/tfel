/*!
 * \file   LambdaScalarExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

#ifndef _LIB_TFEL_LAMBDASCALAREXPR_HXX_
#define _LIB_TFEL_LAMBDASCALAREXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Lambda,typename Scalar,typename Op>
    class LambdaScalarExpr;

    template<typename Lambda,typename Scalar,typename Op>
    struct LambdaTraits<LambdaScalarExpr<Lambda,Scalar,Op> >{
      /*
       * minimal dimension of the argument.
       */
      static const unsigned short dimension = LambdaTraits<Lambda>::dimension; 
      /*
       * variables list
       */
      typedef typename LambdaTraits<Lambda>::Variables Variables; 
    }; // end of LambdaTraits

    template<unsigned short N,typename Lambda,typename Scalar,typename Op>
    class LambdaScalarExprImpl;

    template<typename Lambda,typename Scalar,typename Op>
    class LambdaScalarExpr
      : public LambdaConcept<LambdaScalarExpr<Lambda,Scalar,Op> >,
	public LambdaScalarExprImpl<LambdaTraits<LambdaScalarExpr<Lambda,Scalar,Op> >::dimension,
				    Lambda,Scalar,Op>
    {

      LambdaScalarExpr();

      static const unsigned short 
      dimension =LambdaTraits<LambdaScalarExpr<Lambda,Scalar,Op> >::dimension;
	
      typedef LambdaScalarExprImpl<dimension,Lambda,Scalar,Op> Impl;


    public:


      LambdaScalarExpr(const Lambda a_, const Scalar b_)
	: Impl(a_,b_)
      {}
	
      using Impl::operator();
    };

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/LambdaScalarExpr.ixx"  

#endif /* _LIB_TFEL_LAMBDASCALAREXPR_HXX */

