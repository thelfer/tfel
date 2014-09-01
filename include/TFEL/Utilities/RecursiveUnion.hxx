/*!
 * \file   include/TFEL/Utilities/RecursiveUnion.hxx
 * \brief  This file implements the RecursiveUnion class and some function to manipulate them.
 * \see    TL
 * \author Helfer Thomas
 * \date   31 Jul 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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
     * \brief union that may contains each type of a given TL.
     * 
     * \param T, a TL
     *
     * \see    TL
     * \author Helfer Thomas
     * \date   31 Jul 2006
     */
    template<typename T>
    union RecursiveUnion
    {
      // Concept check
      TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<T,tfel::meta::TL>::cond));
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
    union RecursiveUnion<tfel::meta::TLE>{};

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
       *\brief Returns a reference to the Nth type of the TL used to create a RecursiveUnion
       */
      template<typename List>
      static inline typename tfel::meta::TLFindNthElt<List,N>::type& exe(RecursiveUnion<List>& src)
      {
	TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<List,tfel::meta::TL>::cond));
	TFEL_STATIC_ASSERT((!tfel::meta::IsSameType<typename tfel::meta::TLFindNthElt<List,N>::type,tfel::meta::TLE>::cond));
	return RecursiveRetrieve<N-1>::exe(src.next);
      }

      /*!
       * \brief Returns a const reference to the Nth type of the TL used to 
       *  create a RecursiveUnion
       */
      template<typename List>
      static inline const typename tfel::meta::TLFindNthElt<List,N>::type& 
      exe(const RecursiveUnion<List>& src)
      {
	TFEL_STATIC_ASSERT((tfel::meta::IsSubClassOf<List,tfel::meta::TL>::cond));
	TFEL_STATIC_ASSERT((!tfel::meta::IsSameType<typename tfel::meta::TLFindNthElt<List,N>::type,tfel::meta::TLE>::cond));
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

