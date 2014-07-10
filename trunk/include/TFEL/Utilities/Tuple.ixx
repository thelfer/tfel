/*!
 * \file   Tuple.ixx
 * \see    GenerateTuple.cxx to see how this file was generated.
 * \brief  This file implements the Tuple template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_TUPLE_I_
#define _LIB_TFEL_TUPLE_I_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"

namespace tfel{

  namespace utilities{

    namespace internals{

      /*!
       * base class for tuples
       * \param N    : index
       * \param Type : holded type
       */ 
      template<unsigned short N,typename Type>
      struct TupleContainer
      {
	/*!
	 * \brief constructor
	 * \param val : value holded 
	 */
	TupleContainer(const Type& val)
	  : value(val)
	{}
	/*!
	 * \return holded value
	 */
	inline const Type&
	getValue(void) const
	{
	  return this->value;
	}
	/*!
	 * set value
	 * \param : val : value to be set
	 */
	inline void
	setValue(const Type& val)
	{
	  this->value=val;
	}
      protected:
	/*!
	 * value holded
	 */
	Type value;
      };
  
    } // end of namespace internals

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    class tuple
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>,
	protected tfel::utilities::internals::TupleContainer<5,T5>,
	protected tfel::utilities::internals::TupleContainer<6,T6>,
	protected tfel::utilities::internals::TupleContainer<7,T7>,
	protected tfel::utilities::internals::TupleContainer<8,T8>,
	protected tfel::utilities::internals::TupleContainer<9,T9>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),const T6& v6= T6(),const T7& v7= T7(),const T8& v8= T8(),
	    const T9& v9= T9())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4),
	  tfel::utilities::internals::TupleContainer<5,T5>(v5),
	  tfel::utilities::internals::TupleContainer<6,T6>(v6),
	  tfel::utilities::internals::TupleContainer<7,T7>(v7),
	  tfel::utilities::internals::TupleContainer<8,T8>(v8),
	  tfel::utilities::internals::TupleContainer<9,T9>(v9)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<10,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<10,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,
	      const T3& v3,const T4& v4,const T5& v5,
	      const T6& v6,const T7& v7,const T8& v8,
	      const T9& v9)
    {
      return tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9);
    }


    template<typename T0>
    class tuple<T0,TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>
    {

      typedef typename tfel::meta::GenerateTypeList<T0>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<1,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<1,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0>
    tuple<T0>
    makeTuple(const T0& v0)
    {
      return tuple<T0>(v0);
    }


    template<typename T0,typename T1>
    class tuple<T0,T1,TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<2,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<2,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1>
    tuple<T0,T1>
    makeTuple(const T0& v0,const T1& v1)
    {
      return tuple<T0,T1>(v0,v1);
    }


    template<typename T0,typename T1,typename T2>
    class tuple<T0,T1,T2,TupleEndType,
		TupleEndType,TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<3,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<3,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2>
    tuple<T0,T1,T2>
    makeTuple(const T0& v0,const T1& v1,const T2& v2)
    {
      return tuple<T0,T1,T2>(v0,v1,v2);
    }


    template<typename T0,typename T1,typename T2,typename T3>
    class tuple<T0,T1,T2,T3,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<4,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<4,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3>
    tuple<T0,T1,T2,T3>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3)
    {
      return tuple<T0,T1,T2,T3>(v0,v1,v2,v3);
    }


    template<typename T0,typename T1,typename T2,typename T3,typename T4>
    class tuple<T0,T1,T2,T3,T4,TupleEndType,
		TupleEndType,TupleEndType,
		TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<5,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<5,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4>
    tuple<T0,T1,T2,T3,T4>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4)
    {
      return tuple<T0,T1,T2,T3,T4>(v0,v1,v2,v3,v4);
    }


    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
    class tuple<T0,T1,T2,T3,T4,T5,
		TupleEndType,TupleEndType,
		TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>,
	protected tfel::utilities::internals::TupleContainer<5,T5>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4),
	  tfel::utilities::internals::TupleContainer<5,T5>(v5)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<6,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<6,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5>
    tuple<T0,T1,T2,T3,T4,T5>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5)
    {
      return tuple<T0,T1,T2,T3,T4,T5>(v0,v1,v2,v3,v4,v5);
    }


    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6>
    class tuple<T0,T1,T2,T3,T4,T5,T6,
		TupleEndType,TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>,
	protected tfel::utilities::internals::TupleContainer<5,T5>,
	protected tfel::utilities::internals::TupleContainer<6,T6>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),const T6& v6= T6())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4),
	  tfel::utilities::internals::TupleContainer<5,T5>(v5),
	  tfel::utilities::internals::TupleContainer<6,T6>(v6)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<7,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<7,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6>
    tuple<T0,T1,T2,T3,T4,T5,T6>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,const T6& v6)
    {
      return tuple<T0,T1,T2,T3,T4,T5,T6>(v0,v1,v2,v3,v4,v5,v6);
    }


    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6,typename T7>
    class tuple<T0,T1,T2,T3,T4,T5,T6,T7,TupleEndType,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>,
	protected tfel::utilities::internals::TupleContainer<5,T5>,
	protected tfel::utilities::internals::TupleContainer<6,T6>,
	protected tfel::utilities::internals::TupleContainer<7,T7>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),const T6& v6= T6(),const T7& v7= T7())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4),
	  tfel::utilities::internals::TupleContainer<5,T5>(v5),
	  tfel::utilities::internals::TupleContainer<6,T6>(v6),
	  tfel::utilities::internals::TupleContainer<7,T7>(v7)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<8,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<8,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,const T6& v6,const T7& v7)
    {
      return tuple<T0,T1,T2,T3,T4,T5,T6,T7>(v0,v1,v2,v3,v4,v5,v6,v7);
    }


    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6,typename T7,typename T8>
    class tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,TupleEndType>
      : protected tfel::utilities::internals::TupleContainer<0,T0>,
	protected tfel::utilities::internals::TupleContainer<1,T1>,
	protected tfel::utilities::internals::TupleContainer<2,T2>,
	protected tfel::utilities::internals::TupleContainer<3,T3>,
	protected tfel::utilities::internals::TupleContainer<4,T4>,
	protected tfel::utilities::internals::TupleContainer<5,T5>,
	protected tfel::utilities::internals::TupleContainer<6,T6>,
	protected tfel::utilities::internals::TupleContainer<7,T7>,
	protected tfel::utilities::internals::TupleContainer<8,T8>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8>::type ListOfTypes;

    public:

      tuple(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
	    const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),const T6& v6= T6(),const T7& v7= T7(),const T8& v8= T8())
	: tfel::utilities::internals::TupleContainer<0,T0>(v0),
	  tfel::utilities::internals::TupleContainer<1,T1>(v1),
	  tfel::utilities::internals::TupleContainer<2,T2>(v2),
	  tfel::utilities::internals::TupleContainer<3,T3>(v3),
	  tfel::utilities::internals::TupleContainer<4,T4>(v4),
	  tfel::utilities::internals::TupleContainer<5,T5>(v5),
	  tfel::utilities::internals::TupleContainer<6,T6>(v6),
	  tfel::utilities::internals::TupleContainer<7,T7>(v7),
	  tfel::utilities::internals::TupleContainer<8,T8>(v8)
      {}

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<9,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      get(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::utilities::internals::TupleContainer<N,ResType>::getValue();
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<9,
	  void
	  >::type
      set(const typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type& src)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	tfel::utilities::internals::TupleContainer<N,ResType>::setValue(src);
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8>
    tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>
    makeTuple(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,const T6& v6,const T7& v7,const T8& v8)
    {
      return tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8>(v0,v1,v2,v3,v4,v5,v6,v7,v8);
    }


  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_TUPLE_I_ */
