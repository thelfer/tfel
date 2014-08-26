/*!
 * \file   TensorProductExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 */

#ifndef _LIB_TENSORPRODUCTEXPR_HXX_
#define _LIB_TENSORPRODUCTEXPR_HXX_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel{

  namespace math {

    template<typename A, typename B>
    class TFEL_VISIBILITY_LOCAL TensorProductExprBase
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,TensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,TensorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;

      typedef typename TensorTraits<A>::IndexType AIndexType;
      typedef typename TensorTraits<B>::IndexType BIndexType;

      typedef typename TensorTraits<A>::NumType NumTypeA;
      typedef typename TensorTraits<B>::NumType NumTypeB;

      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      TensorProductExprBase();

    public:

      typedef EmptyRunTimeProperties RunTimeProperties; 
      typedef typename TensorTraits<Result>::IndexType IndexType;
      typedef typename ComputeBinaryResult<NumTypeA,NumTypeB,OpMult>::Handle NumType;
    protected:

      typedef const A first_arg;
      typedef const B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      TFEL_MATH_INLINE TensorProductExprBase(const A& l, const B& r)
	: a(l), b(r)
      {}

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return EmptyRunTimeProperties();
      }
    };

    template<typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr1D
      : public TensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE TensorProductExpr1D(const A& l, const B& r)
	: TensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename TensorProductExprBase<A,B>::NumType 
      operator()(const typename TensorProductExprBase<A,B>::IndexType i) const 
      {
	return (this->a(i))*(this->b(i));
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==1u));
      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<B>::dime==1u));

    };

    template<typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr2D
      : public TensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE TensorProductExpr2D(const A& l, const B& r)
	: TensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename TensorProductExprBase<A,B>::NumType 
      operator()(const typename TensorProductExprBase<A,B>::IndexType i) const 
      {
	typedef typename TensorProductExprBase<A,B>::NumType T;
	switch(i){
	case 0:
	  return (this->a(3))*(this->b(4))+(this->a(0))*(this->b(0));
	  break;
	case 1:
	  return (this->a(4))*(this->b(3))+(this->a(1))*(this->b(1));
	  break;
	case 2:
	  return (this->a(2))*(this->b(2));
	  break;
	case 3:
	  return (this->a(0))*(this->b(3))+(this->a(3))*(this->b(1));
	  break;
	case 4:
	  return (this->a(1))*(this->b(4))+(this->a(4))*(this->b(0));
	  break;
	default:
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==2u));
      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<B>::dime==2u));

    };

    template<typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL TensorProductExpr3D
      : public TensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE TensorProductExpr3D(const A& l, const B& r)
	: TensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename TensorProductExprBase<A,B>::NumType 
      operator()(const typename TensorProductExprBase<A,B>::IndexType i) const 
      {
	using namespace std;
	typedef typename TensorProductExprBase<A,B>::NumType T;
	switch(i){
	case 0: /* a5*b6+a3*b4+a0*b0 */
	  return (this->a(5))*(this->b(6))+(this->a(3))*(this->b(4))+(this->a(0))*(this->b(0));
	  break;
	case 1: /* a7*b8+a4*b3+a1*b1 */
	  return (this->a(7))*(this->b(8))+(this->a(4))*(this->b(3))+(this->a(1))*(this->b(1)); 
	  break;
	case 2: /* a8*b7+a6*b5+a2*b2 */
	  return (this->a(8))*(this->b(7))+(this->a(6))*(this->b(5))+(this->a(2))*(this->b(2)); 
	  break;
	case 3: /* a5*b8+a0*b3+a3*b1 */
	  return (this->a(5))*(this->b(8))+(this->a(0))*(this->b(3))+(this->a(3))*(this->b(1)); 
	  break;
	case 4: /* a7*b6+a1*b4+a4*b0 */
	  return  (this->a(7))*(this->b(6))+(this->a(1))*(this->b(4))+(this->a(4))*(this->b(0)); 
	  break;
	case 5: /* a3*b7+a0*b5+a5*b2 */
	  return (this->a(3))*(this->b(7))+(this->a(0))*(this->b(5))+(this->a(5))*(this->b(2));
	  break;
	case 6: /* a2*b6+a8*b4+a6*b0 */
	  return (this->a(2))*(this->b(6))+(this->a(8))*(this->b(4))+(this->a(6))*(this->b(0));
	  break;
	case 7: /* a1*b7+a4*b5+a7*b2 */
	  return (this->a(1))*(this->b(7))+(this->a(4))*(this->b(5))+(this->a(7))*(this->b(2)); 
	  break;
	case 8: /* a2*b8+a6*b3+a8*b1 */
	  return (this->a(2))*(this->b(8))+(this->a(6))*(this->b(3))+(this->a(8))*(this->b(1));
	  break;
	default:
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==3u));
      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<B>::dime==3u));

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TENSORPRODUCTEXPR_HXX */
