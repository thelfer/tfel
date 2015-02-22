/*!
 * \file   include/TFEL/Math/T2toT2/T2toT2/T2toST2Concept.ixx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_T2TOST2_CONCEPT_IMPL_
#define _LIB_TFEL_MATH_T2TOST2_CONCEPT_IMPL_ 1

#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    typename T2toST2Traits<T>::NumType NumType
    T2toST2Concept<T>::operator()(const unsigned short i,
				  const unsigned short j) const
    {
      return static_cast<const T&>(*this).operator()(i,j);
    } // end of T2toST2Concept<T>::operator()

    template<typename T2toST2Type>
    typename std::enable_if<
      tfel::meta::Implements<T2toST2Type,T2toST2Concept>::cond,
      typename tfel::typetraits::AbsType<typename T2toST2Traits<T2toST2Type>::NumType>::type
    >::type
    abs(const T2toST2Type& v)
    {
      unsigned short i;
      unsigned short j;
      typedef typename T2toST2Traits<T2toST2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for(i=0;i<StensorDimeToSize<T2toST2Traits<T2toST2Type>::dime>::value;++i){
	for(j=0;j<TensorDimeToSize<T2toST2Traits<T2toST2Type>::dime>::value;++j){
	  a += abs(v(i,j));
	}
      }
      return a;
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOST2_CONCEPT_IMPL_ */
