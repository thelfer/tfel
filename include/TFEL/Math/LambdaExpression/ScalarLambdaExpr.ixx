/*!
 * \file   include/TFEL/Math/LambdaExpression/ScalarLambdaExpr.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#ifndef _LIB_TFEL_SCALARLAMBDAEXPR_IXX_
#define _LIB_TFEL_SCALARLAMBDAEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N,typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExprImpl
    {
      const Scalar a;
      const Lambda b;
      
      ScalarLambdaExprImpl();

    protected:

      ScalarLambdaExprImpl(const Scalar a_,const Lambda b_)
	: a(a_),b(b_)
      {}

      template<unsigned short M,typename T>
      typename tfel::meta::EnableIf<
	(M>=N),T>::type
      operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a,this->b(v));
      }

    };

    template<typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExprImpl<1u,Scalar,Lambda,Op>
    {

      const Scalar a;
      const Lambda b;
      
      ScalarLambdaExprImpl();

    protected:

      ScalarLambdaExprImpl(const Scalar a_,const Lambda b_)
	: a(a_),b(b_)
      {}
      
       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=1u),
	   T>::type
       operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a,this->b(v));
      }

       template<typename T>
       typename tfel::meta::EnableIf<
	 tfel::typetraits::IsScalar<T>::cond,
	 T>::type
       operator()(const T s) const
      {
	return Op::apply(this->a,this->b(s));
      }

    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_SCALARLAMBDAEXPR_IXX */

