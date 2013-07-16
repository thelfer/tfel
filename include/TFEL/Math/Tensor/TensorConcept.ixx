#ifndef _TENSOR_CONCEPT_IMPL_
#define _TENSOR_CONCEPT_IMPL_ 1

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"

namespace tfel{

  namespace math{

    namespace internals{

      template<unsigned short N>
      struct TensorAbs;

      template<>
      struct TensorAbs<1u>
      {
	template<typename TensorType>
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
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5))
	    +tfel::math::abs(s(6))+tfel::math::abs(s(7))+tfel::math::abs(s(8));
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
      if((i==j)&&(i<3)){
	return static_cast<const T&>(*this).operator()(i);
      } else if((i==0)&&(j==1)){
	return (TensorTraits<T>::dime<2) ? NumType(0) : static_cast<const T&>(*this).operator()(3);
      } else if((i==1)&&(j==0)){
	return (TensorTraits<T>::dime<2) ? NumType(0) : static_cast<const T&>(*this).operator()(4);
      } else if((i==0)&&(j==2)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(5);
      } else if((i==2)&&(j==0)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(6);
      } else if((i==1)&&(j==2)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(7);
      } else if((i==2)&&(j==1)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(8);
      }
      throw(TensorInvalidIndexesException(i,j));
      return NumType(0);
    }
    
    template<class T>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
      >::type
    trace(const T& s)
      {
	return s(0)+s(1)+s(2);
      }     

  template<typename TensorType>
  typename tfel::meta::EnableIf<
    tfel::meta::Implements<TensorType,TensorConcept>::cond,
    typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
    >::type
  abs(const TensorType& s)
  {
    return tfel::math::internals::TensorAbs<TensorTraits<TensorType>::dime>::exe(s);
  }

  template<class T>
  TFEL_MATH_INLINE 
  typename tfel::meta::EnableIf<
    ((tfel::meta::Implements<T,TensorConcept>::cond) &&
     (TensorTraits<T>::dime==1u)),
    stensor<1u,typename TensorTraits<T>::NumType>
    >::type
  syme(const T& t)
  {
    return stensor<1u,typename TensorTraits<T>::NumType>(t.begin());
  }

  template<class T>
  TFEL_MATH_INLINE 
  typename tfel::meta::EnableIf<
    ((tfel::meta::Implements<T,TensorConcept>::cond) &&
     (TensorTraits<T>::dime==2u)),
    stensor<2u,typename TensorTraits<T>::NumType>
    >::type
  syme(const T& t)
  {
    using namespace std;
    typedef typename TensorTraits<T>::NumType NumType;
    typedef typename tfel::typetraits::BaseType<NumType>::type real;
    static const real cste = sqrt(real(2))/real(2);
    stensor<2u,NumType> s;
    s[0] = t[0];
    s[1] = t[1];
    s[2] = t[2];
    s[3] = cste*(t[3]+t[4]);
    return s;
  }

  template<class T>
  TFEL_MATH_INLINE 
  typename tfel::meta::EnableIf<
    ((tfel::meta::Implements<T,TensorConcept>::cond) &&
     (TensorTraits<T>::dime==3u)),
    stensor<3u,typename TensorTraits<T>::NumType>
    >::type
  syme(const T& t)
  {
    using namespace std;
    typedef typename TensorTraits<T>::NumType NumType;
    typedef typename tfel::typetraits::BaseType<NumType>::type real;
    static const real cste = sqrt(real(2))/real(2);
    stensor<3u,NumType> s;
    s[0] = t[0];
    s[1] = t[1];
    s[2] = t[2];
    s[3] = cste*(t[3]+t[4]);
    s[4] = cste*(t[5]+t[6]);
    s[5] = cste*(t[7]+t[8]);
    return s;
  }

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

#endif /* _TENSOR_CONCEPT_IMPL_ */
