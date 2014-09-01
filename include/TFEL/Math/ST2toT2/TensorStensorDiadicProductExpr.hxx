/*! 
 * \file  include/TFEL/Math/ST2toT2/TensorStensorDiadicProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TENSORSTENSORDIADICPRODUCTEXPR_H_
#define _LIB_TFEL_MATH_TENSORSTENSORDIADICPRODUCTEXPR_H_ 

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

    template<typename TensorType,
	     typename StensorType>
    struct TFEL_VISIBILITY_LOCAL TensorStensorDiadicProductExpr
    {

    protected:

      typedef typename ComputeBinaryResult<TensorType,StensorType,OpDiadicProduct>::Result Result;
      typedef typename TensorTraits<TensorType>::IndexType   AIndexType;
      typedef typename StensorTraits<StensorType>::IndexType BIndexType;
      typedef typename TensorTraits<TensorType>::NumType     NumTypeA;
      typedef typename StensorTraits<StensorType>::NumType   NumTypeB;

    public:

      typedef typename Result::RunTimeProperties RunTimeProperties; 
      typedef typename ST2toT2Traits<Result>::IndexType IndexType;
      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;

      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return this->RTP;
      }

    protected:

      typedef const TensorType  first_arg;
      typedef const StensorType second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE TensorStensorDiadicProductExpr(const TensorType& l,
							 const StensorType& r)
	: a(l), b(r),
	  RTP(l.getRunTimeProperties(),r.getRunTimeProperties())
      {}
#else
      TFEL_MATH_INLINE TensorStensorDiadicProductExpr(const TensorType& l,
							 const StensorType& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE TensorStensorDiadicProductExpr(const TensorStensorDiadicProductExpr& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE NumType 
      operator()(const AIndexType i,
		 const BIndexType j) const 
      {
	return a(i)*b(j);
      }

    private:

      TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));

      TensorStensorDiadicProductExpr();

      typename tfel::meta::IF<tfel::typetraits::IsTemporary<TensorType>::cond,
			      const TensorType,
			      const TensorType&>::type a;
      
      typename tfel::meta::IF<tfel::typetraits::IsTemporary<StensorType>::cond,
			      const StensorType,
			      const StensorType&>::type b;
      
      const RunTimeProperties RTP;

    }; // end of struct TensorStensorDiadicProductExpr

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORSTENSORDIADICPRODUCTEXPR_H */

