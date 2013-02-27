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
       * Says if the Behaviour class uses quantities
       * \see qt.hxx
       */
      static const bool use_quantities = false;
      /*!
       * Says if the Behaviour class is able to compute a consistent
       * tangent operator
       */
      static const bool hasConsistantTangentOperator = false;
      /*!
       * Says if the consistent tangent operator computed by the
       * Behaviour class (if any) is symmetric
       */
      static const bool isConsistantTangentOperatorSymmetric = false;
      /*!
       * Says if the Behaviour class is able the compute a Time Step
       * Scaling factor
       */
      static const bool hasTimeStepScalingFactor = false;
    };

  } // end of namespace material

} // end of namespace tfel

#endif /* _LIB_TFEL_MATERIALTRAITS_H */

