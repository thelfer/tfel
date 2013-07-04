#ifndef _TENSOR_CONCEPT_LIB_
#define _TENSOR_CONCEPT_LIB_ 1

#include <ostream>
#include <string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/TypeTraits/BaseType.hxx"

#include"TFEL/Math/General/Abs.hxx"

namespace tfel{

  namespace math{

    template<class T>
    struct TensorTraits{
      typedef tfel::meta::InvalidType NumType;
      typedef unsigned short          IndexType;
      static const unsigned short dime = 0u;
    };

    /*!
     * \class TensorTag
     * \brief Helper class to characterise tensors.
     */ 
    struct TensorTag
    {
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return const std::string, the name of the class.
       * \see    Name.
       */
      static std::string 
      getName(void);
    }; // end of TensorTag
    
    template<class T>
    class TensorConcept 
    {
      
      typedef TensorTraits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename tfel::meta::IF<isTemporary,
				      typename traits::NumType,
				      const typename traits::NumType&>::type ValueType;
      
    protected:
      ~TensorConcept(){};
      
    public:

      typedef TensorTag ConceptTag;
      
      ValueType 
      operator()(const unsigned short) const;

      ValueType 
      operator[](const unsigned short) const;
      
    };
  
    template<typename T>
    struct TensorType{
      typedef T type;
    };

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
    >::type
    abs(const TensorType&);

    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
    >::type
    trace(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
    >::type
    sigmaeq(const T& s);

    template<class T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorType<T>::type
    >::type
    deviator(const T& s)
    {
      typedef typename TensorType<T>::type Res;
      typedef typename TensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type Base;
      return Res(s-(Base(1)/Base(3))*trace(s)*Res::Id());
    }

    // Serialisation operator
    template<typename T>
    std::ostream&
    operator << (std::ostream &,const TensorConcept<T>&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* _TENSOR_CONCEPT_LIB_ */
