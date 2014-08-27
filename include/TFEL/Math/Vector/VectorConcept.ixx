/*!
 * \file   VectorConcept.ixx
 * \author Helfer Thomas
 */

#ifndef _VECTOR_CONCEPT_IMPL_
#define _VECTOR_CONCEPT_IMPL_ 1

namespace tfel{

  namespace math{
    
    template<class T>
    typename VectorConcept<T>::ValueType
    VectorConcept<T>::operator()(const typename VectorTraits<T>::IndexType i) const
    {
      return static_cast<const T&>(*this).operator()(i);
    } // end of VectorConcept<T>::operator()

    template<class T>
    typename VectorConcept<T>::ValueType
    VectorConcept<T>::operator[](const typename VectorTraits<T>::IndexType i) const
    {
      return static_cast<const T&>(*this).operator()(i);
    } // end of VectorConcept<T>::operator()
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _VECTOR_CONCEPT_IMPL_ */
