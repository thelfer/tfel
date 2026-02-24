/*!
 * \file   mfront/include/MFront/BehaviourDSLDescription.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   29/08/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDSLDECRIPTION_HXX
#define LIB_MFRONT_BEHAVIOURDSLDECRIPTION_HXX

#include <vector>
#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourSymmetryType.hxx"
#include "MFront/IntegrationScheme.hxx"

namespace mfront {

  /*!
   * \brief structure used to give information about a DSL.
   *
   * This structure has been introducted to ease the implementation of
   * `tfel-editor`'s behaviour wizard.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourDSLDescription {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using BehaviourType =
        tfel::material::MechanicalBehaviourBase::BehaviourType;
    //! \brief default constructor
    BehaviourDSLDescription();
    //! \brief copy constructor
    BehaviourDSLDescription(const BehaviourDSLDescription&);
    //! \brief move constructor
    BehaviourDSLDescription(BehaviourDSLDescription&&);
    //! \brief standard assignement
    BehaviourDSLDescription& operator=(const BehaviourDSLDescription&);
    //! \brief move assignement
    BehaviourDSLDescription& operator=(BehaviourDSLDescription&&);
    //! \brief destructor
    ~BehaviourDSLDescription();
    //! \brief type of the behaviour handled
    BehaviourType behaviourType =
        tfel::material::MechanicalBehaviourBase::GENERALBEHAVIOUR;
    //! \brief integration scheme
    IntegrationScheme integrationScheme =
        IntegrationScheme::UNDEFINEDINTEGRATIONSCHEME;
    //! \brief supported modelling hypotheses
    std::vector<Hypothesis> supportedModellingHypotheses =
        ModellingHypothesis::getModellingHypotheses();
    //! \brief supported behaviour symmetry
    std::vector<BehaviourSymmetryType> supportedBehaviourSymmetries = {
        mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    //! \brief list of typical code blocks that may be defined by the user
    std::vector<std::string> typicalCodeBlocks;
    //! \brief minimal body of a MFront file
    std::string minimalMFrontFileBody;
    //! \brief allow user defined material properties
    bool allowUserDefinedMaterialProperties = true;
    //! \brief allow user defined parameters
    bool allowUserDefinedParameters = true;
    //! \brief allow user defined state variables
    bool allowUserDefinedStateVariables = true;
    //! \brief allow user defined auxiliary state variables
    bool allowUserDefinedAuxiliaryStateVariables = true;
    //! \brief allow user defined integration variables
    bool allowUserDefinedIntegrationVariables = true;
    //! \brief allow user defined external state variables
    bool allowUserDefinedExternalStateVariables = true;
    //! \brief allow user defined local variables
    bool allowUserDefinedLocalVariables = true;
    //! \brief allow the definition of the crystal structure
    bool allowCrystalStructureDefinition = true;
    //! \brief require the definition of the crystal structure
    bool requireCrystalStructureDefinition = false;
    //! \brief allow the definition of the elastic properties
    bool allowElasticPropertiesDefinition = true;
    //! \brief allow the definition stiffness tensor
    bool allowStiffnessTensorDefinition = true;
    //! \brief require the definition stiffness tensor
    bool requireStiffnessTensorDefinition = false;

  };  // end of struct BehaviourDSLDescription

  /*!
   * \return a default description for DSLs handling strain based behaviours.
   */
  MFRONT_VISIBILITY_EXPORT BehaviourDSLDescription
  getDefaultStrainBasedBehaviourDSLDescription();
  /*!
   * \return a default description for DSLs handling finite strain behaviours.
   */
  MFRONT_VISIBILITY_EXPORT BehaviourDSLDescription
  getDefaultFiniteStrainBehaviourDSLDescription();

  /*!
   * \brief a small function checking of all the modelling hypotheses can be
   * supported.
   * \param[in] d: behaviour DSL description
   * \return true if all modelling hypotheses can be supported
   */
  MFRONT_VISIBILITY_EXPORT bool supportAllModellingHypothes(
      const BehaviourDSLDescription&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDSLDECRIPTION_HXX */
