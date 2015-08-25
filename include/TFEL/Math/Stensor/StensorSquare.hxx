/*!
 * \file   include/TFEL/Math/Stensor/StensorSquare.hxx
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

#ifndef _LIB_STENSORSQUAREEXPR_HXX_
#define _LIB_STENSORSQUAREEXPR_HXX_ 

#include<cmath>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel{

  namespace math {

    template<typename A>
    class TFEL_VISIBILITY_LOCAL StensorSquareExprBase
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));

      struct invalid_argument;

      typedef typename StensorTraits<A>::IndexType AIndexType;

      typedef typename StensorTraits<A>::NumType NumTypeA;

      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      StensorSquareExprBase();

    public:

      typedef EmptyRunTimeProperties RunTimeProperties; 
      typedef typename StensorTraits<A>::IndexType IndexType;
      typedef typename ComputeBinaryResult<NumTypeA,NumTypeA,OpMult>::Handle NumType;

    protected:

      typedef const A first_arg;
      typedef invalid_argument second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef AIndexType     size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;

      TFEL_MATH_INLINE StensorSquareExprBase(const A& l)
	: a(l)
      {}

    public:
      
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties(void) const
      {
	return EmptyRunTimeProperties();
      }
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL StensorSquareExpr1D
      : public StensorSquareExprBase<A>
    {

      TFEL_MATH_INLINE StensorSquareExpr1D(const A& l)
	: StensorSquareExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename StensorSquareExprBase<A>::NumType 
      operator()(const typename StensorSquareExprBase<A>::IndexType i) const 
      {
	return (this->a(i))*(this->a(i));
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==1u));
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL StensorSquareExpr2D
      : public StensorSquareExprBase<A>
    {

      TFEL_MATH_INLINE StensorSquareExpr2D(const A& l)
	: StensorSquareExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename StensorSquareExprBase<A>::NumType 
      operator()(const typename StensorSquareExprBase<A>::IndexType i) const 
      {
	typedef typename StensorSquareExprBase<A>::NumType T;
	typedef typename tfel::typetraits::BaseType<T>::type base;
	static const base one_half = 1/base(2);
	switch(i){
	case 0:
	  return ((this->a(3))*(this->a(3))+2*(this->a(0))*(this->a(0)))*one_half;
	  break;
	case 1:
	  return ((this->a(3))*(this->a(3))+2*(this->a(1))*(this->a(1)))*one_half;
	  break;
	case 2:
	  return (this->a(2))*(this->a(2));
	  break;
	case 3:
	  return ((this->a(1))+(this->a(0)))*(this->a(3));
	  break;
	default:
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==2u));
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL StensorSquareExpr3D
      : public StensorSquareExprBase<A>
    {

      TFEL_MATH_INLINE StensorSquareExpr3D(const A& l)
	: StensorSquareExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename StensorSquareExprBase<A>::NumType 
      operator()(const typename StensorSquareExprBase<A>::IndexType i) const 
      {
	using namespace std;
	typedef typename StensorSquareExprBase<A>::NumType T;
	typedef typename tfel::typetraits::BaseType<T>::type base;
	static const base one_half = 1/(base(2));
	static const base cste     = sqrt(base(2));
	switch(i){
	case 0:
	  return ((this->a(4))*(this->a(4))+(this->a(3))*(this->a(3))+2*(this->a(0))*(this->a(0)))*one_half;
	  break;
	case 1:  
	  return ((this->a(5))*(this->a(5))+(this->a(3))*(this->a(3))+2*(this->a(1))*(this->a(1)))*one_half;
	  break;
	case 2:
	  return ((this->a(5))*(this->a(5))+(this->a(4))*(this->a(4))+2*(this->a(2))*(this->a(2)))*one_half;
	  break;
	case 3:
	  return (cste*(this->a(4))*(this->a(5))+(2*(this->a(1))+2*(this->a(0)))*(this->a(3)))*one_half;
	  break;
	case 4:
	  return (cste*(this->a(3))*(this->a(5))+(2*(this->a(2))+2*(this->a(0)))*(this->a(4)))*one_half;
	  break;
	case 5:
	  return ((2*(this->a(2))+2*(this->a(1)))*(this->a(5))+cste*(this->a(3))*(this->a(4)))*one_half;
	  break;
	default:
	  return T(0);
	}
	return T(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::StensorTraits<A>::dime==3u));
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_STENSORSQUAREEXPR_HXX */
