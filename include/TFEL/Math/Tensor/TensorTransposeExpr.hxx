/*!
 * \file   include/TFEL/Math/Tensor/TensorTransposeExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   07 avr 2008
 */

#ifndef _LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX_
#define _LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX_ 

#include<cmath>
#include<cstddef>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/Forward/TensorConcept.hxx"
#include"TFEL/Math/General/ResultType.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

namespace tfel{

  namespace math {

    template<typename A>
    class TFEL_VISIBILITY_LOCAL TensorTransposeExprBase
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,TensorConcept>::cond));
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      TensorTransposeExprBase();

    public:

      typedef EmptyRunTimeProperties RunTimeProperties; 
      typedef typename TensorTraits<A>::IndexType IndexType;
      typedef typename TensorTraits<A>::NumType   NumType;

    protected:

      typedef const A first_arg;
      typedef tfel::meta::InvalidType second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;

      TFEL_MATH_INLINE TensorTransposeExprBase(const A& l)
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
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr1D
      : public TensorTransposeExprBase<A>
    {

      TFEL_MATH_INLINE TensorTransposeExpr1D(const A& l)
	: TensorTransposeExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType 
      operator()(const typename TensorTransposeExprBase<A>::IndexType i) const 
      {
	return this->a(i);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==1u));
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr2D
      : public TensorTransposeExprBase<A>
    {

      TFEL_MATH_INLINE TensorTransposeExpr2D(const A& l)
	: TensorTransposeExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType 
      operator()(const typename TensorTransposeExprBase<A>::IndexType i) const 
      {
	typedef typename TensorTransposeExprBase<A>::NumType NumType;
	switch(i){
	case 0:
	  return this->a(0);
	  break;
	case 1:
	  return this->a(1);
	  break;
	case 2:
	  return this->a(2);
	  break;
	case 3:
	  return this->a(4);
	  break;
	case 4:
	  return this->a(3);
	  break;
	default:
	  return NumType(0);
	}
	return NumType(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==2u));
    };

    template<typename A>
    struct TFEL_VISIBILITY_LOCAL TensorTransposeExpr3D
      : public TensorTransposeExprBase<A>
    {

      TFEL_MATH_INLINE TensorTransposeExpr3D(const A& l)
	: TensorTransposeExprBase<A>(l)
      {}
      
      TFEL_MATH_INLINE typename TensorTransposeExprBase<A>::NumType 
      operator()(const typename TensorTransposeExprBase<A>::IndexType i) const 
      {
	typedef typename TensorTransposeExprBase<A>::NumType NumType;
	switch(i){
	case 0:
	  return this->a(0);
	  break;
	case 1:
	  return this->a(1);
	  break;
	case 2:
	  return this->a(2);
	  break;
	case 3:
	  return this->a(4);
	  break;
	case 4:
	  return this->a(3);
	  break;
	case 5:
	  return this->a(6);
	  break;
	case 6:
	  return this->a(5);
	  break;
	case 7:
	  return this->a(8);
	  break;
	case 8:
	  return this->a(7);
	  break;
	default:
	  return NumType(0);
	}
	return NumType(0);
      } // end of operator()

    private:

      TFEL_STATIC_ASSERT((tfel::math::TensorTraits<A>::dime==3u));
    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORTRANSPOSEEXPR_HXX */
