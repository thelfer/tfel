#ifndef _TFEL_MATH_TENSOR_CONCEPT_LIB_
#define _TFEL_MATH_TENSOR_CONCEPT_LIB_ 1

#include <stdexcept>
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
#include"TFEL/Math/Function/Power.hxx"
#include"TFEL/Math/Forward/stensor.hxx"

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

    /*!
     * exception thrown if invalid index is given
     */
    struct TFELMATH_VISIBILITY_EXPORT TensorInvalidIndexException
      : public std::runtime_error
    {
      /*!
       * constructor
       * \param[in] i : index
       */
      TensorInvalidIndexException(const unsigned short);
    private:
      /*!
       * format the error message
       * \param[in] i : index
       */
      static std::string
      getErrorMessage(const unsigned short);
    }; // end of struct TensorInvalidIndexException
    
    /*!
     * exception thrown if invalid indexes were given
     */
    struct TFELMATH_VISIBILITY_EXPORT TensorInvalidIndexesException
      : public std::runtime_error
    {
      /*!
       * constructor
       * \param[in] i : line index
       * \param[in] j : column index
       */
      TensorInvalidIndexesException(const unsigned short,
				    const unsigned short);
    private:
      /*!
       * format the error message
       * \param[in] i : line index
       * \param[in] j : column index
       */
      static std::string
      getErrorMessage(const unsigned short,
		      const unsigned short);
    }; // end of struct TensorInvalidIndexesException

    template<class T>
    class TensorConcept 
    {
      
      typedef TensorTraits<T> traits;
      static const bool isTemporary = tfel::typetraits::IsTemporary<T>::cond;
      typedef typename traits::NumType NumType;
      typedef typename tfel::meta::IF<isTemporary,NumType,
				      const NumType&>::type ValueType;
      
    protected:
      ~TensorConcept(){};
      
    public:

      typedef TensorTag ConceptTag;

      NumType 
      operator()(const unsigned short,
		 const unsigned short) const;
      
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
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T&);

    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T&);

    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T&);
    
    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    computeRightCauchyGreenTensor(const T&);
  
    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeRightCauchyGreenTensor(const T&);

    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
      (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeRightCauchyGreenTensor(const T&);

    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    computeLeftCauchyGreenTensor(const T&);
  
    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeLeftCauchyGreenTensor(const T&);

    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
      (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeLeftCauchyGreenTensor(const T&);

    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    computeGreenLagrangeTensor(const T&);
  
    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeGreenLagrangeTensor(const T&);

    template<class T>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
      (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeGreenLagrangeTensor(const T&);

    template<typename T,typename T2>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==1u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<1u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&,
						    const T2&);

    template<typename T,typename T2>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==2u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<2u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&,
						    const T2&);

    template<typename T,typename T2>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,StensorConcept>::cond) &&
       (StensorTraits<T>::dime==3u)&&
       (tfel::meta::Implements<T2,TensorConcept>::cond) &&
       (TensorTraits<T2>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T2>::NumType>::cond)),
      stensor<3u,typename StensorTraits<T>::NumType>
      >::type
    convertSecondPiolaKirchhoffStressToCauchyStress(const T&,
						    const T2&);
  
    template<typename TensorType>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType&);

    template<typename TensorType>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType&);

    template<typename TensorType>
    TFEL_MATH_INLINE2 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType&);

    // Serialisation operator
    template<typename T>
    std::ostream&
    operator << (std::ostream &,const TensorConcept<T>&);

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Tensor/TensorConcept.ixx"

#endif /* _TFEL_MATH_TENSOR_CONCEPT_LIB_ */
