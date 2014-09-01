/*! 
 * \file  include/TFEL/Metaprogramming/IsReference.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 01 aoû 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef   _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H_
#define   _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{

    /*!
     * \brief  The class IsReference test if a given 
     * class A is reference
     *
     * \param A, type to be tested.
     * \return true if A is a reference
     *
     * \author Helfer Thomas
     * \date   01 Aug 2010
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference
    {
      static const bool cond = false;
    }; // end of struct IsReference

    /*!
     * Partial specialisation for references
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference<A&>
    {
      static const bool cond = true;
    }; // end of struct IsReference

    /*!
     * Partial specialisation for constant references
     */
    template <typename A>
    struct TFEL_VISIBILITY_LOCAL IsReference<const A&>
    {
      static const bool cond = true;
    }; // end of struct IsReference

  } // end of namespace meta

} // end of namespace tfel

#endif /* _LIB_TFEL_METAPROGRAMMMING_ISREFERENCE_H */

