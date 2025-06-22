/*!
 * \file   include/TFEL/Math/ST2toT2/ST2toT2Concept.hxx
 * \brief
 * \author Thomas Helfer
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_ST2TOT2CONCEPT_HXX
#define LIB_TFEL_MATH_ST2TOT2CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"

namespace tfel::math {

  /*!
   * \class ST2toT2Tag
   * \brief Helper class to characterise st2tot2.
   */
  struct ST2toT2Tag {};

  template <class T>
  struct ST2toT2Concept {
    typedef ST2toT2Tag ConceptTag;

   protected:
    ST2toT2Concept() = default;
    ST2toT2Concept(ST2toT2Concept&&) = default;
    ST2toT2Concept(const ST2toT2Concept&) = default;
    ST2toT2Concept& operator=(const ST2toT2Concept&) = default;
    ~ST2toT2Concept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `ST2toT2Concept`.
   * \tparam ST2toT2Type: type tested
   */
  template <typename ST2toT2Type>
  constexpr bool implementsST2toT2Concept() {
    return tfel::meta::implements<ST2toT2Type, ST2toT2Concept>();
  }  // end of implementsST2toT2Concept

  //! paratial specialisation for symmetric tensors
  template <typename Type>
  struct ConceptRebind<ST2toT2Tag, Type> {
    using type = ST2toT2Concept<Type>;
  };

  template <typename ST2toT2Type>
  typename std::enable_if<
      implementsST2toT2Concept<ST2toT2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<ST2toT2Type>>::type>::type
  abs(const ST2toT2Type&);

}  // end of namespace tfel::math

#include "TFEL/Math/ST2toT2/ST2toT2Concept.ixx"

#endif /* LIB_TFEL_MATH_ST2TOT2CONCEPT_HXX */
