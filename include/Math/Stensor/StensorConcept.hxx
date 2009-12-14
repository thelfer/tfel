#ifndef _STENSOR_CONCEPT_LIB_
#define _STENSOR_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include "Config/TFELConfig.hxx"

#include "Utilities/Name.hxx"

#include "Metaprogramming/EnableIf.hxx"
#include "Metaprogramming/Implements.hxx"
#include "Metaprogramming/InvalidType.hxx"
#include "Metaprogramming/IF.hxx"
#include "TypeTraits/IsTemporary.hxx"
#include "TypeTraits/BaseType.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct StensorTraits{
      typedef tfel::meta::InvalidType NumType;
      static const unsigned short dime = 0u;
    };

    /*!
     * \class StensorTag
     * \brief Helper class to characterise stensors.
     */ 
    struct StensorTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static const std::string 
      getName(void);
    }; // end of StensorTag
    
    template<class T>
    class StensorConcept 
    {
      
      typedef StensorTraits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;
      
    protected:
      ~StensorConcept(){};
      
    public:

      typedef StensorTag ConceptTag;
      
      static
      std::string 
      getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorConcept<")+Name<T>::getName()+string(">");
      }

      ValueType 
      operator()(const unsigned short) const;

      ValueType 
      operator[](const unsigned short) const;
      
    };
  
    template<typename T>
    struct StensorType{
      typedef T type;
    };

    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    trace(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    sigmaeq(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorType<T>::type
    >::type
    deviator(const T& s)
    {
      typedef typename StensorType<T>::type Res;
      typedef typename StensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type Base;
      return Res(s-(Base(1)/Base(3))*trace(s)*Res::Id());
    }

    // Serialisation operator
    template<typename T>
    std::ostream&
    operator << (std::ostream &,const StensorConcept<T>&);

  } // end of namespace math

} // end of namespace tfel

#include "Math/Stensor/StensorConcept.ixx"

#endif /* _STENSOR_CONCEPT_LIB_ */
