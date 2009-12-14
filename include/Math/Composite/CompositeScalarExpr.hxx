/*!
 * \file   CompositeScalarExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITESCALAREXPR_HXX_
#define _LIB_TFEL_COMPOSITESCALAREXPR_HXX_ 

#include"Metaprogramming/StaticAssert.hxx"
#include"Metaprogramming/Implements.hxx"
#include"Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace math{

    namespace internals{
      
      template<typename ItemsA,typename B,typename Op>
      struct CompositeScalarItems;

      template<typename B,typename Op>
      struct CompositeScalarItems<tfel::meta::TypeListEndType,B,Op>
      {
	typedef tfel::meta::TypeListEndType type;
      };

      template<typename ItemsA,typename B,typename Op>
      class CompositeScalarItems
      {
	typedef typename ComputeBinaryResult<typename ItemsA::Current,
					     B,
					     Op>::Handle Handle;
	typedef typename CompositeScalarItems<typename ItemsA::Next,
					      B,
					      Op>::type Next;
      public:
	typedef tfel::meta::TypeListNode<Handle,Next> type;
      };

    } // end of namespace internals

    template<typename A,typename B,typename Op>
    class CompositeScalarExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,CompositeConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));

      typedef typename A::Items ItemsA;
      static const unsigned int ItemsASize = tfel::meta::TypeListSize<ItemsA>::value;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      CompositeScalarExpr();

    public:

      typedef typename 
      tfel::math::internals::CompositeScalarItems<ItemsA,B,Op>::type Items;

    protected:

      typedef A first_arg;
      typedef B second_arg;

      TFEL_MATH_INLINE
      CompositeScalarExpr(const A& l, const B& r)
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
	return Op::apply(a.getComponent<N>(),b);
      }

    }; // end of CompositeScalarExpr

  } // end of namespace math

} // end of namespace tfel  
  

#endif /* _LIB_TFEL_COMPOSITESCALAREXPR_HXX */

