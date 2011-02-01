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

#ifdef __GNUG__
    template<typename C>
    template<unsigned short N>
    typename tfel::meta::EnableIf<
      (N<tfel::meta::TLSize<typename CompositeTraits<C>::Items>::value),
	typename CompositeConcept<C>::template ConstValueType<N>::type
    >::type
    CompositeConcept<C>::getComponent(void) const
    {
      return static_cast<const C&>(*this)->template getComponent<N>();
    }
#endif /* __GNUG__ */

  } // end of namespace math

} // end of namespace tfel  


#endif /* _LIB_TFEL_COMPOSITECONCEPT_IXX */

