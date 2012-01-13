/*!
 * \file   CompositeConcept.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   09 mar 2007
 */

#ifndef _LIB_TFEL_COMPOSITECONCEPT_HXX_
#define _LIB_TFEL_COMPOSITECONCEPT_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"

#include"TFEL/Math/Forward/CompositeConcept.hxx"

namespace tfel{

  namespace math{

    template<typename C>
    struct CompositeTraits
    {
      typedef tfel::meta::TLE Items;
    };

    /*!
     * \class CompositeTag
     * \brief Helper class to characterise composite objects.
     */ 
    struct CompositeTag
    {
      
      /*!
       * \brief  Return the name of the class.
       * \param  void.
       * \return std::string, the name of the class.
       * \see    Name.
       */
      static 
      const std::string
      getName(void);

    }; // end of struct CompositeTag

    template<typename C>
    struct CompositeConcept
    {
      static const bool isTemporary = tfel::typetraits::IsTemporary<C>::cond;
      typedef CompositeTag ConceptTag;
      typedef typename CompositeTraits<C>::Items Items;
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<Items,tfel::meta::TL>::cond));

      template<unsigned short N>
      class ConstValueType
      {
	typedef typename CompositeTraits<C>::Items Items;
	typedef typename tfel::meta::TLFindNthElt<Items,N>::type NthElt;
      public:
	typedef typename tfel::meta::IF<isTemporary,const NthElt,const NthElt&>::type type;
      };

#ifdef __GNUG__
      template<unsigned short N>
      typename tfel::meta::EnableIf<
	(N<tfel::meta::TLSize<Items>::value),
	  typename ConstValueType<N>::type
      >::type
      getComponent(void) const;
#else /* __GNUG__ */
      template<unsigned short N>
      typename tfel::meta::EnableIf<
	(N<tfel::meta::TLSize<Items>::value),
	  typename ConstValueType<N>::type
      >::type
      getComponent(void) const
      {
	return static_cast<const C&>(*this)->template getComponent<N>();
      }
#endif /* __GNUG__ */

    };

    template<typename T>
    struct CompositeType
    {
      typedef T type;
    };

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/Composite/CompositeConcept.ixx"
#include"TFEL/Math/Composite/CompositeConceptOperations.ixx"

#endif /* _LIB_TFEL_COMPOSITECONCEPT_HXX */

