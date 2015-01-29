/*!
 * \file   include/TFEL/Math/Vector/VectorConcept.hxx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _VECTOR_CONCEPT_LIB_
#define _VECTOR_CONCEPT_LIB_ 1

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Forward/VectorConcept.hxx"

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
    {}; // end of VectorTag
    
    template<typename T>
    struct VectorConcept 
    {

    private:

      typedef VectorTraits<T> traits;
      static constexpr bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename std::conditional<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;
    protected:
      VectorConcept() = default;
      VectorConcept(VectorConcept&&) = default;
      VectorConcept(const VectorConcept&) = default;
      VectorConcept&
      operator=(const VectorConcept&) = default;
      ~VectorConcept() = default;
    public :
    
      typedef VectorTag ConceptTag;
        
      TFEL_MATH_INLINE ValueType
      operator()(const typename traits::IndexType) const;

      TFEL_MATH_INLINE ValueType
      operator[](const typename traits::IndexType) const;
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
