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
  /*!
   * \brief an helper class that simply exposes publically a member named
   * ConceptTag as an alias to T2toT2Tag.
   *
   * The main reason for this alias is to properly implement the `ConceptRebind`
   * metafunction.
   */
  template <typename T>
  struct T2toT2ConceptBase {
    using ConceptTag = T2toT2Tag;
  };
  /*!
   * \brief definition of the T2toT2Concept
   *
   * A class matching this concept must expose the `T2toT2Tag` and have
   * appropriate access operators.
   */
  template <typename T>
  concept T2toT2Concept =
      (std::is_same_v<typename std::decay_t<T>::ConceptTag, T2toT2Tag>)&&  //
      (requires(const T t, const unsigned short i, const unsigned short j) {
        t(i, j);
      });
  //! \brief partial specialisation for fourth order tensor
  template <typename Type>
  struct ConceptRebind<T2toT2Tag, Type> {
    using type = T2toT2ConceptBase<Type>;
  };
  /*!
   * \return the sum of the absolute values of all components of an
   * linear application transforming a tensor in a tensor
   * \param[in] t: fourth order tensor
   */
  TFEL_HOST_DEVICE constexpr auto abs(const T2toT2Concept auto&) noexcept;
  /*!
   * \return the determinant of a `t2tot2`
   * \param[in] t: fourth order tensor
   */
  TFEL_HOST_DEVICE constexpr auto det(const T2toT2Concept auto&) noexcept;
  /*!
   * \brief an helper function which returns if the given type implements the
   * `T2toT2Concept`.
   * \tparam T2toT2Type: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <T2toT2Concept T2toT2Type>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsT2toT2Concept() {
    return T2toT2Concept<T2toT2Type>;
  }  // end of implementsT2toT2Concept

}  // end of namespace tfel::math

#include "TFEL/Math/T2toT2/T2toT2Concept.ixx"

#endif /* LIB_TFEL_MATH_T2TOT2CONCEPT_HXX */
