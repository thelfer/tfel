/*! 
 * \file  VectorVectorDotProduct.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 04 fév 2010
 */

#ifndef   _LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_H_
#define   _LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"

namespace tfel
{
  
  namespace math
  {

    struct VectorVectorDotProduct
    {
      template<typename Result,
	       typename T1,
	       typename T2>
      TFEL_MATH_INLINE
      static Result
      exe(const T1& v1,
	  const T2& v2)
      {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	typedef typename T1::RunTimeProperties RunTimeProperties;
	RunTimeCheck<RunTimeProperties>::exe(v1.getRunTimeProperties(),
					     v2.getRunTimeProperties());
#endif
	typedef typename T1::size_type size_type;
	size_type i;
	Result r(0);
	for(i=0;i!=v1.size();++i){
	  r += v1(i)*v2(i);
	}
	return r;
      } // end of VectorVectorDotProduct::exe

    }; // end of struct VectorVectorDotProduct

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_MATH_VECTORVECTORDOTPRODUCT_H */

