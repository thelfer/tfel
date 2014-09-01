/*!
 * \file   include/TFEL/Math/Composite/CompositeNegExpr.hxx
 * \brief  This file declares the  class
 * \author Helfer Thomas
 * \date   13 Mar 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_COMPOSITENEGEXPR_H_
#define _LIB_TFEL_COMPOSITENEGEXPR_H_ 

#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/Composite/CompositeConcept.hxx"
#include"TFEL/Math/Function/FunctionConcept.hxx"

namespace tfel{

  namespace math{

    namespace internals{
      
      template<typename Items>
      class CompositeNegItems;

      template<>
      class CompositeNegItems<tfel::meta::TLE>
      {
      public:
	typedef tfel::meta::TLE type;
      };

      template<typename Items>
      class CompositeNegItems
      {
	typedef typename ComputeUnaryResult<typename Items::Current,OpNeg>::Handle Handle;
	typedef typename CompositeNegItems<typename Items::Next>::type Next;
      public:
	typedef tfel::meta::TLNode<Handle,Next> type;
      };

    } // end of namespace internals

    template<typename A>
    class CompositeNegExpr
    {
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,CompositeConcept>::cond));

      typedef typename A::Items ItemsA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;

      CompositeNegExpr();

      struct invalid_argument;

    public:

      typedef typename 
      tfel::math::internals::CompositeNegItems<ItemsA>::type Items;

    protected:

      typedef const A first_arg;
      typedef invalid_argument second_arg;

      TFEL_MATH_INLINE CompositeNegExpr(const A& l)
	: a(l)
      {}

      template<unsigned short N>
      TFEL_MATH_INLINE2
      const typename tfel::meta::EnableIf<
	(N<tfel::meta::TLSize<Items>::value),
	  typename tfel::meta::TLFindNthElt<Items,N>::type
      >::type
      getComponent(void) const
      {
	return -a.template getComponent<N>();
      }

    };

  } // end of namespace math

} // end of namespace tfel  

#endif /* _LIB_TFEL_COMPOSITENEGEXPR_H */

