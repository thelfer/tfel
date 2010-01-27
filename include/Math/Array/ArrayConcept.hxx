#ifndef _TFEL_MATH_ARRAY_CONCEPT_LIB_
#define _TFEL_MATH_ARRAY_CONCEPT_LIB_ 1

#include <string>

#include "Config/TFELConfig.hxx"
#include"Metaprogramming/InvalidType.hxx"
#include"Metaprogramming/IF.hxx"
#include"TypeTraits/IsTemporary.hxx"
#include "Utilities/Name.hxx"

#include "Metaprogramming/InvalidType.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ArrayTraits{
      typedef tfel::meta::InvalidType NumType;
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
      static const std::string 
      getName(void);

    }; // end of arrays

    template<class T>
    class ArrayConcept
      : public ArrayConcept_impl<ArrayTraits<T>::dimension,T>
    {
      
    private:
      typedef ArrayTraits<T> traits;

    protected:
      ~ArrayConcept(){};

    public:

      typedef ArrayTag ConceptTag;
      
      static 
      const std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ArrayConcept<")+Name<T>::getName()+string(">");
      }

    };
        
    template<typename T>
    struct ArrayType
    {
      typedef T type;
    };

  } // end of namespace math

} // end of namespace tfel

#include "Math/Array/ArrayConcept.ixx"

#endif /* _TFEL_MATH_ARRAY_CONCEPT_LIB_ */
