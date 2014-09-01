/*!
 * \file   include/TFEL/Metaprogramming/RemoveConstness.hxx
 * \brief  This file declares the RemoveConstness metafunction
 * \author Helfer Thomas
 * \date   18 Oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_REMOVECONSTNESS_H_
#define _LIB_TFEL_REMOVECONSTNESS_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \class  RemoveConstness
     * \brief  return a the same type without the 
     * const qualifier.
     * \param  typename T. 
     *
     * \author Helfer Thomas
     * \date   02 Mar 2007
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL RemoveConstness
    {
      //! the result
      typedef T type;
    };
    
    /*!
     * Partial specialisation for const T
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL RemoveConstness<const T>
    {
      //! the result
      typedef T type;
    };
    
  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_REMOVECONSTNESS_H */

