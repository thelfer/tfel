/*!
 * \file   FctCompositeExpr.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   13 Mar 2007
 */

#ifndef _LIB_TFEL_FCTCOMPOSITEEXPR_H_
#define _LIB_TFEL_FCTCOMPOSITEEXPR_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/General/FctObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Composite/CompositeConcept.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    namespace internals{
      
      template<typename Items,typename Op>
      struct FctCompositeItems;

      template<typename Op>
      struct FctCompositeItems<tfel::meta::TypeListEndType,Op>
      {
	typedef tfel::meta::TypeListEndType type;
      };

      template<typename Items,typename Op>
      class FctCompositeItems
      {
	typedef typename ComputeUnaryResult<typename Items::Current,Op>::Handle Handle;
	typedef typename FctCompositeItems<typename Items::Next,Op>::type Next;
      public:
	typedef tfel::meta::TypeListNode<Handle,Next> type;
      };

    } // end of namespace internals

    template<typename A,typename B>
    class FctCompositeExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,CompositeConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,FunctionConcept>::cond));

      typedef typename A::Items ItemsA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

      FctCompositeExpr();

    public:

      typedef typename 
      tfel::math::internals::FctCompositeItems<ItemsA,B>::type Items;

    protected:

      typedef A first_arg;
      typedef B second_arg;

      TFEL_MATH_INLINE FctCompositeExpr(const A& l, const B& r)
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
	return b(a.getComponent<N>());
      }

    };

  } // end of namespace math

} // end of namespace tfel  


#endif /* _LIB_TFEL_FCTCOMPOSITEEXPR_H */

