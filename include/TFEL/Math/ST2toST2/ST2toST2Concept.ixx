#ifndef _ST2TOST2_CONCEPT_IMPL_
#define _ST2TOST2_CONCEPT_IMPL_ 1

#include"TFEL/Math/functions.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    std::string
    ST2toST2Concept<T>::getName(void)
    {
      using namespace std;
      using namespace tfel::utilities;
      return string("ST2toST2Concept<")+Name<T>::getName()+string(">");
    } // end of ST2toST2Concept<T>::getName(void)

    template<class T>
    TFEL_MATH_INLINE 
    typename ST2toST2Concept<T>::ValueType
    ST2toST2Concept<T>::operator()(const unsigned short i,
				   const unsigned short j) const
    {
      return static_cast<const T&>(*this).operator()(i,j);
    } // end of ST2toST2Concept<T>::operator()
    
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
