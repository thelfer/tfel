/*!
 * \file   ScalarCompositeExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_SCALARCOMPOSITEEXPR_HXX_
#define _LIB_TFEL_SCALARCOMPOSITEEXPR_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace math{

    namespace internals{
      
      template<typename A,typename ItemsB,typename Op>
      struct ScalarCompositeItems;

      template<typename A,typename Op>
      struct ScalarCompositeItems<A,tfel::meta::TypeListEndType,Op>
      {
	typedef tfel::meta::TypeListEndType type;
      };

      template<typename A,typename ItemsB,typename Op>
      class ScalarCompositeItems
      {
	typedef typename ComputeBinaryResult<A,
					     typename ItemsB::Current,
					     Op>::Handle Handle;
	typedef typename ScalarCompositeItems<A,
					      typename ItemsB::Next,
					      Op>::type Next;
      public:
	typedef tfel::meta::TypeListNode<Handle,Next> type;
      };

    } // end of namespace internals

    template<typename A,typename B,typename Op>
    class ScalarCompositeExpr
    {
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,CompositeConcept>::cond));

      typedef typename B::Items ItemsB;
      static const unsigned int ItemsBSize = tfel::meta::TypeListSize<ItemsB>::value;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      ScalarCompositeExpr();

    public:

      typedef typename 
      tfel::math::internals::ScalarCompositeItems<A,ItemsB,Op>::type Items;

    protected:

      typedef A first_arg;
      typedef B second_arg;

      TFEL_MATH_INLINE ScalarCompositeExpr(const A& l, const B& r)
	: a(l), b(r)
      {}

      template<unsigned short N>
      TFEL_MATH_INLINE2
      const typename tfel::meta::EnableIf<
	(N<tfel::meta::TypeListSize<Items>::value),
	  typename tfel::meta::TypeListFindNthElt<Items,N>::type
      >::type
      getComponent(void) const
      {
	return Op::apply(a,b.template getComponent<N>());
      }

    }; // end of ScalarCompositeExpr

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_SCALARCOMPOSITEEXPR_HXX */

