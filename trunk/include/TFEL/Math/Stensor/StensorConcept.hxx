#ifndef _STENSOR_CONCEPT_LIB_
#define _STENSOR_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

#include"TFEL/Math/General/Abs.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct StensorTraits{
      typedef tfel::meta::InvalidType NumType;
      typedef unsigned short          IndexType;
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
      static std::string 
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
      
      ValueType 
      operator()(const unsigned short) const;

      ValueType 
      operator[](const unsigned short) const;
      
    };
  
    template<typename T>
    struct StensorType{
      typedef T type;
    };

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
    >::type
    abs(const StensorType&);

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

#include"TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* _STENSOR_CONCEPT_LIB_ */
