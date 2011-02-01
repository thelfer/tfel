/*!
 * \file   ScalarLambdaExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

#ifndef _LIB_TFEL_SCALARLAMBDAEXPR_HXX_
#define _LIB_TFEL_SCALARLAMBDAEXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Scalar,typename Lambda,typename Op>
    struct ScalarLambdaExpr;

    template<typename Scalar,typename Lambda,typename Op>
    struct LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >{
      /*
       * minimal dimension of the argument.
       */
      static const unsigned short dimension = LambdaTraits<Lambda>::dimension; 
      /*
       * variables list
       */
      typedef typename LambdaTraits<Lambda>::Variables Variables; 
    }; // end of LambdaTraits

    template<unsigned short N,typename Scalar,typename Lambda,typename Op>
    struct ScalarLambdaExprImpl;

    template<typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExpr
      : public LambdaConcept<ScalarLambdaExpr<Scalar,Lambda,Op> >,
	public ScalarLambdaExprImpl<LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >::dimension,
				    Scalar,Lambda,Op>
    {

      ScalarLambdaExpr();

      static const unsigned short 
      dimension =LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >::dimension;
	
      typedef ScalarLambdaExprImpl<dimension,Scalar,Lambda,Op> Impl;


    public:


      ScalarLambdaExpr(const Scalar a_, const Lambda b_)
	: Impl(a_,b_)
      {}
	
      using Impl::operator();
    };

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/ScalarLambdaExpr.ixx"  

#endif /* _LIB_TFEL_SCALARLAMBDAEXPR_HXX */

