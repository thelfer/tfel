/*!
 * \file   compositeResultType.ixx
 * \see    GenerateComposite.cxx to see how this file was generated.
 * \brief  This file declares operations on the composite template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_COMPOSITERESULTTYPE_I_
#define _LIB_TFEL_COMPOSITERESULTTYPE_I_ 

#include"Math/General/UnaryResultType.hxx"
#include"Math/General/ResultType.hxx"
#include"Math/General/BasicOperations.hxx"

namespace tfel{

namespace math{

namespace internals{

template<unsigned short N,typename List>
struct GenerateCompositeFromTypeList;

template<typename List>
class GenerateCompositeFromTypeList<1,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
public :
typedef tfel::math::composite<Type0> type;
};

template<typename List>
class GenerateCompositeFromTypeList<2,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
public :
typedef tfel::math::composite<Type0,Type1> type;
};

template<typename List>
class GenerateCompositeFromTypeList<3,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
public :
typedef tfel::math::composite<Type0,Type1,Type2> type;
};

template<typename List>
class GenerateCompositeFromTypeList<4,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3> type;
};

template<typename List>
class GenerateCompositeFromTypeList<5,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
typedef typename tfel::meta::TypeListFindNthElt<List,4>::type Type4;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3,Type4> type;
};

template<typename List>
class GenerateCompositeFromTypeList<6,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
typedef typename tfel::meta::TypeListFindNthElt<List,4>::type Type4;
typedef typename tfel::meta::TypeListFindNthElt<List,5>::type Type5;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3,Type4,Type5> type;
};

template<typename List>
class GenerateCompositeFromTypeList<7,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
typedef typename tfel::meta::TypeListFindNthElt<List,4>::type Type4;
typedef typename tfel::meta::TypeListFindNthElt<List,5>::type Type5;
typedef typename tfel::meta::TypeListFindNthElt<List,6>::type Type6;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3,Type4,Type5,Type6> type;
};

template<typename List>
class GenerateCompositeFromTypeList<8,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
typedef typename tfel::meta::TypeListFindNthElt<List,4>::type Type4;
typedef typename tfel::meta::TypeListFindNthElt<List,5>::type Type5;
typedef typename tfel::meta::TypeListFindNthElt<List,6>::type Type6;
typedef typename tfel::meta::TypeListFindNthElt<List,7>::type Type7;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3,Type4,Type5,Type6,Type7> type;
};

template<typename List>
class GenerateCompositeFromTypeList<9,List>
{
typedef typename tfel::meta::TypeListFindNthElt<List,0>::type Type0;
typedef typename tfel::meta::TypeListFindNthElt<List,1>::type Type1;
typedef typename tfel::meta::TypeListFindNthElt<List,2>::type Type2;
typedef typename tfel::meta::TypeListFindNthElt<List,3>::type Type3;
typedef typename tfel::meta::TypeListFindNthElt<List,4>::type Type4;
typedef typename tfel::meta::TypeListFindNthElt<List,5>::type Type5;
typedef typename tfel::meta::TypeListFindNthElt<List,6>::type Type6;
typedef typename tfel::meta::TypeListFindNthElt<List,7>::type Type7;
typedef typename tfel::meta::TypeListFindNthElt<List,8>::type Type8;
public :
typedef tfel::math::composite<Type0,Type1,Type2,Type3,Type4,Type5,Type6,Type7,Type8> type;
};

struct CompositeInvalidSize
{
typedef tfel::meta::InvalidType type;
};

template<typename TList,typename UList,typename Op>
struct CompositeComputeResultList;

template<typename Op>
struct CompositeComputeResultList<tfel::meta::TypeListEndType,
tfel::meta::TypeListEndType,
Op>
{
typedef tfel::meta::TypeListEndType type;
};

template<typename TList,typename UList,typename Op>
class CompositeComputeResultList
{
typedef typename tfel::math::ResultType<typename TList::Current,
typename UList::Current,
Op>::type Result;
typedef typename CompositeComputeResultList<typename TList::Next,
typename UList::Next,
Op>::type Next;
public:
typedef tfel::meta::TypeListNode<Result,Next> type;
};

template<typename TList,typename UList,typename Op>
class CompositeComputeResult_
{
typedef typename CompositeComputeResultList<TList,UList,Op>::type ResList;
typedef typename GenerateCompositeFromTypeList<tfel::meta::TypeListSize<ResList>::value,
ResList>::type CompositeResult;
public:
typedef typename tfel::meta::IF<
tfel::meta::TypeListContainsInvalidType<ResList>::cond,
tfel::meta::InvalidType,
CompositeResult
>::type type;
};

template<typename S,typename TList,typename Op>
struct ScalarCompositeComputeResultList;

template<typename S,typename Op>
struct ScalarCompositeComputeResultList<S,tfel::meta::TypeListEndType,Op>
{
typedef tfel::meta::TypeListEndType type;
};

template<typename S,typename TList,typename Op>
class ScalarCompositeComputeResultList
{
typedef typename tfel::math::ResultType<S,typename TList::Current,
Op>::type Result;
typedef typename ScalarCompositeComputeResultList<S,typename TList::Next,Op>::type Next;
public:
typedef tfel::meta::TypeListNode<Result,Next> type;
};
template<typename TList,typename S,typename Op>
struct CompositeScalarComputeResultList;

template<typename S,typename Op>
struct CompositeScalarComputeResultList<tfel::meta::TypeListEndType,S,Op>
{
typedef tfel::meta::TypeListEndType type;
};

template<typename TList,typename S,typename Op>
class CompositeScalarComputeResultList
{
typedef typename tfel::math::ResultType<typename TList::Current,S,
Op>::type Result;
typedef typename CompositeScalarComputeResultList<typename TList::Next,S,Op>::type Next;
public:
typedef tfel::meta::TypeListNode<Result,Next> type;
};
template<typename TList,typename Op>
struct CompositeComputeUnaryResultList;

template<typename Op>
struct CompositeComputeUnaryResultList<tfel::meta::TypeListEndType,Op>
{
typedef tfel::meta::TypeListEndType type;
};

template<typename TList,typename Op>
class CompositeComputeUnaryResultList
{
typedef typename tfel::math::UnaryResultType<typename TList::Current,
Op>::type Result;
typedef typename CompositeComputeUnaryResultList<typename TList::Next,Op>::type Next;
public:
typedef tfel::meta::TypeListNode<Result,Next> type;
};
template<typename TList,typename UList,typename Op>
class CompositeComputeResult
: public tfel::meta::IF<tfel::meta::TypeListSize<TList>::value!=
tfel::meta::TypeListSize<UList>::value,
CompositeInvalidSize,
CompositeComputeResult_<TList,UList,Op> >::type
{};

template<typename TList,typename Op>
class CompositeComputeUnaryResult
{
typedef typename CompositeComputeUnaryResultList<TList,Op>::type ResList;
typedef typename GenerateCompositeFromTypeList<tfel::meta::TypeListSize<ResList>::value,
ResList>::type CompositeResult;
public:
typedef typename tfel::meta::IF<
tfel::meta::TypeListContainsInvalidType<ResList>::cond,
tfel::meta::InvalidType,
CompositeResult
>::type type;
};

template<typename S,typename TList,typename Op>
class ScalarCompositeComputeResult
{
typedef typename ScalarCompositeComputeResultList<S,TList,Op>::type ResList;
typedef typename GenerateCompositeFromTypeList<tfel::meta::TypeListSize<ResList>::value,
ResList>::type CompositeResult;
public:
typedef typename tfel::meta::IF<
tfel::meta::TypeListContainsInvalidType<ResList>::cond,
tfel::meta::InvalidType,
CompositeResult
>::type type;
};

template<typename TList,typename S,typename Op>
class CompositeScalarComputeResult
{
typedef typename CompositeScalarComputeResultList<TList,S,Op>::type ResList;
typedef typename GenerateCompositeFromTypeList<tfel::meta::TypeListSize<ResList>::value,
ResList>::type CompositeResult;
public:
typedef typename tfel::meta::IF<
tfel::meta::TypeListContainsInvalidType<ResList>::cond,
tfel::meta::InvalidType,
CompositeResult
>::type type;
};

} // end of namespace internals

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8,typename U0,typename U1,typename U2,
typename U3,typename U4,typename U5,typename U6,
typename U7,typename U8>
class ResultType<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,
composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,OpPlus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
typedef typename CompositeTraits<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8> >::Items UItems;
public :
typedef typename tfel::math::internals::CompositeComputeResult<
TItems,UItems,OpPlus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename T0,typename T1,
typename T2,typename T3,typename T4,typename T5,
typename T6,typename T7,typename T8,typename U0,
typename U1,typename U2,typename U3,typename U4,
typename U5,typename U6,typename U7,typename U8>
class ResultType<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,
composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,OpMinus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
typedef typename CompositeTraits<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8> >::Items UItems;
public :
typedef typename tfel::math::internals::CompositeComputeResult<
TItems,UItems,OpMinus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8,typename U0,typename U1,typename U2,
typename U3,typename U4,typename U5,typename U6,
typename U7,typename U8>
class ResultType<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,
composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,OpMult>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
typedef typename CompositeTraits<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8> >::Items UItems;
public :
typedef typename tfel::math::internals::CompositeComputeResult<
TItems,UItems,OpMult>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename T0,typename T1,
typename T2,typename T3,typename T4,typename T5,
typename T6,typename T7,typename T8,typename U0,
typename U1,typename U2,typename U3,typename U4,
typename U5,typename U6,typename U7,typename U8>
class ResultType<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,
composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,OpDiv>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
typedef typename CompositeTraits<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8> >::Items UItems;
public :
typedef typename tfel::math::internals::CompositeComputeResult<
TItems,UItems,OpDiv>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<ScalarTag,CompositeTag,Scal,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,OpPlus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::ScalarCompositeComputeResult<
Scal,TItems,OpPlus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<ScalarTag,CompositeTag,Scal,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,OpMinus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::ScalarCompositeComputeResult<
Scal,TItems,OpMinus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<ScalarTag,CompositeTag,Scal,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,OpMult>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::ScalarCompositeComputeResult<
Scal,TItems,OpMult>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<ScalarTag,CompositeTag,Scal,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,OpDiv>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::ScalarCompositeComputeResult<
Scal,TItems,OpDiv>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<CompositeTag,ScalarTag,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,Scal,OpPlus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeScalarComputeResult<
TItems,Scal,OpPlus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<CompositeTag,ScalarTag,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,Scal,OpMinus>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeScalarComputeResult<
TItems,Scal,OpMinus>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<CompositeTag,ScalarTag,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,Scal,OpMult>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeScalarComputeResult<
TItems,Scal,OpMult>::type type;
};

/*!
* \brief Partial specialisation for composite
* \see   ResultType
*/
template<typename Scal,typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class ResultType_<CompositeTag,ScalarTag,
composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,Scal,OpDiv>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeScalarComputeResult<
TItems,Scal,OpDiv>::type type;
};

/*
* Partial specialisation for functions.
*/
template<typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8,typename Op>
class UnaryResultType_<CompositeTag,FunctionTag,composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,Op>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeComputeUnaryResult<
TItems,Op>::type type;
};

/*
* Partial specialisation for functions.
*/
template<typename T0,typename T1,typename T2,typename T3,
typename T4,typename T5,typename T6,typename T7,
typename T8>
class UnaryResultType<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>,OpNeg>
{
typedef typename CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8> >::Items TItems;
public :
typedef typename tfel::math::internals::CompositeComputeUnaryResult<
TItems,OpNeg>::type type;
};

} // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_COMPOSITERESULTTYPE_I_ */
