/*!
 * \file   include/TFEL/Math/LambdaExpression/LambdaConcept.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 déc 2006
 */

#ifndef _LIB_TFEL_LAMBDACONCEPT_HXX_
#define _LIB_TFEL_LAMBDACONCEPT_HXX_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/Forward/LambdaConcept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct LambdaTraits{
      /*
       * minimal dimension of the argument (0 is not valid)
       */
      static const unsigned short dimension = 0u; 
      /*
       * variables list, empty by default
       */
      typedef tfel::meta::GenerateTypeList<>::type Variables; 
    }; // end of LambdaTraits

    template<unsigned short N,typename LambaExpr>
    struct LambdaConceptImpl;

    /*!
     * \class LambdaTag
     * \brief Helper class to characterise lambda expression.
     */ 
    struct LambdaTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);
    };

    template<typename LambdaExpr>
    struct LambdaConcept
      : public LambdaConceptImpl<LambdaTraits<LambdaExpr>::dimension,LambdaExpr>
    {
      typedef LambdaTag ConceptTag;
    }; // end of LambdaConcept

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/LambdaConcept.ixx"
#include"TFEL/Math/LambdaExpression/LambdaConceptOperations.hxx"

#endif /* _LIB_TFEL_LAMBDACONCEPT_HXX */

