/*!
 * \file   include/TFEL/Math/Tensor/TensorConcept.ixx
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_MATH_TENSOR_CONCEPT_IMPL_
#define _LIB_TFEL_MATH_TENSOR_CONCEPT_IMPL_ 1

#include"TFEL/FSAlgorithm/transform.hxx"
#include"TFEL/FSAlgorithm/copy.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/General/ConstExprMathFunctions.hxx"
#include"TFEL/Math/Forward/stensor.hxx"
#include"TFEL/Math/Forward/tvector.hxx"
#include"TFEL/Math/Forward/tensor.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"
#include"TFEL/Math/Tensor/MatrixViewFromTensor.hxx"

namespace tfel{

  namespace math{

    namespace internals{

      template<unsigned short N>
      struct TensorAbs;

      template<>
      struct TensorAbs<1u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2));
	}
      };

      template<>
      struct TensorAbs<2u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4));
	}
      };

      template<>
      struct TensorAbs<3u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5))
	    +tfel::math::abs(s(6))+tfel::math::abs(s(7))+tfel::math::abs(s(8));
	}
      };
      
      template<unsigned short N>
      struct TensorConceptMatrixAccessOperator;

      template<>
      struct TensorConceptMatrixAccessOperator<1u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& t,
	    const unsigned short i,
	    const unsigned short j)
	{
	  typedef typename TensorTraits<TensorType>::NumType NumType;
	  if((i==j)&&(i<3)){
	    return t(i);
	  }
	  return NumType(0);
	}
      };
      
      template<>
      struct TensorConceptMatrixAccessOperator<2u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& t,
	    const unsigned short i,
	    const unsigned short j)
	{
	  typedef typename TensorTraits<TensorType>::NumType NumType;
	  if((i==j)&&(i<3)){
	    return t(i);
	  } else if((i==0)&&(j==1)){
	    return t(3);
	  } else if((i==1)&&(j==0)){
	    return t(4);
	  }
	  return NumType(0);
	}
      };

      template<>
      struct TensorConceptMatrixAccessOperator<3u>
      {
	template<typename TensorType>
	TFEL_MATH_INLINE
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& t,
	    const unsigned short i,
	    const unsigned short j)
	{
	  typedef typename TensorTraits<TensorType>::NumType NumType;
	  if((i==j)&&(i<3)){
	    return t(i);
	  } else if((i==0)&&(j==1)){
	    return t(3);
	  } else if((i==1)&&(j==0)){
	    return t(4);
	  } else if((i==0)&&(j==2)){
	    return t(5);
	  } else if((i==2)&&(j==0)){
	    return t(6);
	  } else if((i==1)&&(j==2)){
	    return t(7);
	  } else if((i==2)&&(j==1)){
	    return t(8);
	  }
	  throw(TensorInvalidIndexesException(i,j));
	  return NumType(0);
	}
      };

    } // end of namespace internals
    
    template<class T>
    typename TensorConcept<T>::ValueType
    TensorConcept<T>::operator()(const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    typename TensorConcept<T>::ValueType
    TensorConcept<T>::operator[](const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    typename TensorConcept<T>::NumType
    TensorConcept<T>::operator()(const unsigned short i,
				 const unsigned short j) const
    {
      using tfel::math::internals::TensorConceptMatrixAccessOperator;
      if((i>2)||(j>2)){
	throw(TensorInvalidIndexesException(i,j));
      }
      return TensorConceptMatrixAccessOperator<TensorTraits<T>::dime>::exe(static_cast<const T&>(*this),i,j);
    }
    
    template<class T>
    typename std::enable_if<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
      >::type
    trace(const T& s)
    {
      return s(0)+s(1)+s(2);
    }     
    
    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
      >::type
    abs(const TensorType& s){
      return tfel::math::internals::TensorAbs<TensorTraits<TensorType>::dime>::exe(s);
    }
    
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t)
    {
      return stensor<1u,typename TensorTraits<T>::NumType>(t.begin());
    }

    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t){
      typedef typename TensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2))/real(2);
      stensor<2u,NumType> s;
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      s[3] = cste*(t[3]+t[4]);
      return s;
    }

    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t)
    {
      typedef typename TensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2))/real(2);
      stensor<3u,NumType> s;
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      s[3] = cste*(t[3]+t[4]);
      s[4] = cste*(t[5]+t[6]);
      s[5] = cste*(t[7]+t[8]);
      return s;
    }

    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType> >::type
    computeRightCauchyGreenTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      stensor<1u,real> r;
      r[0] = t[0]*t[0];
      r[1] = t[1]*t[1];
      r[2] = t[2]*t[2];
      return r;
    }
  
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeRightCauchyGreenTensor(const T& t){
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<2u,real> r;
      r[0] = t[0]*t[0]+t[4]*t[4];
      r[1] = t[1]*t[1]+t[3]*t[3];
      r[2] = t[2]*t[2];
      r[3] = cste*(t[1]*t[4]+t[0]*t[3]);
      return r;
    }
    
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeRightCauchyGreenTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<3u,real> r;
      r[0] = t[0]*t[0]+t[4]*t[4]+t[6]*t[6];
      r[1] = t[1]*t[1]+t[3]*t[3]+t[8]*t[8];
      r[2] = t[2]*t[2]+t[5]*t[5]+t[7]*t[7];
      r[3] = cste*(t[6]*t[8]+t[1]*t[4]+t[0]*t[3]);
      r[4] = cste*(t[4]*t[7]+t[2]*t[6]+t[0]*t[5]);
      r[5] = cste*(t[2]*t[8]+t[1]*t[7]+t[3]*t[5]);
      return r;
    }

    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType> >::type
    computeLeftCauchyGreenTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      stensor<1u,real> r;
      r[0] = t[0]*t[0];
      r[1] = t[1]*t[1];
      r[2] = t[2]*t[2];
      return r;
    }
  
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeLeftCauchyGreenTensor(const T& t){
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<2u,real> r;
      r[0] = t[0]*t[0]+t[3]*t[3];
      r[1] = t[1]*t[1]+t[4]*t[4];
      r[2] = t[2]*t[2];
      r[3] = cste*(t[0]*t[4]+t[1]*t[3]);
      return r;
    }
    
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeLeftCauchyGreenTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<3u,real> r;
      r[0] = t[0]*t[0]+t[3]*t[3]+t[5]*t[5];
      r[1] = t[1]*t[1]+t[4]*t[4]+t[7]*t[7];
      r[2] = t[2]*t[2]+t[6]*t[6]+t[8]*t[8];
      r[3] = cste*(t[5]*t[7]+t[0]*t[4]+t[1]*t[3]);
      r[4] = cste*(t[3]*t[8]+t[0]*t[6]+t[2]*t[5]);
      r[5] = cste*(t[1]*t[8]+t[2]*t[7]+t[4]*t[6]);
      return r;
    }

    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType> >::type
    computeGreenLagrangeTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      stensor<1u,real> r;
      r[0] = (t[0]*t[0]-1)/2;
      r[1] = (t[1]*t[1]-1)/2;
      r[2] = (t[2]*t[2]-1)/2;
      return r;
    }
  
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeGreenLagrangeTensor(const T& t){
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2))/2;
      stensor<2u,real> r;
      r[0] = (t[0]*t[0]+t[4]*t[4]-1)/2;
      r[1] = (t[1]*t[1]+t[3]*t[3]-1)/2;
      r[2] = (t[2]*t[2]-1)/2;
      r[3] = cste*(t[0]*t[3]+t[1]*t[4]);
      return r;
    }
    
    template<class T>
    typename std::enable_if<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeGreenLagrangeTensor(const T& t)
    {
      typedef typename TensorTraits<T>::NumType real;
      constexpr real cste = constexpr_fct::sqrt(real(2))/2;
      stensor<3u,real> r;
      r[0] = (t[0]*t[0]+t[4]*t[4]+t[6]*t[6]-1)/2;
      r[1] = (t[1]*t[1]+t[3]*t[3]+t[8]*t[8]-1)/2;
      r[2] = (t[2]*t[2]+t[5]*t[5]+t[7]*t[7]-1)/2;
      r[3] = cste*(t[6]*t[8]+t[1]*t[4]+t[0]*t[3]);
      r[4] = cste*(t[4]*t[7]+t[2]*t[6]+t[0]*t[5]);
      r[5] = cste*(t[2]*t[8]+t[1]*t[7]+t[3]*t[5]);
      return r;
    }

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==1u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<1u,typename StensorTraits<T>::NumType>
      >::type
    pushForward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      stensor<1u,NumType> s;
      s[0] = p[0]*F[0]*F[0];
      s[1] = p[1]*F[1]*F[1];
      s[2] = p[2]*F[2]*F[2];
      return s;
    } // end of pushForward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==2u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<2u,typename StensorTraits<T>::NumType>
      >::type
    pushForward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<2u,NumType> s;
      s[0] = p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0];
      s[1] = p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1];
      s[2] = p[2]*F[2]*F[2];
      s[3] = (p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1];
      return s;
    } // end of pushForward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==3u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<3u,typename StensorTraits<T>::NumType>
      >::type
    pushForward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<3u,NumType> s;
      s[0] = p[2]*F[5]*F[5]+(cste*p[5]*F[3]+cste*p[4]*F[0])*F[5]+p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0];
      s[1] = p[2]*F[7]*F[7]+(cste*p[4]*F[4]+cste*p[5]*F[1])*F[7]+p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1];
      s[2] = p[1]*F[8]*F[8]+(cste*p[3]*F[6]+cste*p[5]*F[2])*F[8]+p[0]*F[6]*F[6]+cste*p[4]*F[2]*F[6]+p[2]*F[2]*F[2];
      s[3] = (cste*p[2]*F[5]+p[5]*F[3]+p[4]*F[0])*F[7]+(p[4]*F[4]+p[5]*F[1])*F[5]+(p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1];
      s[4] = (p[5]*F[5]+cste*p[1]*F[3]+p[3]*F[0])*F[8]+(p[4]*F[5]+p[3]*F[3]+cste*p[0]*F[0])*F[6]+cste*p[2]*F[2]*F[5]+p[5]*F[2]*F[3]+p[4]*F[0]*F[2];
      s[5] = (p[5]*F[7]+p[3]*F[4]+cste*p[1]*F[1])*F[8]+(p[4]*F[6]+cste*p[2]*F[2])*F[7]+(cste*p[0]*F[4]+p[3]*F[1])*F[6]+p[4]*F[2]*F[4]+p[5]*F[1]*F[2];
      return s;
    } // end of pushForward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==1u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<1u,typename StensorTraits<T>::NumType>
      >::type
    push_forward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      stensor<1u,NumType> s;
      s[0] = p[0]*F[0]*F[0];
      s[1] = p[1]*F[1]*F[1];
      s[2] = p[2]*F[2]*F[2];
      return s;
    } // end of push_forward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==2u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<2u,typename StensorTraits<T>::NumType>
      >::type
    push_forward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<2u,NumType> s;
      s[0] = p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0];
      s[1] = p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1];
      s[2] = p[2]*F[2]*F[2];
      s[3] = (p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1];
      return s;
    } // end of push_forward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==3u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<3u,typename StensorTraits<T>::NumType>
      >::type
    push_forward(const T&  p,
		const T2& F)
    {
      typedef typename StensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<3u,NumType> s;
      s[0] = p[2]*F[5]*F[5]+(cste*p[5]*F[3]+cste*p[4]*F[0])*F[5]+p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0];
      s[1] = p[2]*F[7]*F[7]+(cste*p[4]*F[4]+cste*p[5]*F[1])*F[7]+p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1];
      s[2] = p[1]*F[8]*F[8]+(cste*p[3]*F[6]+cste*p[5]*F[2])*F[8]+p[0]*F[6]*F[6]+cste*p[4]*F[2]*F[6]+p[2]*F[2]*F[2];
      s[3] = (cste*p[2]*F[5]+p[5]*F[3]+p[4]*F[0])*F[7]+(p[4]*F[4]+p[5]*F[1])*F[5]+(p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1];
      s[4] = (p[5]*F[5]+cste*p[1]*F[3]+p[3]*F[0])*F[8]+(p[4]*F[5]+p[3]*F[3]+cste*p[0]*F[0])*F[6]+cste*p[2]*F[2]*F[5]+p[5]*F[2]*F[3]+p[4]*F[0]*F[2];
      s[5] = (p[5]*F[7]+p[3]*F[4]+cste*p[1]*F[1])*F[8]+(p[4]*F[6]+cste*p[2]*F[2])*F[7]+(cste*p[0]*F[4]+p[3]*F[1])*F[6]+p[4]*F[2]*F[4]+p[5]*F[1]*F[2];
      return s;
    } // end of push_forward

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==1u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<1u,typename StensorTraits<T>::NumType>
      >::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&  s,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      stensor<1u,stress> p;
      p[0] = s[0]*F[1]*F[2]/(F[0]);
      p[1] = s[1]*F[0]*F[2]/(F[1]);
      p[2] = s[2]*F[0]*F[1]/(F[2]);
      return p;
    } // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==2u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<2u,typename StensorTraits<T>::NumType>
      >::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&  s,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      typedef typename tfel::typetraits::BaseType<stress>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      const tensor<2u,typename TensorTraits<T2>::NumType> iF = invert(F);
      const real J = det(F);
      stensor<2u,stress> p;
      p[0] = (s[1]*iF[3]*iF[3]+cste*s[3]*iF[0]*iF[3]+s[0]*iF[0]*iF[0])*J;
      p[1] = (s[0]*iF[4]*iF[4]+cste*s[3]*iF[1]*iF[4]+s[1]*iF[1]*iF[1])*J;
      p[2] = s[2]*iF[2]*iF[2]*J;
      p[3] = ((s[3]*iF[3]+cste*s[0]*iF[0])*iF[4]+cste*s[1]*iF[1]*iF[3]+s[3]*iF[0]*iF[1])*J;
      return p;
    } // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==3u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<3u,typename StensorTraits<T>::NumType>
      >::type
    convertCauchyStressToSecondPiolaKirchhoffStress(const T&  s,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      typedef typename tfel::typetraits::BaseType<stress>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      const tensor<3u,typename TensorTraits<T2>::NumType> iF = invert(F);
      const real J = det(F);
      stensor<3u,stress> p;
      p[0] = (s[2]*iF[5]*iF[5]+(cste*s[5]*iF[3]+cste*s[4]*iF[0])*iF[5]+s[1]*iF[3]*iF[3]+cste*s[3]*iF[0]*iF[3]+s[0]*iF[0]*iF[0])*J;
      p[1] = (s[2]*iF[7]*iF[7]+(cste*s[4]*iF[4]+cste*s[5]*iF[1])*iF[7]+s[0]*iF[4]*iF[4]+cste*s[3]*iF[1]*iF[4]+s[1]*iF[1]*iF[1])*J;
      p[2] = (s[1]*iF[8]*iF[8]+(cste*s[3]*iF[6]+cste*s[5]*iF[2])*iF[8]+s[0]*iF[6]*iF[6]+cste*s[4]*iF[2]*iF[6]+s[2]*iF[2]*iF[2])*J;
      p[3] = ((cste*s[2]*iF[5]+s[5]*iF[3]+s[4]*iF[0])*iF[7]+(s[4]*iF[4]+s[5]*iF[1])*iF[5]+(s[3]*iF[3]+cste*s[0]*iF[0])*iF[4]+cste*s[1]*iF[1]*iF[3]+s[3]*iF[0]*iF[1])*J;
      p[4] = ((s[5]*iF[5]+cste*s[1]*iF[3]+s[3]*iF[0])*iF[8]+(s[4]*iF[5]+s[3]*iF[3]+cste*s[0]*iF[0])*iF[6]+cste*s[2]*iF[2]*iF[5]+s[5]*iF[2]*iF[3]+s[4]*iF[0]*iF[2])*J;
      p[5] = ((s[5]*iF[7]+s[3]*iF[4]+cste*s[1]*iF[1])*iF[8]+(s[4]*iF[6]+cste*s[2]*iF[2])*iF[7]+(cste*s[0]*iF[4]+s[3]*iF[1])*iF[6]+s[4]*iF[2]*iF[4]+s[5]*iF[1]*iF[2])*J;
      return p;
    } // end of convertCauchyStressToSecondPiolaKirchhoffStress

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==1u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<1u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&  p,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      typedef typename tfel::typetraits::BaseType<stress>::type real;
      stensor<1u,stress> s;
      const real inv_J = 1/(F[0]*F[1]*F[2]);
      s[0] = p[0]*F[0]*F[0]*inv_J;
      s[1] = p[1]*F[1]*F[1]*inv_J;
      s[2] = p[2]*F[2]*F[2]*inv_J;
      return s;
    } // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==2u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<2u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&  p,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      typedef typename tfel::typetraits::BaseType<stress>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<2u,stress> s;
      const real inv_J = 1/det(F);
      s[0] = (p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0])*inv_J;
      s[1] = (p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1])*inv_J;
      s[2] = p[2]*F[2]*F[2]*inv_J;
      s[3] = ((p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1])*inv_J;
      return s;
    } // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template<typename T,typename T2>
    typename std::enable_if<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==3u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<3u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&  p,
						    const T2& F)
    {
      typedef typename StensorTraits<T>::NumType stress;
      typedef typename tfel::typetraits::BaseType<stress>::type real;
      constexpr real cste = constexpr_fct::sqrt(real(2));
      stensor<3u,stress> s;
      const real inv_J = 1/det(F);
      s[0] = (p[2]*F[5]*F[5]+(cste*p[5]*F[3]+cste*p[4]*F[0])*F[5]+p[1]*F[3]*F[3]+cste*p[3]*F[0]*F[3]+p[0]*F[0]*F[0])*inv_J;
      s[1] = (p[2]*F[7]*F[7]+(cste*p[4]*F[4]+cste*p[5]*F[1])*F[7]+p[0]*F[4]*F[4]+cste*p[3]*F[1]*F[4]+p[1]*F[1]*F[1])*inv_J;
      s[2] = (p[1]*F[8]*F[8]+(cste*p[3]*F[6]+cste*p[5]*F[2])*F[8]+p[0]*F[6]*F[6]+cste*p[4]*F[2]*F[6]+p[2]*F[2]*F[2])*inv_J;
      s[3] = ((cste*p[2]*F[5]+p[5]*F[3]+p[4]*F[0])*F[7]+(p[4]*F[4]+p[5]*F[1])*F[5]+(p[3]*F[3]+cste*p[0]*F[0])*F[4]+cste*p[1]*F[1]*F[3]+p[3]*F[0]*F[1])*inv_J;
      s[4] = ((p[5]*F[5]+cste*p[1]*F[3]+p[3]*F[0])*F[8]+(p[4]*F[5]+p[3]*F[3]+cste*p[0]*F[0])*F[6]+cste*p[2]*F[2]*F[5]+p[5]*F[2]*F[3]+p[4]*F[0]*F[2])*inv_J;
      s[5] = ((p[5]*F[7]+p[3]*F[4]+cste*p[1]*F[1])*F[8]+(p[4]*F[6]+cste*p[2]*F[2])*F[7]+(cste*p[0]*F[4]+p[3]*F[1])*F[6]+p[4]*F[2]*F[4]+p[5]*F[1]*F[2])*inv_J;
      return s;
    } // end of convertSecondPiolaKirchhoffStressToCauchyStress

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType&t)
    {
      return t(0)*t(1)*t(2);
    } // end of 

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType& t)
    {
      return (t(0)*t(1)-t(3)*t(4))*t(2);
    }

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType& t)
    {
      typedef typename TensorTraits<TensorType>::NumType T;
      const T a = t(0);
      const T b = t(3);
      const T c = t(5);
      const T d = t(4);
      const T e = t(1);
      const T f = t(7);
      const T g = t(6);
      const T h = t(8);
      const T i = t(2);
      return a*(e*i-f*h)+b*(f*g-d*i)+c*(d*h-e*g);
    }

    template<typename TensorResultType,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorResultType,TensorConcept>::cond &&
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u&&
      TensorTraits<TensorResultType>::dime == 1u&&
      tfel::typetraits::IsAssignableTo<typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
								   Power<2> >::Result,
				       typename TensorTraits<TensorResultType>::NumType>::cond,
      void>::type
    computeDeterminantDerivative(TensorResultType& dJ,
				 const TensorType& F)
    {
      dJ[0] = F[1]*F[2];
      dJ[1] = F[0]*F[2];
      dJ[2] = F[0]*F[1];
    } // end of ComputeDeterminantDerivative

    template<typename TensorResultType,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorResultType,TensorConcept>::cond &&
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u&&
      TensorTraits<TensorResultType>::dime == 2u&&
      tfel::typetraits::IsAssignableTo<typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
								   Power<2> >::Result,
				       typename TensorTraits<TensorResultType>::NumType>::cond,
      void>::type
    computeDeterminantDerivative(TensorResultType& dJ,
				 const TensorType& F)
    {
      dJ[0]=F[1]*F[2]; 
      dJ[1]=F[0]*F[2]; 
      dJ[2]=F[0]*F[1]-F[3]*F[4]; 
      dJ[3]=-F[2]*F[4]; 
      dJ[4]=-F[2]*F[3]; 
    } // end of ComputeDeterminantDerivative

    template<typename TensorResultType,
	     typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorResultType,TensorConcept>::cond &&
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u&&
      TensorTraits<TensorResultType>::dime == 3u&&
      tfel::typetraits::IsAssignableTo<typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
								   Power<2> >::Result,
				       typename TensorTraits<TensorResultType>::NumType>::cond,
      void>::type
    computeDeterminantDerivative(TensorResultType& dJ,
				 const TensorType& F)
    {
      dJ[0]=F[1]*F[2]-F[7]*F[8]; 
      dJ[1]=F[0]*F[2]-F[5]*F[6]; 
      dJ[2]=F[0]*F[1]-F[3]*F[4]; 
      dJ[3]=F[6]*F[7]-F[2]*F[4]; 
      dJ[4]=F[5]*F[8]-F[2]*F[3]; 
      dJ[5]=F[4]*F[8]-F[1]*F[6]; 
      dJ[6]=F[3]*F[7]-F[1]*F[5]; 
      dJ[7]=F[3]*F[6]-F[0]*F[8]; 
      dJ[8]=F[4]*F[5]-F[0]*F[7]; 
    } // end of ComputeDeterminantDerivative

    template<typename TensorType,
	     typename StensorType,
	     typename TensorType2>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      tfel::meta::Implements<TensorType2,TensorConcept>::cond &&
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      std::is_same<typename StensorTraits<StensorType>::NumType,
		   typename TensorTraits<TensorType2>::NumType>::value &&
      std::is_same<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType2>::NumType>::type,
		   typename TensorTraits<TensorType>::NumType>::value &&
      (TensorTraits<TensorType>::dime==TensorTraits<TensorType2>::dime)&&
      (TensorTraits<TensorType>::dime==StensorTraits<StensorType>::dime)&&
      (TensorTraits<TensorType>::dime == 1u),
      void
      >::type
    polar_decomposition(TensorType& R,
			StensorType& U,
			const TensorType2& F)
    {
      using namespace tfel::typetraits;
      using tfel::fsalgo::copy;
      typedef typename TensorTraits<TensorType2>::NumType T;
      typedef typename BaseType<T>::type base;
      R = tensor<1u,base>::Id();
      copy<3u>::exe(F.begin(),U.begin());
    } // end of polar_decomposition

    template<typename TensorType,
	     typename StensorType,
	     typename TensorType2>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      tfel::meta::Implements<TensorType2,TensorConcept>::cond &&
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      std::is_same<typename StensorTraits<StensorType>::NumType,
		   typename TensorTraits<TensorType2>::NumType>::value &&
      std::is_same<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType2>::NumType>::type,
		   typename TensorTraits<TensorType>::NumType>::value &&
      (TensorTraits<TensorType>::dime==TensorTraits<TensorType2>::dime)&&
      (TensorTraits<TensorType>::dime==StensorTraits<StensorType>::dime)&&
      ((TensorTraits<TensorType>::dime == 2u)||(TensorTraits<TensorType>::dime == 3u)),
      void
      >::type
    polar_decomposition(TensorType& R,
			StensorType& U,
			const TensorType2& F)
    {
      using namespace std;
      using namespace tfel::typetraits;
      using tfel::fsalgo::transform;
      typedef typename TensorTraits<TensorType2>::NumType T;
      typedef typename BaseType<T>::type base;
      typedef typename ComputeBinaryResult<base,T,OpDiv>::Result inv_T;
      typedef typename ComputeBinaryResult<T,T,OpMult>::Result   T2;
      typedef typename ComputeBinaryResult<T2,T,OpMult>::Result  T3;
      T (*sqrt_ptr)(const T2) = std::sqrt;
      stensor<TensorTraits<TensorType>::dime,T> C   = computeRightCauchyGreenTensor(F);
      tvector<3u,T2> vp_C;
      tvector<3u,T> vp_U;
      C.computeEigenValues(vp_C);
      transform<3u>::exe(vp_C.begin(),vp_U.begin(),ptr_fun(sqrt_ptr));
      const T  i1 = vp_U[0]+vp_U[1]+vp_U[2];
      const T2 i2 = vp_U[0]*vp_U[1]+vp_U[0]*vp_U[2]+vp_U[1]*vp_U[2];
      const T3 i3 = vp_U[0]*vp_U[1]*vp_U[2];
      const T3 D  = i1*i2-i3;
      U = 1/D*(-C*C+(i1*i1-i2)*C+i1*i3*stensor<TensorTraits<TensorType>::dime,base>::Id());
      stensor<TensorTraits<TensorType>::dime,inv_T> U_1 =
	(C-i1*U+i2*stensor<TensorTraits<TensorType>::dime,base>::Id())*(1/i3);
      R = F * U_1;
    } // end of polar_decomposition

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond,
      MatrixExpr<tmatrix<3u,3u,typename TensorTraits<TensorType>::NumType>,
		 MatrixViewFromTensorExpr<TensorType> >
    >::type
    matrix_view(const TensorType& t)
    {
      return MatrixExpr<tmatrix<3u,3u,typename TensorTraits<TensorType>::NumType>,
			MatrixViewFromTensorExpr<TensorType> >(t);
    } // end of matrix_view

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      (TensorTraits<TensorType>::dime==1u),
      TensorExpr<tensor<1u,typename TensorTraits<TensorType>::NumType>,
		 TensorTransposeExpr1D<TensorType> >
      >::type
    transpose(const TensorType& t){
      return TensorExpr<tensor<1u,typename TensorTraits<TensorType>::NumType>,
			TensorTransposeExpr1D<TensorType> >(t);
    } // end of transpose

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      (TensorTraits<TensorType>::dime==2u),
      TensorExpr<tensor<2u,typename TensorTraits<TensorType>::NumType>,
		 TensorTransposeExpr2D<TensorType> >
      >::type
    transpose(const TensorType& t){
      return TensorExpr<tensor<2u,typename TensorTraits<TensorType>::NumType>,
			TensorTransposeExpr2D<TensorType> >(t);
    } // end of transpose

    template<typename TensorType>
    typename std::enable_if<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      (TensorTraits<TensorType>::dime==3u),
      TensorExpr<tensor<3u,typename TensorTraits<TensorType>::NumType>,
		 TensorTransposeExpr3D<TensorType> >
      >::type
    transpose(const TensorType& t){
      return TensorExpr<tensor<3u,typename TensorTraits<TensorType>::NumType>,
			TensorTransposeExpr3D<TensorType> >(t);
    } // end of transpose
  
    template<typename T>
    std::ostream&
    operator << (std::ostream & os,const TensorConcept<T>& s)
    {
      unsigned short i;
      os << "[ ";
      for(i=0;i!=TensorDimeToSize<TensorTraits<T>::dime>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_TENSOR_CONCEPT_IMPL_ */
