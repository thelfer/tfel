/*!
 * \file   ArrayConcept.hxx
 * \author Helfer Thomas
 */

#ifndef _TFEL_MATH_ARRAY_CONCEPT_LIB_
#define _TFEL_MATH_ARRAY_CONCEPT_LIB_ 1

#include <string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Metaprogramming/InvalidType.hxx"

#include"TFEL/Math/Forward/ArrayConcept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ArrayTraits{
      typedef tfel::meta::InvalidType NumType;
      typedef tfel::meta::InvalidType IndexType;
      static const unsigned short dimension = 0; 
    };

    template<unsigned short N,typename T>
    struct ArrayConcept_impl
    {};
  
    /*!
     * \class ArrayTag
     * \brief Helper class to characterise arrays.
     */ 
    struct ArrayTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);

    }; // end of arrays

    template<class T>
    struct ArrayConcept
      : public ArrayConcept_impl<ArrayTraits<T>::dimension,T>
    {
      
    private:
      typedef ArrayTraits<T> traits;

    protected:
      ~ArrayConcept(){};

    public:

      typedef ArrayTag ConceptTag;
      
    };
        
    template<typename T>
    struct ArrayType
    {
      typedef T type;
    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Array/ArrayConcept.ixx"

#endif /* _TFEL_MATH_ARRAY_CONCEPT_LIB_ */
