/*!
 * \file   VectorVectorDiadicProductExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 */

#ifndef _LIB_VECTORVECTORDIADICPRODUCTEXPR_HXX_
#define _LIB_VECTORVECTORDIADICPRODUCTEXPR_HXX_ 

#include<string>
#include<iterator>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/TypeTraits/Promote.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/General/RunTimeCheck.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B>
    class VectorVectorDiadicProductExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,VectorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpDiadicProduct>::Result Result;

      typedef typename VectorTraits<A>::IndexType AIndexType;
      typedef typename VectorTraits<B>::IndexType BIndexType;

      typedef typename VectorTraits<A>::NumType NumTypeA;
      typedef typename VectorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      const typename A::RunTimeProperties RTP;

      VectorVectorDiadicProductExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorVectorDiadicProductExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<OpDiadicProduct>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef typename A::RunTimeProperties RunTimeProperties;
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

#ifndef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE VectorVectorDiadicProductExpr(const A& l, const B& r)
	: a(l), b(r),
	  RTP(RunTimeCheck<RunTimeProperties>::exe(l.getRunTimeProperties(),r.getRunTimeProperties()))
      {}
#else
      TFEL_MATH_INLINE VectorVectorDiadicProductExpr(const A& l, const B& r)
	: a(l), b(r),RTP()
      {}
#endif

      TFEL_MATH_INLINE VectorVectorDiadicProductExpr(const VectorVectorDiadicProductExpr<A,B>& src)
	: a(src.a), b(src.b), RTP(src.RTP)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const AIndexType i,
		 const BIndexType j) const 
      {
	return a(i)*b(j);
      }

    public:
      
      TFEL_MATH_INLINE const RunTimeProperties
      getRunTimeProperties(void) const
      {
	return RTP;
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_VECTORVECTORDIADICPRODUCTEXPR_HXX */

