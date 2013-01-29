/*!
 * \file   MechanicalBehaviourTraits.hxx
 * \brief  This file declares the MechanicalBehaviourTraits template class
 * \author Helfer Thomas
 * \date   10 Aug 2006
 */

#ifndef _LIB_TFEL_MATERIALTRAITS_H_
#define _LIB_TFEL_MATERIALTRAITS_H_ 

namespace tfel{
  
  namespace material {

    /*!
     * \class MechanicalBehaviourTraits
     * \brief Gives information about its Behaviour argument.
     * All behaviour shall specialise this class.
     * \param Behaviour.
     * \author Helfer Thomas
     * \date   10 Aug 2006
     */
    template<class Behaviour>
    struct MechanicalBehaviourTraits{
      /*!
       * Says if the traits class has been specialised 
       */
      static const bool is_defined = false;
      /*!
       * Number of material properties.
       */
      static const unsigned short material_properties_nb = 0;
      /*!
       * Number of external variables.
       */
      static const unsigned short external_variables_nb  = 0;
      /*!
       * Number of internal variables.
       */
      static const unsigned short internal_variables_nb  = 0;
      /*!
       * Says if the behaviour uses quantities
       * \see qt.hxx
       */
      static const bool use_quantities = false;
      /*!
       * Says if behaviour provides a consistent tangent operator
       */
      static const bool hasConsistantTangentOperator = false;
      /*!
       * Says if behaviour the consistent tangent operator (if any) is
       * symmetric
       */
      static const bool isConsistantTangentOperatorSymmetric = false;
    };

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIALTRAITS_H */

