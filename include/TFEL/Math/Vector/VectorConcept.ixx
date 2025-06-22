/*!
 * \file   include/TFEL/Math/Vector/VectorConcept.ixx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_MATH_VECTOR_CONCEPT_IXX
#define TFEL_MATH_VECTOR_CONCEPT_IXX 1

namespace tfel {

  namespace math {

    template <class T>
    typename VectorTraits<T>::NumType VectorConcept<T>::operator()(
        const typename VectorTraits<T>::IndexType i) const {
      return static_cast<const T&>(*this).operator()(i);
    }  // end of VectorConcept<T>::operator()

    template <class T>
    typename VectorTraits<T>::NumType VectorConcept<T>::operator[](
        const typename VectorTraits<T>::IndexType i) const {
      return static_cast<const T&>(*this).operator()(i);
    }  // end of VectorConcept<T>::operator()

  }  // end of namespace math

}  // end of namespace tfel

#endif /* TFEL_MATH_VECTOR_CONCEPT_IXX */
