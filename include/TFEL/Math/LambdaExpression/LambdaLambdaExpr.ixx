/*!
 * \file   include/TFEL/Math/LambdaExpression/LambdaLambdaExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 déc 2006
 */

#ifndef _LIB_TFEL_LAMBDALAMBDAEXPR_IXX_
#define _LIB_TFEL_LAMBDALAMBDAEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N,typename Lambda1,typename Lambda2,typename Op>
    class LambdaLambdaExprImpl
    {
      const Lambda1 a;
      const Lambda2 b;
      
      LambdaLambdaExprImpl();

    protected:

      LambdaLambdaExprImpl(const Lambda1 a_,const Lambda2 b_)
	: a(a_),b(b_)
      {}

      template<unsigned short M,typename T>
      typename tfel::meta::EnableIf<
	(M>=N),T>::type
      operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a(v),this->b(v));
      }

    };

    template<typename Lambda1,typename Lambda2,typename Op>
    class LambdaLambdaExprImpl<1u,Lambda1,Lambda2,Op>
    {

      const Lambda1 a;
      const Lambda2 b;
      
      LambdaLambdaExprImpl();

    protected:

      LambdaLambdaExprImpl(const Lambda1 a_,const Lambda2 b_)
	: a(a_),b(b_)
      {}
      
       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=1u),
	   T>::type
       operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a(v),this->b(v));
      }

       template<typename T>
       typename tfel::meta::EnableIf<
	 tfel::typetraits::IsScalar<T>::cond,
	 T>::type
       operator()(const T s) const
      {
	return Op::apply(this->a(s),this->b(s));
      }

    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_LAMBDALAMBDAEXPR_IXX */

