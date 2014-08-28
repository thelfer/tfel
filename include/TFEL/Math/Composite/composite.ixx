/*!
 * \file   include/TFEL/Math/Composite/composite.ixx
 * \see    GenerateComposite.cxx to see how this file was generated.
 * \brief  This file implements the composite template class.
 * \author Helfer Thomas
 */

#ifndef _LIB_TFEL_COMPOSITE_I_
#define _LIB_TFEL_COMPOSITE_I_ 

#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/IsSameType.hxx"
#include"TFEL/TypeTraits/IsScalar.hxx"
#include"TFEL/TypeTraits/IsAssignableTo.hxx"
#include"TFEL/Utilities/Name.hxx"

namespace tfel{

  namespace math{

    namespace internals{

      template<unsigned short N,typename Type>
      class CompositeContainer
      {
      protected:

	Type value;

	CompositeContainer(const Type& val)
	  : value(val)
	{}

      };

    } // end of namespace internals

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    struct CompositeTraits<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    class composite
      : public CompositeConcept<composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> >,
	protected tfel::math::internals::CompositeContainer<0,T0>,
	protected tfel::math::internals::CompositeContainer<1,T1>,
	protected tfel::math::internals::CompositeContainer<2,T2>,
	protected tfel::math::internals::CompositeContainer<3,T3>,
	protected tfel::math::internals::CompositeContainer<4,T4>,
	protected tfel::math::internals::CompositeContainer<5,T5>,
	protected tfel::math::internals::CompositeContainer<6,T6>,
	protected tfel::math::internals::CompositeContainer<7,T7>,
	protected tfel::math::internals::CompositeContainer<8,T8>,
	protected tfel::math::internals::CompositeContainer<9,T9>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),
		const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),
		const T6& v6= T6(),const T7& v7= T7(),const T8& v8= T8(),
		const T9& v9= T9())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4),
	  tfel::math::internals::CompositeContainer<5,T5>(v5),
	  tfel::math::internals::CompositeContainer<6,T6>(v6),
	  tfel::math::internals::CompositeContainer<7,T7>(v7),
	  tfel::math::internals::CompositeContainer<8,T8>(v8),
	  tfel::math::internals::CompositeContainer<9,T9>(v9)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static
      const std::string
      getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=",";
	name+=Name<T5>::getName();
	name+=",";
	name+=Name<T6>::getName();
	name+=",";
	name+=Name<T7>::getName();
	name+=",";
	name+=Name<T8>::getName();
	name+=",";
	name+=Name<T9>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<10,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<10,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&
      operator=(const composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T0>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T0>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T0>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T0>::value = src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T0>::value = src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U8,T8>::cond&&
      tfel::typetraits::IsAssignableTo<U9,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value = src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value = src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpPlus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpPlus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value += src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value += src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMinus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpMinus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value -= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value -= src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMult>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpMult>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value *= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value *= src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpDiv>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpDiv>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value /= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value /= src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U,T8>::cond&&
      tfel::typetraits::IsAssignableTo<U,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	tfel::math::internals::CompositeContainer<5,T5>::value = val;
	tfel::math::internals::CompositeContainer<6,T6>::value = val;
	tfel::math::internals::CompositeContainer<7,T7>::value = val;
	tfel::math::internals::CompositeContainer<8,T8>::value = val;
	tfel::math::internals::CompositeContainer<9,T9>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpPlus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T9,OpPlus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	tfel::math::internals::CompositeContainer<5,T5>::value += val;
	tfel::math::internals::CompositeContainer<6,T6>::value += val;
	tfel::math::internals::CompositeContainer<7,T7>::value += val;
	tfel::math::internals::CompositeContainer<8,T8>::value += val;
	tfel::math::internals::CompositeContainer<9,T9>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpMinus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T9,OpMinus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	tfel::math::internals::CompositeContainer<5,T5>::value -= val;
	tfel::math::internals::CompositeContainer<6,T6>::value -= val;
	tfel::math::internals::CompositeContainer<7,T7>::value -= val;
	tfel::math::internals::CompositeContainer<8,T8>::value -= val;
	tfel::math::internals::CompositeContainer<9,T9>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpMult>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T9,OpMult>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	tfel::math::internals::CompositeContainer<5,T5>::value *= val;
	tfel::math::internals::CompositeContainer<6,T6>::value *= val;
	tfel::math::internals::CompositeContainer<7,T7>::value *= val;
	tfel::math::internals::CompositeContainer<8,T8>::value *= val;
	tfel::math::internals::CompositeContainer<9,T9>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpDiv>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T9,OpDiv>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	tfel::math::internals::CompositeContainer<5,T5>::value /= val;
	tfel::math::internals::CompositeContainer<6,T6>::value /= val;
	tfel::math::internals::CompositeContainer<7,T7>::value /= val;
	tfel::math::internals::CompositeContainer<8,T8>::value /= val;
	tfel::math::internals::CompositeContainer<9,T9>::value /= val;
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U8,T8>::cond&&
      tfel::typetraits::IsAssignableTo<U9,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value = src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value = src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpPlus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpPlus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value += src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value += src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMinus>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpMinus>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value -= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value -= src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMult>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpMult>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value *= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value *= src.template getComponent<9>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename U9,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpDiv>::type,T8>::cond&&
      tfel::meta::IsSameType<typename ResultType<U9,T9,OpDiv>::type,T9>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8,U9>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value /= src.template getComponent<8>();
	tfel::math::internals::CompositeContainer<9,T9>::value /= src.template getComponent<9>();
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8,typename T9>
    composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,
		  const T6& v6,const T7& v7,const T8& v8,const T9& v9)
    {
      return composite<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9>(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9);
    }


    template<typename T0>
    struct CompositeTraits<composite<T0,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0>::type Items;
    };

    template<typename T0>
    class composite<T0,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0> >,
			protected tfel::math::internals::CompositeContainer<0,T0>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T0>::type ListOfTypes;

    public:

      composite(const T0& v0= T0())
	: tfel::math::internals::CompositeContainer<0,T0>(v0)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<1,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<1,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0>&
      operator=(const composite<T0>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond,
	composite<T0>&>::type
      operator=(const composite<U0>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond,
	composite<T0>&>::type
      operator+=(const composite<U0>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond,
	composite<T0>&>::type
      operator-=(const composite<U0>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond,
	composite<T0>&>::type
      operator*=(const composite<U0>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond,
	composite<T0>&>::type
      operator/=(const composite<U0>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	return *this;
      }

      template<typename U0,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond,
	composite<T0>&>::type
      operator=(const CompositeExpr<composite<U0>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond,
	composite<T0>&>::type
      operator+=(const CompositeExpr<composite<U0>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond,
	composite<T0>&>::type
      operator-=(const CompositeExpr<composite<U0>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond,
	composite<T0>&>::type
      operator*=(const CompositeExpr<composite<U0>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond,
	composite<T0>&>::type
      operator/=(const CompositeExpr<composite<U0>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond,
	composite<T0>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond,
	composite<T0>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond,
	composite<T0>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond,
	composite<T0>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond,
	composite<T0>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	return *this;
      }

    };

    template<typename T0>
    composite<T0>
    makeComposite(const T0& v0)
    {
      return composite<T0>(v0);
    }


    template<typename T0,typename T1>
    struct CompositeTraits<composite<T0,T1,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1>::type Items;
    };

    template<typename T0,typename T1>
    class composite<T0,T1,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T0,T1>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<2,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<2,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1>&
      operator=(const composite<T0,T1>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond,
	composite<T0,T1>&>::type
      operator=(const composite<U0,U1>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator+=(const composite<U0,U1>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator-=(const composite<U0,U1>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator*=(const composite<U0,U1>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator/=(const composite<U0,U1>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	return *this;
      }

      template<typename U0,typename U1,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond,
	composite<T0,T1>&>::type
      operator=(const CompositeExpr<composite<U0,U1>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator+=(const CompositeExpr<composite<U0,U1>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator-=(const CompositeExpr<composite<U0,U1>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator*=(const CompositeExpr<composite<U0,U1>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator/=(const CompositeExpr<composite<U0,U1>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond,
	composite<T0,T1>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond,
	composite<T0,T1>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1>
    composite<T0,T1>
    makeComposite(const T0& v0,const T1& v1)
    {
      return composite<T0,T1>(v0,v1);
    }


    template<typename T0,typename T1,typename T2>
    struct CompositeTraits<composite<T0,T1,
				     T2,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2>::type Items;
    };

    template<typename T0,typename T1,typename T2>
    class composite<T0,T1,T2,CompositeEndType,
		    CompositeEndType,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T0,T1,T2>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<3,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<3,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2>&
      operator=(const composite<T0,T1,T2>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator=(const composite<U0,U1,U2>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator+=(const composite<U0,U1,U2>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator-=(const composite<U0,U1,U2>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator*=(const composite<U0,U1,U2>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator/=(const composite<U0,U1,U2>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond,
	composite<T0,T1,T2>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2>
    composite<T0,T1,T2>
    makeComposite(const T0& v0,const T1& v1,const T2& v2)
    {
      return composite<T0,T1,T2>(v0,v1,v2);
    }


    template<typename T0,typename T1,typename T2,typename T3>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3>
    class composite<T0,T1,T2,T3,
		    CompositeEndType,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T0,T1,T2,T3>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<4,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<4,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3>&
      operator=(const composite<T0,T1,T2,T3>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator=(const composite<U0,U1,U2,U3>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator+=(const composite<U0,U1,U2,U3>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator-=(const composite<U0,U1,U2,U3>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator*=(const composite<U0,U1,U2,U3>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator/=(const composite<U0,U1,U2,U3>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond,
	composite<T0,T1,T2,T3>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3>
    composite<T0,T1,T2,T3>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3)
    {
      return composite<T0,T1,T2,T3>(v0,v1,v2,v3);
    }


    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     T4,CompositeEndType,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4>
    class composite<T0,T1,T2,T3,
		    T4,CompositeEndType,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3,
							  T4> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>,
			protected tfel::math::internals::CompositeContainer<4,T4>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T0,T1,T2,T3,T4>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3(),const T4& v4= T4())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<5,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<5,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4>&
      operator=(const composite<T0,T1,T2,T3,T4>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator=(const composite<U0,U1,U2,U3,U4>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond,
	composite<T0,T1,T2,T3,T4>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4>
    composite<T0,T1,T2,T3,T4>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4)
    {
      return composite<T0,T1,T2,T3,T4>(v0,v1,v2,v3,v4);
    }


    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     T4,T5,
				     CompositeEndType,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
    class composite<T0,T1,T2,T3,
		    T4,T5,CompositeEndType,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3,
							  T4,T5> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>,
			protected tfel::math::internals::CompositeContainer<4,T4>,
			protected tfel::math::internals::CompositeContainer<5,T5>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T0,T1,T2,T3,T4,T5>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4),
	  tfel::math::internals::CompositeContainer<5,T5>(v5)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=",";
	name+=Name<T5>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<6,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<6,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4,T5>&
      operator=(const composite<T0,T1,T2,T3,T4,T5>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T0>::value = src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator=(const composite<U0,U1,U2,U3,U4,U5>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4,U5>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4,U5>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4,U5>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4,U5>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	tfel::math::internals::CompositeContainer<5,T5>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpPlus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	tfel::math::internals::CompositeContainer<5,T5>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMinus>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	tfel::math::internals::CompositeContainer<5,T5>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMult>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	tfel::math::internals::CompositeContainer<5,T5>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpDiv>::type,T5>::cond,
	composite<T0,T1,T2,T3,T4,T5>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	tfel::math::internals::CompositeContainer<5,T5>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5>
    composite<T0,T1,T2,T3,T4,T5>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5)
    {
      return composite<T0,T1,T2,T3,T4,T5>(v0,v1,v2,v3,v4,v5);
    }


    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     T4,T5,
				     T6,CompositeEndType,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6>
    class composite<T0,T1,T2,T3,
		    T4,T5,T6,CompositeEndType,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3,
							  T4,T5,T6> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>,
			protected tfel::math::internals::CompositeContainer<4,T4>,
			protected tfel::math::internals::CompositeContainer<5,T5>,
			protected tfel::math::internals::CompositeContainer<6,T6>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T0,T1,T2,T3,T4,T5,T6>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),
		const T6& v6= T6())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4),
	  tfel::math::internals::CompositeContainer<5,T5>(v5),
	  tfel::math::internals::CompositeContainer<6,T6>(v6)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=",";
	name+=Name<T5>::getName();
	name+=",";
	name+=Name<T6>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<7,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<7,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4,T5,T6>&
      operator=(const composite<T0,T1,T2,T3,T4,T5,T6>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T0>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T0>::value = src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator=(const composite<U0,U1,U2,U3,U4,U5,U6>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4,U5,U6>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4,U5,U6>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4,U5,U6>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4,U5,U6>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	tfel::math::internals::CompositeContainer<5,T5>::value = val;
	tfel::math::internals::CompositeContainer<6,T6>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpPlus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	tfel::math::internals::CompositeContainer<5,T5>::value += val;
	tfel::math::internals::CompositeContainer<6,T6>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMinus>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	tfel::math::internals::CompositeContainer<5,T5>::value -= val;
	tfel::math::internals::CompositeContainer<6,T6>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMult>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	tfel::math::internals::CompositeContainer<5,T5>::value *= val;
	tfel::math::internals::CompositeContainer<6,T6>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpDiv>::type,T6>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	tfel::math::internals::CompositeContainer<5,T5>::value /= val;
	tfel::math::internals::CompositeContainer<6,T6>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6>
    composite<T0,T1,T2,T3,T4,T5,T6>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,
		  const T6& v6)
    {
      return composite<T0,T1,T2,T3,T4,T5,T6>(v0,v1,v2,v3,v4,v5,v6);
    }


    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     T4,T5,
				     T6,T7,
				     CompositeEndType,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6,typename T7>
    class composite<T0,T1,T2,T3,
		    T4,T5,T6,T7,
		    CompositeEndType,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3,
							  T4,T5,T6,T7> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>,
			protected tfel::math::internals::CompositeContainer<4,T4>,
			protected tfel::math::internals::CompositeContainer<5,T5>,
			protected tfel::math::internals::CompositeContainer<6,T6>,
			protected tfel::math::internals::CompositeContainer<7,T7>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T0,T1,T2,T3,T4,T5,T6,T7>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),
		const T6& v6= T6(),const T7& v7= T7())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4),
	  tfel::math::internals::CompositeContainer<5,T5>(v5),
	  tfel::math::internals::CompositeContainer<6,T6>(v6),
	  tfel::math::internals::CompositeContainer<7,T7>(v7)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=",";
	name+=Name<T5>::getName();
	name+=",";
	name+=Name<T6>::getName();
	name+=",";
	name+=Name<T7>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<8,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<8,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4,T5,T6,T7>&
      operator=(const composite<T0,T1,T2,T3,T4,T5,T6,T7>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T0>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T0>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T0>::value = src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator=(const composite<U0,U1,U2,U3,U4,U5,U6,U7>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4,U5,U6,U7>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4,U5,U6,U7>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4,U5,U6,U7>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4,U5,U6,U7>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	tfel::math::internals::CompositeContainer<5,T5>::value = val;
	tfel::math::internals::CompositeContainer<6,T6>::value = val;
	tfel::math::internals::CompositeContainer<7,T7>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpPlus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	tfel::math::internals::CompositeContainer<5,T5>::value += val;
	tfel::math::internals::CompositeContainer<6,T6>::value += val;
	tfel::math::internals::CompositeContainer<7,T7>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMinus>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	tfel::math::internals::CompositeContainer<5,T5>::value -= val;
	tfel::math::internals::CompositeContainer<6,T6>::value -= val;
	tfel::math::internals::CompositeContainer<7,T7>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMult>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	tfel::math::internals::CompositeContainer<5,T5>::value *= val;
	tfel::math::internals::CompositeContainer<6,T6>::value *= val;
	tfel::math::internals::CompositeContainer<7,T7>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpDiv>::type,T7>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	tfel::math::internals::CompositeContainer<5,T5>::value /= val;
	tfel::math::internals::CompositeContainer<6,T6>::value /= val;
	tfel::math::internals::CompositeContainer<7,T7>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7>
    composite<T0,T1,T2,T3,T4,T5,T6,T7>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,
		  const T6& v6,const T7& v7)
    {
      return composite<T0,T1,T2,T3,T4,T5,T6,T7>(v0,v1,v2,v3,v4,v5,v6,v7);
    }


    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8>
    struct CompositeTraits<composite<T0,T1,
				     T2,T3,
				     T4,T5,
				     T6,T7,
				     T8,CompositeEndType> >
    {
      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8>::type Items;
    };

    template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,
	     typename T6,typename T7,typename T8>
    class composite<T0,T1,T2,T3,
		    T4,T5,T6,T7,
		    T8,CompositeEndType>
		      : public CompositeConcept<composite<T0,T1,T2,T3,
							  T4,T5,T6,T7,
							  T8> >,
			protected tfel::math::internals::CompositeContainer<0,T0>,
			protected tfel::math::internals::CompositeContainer<1,T1>,
			protected tfel::math::internals::CompositeContainer<2,T2>,
			protected tfel::math::internals::CompositeContainer<3,T3>,
			protected tfel::math::internals::CompositeContainer<4,T4>,
			protected tfel::math::internals::CompositeContainer<5,T5>,
			protected tfel::math::internals::CompositeContainer<6,T6>,
			protected tfel::math::internals::CompositeContainer<7,T7>,
			protected tfel::math::internals::CompositeContainer<8,T8>
    {

      typedef typename tfel::meta::GenerateTypeList<T0,T1,T2,T3,T4,T5,T6,T7,T8,T0,T1,T2,T3,T4,T5,T6,T7,T8>::type ListOfTypes;

    public:

      composite(const T0& v0= T0(),const T1& v1= T1(),const T2& v2= T2(),const T3& v3= T3(),const T4& v4= T4(),const T5& v5= T5(),
		const T6& v6= T6(),const T7& v7= T7(),const T8& v8= T8())
	: tfel::math::internals::CompositeContainer<0,T0>(v0),
	  tfel::math::internals::CompositeContainer<1,T1>(v1),
	  tfel::math::internals::CompositeContainer<2,T2>(v2),
	  tfel::math::internals::CompositeContainer<3,T3>(v3),
	  tfel::math::internals::CompositeContainer<4,T4>(v4),
	  tfel::math::internals::CompositeContainer<5,T5>(v5),
	  tfel::math::internals::CompositeContainer<6,T6>(v6),
	  tfel::math::internals::CompositeContainer<7,T7>(v7),
	  tfel::math::internals::CompositeContainer<8,T8>(v8)
      {}

      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static std::string getName(void){
	using namespace tfel::utilities;
	std::string name("composite<");
	name+=Name<T0>::getName();
	name+=",";
	name+=Name<T1>::getName();
	name+=",";
	name+=Name<T2>::getName();
	name+=",";
	name+=Name<T3>::getName();
	name+=",";
	name+=Name<T4>::getName();
	name+=",";
	name+=Name<T5>::getName();
	name+=",";
	name+=Name<T6>::getName();
	name+=",";
	name+=Name<T7>::getName();
	name+=",";
	name+=Name<T8>::getName();
	name+=">";
	return name;
      }

      template<unsigned short N>
      const typename tfel::meta::EnableIf<
	N<9,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void) const
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      template<unsigned short N>
      typename tfel::meta::EnableIf<
	N<9,
	  typename tfel::meta::TLFindNthElt<ListOfTypes,N>::type
      >::type&
      getComponent(void)
      {
	using tfel::meta::TLFindNthElt;
	typedef typename TLFindNthElt<ListOfTypes,N>::type ResType;
	return tfel::math::internals::CompositeContainer<N,ResType>::value;
      }

      /*
       * Assignement Operator
       */
      composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&
      operator=(const composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T0>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T0>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T0>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T0>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T0>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T0>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T0>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T0>::value = src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U8,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value = src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpPlus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator+=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value += src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMinus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator-=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value -= src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMult>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator*=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value *= src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpDiv>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator/=(const composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value /= src.template getComponent<8>();
	return *this;
      }

      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<U0,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U1,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U2,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U3,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U4,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U5,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U6,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U7,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U8,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,Expr>& src){
	tfel::math::internals::CompositeContainer<0,T0>::value = src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value = src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value = src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value = src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value = src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value = src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value = src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value = src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value = src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpPlus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator+=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value += src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value += src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value += src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value += src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value += src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value += src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value += src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value += src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMinus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator-=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value -= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value -= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value -= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value -= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value -= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value -= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value -= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value -= src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpMult>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator*=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value *= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value *= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value *= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value *= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value *= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value *= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value *= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value *= src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U0,typename U1,typename U2,
	       typename U3,typename U4,typename U5,typename U6,
	       typename U7,typename U8,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::meta::IsSameType<typename ResultType<U0,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U1,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U2,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U3,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U4,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U5,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U6,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U7,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U8,T8,OpDiv>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator/=(const CompositeExpr<composite<U0,U1,U2,U3,U4,U5,U6,U7,U8>,Expr>& src)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= src.template getComponent<0>();
	tfel::math::internals::CompositeContainer<1,T1>::value /= src.template getComponent<1>();
	tfel::math::internals::CompositeContainer<2,T2>::value /= src.template getComponent<2>();
	tfel::math::internals::CompositeContainer<3,T3>::value /= src.template getComponent<3>();
	tfel::math::internals::CompositeContainer<4,T4>::value /= src.template getComponent<4>();
	tfel::math::internals::CompositeContainer<5,T5>::value /= src.template getComponent<5>();
	tfel::math::internals::CompositeContainer<6,T6>::value /= src.template getComponent<6>();
	tfel::math::internals::CompositeContainer<7,T7>::value /= src.template getComponent<7>();
	tfel::math::internals::CompositeContainer<8,T8>::value /= src.template getComponent<8>();
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::typetraits::IsAssignableTo<U,T0>::cond&&
      tfel::typetraits::IsAssignableTo<U,T1>::cond&&
      tfel::typetraits::IsAssignableTo<U,T2>::cond&&
      tfel::typetraits::IsAssignableTo<U,T3>::cond&&
      tfel::typetraits::IsAssignableTo<U,T4>::cond&&
      tfel::typetraits::IsAssignableTo<U,T5>::cond&&
      tfel::typetraits::IsAssignableTo<U,T6>::cond&&
      tfel::typetraits::IsAssignableTo<U,T7>::cond&&
      tfel::typetraits::IsAssignableTo<U,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value = val;
	tfel::math::internals::CompositeContainer<1,T1>::value = val;
	tfel::math::internals::CompositeContainer<2,T2>::value = val;
	tfel::math::internals::CompositeContainer<3,T3>::value = val;
	tfel::math::internals::CompositeContainer<4,T4>::value = val;
	tfel::math::internals::CompositeContainer<5,T5>::value = val;
	tfel::math::internals::CompositeContainer<6,T6>::value = val;
	tfel::math::internals::CompositeContainer<7,T7>::value = val;
	tfel::math::internals::CompositeContainer<8,T8>::value = val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpPlus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpPlus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpPlus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpPlus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpPlus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpPlus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpPlus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpPlus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpPlus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator+=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value += val;
	tfel::math::internals::CompositeContainer<1,T1>::value += val;
	tfel::math::internals::CompositeContainer<2,T2>::value += val;
	tfel::math::internals::CompositeContainer<3,T3>::value += val;
	tfel::math::internals::CompositeContainer<4,T4>::value += val;
	tfel::math::internals::CompositeContainer<5,T5>::value += val;
	tfel::math::internals::CompositeContainer<6,T6>::value += val;
	tfel::math::internals::CompositeContainer<7,T7>::value += val;
	tfel::math::internals::CompositeContainer<8,T8>::value += val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMinus>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMinus>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMinus>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMinus>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMinus>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMinus>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMinus>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMinus>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpMinus>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator-=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value -= val;
	tfel::math::internals::CompositeContainer<1,T1>::value -= val;
	tfel::math::internals::CompositeContainer<2,T2>::value -= val;
	tfel::math::internals::CompositeContainer<3,T3>::value -= val;
	tfel::math::internals::CompositeContainer<4,T4>::value -= val;
	tfel::math::internals::CompositeContainer<5,T5>::value -= val;
	tfel::math::internals::CompositeContainer<6,T6>::value -= val;
	tfel::math::internals::CompositeContainer<7,T7>::value -= val;
	tfel::math::internals::CompositeContainer<8,T8>::value -= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpMult>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpMult>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpMult>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpMult>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpMult>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpMult>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpMult>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpMult>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpMult>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator*=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value *= val;
	tfel::math::internals::CompositeContainer<1,T1>::value *= val;
	tfel::math::internals::CompositeContainer<2,T2>::value *= val;
	tfel::math::internals::CompositeContainer<3,T3>::value *= val;
	tfel::math::internals::CompositeContainer<4,T4>::value *= val;
	tfel::math::internals::CompositeContainer<5,T5>::value *= val;
	tfel::math::internals::CompositeContainer<6,T6>::value *= val;
	tfel::math::internals::CompositeContainer<7,T7>::value *= val;
	tfel::math::internals::CompositeContainer<8,T8>::value *= val;
	return *this;
      }

      /*
       * Assignement Operator
       */
      template<typename U>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<U>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T0,OpDiv>::type,T0>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T1,OpDiv>::type,T1>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T2,OpDiv>::type,T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T3,OpDiv>::type,T3>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T4,OpDiv>::type,T4>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T5,OpDiv>::type,T5>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T6,OpDiv>::type,T6>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T7,OpDiv>::type,T7>::cond&&
      tfel::meta::IsSameType<typename ResultType<U,T8,OpDiv>::type,T8>::cond,
	composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>&>::type
      operator/=(const U& val)
      {
	tfel::math::internals::CompositeContainer<0,T0>::value /= val;
	tfel::math::internals::CompositeContainer<1,T1>::value /= val;
	tfel::math::internals::CompositeContainer<2,T2>::value /= val;
	tfel::math::internals::CompositeContainer<3,T3>::value /= val;
	tfel::math::internals::CompositeContainer<4,T4>::value /= val;
	tfel::math::internals::CompositeContainer<5,T5>::value /= val;
	tfel::math::internals::CompositeContainer<6,T6>::value /= val;
	tfel::math::internals::CompositeContainer<7,T7>::value /= val;
	tfel::math::internals::CompositeContainer<8,T8>::value /= val;
	return *this;
      }

    };

    template<typename T0,typename T1,typename T2,typename T3,
	     typename T4,typename T5,typename T6,typename T7,
	     typename T8>
    composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>
    makeComposite(const T0& v0,const T1& v1,const T2& v2,const T3& v3,const T4& v4,const T5& v5,
		  const T6& v6,const T7& v7,const T8& v8)
    {
      return composite<T0,T1,T2,T3,T4,T5,T6,T7,T8>(v0,v1,v2,v3,v4,v5,v6,v7,v8);
    }


  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_COMPOSITE_I_ */
