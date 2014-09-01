/*!
 * \file   include/TFEL/Math/LambdaExpression/LambdaScalarExpr.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 déc 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_LAMBDASCALAREXPR_IXX_
#define _LIB_TFEL_LAMBDASCALAREXPR_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N,typename Lambda,typename Scalar,typename Op>
    class LambdaScalarExprImpl
    {
      const Lambda a;
      const Scalar b;
      
      LambdaScalarExprImpl();

    protected:

      LambdaScalarExprImpl(const Lambda a_,const Scalar b_)
	: a(a_),b(b_)
      {}

      template<unsigned short M,typename T>
      typename tfel::meta::EnableIf<
	(M>=N),const T>::type
      operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a(v),this->b);
      }

    };

    template<typename Lambda,typename Scalar,typename Op>
    class LambdaScalarExprImpl<1u,Lambda,Scalar,Op>
    {

      const Lambda a;
      const Scalar b;
      
      LambdaScalarExprImpl();

    protected:

      LambdaScalarExprImpl(const Lambda a_,const Scalar b_)
	: a(a_),b(b_)
      {}
      
       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=1u),
	   T>::type
       operator()(const tvector<M,T>& v) const
      {
	return Op::apply(this->a(v),this->b);
      }

       template<typename T>
       typename tfel::meta::EnableIf<
	 tfel::typetraits::IsScalar<T>::cond,
	 T>::type
       operator()(const T s) const
      {
	return Op::apply(this->a(s),this->b);
      }

    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_LAMBDASCALAREXPR_IXX */

