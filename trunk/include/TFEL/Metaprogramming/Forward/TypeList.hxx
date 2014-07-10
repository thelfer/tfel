/*! 
 * \file  TypeList.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 */

#ifndef _LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_H_
#define _LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_H_ 

namespace tfel{
  
  namespace meta{

    /*!
     * Node of TypeList
     * \param T : current type
     * \param U : next type
     */
    template<typename T, typename U>
    struct TLNode;

    struct TLE;
    
  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_METAPROGRAMMING_FORWARD_TYPELIST_H */

