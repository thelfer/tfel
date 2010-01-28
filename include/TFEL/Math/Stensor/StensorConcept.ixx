#ifndef _STENSOR_CONCEPT_IMPL_
#define _STENSOR_CONCEPT_IMPL_ 1

#include"TFEL/Math/functions.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    TFEL_MATH_INLINE 
    typename StensorConcept<T>::ValueType
    StensorConcept<T>::operator()(const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    TFEL_MATH_INLINE 
    typename StensorConcept<T>::ValueType
    StensorConcept<T>::operator[](const unsigned short i) const {
      return static_cast<const T&>(*this).operator[](i);
    }
    
    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    trace(const T& s)
    {
      return s(0)+s(1)+s(2);
    }     

    namespace internals{
      
      template<unsigned short N>
      class SigmaEqImpl;

      template<>
      struct SigmaEqImpl<1u>
      {
	template<class T>
	TFEL_MATH_INLINE2
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  USING_TFEL_FUNCTIONS;
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(Power<2>::exe(s(0)-tr)+
				  Power<2>::exe(s(1)-tr)+
				  Power<2>::exe(s(2)-tr)));
	}
      };
      
      template<>
      struct SigmaEqImpl<2u>
      {
	template<class T>
	TFEL_MATH_INLINE2
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  USING_TFEL_FUNCTIONS;
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(Power<2>::exe(s(0)-tr)+
				  Power<2>::exe(s(1)-tr)+
				  Power<2>::exe(s(2)-tr)+
				  Power<2>::exe(s(3))));
	}
      };

      template<>
      struct SigmaEqImpl<3u>
      {
	template<class T>
	TFEL_MATH_INLINE2
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  USING_TFEL_FUNCTIONS;
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(Power<2>::exe(s(0)-tr)+
				  Power<2>::exe(s(1)-tr)+
				  Power<2>::exe(s(2)-tr)+
				  Power<2>::exe(s(3))   +
				  Power<2>::exe(s(4))   +
				  Power<2>::exe(s(5))));
	}
      };
      
    }

    template<class T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    sigmaeq(const T& s)
    {
      typedef tfel::math::internals::SigmaEqImpl<StensorTraits<T>::dime> Impl;
      return Impl::exe(s);
    }

    template<typename T>
    std::ostream&
    operator << (std::ostream & os,const StensorConcept<T>& s)
    {
      unsigned short i;
      os << "[ ";
      for(i=0;i!=StensorDimeToSize<StensorTraits<T>::dime>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 

  } // end of namespace math

} // end of namespace tfel

#endif /* _STENSOR_CONCEPT_IMPL_ */
