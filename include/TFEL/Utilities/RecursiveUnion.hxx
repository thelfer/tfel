/*!
 * \file   RecursiveUnion.hxx
 * \brief  This file implements the RecursiveUnion class and some function to manipulate them.
 * \see    TypeList
 * \author Helfer Thomas
 * \date   31 Jul 2006
 */

#ifndef _LIB_TFEL_RECURSIVEUNIONS_H_
#define _LIB_TFEL_RECURSIVEUNIONS_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Metaprogramming/IsSubClassOf.hxx"

namespace tfel{
  
  namespace utilities{

    /*!
     * \class RecursiveUnion 
     * \brief union that may contains each type of a given TypeList.
     * 
     * \param T, a TypeList
     *
     * \see    TypeList
     * \author Helfer Thomas
     * \date   31 Jul 2006
     */
    template<typename T>
    union RecursiveUnion
    {
      // Concept check
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TypeList>::cond));
      typename T::Current current;
      RecursiveUnion<typename T::Next> next;
    };
    
    /*!
     * \brief Specialisation to end the recursion.
     * \see    RecursiveUnion
     * \author Helfer Thomas
     * \date   31 Jul 2006
     */     
    template<>
    union RecursiveUnion<tfel::meta::TypeListEndType>{};

    /*!
     * \class  RecursiveRetrieve
     * \brief  An helper class.
     * \see    RecursiveUnion
     * \author Helfer Thomas
     * \date   31 Jul 2006
     */
    template<unsigned int N>
    struct RecursiveRetrieve
    {

      /*!
       *\brief Returns a reference to the Nth type of the TypeList used to create a RecursiveUnion
       */
      template<typename List>
      static inline typename tfel::meta::TypeListFindNthElt<List,N>::type& exe(RecursiveUnion<List>& src)
      {
	TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<List,tfel::meta::TypeList>::cond));
	TFEL_STATIC_ASSERT((!tfel::meta::IsSameType<typename tfel::meta::TypeListFindNthElt<List,N>::type,tfel::meta::TypeListEndType>::cond));
	return RecursiveRetrieve<N-1>::exe(src.next);
      }

      /*!
       * \brief Returns a const reference to the Nth type of the TypeList used to 
       *  create a RecursiveUnion
       */
      template<typename List>
      static inline const typename tfel::meta::TypeListFindNthElt<List,N>::type& 
      exe(const RecursiveUnion<List>& src)
      {
	TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<List,tfel::meta::TypeList>::cond));
	TFEL_STATIC_ASSERT((!tfel::meta::IsSameType<typename tfel::meta::TypeListFindNthElt<List,N>::type,tfel::meta::TypeListEndType>::cond));
	return RecursiveRetrieve<N-1>::exe(src.next);
      }

    };

    /*!
     * \brief Partial Specialisation to end the recursion.
     * \see RecursiveRetrieve
     * \author Helfer Thomas
     * \date   31 Jul 2006   
     */
    template<>
    struct RecursiveRetrieve<0u>{
      /*!
       * \brief non-const version.
       */
      template<typename List>
      static inline typename List::current& exe(RecursiveUnion<List>& src)
      {
	return src.current;
      }
      /*!
       * \brief const version.
       */
      template<typename List>
      static inline const typename List::current&  exe(const RecursiveUnion<List>& src)
      {
	return src.current;
      }
    };

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_RECURSIVEUNIONS_H */

