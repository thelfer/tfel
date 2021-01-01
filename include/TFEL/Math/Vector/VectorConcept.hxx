/*!
 * \file   include/TFEL/Math/Vector/VectorConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_VECTOR_CONCEPT_HXX
#define LIB_TFEL_MATH_VECTOR_CONCEPT_HXX 1

#include <type_traits>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Metaprogramming/InvalidType.hxx"
#include "TFEL/Metaprogramming/Implements.hxx"
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/MathObjectTraits.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"

namespace tfel::math {

  /*!
   * \class VectorTag
   * \brief Helper class to characterise vectors.
   */
  struct VectorTag {};  // end of VectorTag

  template <typename T>
  struct VectorConcept {
    typedef VectorTag ConceptTag;

    TFEL_MATH_INLINE numeric_type<T> operator()(
        const index_type<T>) const;

    TFEL_MATH_INLINE numeric_type<T> operator[](
        const index_type<T>) const;

   protected:
    VectorConcept() = default;
    VectorConcept(VectorConcept&&) = default;
    VectorConcept(const VectorConcept&) = default;
    VectorConcept& operator=(const VectorConcept&) = default;
    ~VectorConcept() = default;
  };

  /*!
   * \brief an helper function which returns if the given type implements the
   * `VectorConcept`.
   * \tparam VectorType: type tested
   */
  template <typename VectorType>
  constexpr bool implementsVectorConcept() {
    return tfel::meta::implements<VectorType, VectorConcept>();
  }  // end of implementsVectorConcept

  //! paratial specialisation for vectors
  template <typename Type>
  struct ConceptRebind<VectorTag, Type> {
    using type = VectorConcept<Type>;
  };

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/VectorConcept.ixx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

#endif /* LIB_TFEL_MATH_VECTOR_CONCEPT_HXX */
