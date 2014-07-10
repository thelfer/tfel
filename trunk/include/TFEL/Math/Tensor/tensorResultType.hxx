/*!
 * \file   tensorResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for tensor.
 * \author Helfer Thomas
 * \date   03 Jul 2006
 */

#ifndef _LIB_TFEL_TENSOR_RESULT_TYPE_HXX_
#define _LIB_TFEL_TENSOR_RESULT_TYPE_HXX_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
// #include"TFEL/Math/General/DotProduct.hxx"
#include"TFEL/Math/tensor.hxx"

namespace tfel{
  
  namespace math {

    /*
     * Partial specialisation for unary operations
     */
    template<unsigned short N,typename T>
    class UnaryResultType<tensor<N,T>,OpNeg>
    {
      typedef typename UnaryResultType<T,OpNeg>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for tensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T, 
	     typename Scal>
    class ResultType_<TensorTag,ScalarTag,tensor<N,T>,Scal,OpMult>
    {
      typedef typename ResultType<T,Scal,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     typename Scal>
    class ResultType_<TensorTag,ScalarTag,tensor<N,T>,Scal,OpDiv>
    {
      typedef typename ResultType<T,Scal,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for tensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N,typename T>
    class ResultType_<ScalarTag,TensorTag,Scal,tensor<N,T>,OpMult>
    {
      typedef typename ResultType<Scal,T,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2>
    class ResultType<tensor<N,T>,tensor<N,T2>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2>
    class ResultType<tensor<N,T>,tensor<N,T2>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };


    // /*!
    //  * \brief Partial specialisation for tensor
    //  * \see   ResultType
    //  */
    // template<unsigned short N,typename T,typename T2>
    // class ResultType<tensor<N,T>,tensor<N,T2>,OpDiadicProduct>
    // {
    //   typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    // public:
    //   typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
    // 				      tfel::meta::InvalidType,
    // 				      st2tost2<N,ResBase_> >::type type;
    // };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    // template<unsigned short N,typename T,typename T2>
    // class ResultType<tensor<N,T>,
    // 		     tensor<N,T2>,OpDotProduct>
    // {
    //   typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    //   static const bool isValid = tfel::typetraits::IsScalar<T>::cond  &&
    // 	                          tfel::typetraits::IsScalar<T2>::cond &&
    // 	                          tfel::typetraits::IsInvalid<ResBase_>::cond;
    // public:
    // 	typedef typename tfel::meta::IF<isValid,
    // 					tfel::meta::InvalidType,
    // 					ResBase_>::type type;
    // }; // end of class ResultType<tensor<N,T>,tensor<M,T2>,OpDotProduct>

    // template<unsigned short N,typename T,typename T2>
    // struct TensorDotProductHandle<tensor<N,T>,
    // 				   tensor<N,T2> >
    // {
    //   typedef DotProduct<TensorDimeToSize<N>::value,0u> type;
    // };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage>
    class ResultType<tensor<N,T>,stensor<N,T2,Storage>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage>
    class ResultType<stensor<N,T2,Storage>,tensor<N,T>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage>
    class ResultType<tensor<N,T>,stensor<N,T2,Storage>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for tensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage>
    class ResultType<stensor<N,T2,Storage>,tensor<N,T>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      tensor<N,ResBase_> >::type type;
    };

  } // end of namespace math

} // end of namespace tfel
  

#endif /* _LIB_TFEL_TENSOR_RESULT_TYPE_HXX */

