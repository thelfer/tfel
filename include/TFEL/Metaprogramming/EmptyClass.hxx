/*!
 * \file   include/TFEL/Metaprogramming/EmptyClass.hxx
 * \brief  
 * \author Helfer Thomas
 * \date   22 oct 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_EMPTYCLASS_HXX_
#define _LIB_TFEL_EMPTYCLASS_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace meta{

    /*!
     * \brief Empty class is a dummy class.
     * \author Helfer Thomas
     * \date   01 Mar 06
     */
    struct TFEL_VISIBILITY_LOCAL EmptyClass
    {};

    /*!
     * \brief  EmptyClassA is a dummy class.
     * \author Helfer Thomas
     * \date   01 Mar 06
     */
    struct TFEL_VISIBILITY_LOCAL EmptyClassA
    {};


    /*!
     * \brief  EmptyClassB is a dummy class.
     * \author Helfer Thomas
     * \date   01 Mar 06
     */
    struct TFEL_VISIBILITY_LOCAL EmptyClassB
    {};

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_EMPTYCLASS_HXX */

