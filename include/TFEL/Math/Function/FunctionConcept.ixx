/*!
 * \file   include/TFEL/Math/Function/FunctionConcept.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_FUNCTIONCONCEPT_IXX_
#define _LIB_TFEL_FUNCTIONCONCEPT_IXX_ 

#include"TFEL/TypeTraits/IsUnaryOperator.hxx"
#include"TFEL/Math/Function/FunctionExpr.hxx"
#include"TFEL/Math/Function/FunctionsCompositionExpr.hxx"

namespace tfel{

  namespace math{

    struct OpDiff
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);

      template<typename F>
      static constexpr typename 
      std::enable_if<
	tfel::meta::Implements<F,FunctionConcept>::cond,
	typename ComputeUnaryResult<F,OpDiff>::Handle
      >::type
      apply(const F& a) 
      {
	return derivate(a);
      }

      template<typename F>
      const typename 
      std::enable_if<
	tfel::meta::Implements<F,FunctionConcept>::cond,
	typename ComputeUnaryResult<F,OpDiff>::Handle
      >::type
      operator[](const F& a) 
      {
	return derivate(a);
      }

    }; // end of OpDiff

    namespace stdfunctions{
      extern TFELMATH_VISIBILITY_EXPORT OpDiff D TFEL_UNUSED_ATTRIBUTE;
    } // end of namespace stdfunctions

    /*
     * Partial specialisation for differentiation of functions
     */
    template<typename Func>
    struct ComputeUnaryResult_<FunctionTag,UnaryOperatorTag,Func,OpDiff>
    {
      typedef typename FunctionTraits<Func>::DerivateFunc Result;
      typedef typename FunctionTraits<Func>::DerivateFunc Handle;
    }; // end of ComputeUnaryResult_

    template<typename Func>
    template<typename T>      			                                  
    typename std::enable_if<                                                      
      !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Func>::Result>::cond, 
      const typename ComputeUnaryResult<T,Func>::Handle                                
    >::type                                                                             
    FunctionConcept<Func>::operator()(const T& x) const{
      return static_cast<const Func&>(*this).operator()(x);
    } // end of operator FunctionConcept::operator()

    template<typename Func>
    template<typename F>
    TFEL_MATH_INLINE
    typename std::enable_if<
      tfel::meta::Implements<F,FunctionConcept>::cond,
      FunctionExpr<FunctionsCompositionExpr<Func,F> >
    >::type
    FunctionConcept<Func>::operator[](const F f) const
    {
      typedef FunctionExpr<FunctionsCompositionExpr<Func,F> > Res; 
      return Res(static_cast<const Func&>(*this),f);
    }

    
//     template<typename Func>
//     const typename FunctionTraits<Func>::DerivateFunc
//     FunctionConcept<Func>::d(void) const
//     {
//       return static_cast<const Func *>(this)->d();
//     }

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_FUNCTIONCONCEPT_IXX */

