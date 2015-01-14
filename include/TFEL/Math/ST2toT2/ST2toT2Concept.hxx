/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2Concept.hxx  
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

#ifndef _TFEL_MATH_ST2TOT2_CONCEPT_LIB_
#define _TFEL_MATH_ST2TOT2_CONCEPT_LIB_ 1

#include<iosfwd>
#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Forward/ST2toT2Concept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ST2toT2Traits{
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };

    /*!
     * \class ST2toT2Tag
     * \brief Helper class to characterise st2tot2.
     */ 
    struct ST2toT2Tag
    {};

    template<class T>
    struct ST2toT2Concept 
    {

    private:

      typedef ST2toT2Traits<T> traits;
      static constexpr bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename std::conditional<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;

    protected:
      ST2toT2Concept() = default;
      ST2toT2Concept(ST2toT2Concept&&) = default;
      ST2toT2Concept(const ST2toT2Concept&) = default;
      ST2toT2Concept&
      operator=(const ST2toT2Concept&) = default;
      ~ST2toT2Concept() = default;
    public:
      
      typedef ST2toT2Tag ConceptTag;

      ValueType
      operator()(const unsigned short,
		 const unsigned short) const;
      
    };

    // Serialisation operator
    template<typename T>
    std::ostream &
    operator << (std::ostream &,
		 const ST2toT2Concept<T>&);

    template<typename T>
    struct ST2toT2Type{
      typedef T type;
    };

    template<typename ST2toT2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toT2Traits<ST2toT2Type>::NumType>::type
    >::type
    abs(const ST2toT2Type&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/ST2toT2/ST2toT2Concept.ixx"

#endif /* _LIB_TFEL_MATH_ST2TOT2_CONCEPT_LIB_ */
