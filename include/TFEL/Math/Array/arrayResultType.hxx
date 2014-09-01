/*!
 * \file   include/TFEL/Math/Array/arrayResultType.hxx
 * \brief  this file implements partial specialisations of the 
 * ResultType class.
 * \author Helfer Thomas
 * \date   01 jui 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_ARRAYRESULTTYPE_HXX_
#define _LIB_TFEL_ARRAYRESULTTYPE_HXX_ 

#include"TFEL/Math/General/UnaryResultType.hxx"
#include"TFEL/Math/array.hxx"

namespace tfel{

  namespace math {
    
    /*
     * Partial specialisation for functions.
     */
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage,typename Op>
    class UnaryResultType_<ArrayTag,FunctionTag,array<N,T,Storage>,Op>
    {
      typedef typename UnaryResultType<T,Op>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };
    
    /*
     * Partial specialisation for unary negation
     */
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    class UnaryResultType<array<N,T,Storage>,OpNeg>
    {
      typedef typename UnaryResultType<T,OpNeg>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,typename Scal>
    class ResultType_<ArrayTag,ScalarTag,array<N,T,Storage>,Scal,OpPlus>
    {
      typedef typename ResultType<T,Scal,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,typename Scal>
    class ResultType_<ArrayTag,ScalarTag,array<N,T,Storage>,Scal,OpMinus>
    {
      typedef typename ResultType<T,Scal,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,typename Scal>
    class ResultType_<ArrayTag,ScalarTag,array<N,T,Storage>,Scal,OpMult>
    {
      typedef typename ResultType<T,Scal,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<unsigned short N, typename T,
             template<unsigned short,typename> class Storage,typename Scal>
    class ResultType_<ArrayTag,ScalarTag,array<N,T,Storage>,Scal,OpDiv>
    {
      typedef typename ResultType<T,Scal,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N, 
	     typename T,template<unsigned short,typename> class Storage>
    class ResultType_<ScalarTag,ArrayTag,Scal,array<N,T,Storage>,OpPlus>
    {
      typedef typename ResultType<Scal,T,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N, 
	     typename T,template<unsigned short,typename> class Storage>
    class ResultType_<ScalarTag,ArrayTag,Scal,array<N,T,Storage>,OpMinus>
    {
      typedef typename ResultType<Scal,T,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N, 
	     typename T,template<unsigned short,typename> class Storage>
    class ResultType_<ScalarTag,ArrayTag,Scal,array<N,T,Storage>,OpMult>
    {
      typedef typename ResultType<Scal,T,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array and 
     * scalars operations
     * \see   ResultType_
     */
    template<typename Scal,unsigned short N, 
	     typename T,template<unsigned short,typename> class Storage>
    class ResultType_<ScalarTag,ArrayTag,Scal,array<N,T,Storage>,OpDiv>
    {
      typedef typename ResultType<Scal,T,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };
    
    /*!
     * \brief Partial specialisation for array
     * \see   ResultType
     */
    template<unsigned short N, typename T, 
	     typename T2,template<unsigned short,typename> class Storage>
    class ResultType<array<N,T,Storage>,array<N,T2,Storage>,OpPlus>
    {
      typedef typename ResultType<T,T2,OpPlus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array
     * \see   ResultType
     */
    template<unsigned short N, typename T, 
	     typename T2,template<unsigned short,typename> class Storage>
    class ResultType<array<N,T,Storage>,array<N,T2,Storage>,OpMinus>
    {
      typedef typename ResultType<T,T2,OpMinus>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array
     * \see   ResultType
     */
    template<unsigned short N, typename T, 
	     typename T2,template<unsigned short,typename> class Storage>
    class ResultType<array<N,T,Storage>,array<N,T2,Storage>,OpMult>
    {
      typedef typename ResultType<T,T2,OpMult>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

    /*!
     * \brief Partial specialisation for array
     * \see   ResultType
     */
    template<unsigned short N, typename T, 
	     typename T2,template<unsigned short,typename> class Storage>
    class ResultType<array<N,T,Storage>,array<N,T2,Storage>,OpDiv>
    {
      typedef typename ResultType<T,T2,OpDiv>::type ResBase_;
    public:
      typedef typename tfel::meta::IF<tfel::typetraits::IsInvalid<ResBase_>::cond,
				      tfel::meta::InvalidType,
				      array<N,ResBase_,Storage> >::type type;
    };

  } //end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_ARRAYRESULTTYPE_HXX */

