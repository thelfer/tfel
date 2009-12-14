/*!
 * \file   CompositeConcept.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITECONCEPT_IXX_
#define _LIB_TFEL_COMPOSITECONCEPT_IXX_ 

namespace tfel{

  namespace math{

    template<typename C>
    template<unsigned short N>
    typename tfel::meta::EnableIf<
      (N<tfel::meta::TypeListSize<typename CompositeTraits<C>::Items>::value),
	typename CompositeConcept<C>::template ConstValueType<N>::type
    >::type
    CompositeConcept<C>::getComponent(void) const
    {
      return static_cast<const C&>(*this)->template getComponent<N>();
    }


//     template<typename C>
//     template<unsigned short N>
//     typename tfel::meta::EnableIf<
//       (N<tfel::meta::TypeListSize<typename CompositeTraits<C>::Items>::value),
// 	typename tfel::meta::IF<CompositeConcept<C>::isTemporary,
// 				const typename tfel::meta::TypeListFindNthElt<typename CompositeTraits<C>::Items,N>::type,
// 				const typename tfel::meta::TypeListFindNthElt<typename CompositeTraits<C>::Items,N>::type&>::type
//     >::type
//     CompositeConcept<C>::getComponent(void) const
//     {
//       return static_cast<const C&>(*this)->template getComponent<N>();
//     }

  } // end of namespace math

} // end of namespace tfel  


#endif /* _LIB_TFEL_COMPOSITECONCEPT_IXX */

