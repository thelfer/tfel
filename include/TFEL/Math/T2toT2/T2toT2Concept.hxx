/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2Concept.hxx
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

#ifndef LIB_TFEL_MATH_T2TOT2CONCEPT_HXX
#define LIB_TFEL_MATH_T2TOT2CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/Abs.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"

namespace tfel::math {

  /*!
   * \class T2toT2Tag
   * \brief Helper class to characterise t2tot2.
   */
  struct T2toT2Tag {};

  template <typename T>
  struct T2toT2Concept {
    typedef T2toT2Tag ConceptTag;

   protected:
    T2toT2Concept() = default;
    T2toT2Concept(T2toT2Concept&&) = default;
    T2toT2Concept(const T2toT2Concept&) = default;
    T2toT2Concept& operator=(const T2toT2Concept&) = default;
    ~T2toT2Concept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `T2toT2Concept`.
   * \tparam T2toT2Type: type tested
   */
  template <typename T2toT2Type>
  constexpr bool implementsT2toT2Concept() {
    return tfel::meta::implements<T2toT2Type, T2toT2Concept>();
  }  // end of implementsT2toT2Concept

  //! paratial specialisation for symmetric tensors
  template <typename Type>
  struct ConceptRebind<T2toT2Tag, Type> {
    using type = T2toT2Concept<Type>;
  };

  template <typename T2toT2Type>
  std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>(),
      typename tfel::typetraits::AbsType<numeric_type<T2toT2Type>>::type>
  abs(const T2toT2Type&);
  /*!
   * \return the determinant of a `st2tost2`
   * \param[in] s: fourth order tensor
   */
  template <typename T2toT2Type>
  std::enable_if_t<
      implementsT2toT2Concept<T2toT2Type>() &&
          isScalar<numeric_type<T2toT2Type>>(),
      typename ComputeUnaryResult<numeric_type<T2toT2Type>, Power<3>>::Result>
  det(const T2toT2Type&);

}  // end of namespace tfel::math

#include "TFEL/Math/T2toT2/T2toT2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOT2CONCEPT_HXX */
