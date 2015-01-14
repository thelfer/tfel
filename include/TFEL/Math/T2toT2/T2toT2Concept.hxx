/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2Concept.hxx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _TFEL_MATH_T2TOT2_CONCEPT_LIB_
#define _TFEL_MATH_T2TOT2_CONCEPT_LIB_ 1

#include <ostream>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Forward/T2toT2Concept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct T2toT2Traits{
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };

    /*!
     * \class T2toT2Tag
     * \brief Helper class to characterise t2tot2.
     */ 
    struct T2toT2Tag
    {};

    template<class T>
    struct T2toT2Concept 
    {

    private:

      typedef T2toT2Traits<T> traits;
      static constexpr bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename std::conditional<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      T2toT2Concept() = default;
      T2toT2Concept(T2toT2Concept&&) = default;
      T2toT2Concept(const T2toT2Concept&) = default;
      T2toT2Concept&
      operator=(const T2toT2Concept&) = default;
      ~T2toT2Concept() = default;      
    public:
      
      typedef T2toT2Tag ConceptTag;

      ValueType
      operator()(const unsigned short,
		 const unsigned short) const;
      
    };

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const T2toT2Concept<T>&);

    template<typename T>
    struct T2toT2Type{
      typedef T type;
    };

    template<typename T2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond,
      typename tfel::typetraits::AbsType<typename T2toT2Traits<T2toT2Type>::NumType>::type
    >::type
    abs(const T2toT2Type&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/T2toT2/T2toT2Concept.ixx"

#endif /* _LIB_TFEL_MATH_T2TOT2_CONCEPT_LIB_ */
