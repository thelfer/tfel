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
#include "TFEL/Math/General/ResultType.hxx"
#include "TFEL/Math/General/ConceptRebind.hxx"
#include "TFEL/Math/General/BasicOperations.hxx"
#include "TFEL/Math/Forward/VectorConcept.hxx"

namespace tfel {

  namespace math {

    template <typename T>
    struct VectorTraits {
      typedef tfel::meta::InvalidType NumType;
      typedef tfel::meta::InvalidType IndexType;
      typedef tfel::meta::InvalidType RunTimeProperties;
    };

    //! a simple alias
    template <typename T>
    using VectorNumType = typename VectorTraits<T>::NumType;

    /*!
     * \class VectorTag
     * \brief Helper class to characterise vectors.
     */
    struct VectorTag {};  // end of VectorTag

    template <typename T>
    struct VectorConcept {
      typedef VectorTag ConceptTag;

      TFEL_MATH_INLINE typename VectorTraits<T>::NumType operator()(
          const typename VectorTraits<T>::IndexType) const;

      TFEL_MATH_INLINE typename VectorTraits<T>::NumType operator[](
          const typename VectorTraits<T>::IndexType) const;

     protected:
      VectorConcept() = default;
      VectorConcept(VectorConcept&&) = default;
      VectorConcept(const VectorConcept&) = default;
      VectorConcept& operator=(const VectorConcept&) = default;
      ~VectorConcept() = default;
    };

    //! paratial specialisation for vectors
    template <typename Type>
    struct ConceptRebind<VectorTag, Type> {
      using type = VectorConcept<Type>;
    };

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Vector/VectorConcept.ixx"
#include "TFEL/Math/Vector/VectorConceptOperations.hxx"

#endif /* LIB_TFEL_MATH_VECTOR_CONCEPT_HXX */
