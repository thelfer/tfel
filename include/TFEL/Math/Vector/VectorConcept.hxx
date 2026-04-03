/*!
 * \file   include/TFEL/Math/Vector/VectorConcept.hxx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
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
  struct VectorConceptBase {
    using ConceptTag = VectorTag;
  };

  /*!
   * \brief definition of the VectorConcept concept
   * a class matching the vector concept must expose the `VectorTag` and have
   * access operators.
   */
  template <typename VectorType>
  concept VectorConcept =
      (std::is_same_v<typename std::decay_t<VectorType>::ConceptTag,
                      VectorTag>)&&  //
      (requires(const VectorType t, const index_type<VectorType> i) {
        t[i];
      }) &&  //
      (requires(const VectorType t, const index_type<VectorType> i) { t(i); });

  //! paratial specialisation for vectors
  template <typename Type>
  struct ConceptRebind<VectorTag, Type> {
    using type = VectorConceptBase<Type>;
  };

  //! \brief a simple alias for backward compatibility with versions prior
  //! to 4.0
  template <typename VectorType>
  using VectorTraits =
      std::conditional_t<VectorConcept<VectorType>,
                         MathObjectTraits<VectorType>,
                         MathObjectTraits<tfel::meta::InvalidType>>;

  /*!
   * \brief an helper function which returns if the given type implements the
   * `VectorConcept`.
   * \tparam VectorType: type tested
   * \note function given for backward compatibility with versions prior
   * to 5.0
   */
  template <typename VectorType>
  [[deprecated]] TFEL_HOST_DEVICE constexpr bool implementsVectorConcept() {
    return VectorConcept<VectorType>;
  }  // end of implementsVectorConcept

}  // end of namespace tfel::math

#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

#endif /* LIB_TFEL_MATH_VECTOR_CONCEPT_HXX */
