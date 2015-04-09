/*!
 * \file   include/TFEL/Math/Stensor/StensorProduct.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_STENSORPRODUCTEXPR_HXX_
#define _LIB_STENSORPRODUCTEXPR_HXX_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel{

  namespace math {

    template<typename A, typename B>
    class TFEL_VISIBILITY_LOCAL StensorProductExprBase
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,StensorConcept>::cond));
      
      typedef typename ComputeBinaryResult<A,B,OpMult>::Result Result;

      typedef typename StensorTraits<A>::IndexType AIndexType;
      typedef typename StensorTraits<B>::IndexType BIndexType;

      typedef typename StensorTraits<A>::NumType NumTypeA;
      typedef typename StensorTraits<B>::NumType NumTypeB;

      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      StensorProductExprBase();

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

      TFEL_MATH_INLINE StensorProductExprBase(const A& l, const B& r)
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
    struct TFEL_VISIBILITY_LOCAL StensorProductExpr1D
      : public StensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE StensorProductExpr1D(const A& l, const B& r)
	: StensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename StensorProductExprBase<A,B>::NumType 
      operator()(const typename StensorProductExprBase<A,B>::IndexType i) const 
      {
	return (this->a(i))*(this->b(i));
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==1u));
      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<B>::dime==1u));

    };

    template<typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL StensorProductExpr2D
      : public StensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE StensorProductExpr2D(const A& l, const B& r)
	: StensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename StensorProductExprBase<A,B>::NumType 
      operator()(const typename StensorProductExprBase<A,B>::IndexType i) const 
      {
	typedef typename StensorProductExprBase<A,B>::NumType T;
	typedef typename tfel::typetraits::BaseType<T>::type base;
	static const base cste = 1/std::sqrt(base(2));
	switch(i){
	case 0:
	  return (this->a(3)*this->b(3))/2+this->a(0)*this->b(0);
	  break;
	case 1:
	  return (this->a(3)*this->b(3))/2+this->a(1)*this->b(1);
	  break;
	case 2:
	  return this->a(2)*this->b(2);
	  break;
	case 3:
	  return ((this->a(0)*this->b(3))+(this->a(3)*this->b(1)))*cste;
	  break;
	case 4:
	  return ((this->a(1)*this->b(3))+(this->a(3)*this->b(0)))*cste;
	  break;
	default:
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==2u));
      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<B>::dime==2u));

    };

    template<typename A, typename B>
    struct TFEL_VISIBILITY_LOCAL StensorProductExpr3D
      : public StensorProductExprBase<A,B>
    {

      TFEL_MATH_INLINE StensorProductExpr3D(const A& l, const B& r)
	: StensorProductExprBase<A,B>(l,r)
      {}
      
      TFEL_MATH_INLINE typename StensorProductExprBase<A,B>::NumType 
      operator()(const typename StensorProductExprBase<A,B>::IndexType i) const 
      {
	using namespace std;
	typedef typename StensorProductExprBase<A,B>::NumType T;
	typedef typename tfel::typetraits::BaseType<T>::type base;
	static const base cste = base(1)/sqrt(base(2));
	switch(i){
	default:
	case 0:
	  return (this->a(4)*this->b(4))/2+(this->a(3)*this->b(3))/2+this->a(0)*this->b(0);
	  break;
	case 1:
	  return (this->a(5)*this->b(5))/2+(this->a(3)*this->b(3))/2+this->a(1)*this->b(1);
	  break;
	case 2:
	  return (this->a(5)*this->b(5))/2+(this->a(4)*this->b(4))/2+this->a(2)*this->b(2);
	  break;
	case 3:
	  return (this->a(4)*this->b(5))/2+(this->a(0)*this->b(3)+this->a(3)*this->b(1))*cste;
	  break;
	case 4:
	  return (this->a(5)*this->b(4))/2+(this->a(1)*this->b(3)+this->a(3)*this->b(0))*cste;
	  break;
	case 5:
	  return (this->a(3)*this->b(5))/2+(this->a(0)*this->b(4)+this->a(4)*this->b(2))*cste;
	  break;
	case 6:
	  return (this->a(5)*this->b(3))/2+(this->a(2)*this->b(4)+this->a(4)*this->b(0))*cste;
	  break;
	case 7:
	  return (this->a(3)*this->b(4))/2+(this->a(1)*this->b(5)+this->a(5)*this->b(2))*cste;
	  break;
	case 8:
	  return (this->a(4)*this->b(3))/2+(this->a(2)*this->b(5)+this->a(5)*this->b(1))*cste;
	  break;
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==3u));
      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<B>::dime==3u));

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_STENSORPRODUCTEXPR_HXX */
