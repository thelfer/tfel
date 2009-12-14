/*!
 * \file   IF.hxx
 * \brief  Implements the IF metafunction.
 * \author Helfer Thomas
 * \date   19 aoû 2006
 */

#ifndef _LIB_TFEL_IF_HXX_
#define _LIB_TFEL_IF_HXX_ 

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
    struct IF{
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
    struct IF<false,ThenType,ElseType>{
      /*!
       * Returned type
       */
      typedef ElseType type;
    };
    
  } // end of meta namespace

} // end of TFEL namespace
  
#endif /* _LIB_TFEL_IF_HXX */

