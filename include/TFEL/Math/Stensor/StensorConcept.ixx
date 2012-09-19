#ifndef _STENSOR_CONCEPT_IMPL_
#define _STENSOR_CONCEPT_IMPL_ 1

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Stensor/StensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
    template<class T>
    typename StensorConcept<T>::ValueType
    StensorConcept<T>::operator()(const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    typename StensorConcept<T>::ValueType
    StensorConcept<T>::operator[](const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }
    
    template<class T>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,StensorConcept>::cond,
      typename StensorTraits<T>::NumType
    >::type
    trace(const T& s)
    {
      return s(0)+s(1)+s(2);
    }     

    namespace internals{
      
      struct SigmaEqImplBase
      {
	template<typename T>
	static T
	square(const T& x)
	{
	  return x*x;
	}
      };

      template<unsigned short N>
      class SigmaEqImpl;

      template<>
      struct SigmaEqImpl<1u>
	: public SigmaEqImplBase
      {
	template<class T>
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(SigmaEqImplBase::square(s(0)-tr)+
				  SigmaEqImplBase::square(s(1)-tr)+
				  SigmaEqImplBase::square(s(2)-tr)));
	}
      };
      
      template<>
      struct SigmaEqImpl<2u>
	: public SigmaEqImplBase
      {
	template<class T>
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(SigmaEqImplBase::square(s(0)-tr)+
				  SigmaEqImplBase::square(s(1)-tr)+
				  SigmaEqImplBase::square(s(2)-tr)+
				  SigmaEqImplBase::square(s(3))));
	}
      };

      template<>
      struct SigmaEqImpl<3u>
	: public SigmaEqImplBase
      {
	template<class T>
	static typename tfel::meta::EnableIf<
	  tfel::meta::Implements<T,StensorConcept>::cond,
	  typename StensorTraits<T>::NumType
	>::type
	exe(const T& s)
	{
	  typedef typename StensorTraits<T>::NumType NumType;
	  typedef typename tfel::typetraits::BaseType<NumType>::type Base;
	  NumType tr = (Base(1)/Base(3))*trace(s);
	  return sqrt(Base(1.5f)*(SigmaEqImplBase::square(s(0)-tr)+
				  SigmaEqImplBase::square(s(1)-tr)+
				  SigmaEqImplBase::square(s(2)-tr)+
				  SigmaEqImplBase::square(s(3))   +
				  SigmaEqImplBase::square(s(4))   +
				  SigmaEqImplBase::square(s(5))));
	}
      };

      template<unsigned short N>
      struct StensorAbs;

      template<>
      struct StensorAbs<1u>
      {
	template<typename StensorType>
	static typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2));
	}
      };

      template<>
      struct StensorAbs<2u>
      {
	template<typename StensorType>
	static typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3));
	}
      };

      template<>
      struct StensorAbs<3u>
      {
	template<typename StensorType>
	static typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
	exe(const StensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5));
	}
      };
      
    } // end of namespace internals

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename StensorTraits<StensorType>::NumType>::type
    >::type
    abs(const StensorType& s)
    {
      return tfel::math::internals::StensorAbs<StensorTraits<StensorType>::dime>::exe(s);
    }

    template<class T>
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
