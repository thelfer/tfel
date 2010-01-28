/*!
 * \file   FunctionConcept.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 nov 2006
 */

#ifndef _LIB_TFEL_FUNCTIONCONCEPT_HXX_
#define _LIB_TFEL_FUNCTIONCONCEPT_HXX_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/Math/General/ComputeUnaryResult.hxx"

namespace tfel{

  namespace math{

    template<typename Expr>
    struct FunctionExpr;

    template<typename F1,typename F2>
    struct FunctionsCompositionExpr;

    template<typename Func>
    struct FunctionTraits;
    
    /*!
     * \class FunctionTag
     * \brief Helper class to characterise functions.
     */ 
    struct FunctionTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string getName(void);
    }; // end of FunctionTag

    template<typename Func>
    struct FunctionConcept
    {
      typedef FunctionTag ConceptTag;

      static const std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("FunctionConcept<")+Name<Func>::getName()+string(">");
      }

      template<typename T>		                                  
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<                                                      
        !tfel::typetraits::IsInvalid<typename ComputeUnaryResult<T,Func>::Result>::cond, 
        const typename ComputeUnaryResult<T,Func>::Handle                                
      >::type                                                                             
      operator()(const T&) const;

      template<typename F>
      TFEL_MATH_INLINE
      typename tfel::meta::EnableIf<
	tfel::meta::Implements<F,tfel::math::FunctionConcept>::cond,
	FunctionExpr<FunctionsCompositionExpr<Func,F> >
      >::type
      operator[](const F f) const;

    }; // end of FunctionConcept

    struct OpDiff;

  } // end of namespace math

  namespace typetraits{

    /*
     * \brief Partial specialisation for OpDiff
     * \see IsUnaryOperator
     */
    template<>
    struct IsUnaryOperator<tfel::math::OpDiff>
    {
      static const bool cond = true;
    }; // end of IsUnaryOperator

  } // end of namespace typetraits
    
} // end of namespace tfel

#include"TFEL/Math/Function/FunctionConcept.ixx"
#include"TFEL/Math/Function/FunctionConceptOperations.hxx"

#endif /* _LIB_TFEL_FUNCTIONCONCEPT_HXX */

