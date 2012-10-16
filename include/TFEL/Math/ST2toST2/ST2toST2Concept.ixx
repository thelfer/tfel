#ifndef _ST2TOST2_CONCEPT_IMPL_
#define _ST2TOST2_CONCEPT_IMPL_ 1

#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    TFEL_MATH_INLINE 
    typename ST2toST2Concept<T>::ValueType
    ST2toST2Concept<T>::operator()(const unsigned short i,
				   const unsigned short j) const
    {
      return static_cast<const T&>(*this).operator()(i,j);
    } // end of ST2toST2Concept<T>::operator()

    template<typename ST2toST2Type>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond,
      typename tfel::typetraits::AbsType<typename ST2toST2Traits<ST2toST2Type>::NumType>::type
    >::type
    abs(const ST2toST2Type& v)
    {
      unsigned short i;
      unsigned short j;
      typedef typename ST2toST2Traits<ST2toST2Type>::NumType NumType;
      typedef typename tfel::typetraits::AbsType<NumType>::type AbsNumType;
      AbsNumType a(0);
      for(i=0;i<StensorDimeToSize<ST2toST2Traits<ST2toST2Type>::dime>::value;++i){
	for(j=0;j<StensorDimeToSize<ST2toST2Traits<ST2toST2Type>::dime>::value;++j){
	  a += abs(v(i,j));
	}
      }
      return a;
    }
    
    template<typename T>
    std::ostream &
    operator << (std::ostream & os,
		 const ST2toST2Concept<T>& s)
    {
      unsigned short i;
      unsigned short j;
      os << "[";
      for(i=0;i<StensorDimeToSize<ST2toST2Traits<T>::dime>::value;++i){
	if(i!=0){
	  os << " [";
	} else {
	  os << "[";
	}
	for(j=0;j<StensorDimeToSize<ST2toST2Traits<T>::dime>::value;++j){
	  os << s(i,j);
	  if(j!=StensorDimeToSize<ST2toST2Traits<T>::dime>::value-1){
	    os << ",";
	  }
	}
	if(i!=StensorDimeToSize<ST2toST2Traits<T>::dime>::value-1){
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

#endif /* _ST2TOST2_CONCEPT_IMPL_ */
