#ifndef _VECTOR_CONCEPT_LIB_
#define _VECTOR_CONCEPT_LIB_ 1

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"

namespace tfel{

  namespace math{
    
    template<typename T>
    struct VectorTraits{
      typedef tfel::meta::InvalidType NumType;
      typedef tfel::meta::InvalidType IndexType;
      typedef tfel::meta::InvalidType RunTimeProperties;
    };

    /*!
     * \class VectorTag
     * \brief Helper class to characterise vectors.
     */ 
    struct VectorTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string 
      getName(void);

    }; // end of VectorTag
    
    template<typename T>
    class VectorConcept 
    {

      typedef VectorTraits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      ~VectorConcept(){};
      
    public :
    
      typedef VectorTag ConceptTag;
  
      static
      std::string
      getName(void);
      
      TFEL_MATH_INLINE 
      ValueType
      operator()(const typename traits::IndexType) const ;

      TFEL_MATH_INLINE 
      ValueType
      operator[](const typename traits::IndexType) const ;
      
    };

    template<typename T>
    struct VectorType
    {
      typedef T type;
    };
  
  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Vector/VectorConcept.ixx"

#endif /* _VECTOR_CONCEPT_LIB_ */
