/*!
 * \file   GenerateTypeList.ixx
 * \see    GenerateTypeList.cxx to see how this file was generated.
 * \brief  This file implements the GenerateTypeList template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_GENERATETYPELIST_I_
#define _LIB_TFEL_GENERATETYPELIST_I_ 

namespace tfel{

  namespace meta{

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9,typename T10,typename T11,
	     typename T12,typename T13,typename T14,typename T15,
	     typename T16,typename T17,typename T18,typename T19,
	     typename T20,typename T21,typename T22,typename T23,
	     typename T24,typename T25,typename T26,typename T27,
	     typename T28,typename T29,typename T30,typename T31,
	     typename T32,typename T33,typename T34,typename T35,
	     typename T36,typename T37,typename T38,typename T39,
	     typename T40,typename T41,typename T42,typename T43,
	     typename T44,typename T45,typename T46,typename T47,
	     typename T48,typename T49>
    struct GenerateTypeList{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListNode<T45,TypeListNode<T46,TypeListNode<T47,TypeListNode<T48,TypeListNode<T49,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<>
    struct GenerateTypeList<TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,
			    TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>
    {
      typedef TypeListEndType type;
    };

    template<typename T0>
    struct GenerateTypeList<T0,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListEndType > type;
    };

    template<typename T0,typename T1>
    struct GenerateTypeList<T0,T1,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListEndType > > type;
    };

    template<typename T0,typename T1,typename T2>
    struct GenerateTypeList<T0,T1,T2,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListEndType > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3>
    struct GenerateTypeList<T0,T1,T2,T3,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListEndType > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4>
    struct GenerateTypeList<T0,T1,T2,T3,T4,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListEndType > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListEndType > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListEndType > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListEndType > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListEndType > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListEndType > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListEndType > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListEndType > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListEndType > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListEndType > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListEndType > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListEndType > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListEndType > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListEndType > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListEndType > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListEndType > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListEndType > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43,typename T44>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,T44,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43,typename T44,typename T45>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,T44,T45,TypeListEndType,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListNode<T45,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43,typename T44,typename T45,typename T46>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,T44,T45,T46,TypeListEndType,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListNode<T45,TypeListNode<T46,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43,typename T44,typename T45,typename T46,typename T47>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,T44,T45,T46,T47,TypeListEndType,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListNode<T45,TypeListNode<T46,TypeListNode<T47,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
             typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,
             typename T12,typename T13,typename T14,typename T15,typename T16,typename T17,
             typename T18,typename T19,typename T20,typename T21,typename T22,typename T23,
             typename T24,typename T25,typename T26,typename T27,typename T28,typename T29,
             typename T30,typename T31,typename T32,typename T33,typename T34,typename T35,
             typename T36,typename T37,typename T38,typename T39,typename T40,typename T41,
             typename T42,typename T43,typename T44,typename T45,typename T46,typename T47,
             typename T48>
    struct GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14,T15,T16,T17,T18,T19,T20,T21,T22,T23,T24,T25,T26,T27,T28,T29,T30,T31,T32,T33,T34,T35,T36,T37,T38,T39,T40,T41,T42,T43,T44,T45,T46,T47,T48,TypeListEndType>{
      typedef TypeListNode<T0,TypeListNode<T1,TypeListNode<T2,TypeListNode<T3,TypeListNode<T4,TypeListNode<T5,TypeListNode<T6,TypeListNode<T7,TypeListNode<T8,TypeListNode<T9,TypeListNode<T10,TypeListNode<T11,TypeListNode<T12,TypeListNode<T13,TypeListNode<T14,TypeListNode<T15,TypeListNode<T16,TypeListNode<T17,TypeListNode<T18,TypeListNode<T19,TypeListNode<T20,TypeListNode<T21,TypeListNode<T22,TypeListNode<T23,TypeListNode<T24,TypeListNode<T25,TypeListNode<T26,TypeListNode<T27,TypeListNode<T28,TypeListNode<T29,TypeListNode<T30,TypeListNode<T31,TypeListNode<T32,TypeListNode<T33,TypeListNode<T34,TypeListNode<T35,TypeListNode<T36,TypeListNode<T37,TypeListNode<T38,TypeListNode<T39,TypeListNode<T40,TypeListNode<T41,TypeListNode<T42,TypeListNode<T43,TypeListNode<T44,TypeListNode<T45,TypeListNode<T46,TypeListNode<T47,TypeListNode<T48,TypeListEndType > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > > type;
    };

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_GENERATETYPELIST_I_ */
