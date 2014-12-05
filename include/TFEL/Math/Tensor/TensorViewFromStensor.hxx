/*! 
 * \file  include/TFEL/Math/Tensor/TensorViewFromStensor.hxx
 * \brief The TensorViewFromStensor describes how a stensor can be interpreted as a
 * tensor
 * \author Helfer Thomas
 * \brief 04 juil. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_H_
#define _LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Forward/tensor.hxx"

namespace tfel
{
  
  namespace math
  {

    /*!
     * Tensor View From Stensor
     */
    template<typename T>
    struct TensorViewFromStensorExpr
    {}; // end of struct TensorViewFromStensorExpr

    template<unsigned short N,
	     typename ValueType,
	     typename T>
    struct TensorExpr<tensor<N,ValueType>,TensorViewFromStensorExpr<T> >
      : public TensorConcept<TensorExpr<tensor<N,ValueType>,TensorViewFromStensorExpr<T> > >
    {

      typedef EmptyRunTimeProperties RunTimeProperties;

      RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }

      TensorExpr(const T& s_)
	: s(s_)
      {} // end of TensorExpr

      const ValueType
      operator()(const unsigned short i) const
      {
	using std::sqrt;
	using tfel::typetraits::BaseType;
	typedef typename BaseType<ValueType>::type real;
	static constexpr real cste = real(1)/sqrt(real(2));
	switch(i){
	case 0:
	  return this->s(0);
	case 1:
	  return this->s(1);
	case 2:
	  return this->s(2);
	case 3:
	case 4:
	  if(StensorTraits<T>::dime<2){
	    throw(TensorInvalidIndexException(i));
	  }
	  return this->s(3)*cste;
	case 5:
	case 6:
	  if(StensorTraits<T>::dime<3){
	    throw(TensorInvalidIndexException(i));
	  }
	  return this->s(4)*cste;
	case 7:
	case 8:
	  if(StensorTraits<T>::dime<3){
	    throw(TensorInvalidIndexException(i));
	  }
	  return this->s(5)*cste;
	default:
	  ;
	}
	throw(TensorInvalidIndexException(i));
	return ValueType(0);
      } // end of operator() const

    protected:

      //! says if the underlying object is a temporary
      static constexpr bool IsTemporary = tfel::typetraits::IsTemporary<T>::cond;
      //! The stensor object
      typename std::conditional<IsTemporary,const T,const T&>::type s;

    private:

      typedef typename StensorTraits<T>::NumType StensorValueType;
      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((StensorTraits<T>::dime==N));
      TFEL_STATIC_ASSERT((std::is_same<ValueType,StensorValueType>::value));

    }; // end of struct TensorExpr

    template<typename T>
    struct TensorViewFromStensor
    {
      typedef typename StensorTraits<T>::NumType NumType;
      static  const unsigned short N = StensorTraits<T>::dime;
      typedef TensorExpr<tensor<N,NumType>,TensorViewFromStensorExpr<T> > type;
    }; // end of struct TensorViewFromStensor
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSORVIEWFROMSTENSOR_H */

