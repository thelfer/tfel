/*!
 * \file   StensorStensorDiadicProductExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 */

#ifndef _LIB_STENSORSTENSORDIADICPRODUCTEXPR_HXX_
#define _LIB_STENSORSTENSORDIADICPRODUCTEXPR_HXX_ 

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
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math {
    
    template<typename A, typename B>
    class StensorStensorDiadicProductExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,StensorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpDiadicProduct>::Result Result;

      typedef typename StensorTraits<A>::NumType NumTypeA;
      typedef typename StensorTraits<B>::NumType NumTypeB;
  
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      StensorStensorDiadicProductExpr();

    public:

      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
      
      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorStensorDiadicProductExpr<")+
	  Name<A>::getName()+string(",")+
	  Name<B>::getName()+string(",")+
	  Name<OpDiadicProduct>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE
      StensorStensorDiadicProductExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE
      StensorStensorDiadicProductExpr(const StensorStensorDiadicProductExpr<A,B>& src)
	: a(src.a), b(src.b)
      {}

      TFEL_MATH_INLINE const NumType 
      operator()(const size_type i,
		 const size_type j) const 
      {
	return a(i)*b(j);
      }

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_STENSORSTENSORDIADICPRODUCTEXPR_HXX */

