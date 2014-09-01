/*!
 * \file   include/TFEL/Math/LambdaExpression/LambdaConcept.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 déc 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_LAMBDACONCEPT_IXX_
#define _LIB_TFEL_LAMBDACONCEPT_IXX_ 

namespace tfel{

  namespace math{

    template<unsigned short N,typename LambaExpr>
    struct LambdaConceptImpl
    {
      
       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=N),const T>::type
       operator()(const tvector<M,T>&) const;

    };

    template<typename LambdaExpr>
    struct LambdaConceptImpl<1u,LambdaExpr>
    {

       template<unsigned short M,typename T>
       typename tfel::meta::EnableIf<
	 (M>=1u),
	 const T>::type
       operator()(const tvector<M,T>&) const;

       template<typename T>
       typename tfel::meta::EnableIf<
	 tfel::typetraits::IsScalar<T>::cond,
	 const T>::type
       operator()(const T) const;

    };

    template<unsigned short N,typename LambdaExpr>
    template<unsigned short M,typename T>
    typename tfel::meta::EnableIf<
      (M>=N),const T>::type
    LambdaConceptImpl<N,LambdaExpr>::operator()(const tvector<M,T>& v) const
    {
      return static_cast<const LambdaExpr>(this)->operator()(v);
    }

    template<typename LambdaExpr>
    template<unsigned short M,typename T>
    typename tfel::meta::EnableIf<
      (M>=1u),const T>::type
    LambdaConceptImpl<1u,LambdaExpr>::operator()(const tvector<M,T>& v) const
    {
      return static_cast<const LambdaExpr>(this)->operator()(v);
    }

    template<typename LambdaExpr>
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      const T>::type
    LambdaConceptImpl<1u,LambdaExpr>::operator()(const T a) const
    {
      return static_cast<const LambdaExpr>(this)->operator()(a);
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_LAMBDACONCEPT_IXX */

