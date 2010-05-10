/*!
 * \file   CompositeCompositeExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITECOMPOSITEEXPR_HXX_
#define _LIB_TFEL_COMPOSITECOMPOSITEEXPR_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"

namespace tfel{

  namespace math{

    namespace internals{
      
      template<typename ItemsA,typename ItemsB,typename Op>
      struct CompositeCompositeItems;

      template<typename Op>
      struct CompositeCompositeItems<tfel::meta::TypeListEndType,
				     tfel::meta::TypeListEndType,
				     Op>
      {
	typedef tfel::meta::TypeListEndType type;
      };

      template<typename ItemsA,typename ItemsB,typename Op>
      struct CompositeCompositeItems
      {
	typedef tfel::meta::TypeListNode<typename ComputeBinaryResult<typename ItemsA::Current,
								      typename ItemsB::Current,
								      Op>::Handle,
					 typename CompositeCompositeItems<typename ItemsA::Next,
									  typename ItemsB::Next,
									  Op>::type
					 > type;
      };

    }

    template<typename A,typename B,typename Op>
    class CompositeCompositeExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,CompositeConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,CompositeConcept>::cond));

      typedef typename A::Items ItemsA;
      typedef typename B::Items ItemsB;
      static const unsigned int ItemsASize = tfel::meta::TypeListSize<ItemsA>::value;
      static const unsigned int ItemsBSize = tfel::meta::TypeListSize<ItemsB>::value;
      TFEL_STATIC_ASSERT(ItemsASize==ItemsBSize);
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;

      CompositeCompositeExpr();

    public:

      typedef typename 
      tfel::math::internals::CompositeCompositeItems<ItemsA,ItemsB,Op>::type Items;

    protected:

      typedef A first_arg;
      typedef B second_arg;

      TFEL_MATH_INLINE CompositeCompositeExpr(const A& l, const B& r)
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
	return Op::apply(a.template getComponent<N>(),
			 b.template getComponent<N>());
      }

    };

  } // end of namespace math

} // end of namespace tfel  
  

#endif /* _LIB_TFEL_COMPOSITECOMPOSITEEXPR_HXX */

