/*!
 * \file   BehaviourBrickDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   06/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#ifndef LIB_MFRONT_BBRICK_BEHAVIOURBRICKDESCRIPTION_HXX
#define LIB_MFRONT_BBRICK_BEHAVIOURBRICKDESCRIPTION_HXX

#include <vector>
#include <string>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/BehaviourSymmetryType.hxx"
#include "MFront/IntegrationScheme.hxx"

namespace mfront {

  // forward declaration
  struct BehaviourDSLDescription;

  /*!
   * \brief structure holding the main characteristics of a behaviour brick.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourBrickDescription {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using BehaviourType =
        tfel::material::MechanicalBehaviourBase::BehaviourType;
    //! a simple alias
    using IntegrationScheme = mfront::IntegrationScheme;
    //! a simple alias
    using BehaviourSymmetryType = mfront::BehaviourSymmetryType;
    //! \brief default constructor
    BehaviourBrickDescription();
    //! \brief copy constructor
    BehaviourBrickDescription(const BehaviourBrickDescription&);
    //! \brief move constructor
    BehaviourBrickDescription(BehaviourBrickDescription&&);
    //! \brief standard assignement
    BehaviourBrickDescription& operator=(const BehaviourBrickDescription&);
    //! \brief move assignement
    BehaviourBrickDescription& operator=(BehaviourBrickDescription&&);
    //! \brief destructor
    ~BehaviourBrickDescription();
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
    /*!
     * \brief integration variables whose associated equation are defined by the
     * brick.
     */
    std::vector<std::string> managedIntegrationVariables = {};
    //! \brief list of code blocks handled by the brick
    std::vector<std::string> managedCodeBlocks = {};
    //! \brief allow the definition of the crystal structure
    bool allowCrystalStructureDefinition = true;
    //! \brief require the definition of the crystal structure
    bool requireCrystalStructureDefinition = false;
    //! \brief allow elastic properties definition as brick options
    bool allowElasticPropertiesDefinitionAsBrickOptions = true;
    //! \brief allow the definition of the elastic properties (outside brick
    //! options)
    bool allowElasticPropertiesDefinition = true;
    //! \brief allow the definition stiffness tensor (outside brick options)
    bool allowStiffnessTensorDefinition = true;
    //! \brief require the definition stiffness tensor
    bool requireStiffnessTensorDefinition = false;
  };  // end of BehaviourBrickDescription

  /*!
   * \brief check of if the the DSL' description is compatible with the
   * description of the brick. This is mostly an hint provided to build the
   * behaviour wizard of `tfel-editor`: additional checks are performed when the
   * brick is added to the behaviour description.
   * \note if the user want to see why the given arguments are not compatible,
   * they must use the `complete` function and catch the exception (the
   * `BehaviourDSLDescription` is not modified on error).
   * \param[in] d: behaviour DSL description
   * \param[in] bbd: behaviour brick description
   * \return true if both description are compatible
   */
  MFRONT_VISIBILITY_EXPORT bool areCompatible(BehaviourDSLDescription&,
                                              const BehaviourBrickDescription&);
  /*!
   * \brief complete the DSL' description using the description of the brick.
   * The resulting description mixes both information.
   * \param[in] d: behaviour DSL description
   * \param[in] bbd: behaviour brick description
   * \note on error, the behaviour DSL description is left unchanged
   */
  MFRONT_VISIBILITY_EXPORT void complete(BehaviourDSLDescription&,
                                         const BehaviourBrickDescription&);

}  // end of namespace mfront

#endif /* LIB_MFRONT_BBRICK_BEHAVIOURBRICKDESCRIPTION_HXX */
