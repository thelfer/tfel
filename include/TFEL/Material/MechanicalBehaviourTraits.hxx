/*!
 * \file   include/TFEL/Material/MechanicalBehaviourTraits.hxx
 * \brief  This file declares the MechanicalBehaviourTraits template class
 * \author Thomas Helfer
 * \date   10 Aug 2006
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATERIALTRAITS_HXX
#define LIB_TFEL_MATERIALTRAITS_HXX

namespace tfel {

  namespace material {

    /*!
     * \class MechanicalBehaviourTraits
     * \brief Gives information about its Behaviour argument.
     * All behaviour shall specialise this class.
     * \param Behaviour.
     * \author Thomas Helfer
     * \date   10 Aug 2006
     */
    template <class Behaviour>
    struct MechanicalBehaviourTraits {
      /*!
       * Says if the traits class has been specialised
       */
      static constexpr bool is_defined = false;
      /*!
       * if true, the behaviour must provide a
       * compouteStressFreeExpansion method.
       */
      static constexpr bool hasStressFreeExpansion = false;
      /*!
       * if true, the hasStressFreeExpansion must also be ture.
       */
      static constexpr bool handlesThermalExpansion = false;
      /*!
       * Number of material properties.
       */
      static constexpr unsigned short material_properties_nb = 0;
      /*!
       * Number of external variables.
       */
      static constexpr unsigned short external_variables_nb = 0;
      /*!
       * Number of internal variables.
       */
      static constexpr unsigned short internal_variables_nb = 0;
      /*!
       * Says if the Behaviour class uses quantities
       * \see qt.hxx
       */
      static constexpr bool use_quantities = false;
      /*!
       * Says if the Behaviour class is able to compute a consistent
       * tangent operator. More precisely, this must be true if the
       * behaviour is able to compute an approximation of the
       * consistent tangent operator.
       */
      static constexpr bool hasConsistentTangentOperator = false;
      /*!
       * Says if the consistent tangent operator computed by the
       * Behaviour class (if any) is symmetric. If true, we also
       * consider that all the possible stiffness matrix are
       * symmetric.
       */
      static constexpr bool isConsistentTangentOperatorSymmetric = false;
      /*!
       * Says if the Behaviour class is able to compute a prediction
       * operator.
       */
      static constexpr bool hasPredictionOperator = false;
      /*!
       * Says if the Behaviour class is able the compute a Time Step
       * Scaling factor
       */
      static constexpr bool hasTimeStepScalingFactor = false;
      /*!
       * \return the name of the behaviour.
       * \note This method can't be a static method of the
       * behaviour. If it were, requesting the name of the behaviour
       * would instantiate the class even if it is not defined. This
       * can lead to static assertion failures.
       */
      static const char* getName() { return ""; }
    };

  }  // end of namespace material

}  // end of namespace tfel

#endif /* LIB_TFEL_MATERIALTRAITS_HXX */
