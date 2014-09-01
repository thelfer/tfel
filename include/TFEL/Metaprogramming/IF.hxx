/*!
 * \file   include/TFEL/Metaprogramming/IF.hxx
 * \brief  Implements the IF metafunction.
 * \author Helfer Thomas
 * \date   19 aoû 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_IF_HXX_
#define _LIB_TFEL_IF_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{

  namespace meta{
    
    /*!
     * \class IF 
     * \brief The IF template class enables type selection at 
     * compile-time given a condition.
     * 
     * \param bool condition.
     * \param ThenType type selected if the condition holds true.
     * \param ElseType type selected if the condition is false.
     * 
     * \return type, type selected.
     *
     * \see
     * - Traits: The else-if-then of Types, by A. Alexandrescu, 
     * available at http://erdani.org/
     *
     * \author Helfer Thomas
     * \date   19 aoû 2006
     */
    template<bool c, typename ThenType, typename ElseType>
    struct TFEL_VISIBILITY_LOCAL IF{
      /*!
       * Returned type
       */
      typedef ThenType type;
    };
    
    /*!
     *\brief partial specialisation of condition is false.
     *
     * \author Helfer Thomas
     * \date   19 aoû 2006
     */
    template<typename ThenType, typename ElseType>
    struct TFEL_VISIBILITY_LOCAL IF<false,ThenType,ElseType>{
      /*!
       * Returned type
       */
      typedef ElseType type;
    };
    
  } // end of meta namespace

} // end of TFEL namespace
  
#endif /* _LIB_TFEL_IF_HXX */

