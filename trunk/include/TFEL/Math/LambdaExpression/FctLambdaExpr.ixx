/*!
 * \file   FctLambaExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 jan 2007
 */

#ifndef _LIB_TFEL_FCTLAMBDAEXPR_IXX_
#define _LIB_TFEL_FCTLAMBDAEXPR_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N,typename Lambda,typename Fct>
    class FctLambdaExprImpl
    {
      const Lambda a;
      const Fct b;
      
      FctLambdaExprImpl();

    protected:

      FctLambdaExprImpl(const Lambda a_,const Fct b_)
	: a(a_),b(b_)
      {}

      template<unsigned short M,typename T>
      typename tfel::meta::EnableIf<
	(M>=N),const T>::type
      operator()(const tvector<M,T>& v) const
      {
	return (this->b)((this->a)(v));
      }

    };

    template<typename Lambda,typename Fct>
    class FctLambdaExprImpl<1u,Lambda,Fct>
    {

      const Lambda a;
      const Fct b;
      
      FctLambdaExprImpl();

    protected:

      FctLambdaExprImpl(const Lambda a_,const Fct b_)
	: a(a_),b(b_)
      {}
      
       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=1u),
	 const T>::type
       operator()(const tvector<M,T>& v) const
      {
	return (this->b)((this->a)(v));
      }

       template<typename T>
       typename tfel::meta::EnableIf<
	 tfel::typetraits::IsScalar<T>::cond,
	 T>::type
       operator()(const T s) const
      {
	return (this->b)((this->a)(s));
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_FCTLAMBAEXPR_IXX */

