/*!
 * \file   include/TFEL/Math/Vector/VectorConcept.ixx
 * \author Thomas Helfer
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef TFEL_MATH_VECTOR_CONCEPT_IXX
#define TFEL_MATH_VECTOR_CONCEPT_IXX 1

namespace tfel{

  namespace math{
    
    template<class T>
    MathObjectNumType<T>
    VectorConcept<T>::operator()(const typename MathObjectTraits<T>::IndexType i) const
    {
      return static_cast<const T&>(*this).operator()(i);
    } // end of VectorConcept<T>::operator()

    template<class T>
    MathObjectNumType<T>
    VectorConcept<T>::operator[](const typename MathObjectTraits<T>::IndexType i) const
    {
      return static_cast<const T&>(*this).operator()(i);
    } // end of VectorConcept<T>::operator()
    
  } // end of namespace math

} // end of namespace tfel

#endif /* TFEL_MATH_VECTOR_CONCEPT_IXX */
