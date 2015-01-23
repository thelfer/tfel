/*!
 * \file   include/TFEL/Math/Stensor/StensorConcept.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _STENSOR_CONCEPT_LIB_
#define _STENSOR_CONCEPT_LIB_ 1

#include<iosfwd>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

#include"TFEL/Math/General/Abs.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct StensorTraits{
      typedef tfel::meta::InvalidType NumType;
      typedef unsigned short          IndexType;
      static constexpr unsigned short dime = 0u;
    };

    /*!
     * \class StensorTag
     * \brief Helper class to characterise stensors.
     */ 
    struct StensorTag
    {}; // end of StensorTag
    
    template<class T>
    class StensorConcept 
    {
      
      typedef StensorTraits<T> traits;
      static constexpr bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename std::conditional<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;
      
    protected:
      StensorConcept() = default;
      StensorConcept(StensorConcept&&) = default;
      StensorConcept(const StensorConcept&) = default;
      StensorConcept&
      operator=(const StensorConcept&) = default;
      ~StensorConcept() = default;
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
    typename std::enable_if<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
    >::type
    abs(const StensorType&);

    template<class T>
    TFEL_MATH_INLINE 
    typename std::enable_if<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    trace(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    sigmaeq(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename std::enable_if<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorType<T>::type
    >::type
    deviator(const T&);

    //! Serialisation operator
    template<typename T>
    std::ostream&
    operator << (std::ostream &,const StensorConcept<T>&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Stensor/StensorConcept.ixx"

#endif /* _STENSOR_CONCEPT_LIB_ */
