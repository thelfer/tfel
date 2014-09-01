/*! 
 * \file  include/TFEL/Metaprogramming/Forward/TypeList.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 janv. 2012
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
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

