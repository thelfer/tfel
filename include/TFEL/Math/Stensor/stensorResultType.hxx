/*!
 * \file   stensorResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for stensor.
 * \author Helfer Thomas
 * \date   03 Jul 2006
 */

#ifndef _LIB_TFEL_STENSOR_RESULT_TYPE_HXX_
#define _LIB_TFEL_STENSOR_RESULT_TYPE_HXX_ 

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/Math/General/DotProduct.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"

namespace tfel{
  
  namespace math {

    /*
     * Partial specialisation for unary operations
     */
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    class UnaryResultType<stensor<N,T,Storage>,OpNeg>
    {
      typedef typename UnaryResultType<T,OpNeg>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for stensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage, 
	     typename Scal>
    class ResultType_<StensorTag,ScalarTag,stensor<N,T,Storage>,Scal,OpMult>
    {
      typedef typename ResultType<T,Scal,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,
	     typename Scal>
    class ResultType_<StensorTag,ScalarTag,stensor<N,T,Storage>,Scal,OpDiv>
    {
      typedef typename ResultType<T,Scal,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for stensor and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    class ResultType_<ScalarTag,StensorTag,Scal,stensor<N,T,Storage>,OpMult>
    {
      typedef typename ResultType<Scal,T,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    class ResultType<stensor<N,T,Storage>,stensor<N,T2,Storage2>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    class ResultType<stensor<N,T,Storage>,stensor<N,T2,Storage2>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    class ResultType<stensor<N,T,Storage>,stensor<N,T2,Storage2>,OpMult>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      stensor<N,ResBase_,StensorStatic> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    class ResultType<stensor<N,T,Storage>,stensor<N,T2,Storage2>,OpDiadicProduct>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tost2<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    class ResultType<stensor<N,T,Storage>,
		     stensor<N,T2,Storage2>,OpDotProduct>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
      static const bool isValid = tfel::typetraits::IsScalar<T>::cond  &&
	                          tfel::typetraits::IsScalar<T2>::cond &&
	                          tfel::typetraits::IsInvalid<ResBase_>::cond;
    public:
	typedef typename tfel::meta::IF<isValid,
					tfel::meta::InvalidType,
					ResBase_>::type type;
    }; // end of class ResultType<stensor<N,T>,stensor<M,T2>,OpDotProduct>

    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage,
	     template<unsigned short,typename> class Storage2>
    struct StensorDotProductHandle<stensor<N,T,Storage>,
				   stensor<N,T2,Storage2> >
    {
      typedef DotProduct<StensorDimeToSize<N>::value,0u> type;
    };

  } // end of namespace math

} // end of namespace tfel
  

#endif /* _LIB_TFEL_STENSOR_RESULT_TYPE_HXX */

