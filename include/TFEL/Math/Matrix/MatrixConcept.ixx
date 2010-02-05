

#ifndef _TFEL_MATH_MATRIX_CONCEPT_IMPL_
#define _TFEL_MATH_MATRIX_CONCEPT_IMPL_ 1

namespace tfel{

  namespace math{
    
    template<class T>
    TFEL_MATH_INLINE 
    typename MatrixConcept<T>::ValueType
    MatrixConcept<T>::operator()(const typename MatrixTraits<T>::IndexType i,
				 const typename MatrixTraits<T>::IndexType j) const {
      return static_cast<const T&>(*this).operator()(i,j);
    }
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _TFEL_MATH_MATRIX_CONCEPT_IMPL_ */
