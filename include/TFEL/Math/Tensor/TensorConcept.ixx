#ifndef _TENSOR_CONCEPT_IMPL_
#define _TENSOR_CONCEPT_IMPL_ 1

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"

namespace tfel{

  namespace math{
    
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
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
    >::type
    trace(const T& s)
    {
      return s(0)+s(1)+s(2);
    }     

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
	    +tfel::math::abs(s(3));
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
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5));
	}
      };
      
    } // end of namespace internals

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
    >::type
    abs(const TensorType& s)
    {
      return tfel::math::internals::TensorAbs<TensorTraits<TensorType>::dime>::exe(s);
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
