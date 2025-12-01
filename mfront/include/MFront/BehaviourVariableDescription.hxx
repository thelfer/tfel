/*!
 * \file   MFront/BehaviourVariableDescription.hxx
 * \brief  This file declares the BehaviourVariableDescription class
 * \author Thomas Helfer
 * \date   06/01/2025
 */

#ifndef LIB_MFRONT_BEHAVIOURVARIABLEDESCRIPTION_HXX
#define LIB_MFRONT_BEHAVIOURVARIABLEDESCRIPTION_HXX

#include <regex>
#include <string>
#include <vector>
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/BehaviourDescription.hxx"

namespace mfront {

  /*!
   * \brief this class describes a variable associated with an external
   * behaviour.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourVariableDescription {
    //
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! \brief file name
    std::string file;
    //! \brief symbolic representation of the variable
    std::string symbolic_form;
    //! \brief name of the variable
    std::string name;
    //! \brief description of the behaviour variable
    std::string description;
    //! \brief line at wich the variable has been declared
    size_t line_number;
    /*!
     * \brief prefix added to the names of the variables not
     * shared with the behaviour using the behaviour variable
     */
    std::string variables_prefix;
    /*!
     * \brief suffix added to the names of the variables not
     * shared with the behaviour using the behaviour variable
     */
    std::string variables_suffix;
    /*!
     * \brief prefix added to the external names of the variables
     * not shared with the behaviour using the behaviour variable
     */
    std::string external_names_prefix;
    /*!
     * \brief suffix added to the external names of the variables
     * not shared with the behaviour using the behaviour variable
     */
    std::string external_names_suffix;
    /*!
     * \brief list of regular expressions allowing to select the material
     * properties that shall be shared with the behaviour using the behaviour
     * variable
     */
    std::vector<std::regex> shared_material_properties;
    /*!
     * \brief list of regular expressions allowing to select the material
     * properties that shall be evaluated by the behaviour using the behaviour
     * variable
     */
    std::vector<std::regex> evaluated_material_properties;
    /*!
     * \brief list of regular expressions allowing to select the external
     * state variables that shall be shared with the behaviour using the
     * behaviour variable
     */
    std::vector<std::regex> shared_external_state_variables;
    /*!
     * \brief list of regular expressions allowing to select the external
     * state variables that shall be evaluated with the behaviour using the
     * behaviour variable
     */
    std::vector<std::regex> evaluated_external_state_variables;
    /*!
     * \brief flag stating if the gradients of the behaviour
     * variable shall be stored in dedicated auxiliary state variables
     */
    bool store_gradients = true;
    /*!
     * \brief flag stating if the thermodynamic forces of the behaviour
     * variable shall be stored in dedicated auxiliary state variables
     */
    bool store_thermodynamic_forces = true;
    /*!
     * \brief flag stating if the persistent variables, gradients (if
     * `stored_gradients` is true) or thermodynamic forces (if
     * `stored_thermodynamic_forces` is true) associated with this behaviour
     * variable shall automatically be updated in the
     * `updateAuxiliaryStateVariables` method. If false, the user is responsible
     * for updating these auxiliary state variables.
     */
    bool automatically_save_associated_auxiliary_state_variables = true;
    //! \brief behaviour description
    BehaviourDescription behaviour;
  };  // end of BehaviourVariableDescription

  /*!
   * \brief create a new variable description by applying the names changes
   * defined in the behaviour variable to the given variable description
   *
   * \param[in] d: behaviour variable description
   * \param[in] v: variable description
   */
  MFRONT_VISIBILITY_EXPORT VariableDescription applyNamesChanges(
      const BehaviourVariableDescription&, const VariableDescription&);
  /*!
   * \brief return the list of material properties which are shared
   * with the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getSharedMaterialProperties(const BehaviourVariableDescription&,
                              const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \brief return the list of material properties which shall be
   * evaluated by the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getEvaluatedMaterialProperties(
      const BehaviourVariableDescription&,
      const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \brief return the list of material properties which are not shared
   * nor evaluated by the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getUnSharedNorEvaluatedMaterialProperties(
      const BehaviourVariableDescription&,
      const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \brief return the list of external state variables which are
   * shared with the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getSharedExternalStateVariables(
      const BehaviourVariableDescription&,
      const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \brief return the list of external state variables which shall
   * be evaluated by the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getEvaluatedExternalStateVariables(
      const BehaviourVariableDescription&,
      const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \brief return the list of external state variables which are
   * not shared nor evaluated by the behaviour using the behaviour variable
   * \param[in] d: behaviour variable description
   * \param[in] h: modelling hypothesis
   */
  MFRONT_VISIBILITY_EXPORT VariableDescriptionContainer
  getUnSharedNorEvaluatedExternalStateVariables(
      const BehaviourVariableDescription&,
      const BehaviourVariableDescription::Hypothesis&);
  /*!
   * \return the name of behaviour wrapper class
   * \param[in] d: behaviour variable description
   */
  MFRONT_VISIBILITY_EXPORT std::string getBehaviourWrapperClassName(
      const BehaviourVariableDescription&);
  /*!
   * \return the name of behaviour variable factory class
   * \param[in] d: behaviour variable description
   */
  MFRONT_VISIBILITY_EXPORT std::string getBehaviourVariableFactoryClassName(
      const BehaviourVariableDescription&);

}  // end of namespace mfront

#endif /* LIB_BEHAVIOURVARIABLEDESCRIPTION_HXX */
