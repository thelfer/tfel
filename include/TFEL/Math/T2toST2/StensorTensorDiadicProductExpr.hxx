/*! 
 * \file  StensorTensorDiadicProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2014
 */

#ifndef _LIB_TFEL_MATH_STENSORTENSORDIADICPRODUCTEXPR_H_
#define _LIB_TFEL_MATH_STENSORTENSORDIADICPRODUCTEXPR_H_ 

#include<string>
#include<cstddef>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel
{

  namespace math
  {

    template<typename StensorType,
	     typename TensorType>
    struct TFEL_VISIBILITY_LOCAL StensorTensorDiadicProductExpr
    {

    protected:

      typedef typename ComputeBinaryResult<StensorType,TensorType,OpDiadicProduct>::Result Result;
      typedef typename StensorTraits<StensorType>::IndexType AIndexType;
      typedef typename TensorTraits<TensorType>::IndexType BIndexType;
      typedef typename StensorTraits<StensorType>::NumType NumTypeA;
      typedef typename TensorTraits<TensorType>::NumType NumTypeB;

    public:

      typedef typename Result::RunTimeProperties RunTimeProperties; 
      typedef typename T2toST2Traits<Result>::IndexType IndexType;
      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;

      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return this->RTP;
      }

    protected:

      typedef const StensorType first_arg;
      typedef const TensorType  second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE StensorTensorDiadicProductExpr(const StensorType& l,
						      const TensorType& r)
	: a(l), b(r),
	  RTP(l.getRunTimeProperties(),r.getRunTimeProperties())
      {}
#else
      TFEL_MATH_INLINE StensorTensorDiadicProductExpr(const StensorType& l,
						      const TensorType& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE StensorTensorDiadicProductExpr(const StensorTensorDiadicProductExpr& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const AIndexType i,
		 const BIndexType j) const 
      {
	return a(i)*b(j);
      }

    private:

      TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));

      StensorTensorDiadicProductExpr();

      typename tfel::meta::IF<tfel::typetraits::IsTemporary<StensorType>::cond,
			      const StensorType,
			      const StensorType&>::type a;
      
      typename tfel::meta::IF<tfel::typetraits::IsTemporary<TensorType>::cond,
			      const TensorType,
			      const TensorType&>::type b;
      
      const RunTimeProperties RTP;

    }; // end of struct StensorTensorDiadicProductExpr

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_STENSORTENSORDIADICPRODUCTEXPR_H */

