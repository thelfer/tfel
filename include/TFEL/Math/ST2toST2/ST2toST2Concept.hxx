/*! 
 * \file  include/TFEL/Math/ST2toST2/ST2toST2Concept.hxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _ST2TOST2_CONCEPT_LIB_
#define _ST2TOST2_CONCEPT_LIB_ 1

#include<type_traits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/General/ConceptRebind.hxx"
#include"TFEL/Math/Forward/ST2toST2Concept.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct ST2toST2Traits{
      typedef tfel::meta::InvalidType NumType;
      static constexpr unsigned short dime = 0u;
    };

    /*!
     * \class ST2toST2Tag
     * \brief Helper class to characterise st2tost2.
     */ 
    struct ST2toST2Tag
    {};

    template<class T>
    struct ST2toST2Concept 
    {
      typedef ST2toST2Tag ConceptTag;

      typename ST2toST2Traits<T>::NumType
      operator()(const unsigned short,
		 const unsigned short) const;

    protected:
      ST2toST2Concept() = default;
      ST2toST2Concept(ST2toST2Concept&&) = default;
      ST2toST2Concept(const ST2toST2Concept&) = default;
      ST2toST2Concept&
      operator=(const ST2toST2Concept&) = default;
      ~ST2toST2Concept() = default;
    };

    template<typename T>
    struct ST2toST2Type{
      typedef T type;
    };

    template<typename T>
    struct ConceptRebind<ST2toST2Tag,T>
    {
      typedef ST2toST2Concept<T> type;
    };

    template<typename ST2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toST2Traits<ST2toST2Type>::NumType>::type
    >::type
    abs(const ST2toST2Type&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/ST2toST2/ST2toST2Concept.ixx"

#endif /* _ST2TOST2_CONCEPT_LIB_ */
