/*!
 * \file   st2tot2ResultType.hxx
 * \brief  this file implements partial specialisations of the
 * ResultType class for st2tot2.
 * \author Helfer Thomas
 * \date   19 November 2013
 */

#ifndef _LIB_TFEL_MATH_ST2TOT2_RESULT_TYPE_HXX_
#define _LIB_TFEL_MATH_ST2TOT2_RESULT_TYPE_HXX_ 

#include"TFEL/Math/st2tot2.hxx"
#include"TFEL/Math/stensor.hxx"

namespace tfel{
  
  namespace math {

    /*
     * Partial specialisation for negation
     */
    template<unsigned short N,typename T>
    class UnaryResultType<st2tot2<N,T>,OpNeg>
    {
      typedef typename UnaryResultType<T,OpNeg>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for st2tot2 and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,typename Scal>
    class ResultType_<ST2toT2Tag,ScalarTag,st2tot2<N,T>,Scal,OpMult>
    {
      typedef typename ResultType<T,Scal,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
    				      tfel::meta::InvalidType,
    				      st2tot2<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for st2tot2 and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,typename Scal>
    class ResultType_<ST2toT2Tag,ScalarTag,st2tot2<N,T>,Scal,OpDiv>
    {
      typedef typename ResultType<T,Scal,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for st2tot2 and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N,typename T>
    class ResultType_<ScalarTag,ST2toT2Tag,Scal,st2tot2<N,T>,OpMult>
    {
      typedef typename ResultType<Scal,T,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for st2tot2
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2>
    class ResultType<st2tot2<N,T>,st2tot2<N,T2>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for st2tot2
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2>
    class ResultType<st2tot2<N,T>,st2tot2<N,T2>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };

    /*!
     * \brief Partial specialisation for stensor
     * \see   ResultType
     */
    template<unsigned short N,typename T,typename T2,
	     template<unsigned short,typename> class Storage>
    class ResultType<tensor<N,T>,stensor<N,T2,Storage>,OpDiadicProduct>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      st2tot2<N,ResBase_> >::type type;
    };

  } // end of namespace math

} // end of namespace tfel
  
#endif /* _LIB_TFEL_MATH_ST2TOT2_RESULT_TYPE_HXX */

