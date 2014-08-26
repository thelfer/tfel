/*!
 * \file   ST2toT2Concept.ixx  
 * \brief    
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_ST2TOT2_CONCEPT_IMPL_
#define _LIB_TFEL_MATH_ST2TOT2_CONCEPT_IMPL_ 1

#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    TFEL_MATH_INLINE 
    typename ST2toT2Concept<T>::ValueType
    ST2toT2Concept<T>::operator()(const unsigned short i,
				  const unsigned short j) const
    {
      return static_cast<const T&>(*this).operator()(i,j);
    } // end of ST2toT2Concept<T>::operator()

    template<typename ST2toT2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toT2Type,ST2toT2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toT2Traits<ST2toT2Type>::NumType>::type
    >::type
    abs(const ST2toT2Type& v)
    {
      unsigned short i;
      unsigned short j;
      typedef typename ST2toT2Traits<ST2toT2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for(i=0;i<TensorDimeToSize<ST2toT2Traits<ST2toT2Type>::dime>::value;++i){
	for(j=0;j<StensorDimeToSize<ST2toT2Traits<ST2toT2Type>::dime>::value;++j){
	  a += abs(v(i,j));
	}
      }
      return a;
    }
    
    template<typename T>
    std::ostream &
    operator << (std::ostream & os,
		 const ST2toT2Concept<T>& s)
    {
      unsigned short i;
      unsigned short j;
      os << "[";
      for(i=0;i<StensorDimeToSize<ST2toT2Traits<T>::dime>::value;++i){
	if(i!=0){
	  os << " [";
	} else {
	  os << "[";
	}
	for(j=0;j<StensorDimeToSize<ST2toT2Traits<T>::dime>::value;++j){
	  os << s(i,j);
	  if(j!=StensorDimeToSize<ST2toT2Traits<T>::dime>::value-1){
	    os << ",";
	  }
	}
	if(i!=TensorDimeToSize<ST2toT2Traits<T>::dime>::value-1){
	  os << "]\n";
	} else {
	  os << "]";
	}
      }
      os << "]";
      return os;
    } // end of operator <<

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ST2TOT2_CONCEPT_IMPL_ */
