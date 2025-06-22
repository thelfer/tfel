/*!
 * \file  mfront/include/MFront/BehaviourDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDESCRIPTION_HXX
#define LIB_MFRONT_BEHAVIOURDESCRIPTION_HXX

#include <set>
#include <vector>
#include <memory>
#include <utility>

#include "TFEL/Utilities/GenTypeBase.hxx"
#include "TFEL/Math/tvector.hxx"
#include "TFEL/Material/CrystalStructure.hxx"
#include "TFEL/Material/SlipSystemsDescription.hxx"
#include "TFEL/Material/MechanicalBehaviour.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "TFEL/Material/OrthotropicAxesConvention.hxx"

#include "MFront/MFrontConfig.hxx"
#include "MFront/CodeBlock.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/BehaviourAttribute.hxx"
#include "MFront/BehaviourData.hxx"
#include "MFront/BehaviourSymmetryType.hxx"

namespace mfront {

  // forward declaration
  struct LocalDataStructure;
  // forward declaration
  struct MaterialPropertyDescription;

  /*!
   * This structure describes a mechanical behaviour
   *
   * This class handles the fact that the mechanical behaviour
   * data may depend on the modelling hypothesis considered,
   * altough most of the time they won't.
   *
   * This class thus stores a default mechanical behaviour data
   * which is intendend to be valid for most hypotheses, and specific
   * mechanical behaviour datas when required.
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourDescription
      : public tfel::material::MechanicalBehaviourBase,
        public SupportedTypes {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using CrystalStructure = tfel::material::CrystalStructure;
    //! a simple alias
    using SlipSystemsDescription = tfel::material::SlipSystemsDescription;
    //! a simple alias
    using SlipSystem = SlipSystemsDescription::system;
    //! a simple alias
    using InteractionMatrixStructure =
        SlipSystemsDescription::InteractionMatrixStructure;
    //! a simple alias
    using OrthotropicAxesConvention = tfel::material::OrthotropicAxesConvention;
    //! a simple alias
    using Mode = BehaviourData::Mode;
    //! a simple alias
    using Position = BehaviourData::Position;
    //! validation tatus
    enum VerificationStatus {
      UNVERIFIED,
      VERIFIED
    };  // end of VerificationStatus
    //! validation tatus
    enum ValidationStatus {
      UNVALIDATED,
      VALIDATED
    };  // end of ValidationStatus
    //! Strain measure
    enum StrainMeasure {
      LINEARISED,
      GREENLAGRANGE,
      HENCKY
    };  // end of enum StrainMeasure
        /*!
         * \brief this structure holds the value of a constant material
         * property
         */
    struct ConstantMaterialProperty {
      //! parameter name associated with the material property
      std::string name;
      //! default value for the constant material property
      double value;
    };
    /*!
     * \brief this structure holds the value of a material
     * property defined through an analytic expression
     */
    struct AnalyticMaterialProperty {
      //! return the list of the variables
      std::vector<std::string> getVariablesNames() const;
      //! description of a material property
      std::string f;
    };  // end of AnalyticMaterialProperty
        /*!
         * \brief this structure holds the value of a material
         * property defined through an mfront file
         */
    struct ExternalMFrontMaterialProperty {
      //! description of a material property
      std::shared_ptr<MaterialPropertyDescription> mpd;
    };
    //! list of supported material properties types
    using MaterialPropertyTypes =
        tfel::meta::GenerateTypeList<ConstantMaterialProperty,
                                     AnalyticMaterialProperty,
                                     ExternalMFrontMaterialProperty>::type;
    //! a simple alias
    using StressFreeExpansionDescription =
        BehaviourData::StressFreeExpansionDescription;
    /*!
     * The description of a material property from the point of
     * view of a behaviour.
     */
    using MaterialProperty =
        tfel::utilities::GenTypeBase<MaterialPropertyTypes>;
    /*!
     * structure used to defined a Hill tensor
     */
    struct HillTensor {
      //! name of the Hill tensor
      std::string name;
      //! Hill coeffients
      std::vector<MaterialProperty> c;
    };  // end of struct HillTensor
        /*!
         * \brief Available integration schemes.
         * One of the first thing a dsl shall do is to set the
         * integration scheme it uses.
         */
    enum IntegrationScheme {
      IMPLICITSCHEME,  //!< value set by the `Implicit` Dsl family
      EXPLICITSCHEME,  //!< value set by the `Runge-Kutta` Dsl
      SPECIFICSCHEME,  /*!< value set by the `	IsotropicMisesCreepDSL`,
                        * `IsotropicMisesPlasticFlowDSL`,
                        * `IsotropicStrainHardeningMisesCreepDSL`
                        * `MultipleIsotropicMisesFlowsDSL` dsls. */
      UNDEFINEDINTEGRATIONSCHEME  //! default value.
    };
    /*!
     * a simple structure used to compte the value of a material
     * property
     */
    struct MaterialPropertyInput {
      //! variable name
      std::string name;
      //! external name
      std::string ename;
      //! variable type
      enum Category {
        TEMPERATURE,
        MATERIALPROPERTY,
        STATEVARIABLE,
        AUXILIARYSTATEVARIABLEFROMEXTERNALMODEL,
        EXTERNALSTATEVARIABLE,
        PARAMETER,
        STATICVARIABLE
      };
      Category category;
    };  // end of
    //! attribute name
    static const char* const requiresStiffnessTensor;
    //! attribute name
    static const char* const computesStiffnessTensor;
    //! attribute name
    static const char* const requiresUnAlteredStiffnessTensor;
    //! attribute name
    static const char* const requiresThermalExpansionCoefficientTensor;
    //! attribute name
    static const char* const setRequireThermalExpansionCoefficientTensor;
    //! constructor
    BehaviourDescription();
    //! copy constructor
    BehaviourDescription(const BehaviourDescription&);
    /*!
     * \brief return true if the behaviour description allows the
     * declaration of user defined variables
     */
    bool allowsNewUserDefinedVariables() const;
    //! \brief disallow the declaration of user defined variables
    void disallowNewUserDefinedVariables();
    //! \brief set if dynamically allocated vectors are allowed
    void areDynamicallyAllocatedVectorsAllowed(const bool);
    //! \return true if dynamically allocated vectors are allowed
    bool areDynamicallyAllocatedVectorsAllowed() const;
    //! \return true of the parser shall declare a dynamically
    bool useDynamicallyAllocatedVector(const unsigned short) const;
    /*!
     * \brief set the behaviour name
     * \param[in] b: behaviour name
     */
    void setBehaviourName(const std::string&);
    //! \return the behaviour name
    const std::string& getBehaviourName() const;
    /*!
     * \brief set the dsl name
     * \param[in] b: dsl name
     */
    void setDSLName(const std::string&);
    //! \return the dsl name
    const std::string& getDSLName() const;
    /*!
     * \brief set the material name
     * \param[in] m: material name
     */
    void setMaterialName(const std::string&);
    //! \return the material name
    const std::string& getMaterialName() const;
    /*!
     * \brief set the library name
     * \param[in] l: library name
     */
    void setLibrary(const std::string&);
    //! \return the library name
    const std::string& getLibrary() const;
    /*!
     * \brief set the class name
     * \param[in] n: class name
     */
    void setClassName(const std::string&);
    //! \return the class name
    const std::string& getClassName() const;
    /*!
     * \return the inputs of a material property
     * \param[in] i: list of variables used to evaluated the material
     * property.
     * \param[in] b: if true, the modelling hypotheses must be
     * defined. In this case, inputs are searched in the BehaviourData
     * associed with each modelling hypothesis and consistency of the
     * definition of each input is checked. If false, all inputs are
     * searched in the root BehaviourData only (the one associated
     * with UNDEFINEDHYPOTESIS).
     */
    std::vector<MaterialPropertyInput> getMaterialPropertyInputs(
        const std::vector<std::string>&, const bool = true) const;
    /*!
     * \return the inputs of a material property
     * \param[in] mpd: material property description
     * \param[in] b: if true, the modelling hypotheses must be
     * defined. In this case, inputs are searched in the BehaviourData
     * associed with each modelling hypothesis and consistency of the
     * definition of each input is checked. If false, all inputs are
     * searched in the root BehaviourData only (the one associated
     * with UNDEFINEDHYPOTESIS).
     */
    std::vector<MaterialPropertyInput> getMaterialPropertyInputs(
        const MaterialPropertyDescription&, const bool = true) const;
    /*!
     * \brief append the given code to the sources
     * \param[in] s: sources
     */
    void appendToSources(const std::string&);
    //! \return the material name
    const std::string& getSources() const;
    /*!
     * \brief append the given code to the members
     * \param[in] h: hypothesis
     * \param[in] s: members
     * \param[in] b: if h is UNDEFINEDHYPOTHIS, propagate to all specialised
     * data
     */
    void appendToMembers(const Hypothesis, const std::string&, const bool);
    //! \return the aditional members
    std::string getMembers(const Hypothesis) const;
    /*!
     * \brief append the given code to the members
     * \param[in] h: hypothesis
     * \param[in] s: members
     * \param[in] b: if h is UNDEFINEDHYPOTHIS, propagate to all specialised
     * data
     */
    void appendToPrivateCode(const Hypothesis, const std::string&, const bool);
    /*!
     * \return the material private code
     * \param[in] h: hypothesis
     */
    std::string getPrivateCode(const Hypothesis) const;
    /*!
     * \brief append the given code to the includes
     * \param[in] s: includes
     */
    void appendToIncludes(const std::string&);
    //! \return the includes
    const std::string& getIncludes() const;
    /*!
     * add a material law
     * \param[in] m:material law
     */
    void addMaterialLaw(const std::string&);
    /*!
     * \brief add a model description.
     * All outputs of the model are automatically added as auxiliary state
     * variables
     * \param[in] md: model description
     */
    void addModelDescription(const ModelDescription&);
    /*!
     * add a local data structure
     * \param[in] lds: local data structure
     * \param[in] s: registration status
     */
    void addLocalDataStructure(
        const LocalDataStructure&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    //! \return the list of material laws
    const std::vector<std::string>& getMaterialLaws() const;
    //! \return the size of the main variables
    std::pair<SupportedTypes::TypeSize, SupportedTypes::TypeSize>
    getMainVariablesSize() const;
    /*!
     * \brief add a new main variable
     * \note using this method means that the behaviour type is always
     * 'GENERALBEHAVIOUR'. This can't be changed afterwards.
     */
    void addMainVariable(const Gradient&, const ThermodynamicForce&);
    //! \return the main variables of the behaviour
    const std::vector<std::pair<Gradient, ThermodynamicForce>>&
    getMainVariables() const;
    /*!
     * \return the driving variable with the associated name
     * \param[in] n: name
     */
    const Gradient& getGradient(const std::string&) const;
    /*!
     * \return the thermodynamic force with the associated name
     * \param[in] n: name
     */
    const ThermodynamicForce& getThermodynamicForce(const std::string&) const;
    /*!
     * \brief set the behaviour to be a small strain standard
     * behaviour
     */
    void declareAsASmallStrainStandardBehaviour();
    /*!
     * \brief set the behaviour to be a finite strain standard
     * behaviour
     * \param[in] b: if true, declare F and dF as members
     */
    void declareAsAFiniteStrainStandardBehaviour(const bool);
    //! \brief set the behaviour to be a cohesive zone model
    void declareAsACohesiveZoneModel();
    /*!
     * \brief set the strain measure to be used
     * \param[in] m: measure
     */
    void setStrainMeasure(const StrainMeasure);
    //! \return the strain measure associated with the behaviour
    StrainMeasure getStrainMeasure() const;
    //! \return if the strain measure has been set
    bool isStrainMeasureDefined() const;
    //! \return the type of the stiffness operator
    std::string getTangentOperatorType() const;

    bool useQt() const;

    void setUseQt(const bool);

    bool isGradientName(const std::string&) const;

    bool isGradientIncrementName(const std::string&) const;

    bool isThermodynamicForceName(const std::string&) const;
    //! \return the behaviour type
    BehaviourType getBehaviourType() const;
    /*!
     * \brief set the integration scheme
     * \param[in] s: integration scheme
     */
    void setIntegrationScheme(const IntegrationScheme);
    //! \return the integration scheme used
    IntegrationScheme getIntegrationScheme() const;
    //! \return a string describing the behaviour type
    std::string getBehaviourTypeFlag() const;
    //! \return the symmetry type of the behaviour
    BehaviourSymmetryType getSymmetryType() const;

    void setSymmetryType(const BehaviourSymmetryType);

    bool isSymmetryTypeDefined() const;

    BehaviourSymmetryType getElasticSymmetryType() const;

    void setElasticSymmetryType(const BehaviourSymmetryType);

    bool isElasticSymmetryTypeDefined() const;
    /*!
     * \brief set the orthotropic axes convention.
     * \param[in] c: new value for the orthotropic axes convention.
     * \note this method can be only call once.
     */
    void setOrthotropicAxesConvention(const OrthotropicAxesConvention);
    /*!
     * \return the orthotropic axes convention.
     * \note if the orthotropic axes convention has not been defined,
     * the UNDEFINED convention is assumed and oacIsDefined is set to
     * true.
     */
    OrthotropicAxesConvention getOrthotropicAxesConvention() const;
    /*!
     * \return true if the crystal structure of the material has been
     * defined.
     */
    bool hasCrystalStructure() const;
    //! \return the crystal structure of the material
    CrystalStructure getCrystalStructure() const;
    /*!
     * \brief set the crystal structure
     * \param[in] cs: crystal structure
     */
    void setCrystalStructure(const CrystalStructure);
    //! \return true if slip systems were defined
    bool areSlipSystemsDefined() const;
    /*!
     * \brief set the behaviour slip systems
     * \param[in] gss: slip systems
     */
    void setSlipSystems(const std::vector<SlipSystem>&);
    //! \return the slip systemss
    const SlipSystemsDescription& getSlipSystems() const;
    /*!
     * \brief set the interaction matrix.
     * \param[in] m: interaction matrix
     * \note: this method is only valid if one and only one slip
     * system is defined.
     */
    void setInteractionMatrix(const std::vector<long double>&);
    /*!
     * \brief return the interaction matrix structure.
     * \note: this method is only valid if one and only one slip
     * system is defined.
     */
    InteractionMatrixStructure getInteractionMatrixStructure() const;
    /*!
     * \brief return true if an interaction matrix is defined.
     * \note: this method is only valid if one and only one slip
     * system is defined.
     */
    bool hasInteractionMatrix() const;
    /*!
     * \brief return true if an interaction matrix is defined.
     * \note: this method is only valid if one and only one slip
     * system is defined.
     */
    bool hasDislocationsMeanFreePathInteractionMatrix() const;
    /*!
     * \brief set the interaction matrix.
     * \param[in] m: interaction matrix
     * \note: this method is only valid if one and only one slip
     * system is defined.
     */
    void setDislocationsMeanFreePathInteractionMatrix(
        const std::vector<long double>&);
    /*!
     * \return true if the material property is constant over the time step
     * \param[in] mp: material property
     */
    bool isMaterialPropertyConstantDuringTheTimeStep(
        const MaterialProperty&) const;
    /*!
     * \return true if the material property depends on state variables
     * \param[in] mp: material property
     */
    bool isMaterialPropertyDependantOnStateVariables(
        const MaterialProperty&) const;
    //! \return registred models
    const std::vector<ModelDescription>& getModelsDescriptions() const;
    /*!
     * \brief set the elastic material properties
     * \param[in] emps: elastic material properties
     */
    void setElasticMaterialProperties(const std::vector<MaterialProperty>&);
    /*!
     * \return the elastic material properties
     */
    const std::vector<MaterialProperty>& getElasticMaterialProperties() const;
    /*!
     * \return true if the elastic material properties have been defined
     */
    bool areElasticMaterialPropertiesDefined() const;
    /*!
     * \return true if the elastic material properties are constant
     * over the time step
     */
    bool areElasticMaterialPropertiesConstantDuringTheTimeStep() const;
    /*!
     * \return true if the given material properties are constant over
     * the time step
     * \param[in] mps: list of material properties
     */
    bool areMaterialPropertiesConstantDuringTheTimeStep(
        const std::vector<MaterialProperty>&) const;
    /*!
     * \return true if the elastic material properties depend on state
     * variables
     */
    bool areElasticMaterialPropertiesDependantOnStateVariables() const;
    /*!
     * \return true if the given material properties depend on state variables
     * \param[in] mps: list of material properties
     */
    bool areMaterialPropertiesDependantOnStateVariables(
        const std::vector<MaterialProperty>&) const;
    /*!
     * \brief add a new Hill tensor
     * \param[in] v:   variable description
     * \param[in] hcs: Hill tensor coefficents
     * \pre the behaviour must be either small strain or finite strain
     * \pre the material must be orthotropic
     * \pre the number of Hill tensor coefficients must be 6
     * \pre the variable name must be valid and unused yet
     * \pre the type of the variable must be "tfel::math::st2tost2<N,stress>"
     * \pre the array size of the variable must be 1
     */
    void addHillTensor(const VariableDescription&,
                       const std::vector<MaterialProperty>&);
    /*!
     * \return the list of Hill tensors that have been defined
     */
    const std::vector<HillTensor>& getHillTensors() const;
    /*!
     * \return a mechanical behaviour data associated with the
     * given modelling hypothesis
     * \param[in] h: modelling hypothesis
     */
    const BehaviourData& getBehaviourData(const Hypothesis&) const;
    //! \return true if modelling hypotheses are defined.
    bool areModellingHypothesesDefined() const;
    /*!
     * \return all the modelling hypotheses supported by the
     * behaviour.
     */
    const std::set<Hypothesis>& getModellingHypotheses() const;
    /*!
     * \return all the modelling hypotheses which are distinct.
     *
     * This method returns, among all the supported modelling
     * hypotheses (see getModellingHypotheses), the ones which have
     * specialised mechanical data and, if at least one modelling
     * hypothesis has no specialisation, UNDEFINEDHYPOTHESIS.
     *
     * This method has been introduced for iteration purpose in
     * behaviour dsls.
     */
    std::set<Hypothesis> getDistinctModellingHypotheses() const;
    /*!
     * \return true if the given modelling hypothesis is supported
     * \param[in] h: modelling hypothesis
     * \see hasSpecialisedMechanicalData
     */
    bool isModellingHypothesisSupported(const Hypothesis) const;
    /*!
     * \brief set the list of supported modelling hypotheses
     * \param[in] h: supported modelling hypotheses
     * \param[in] b: parameter used if some modelling hypotheses are
     * already defined. In this case:
     * - if b is true, the resulting modelling hypotheses
     *   will be the intersection of the given modelling hypotheses and
     *   the already declared ones. If such intersection is empty, an
     *   exception is thrown.
     * - if b is false, an exception is thrown.
     */
    void setModellingHypotheses(const std::set<Hypothesis>&,
                                const bool b = false);
    /*!
     * \brief add material properties
     * \param[in] h: modelling hypothesis
     * \param[in] v: material properties added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void addMaterialProperties(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a material property
     * \param[in] h: modelling hypothesis
     * \param[in] v: material property added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void addMaterialProperty(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: state variable added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void addIntegrationVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: state variables added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void addIntegrationVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: state variable added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void addStateVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: state variables added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the state variables
     * to the default data and to all the specialisations
     */
    void addStateVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add auxiliary state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: auxiliary state variables added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void addAuxiliaryStateVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add an auxiliary state variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: auxiliary state variable added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the auxiliary state
     * variables to the default data and to all the specialisations
     */
    void addAuxiliaryStateVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add external state variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: external state variables added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void addExternalStateVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add external state variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: external state variable added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the external state
     * variables to the default data and to all the specialisations
     */
    void addExternalStateVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a local variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: local variable added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void addLocalVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add local variables
     * \param[in] h: modelling hypothesis
     * \param[in] v: local variables added
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variables to
     * the default data and to all the specialisations
     */
    void addLocalVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief add a static variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: static variable
     * \param[in] s: registration status
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the local variable to
     * the default data and to all the specialisations
     */
    void addStaticVariable(
        const Hypothesis,
        const StaticVariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \return the value of an integer constant
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     */
    int getIntegerConstant(const Hypothesis, const std::string&) const;
    /*!
     * \brief add a material property
     * \param[in] h: modelling hypothesis
     * \param[in] v: parameter
     * \param[in] s: registration status
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void addParameter(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus = BehaviourData::UNREGISTRED);
    /*!
     * \brief assign an attribute to variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: variable name
     * \param[in] n: attribute name
     * \param[in] a: attribute
     * \param[in] b: don't throw if the attribute already exists.  In
     * this case, the attribute is left unchanged. However the type of
     * the attribute is checked.
     */
    void setVariableAttribute(const Hypothesis,
                              const std::string&,
                              const std::string&,
                              const VariableAttribute&,
                              const bool);
    //! \return a type suitable for storing stress-free expansion
    std::string getStressFreeExpansionType() const;
    /*!
     * \brief add a new stress free expansion description
     * \param[in] h: modelling hypothesis
     * \param[in] sfed: stress free expansion description
     */
    void addStressFreeExpansion(const Hypothesis,
                                const StressFreeExpansionDescription&);
    /*!
     * \return the registred stress free expansion descriptions
     * \param[in] h: modelling hypothesis
     */
    const std::vector<BehaviourData::StressFreeExpansionDescription>&
    getStressFreeExpansionDescriptions(const Hypothesis) const;
    /*!
     * \return true if a least one modelling hypothesis is anisotropic
     * \param[in] h: modelling hypothesis
     */
    bool isStressFreeExansionAnisotropic(const Hypothesis) const;
    /*!
     * \return true if the given name is the one of a material
     * property.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isMaterialPropertyName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is the one of a local variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isLocalVariableName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is the one of an persistent
     * variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isPersistentVariableName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isIntegrationVariableName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is the one of an integration
     * variable increment.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isIntegrationVariableIncrementName(const Hypothesis,
                                            const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isStateVariableName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is the one of an state
     * variable increment.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isStateVariableIncrementName(const Hypothesis,
                                      const std::string&) const;
    /*!
     * \return true if the given name is the one of an auxiliary
     * internal state variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isAuxiliaryStateVariableName(const Hypothesis,
                                      const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable.
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isExternalStateVariableName(const Hypothesis,
                                     const std::string&) const;
    /*!
     * \return true if the given name is the one of an external state
     * variable increment
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isExternalStateVariableIncrementName(const Hypothesis,
                                              const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isStaticVariableName(const Hypothesis, const std::string&) const;
    /*!
     * \brief check if one has to include tvector.hxx or vector.hxx
     * \param[in] b1: requires true if one has to include tvector.hxx
     * \param[in] b2: requires true if one has to include vector.hxx
     */
    void requiresTVectorOrVectorIncludes(bool&, bool&) const;
    /*!
     * \brief check if one has to include tvector.hxx or vector.hxx
     * \param[in] d:  behaviour data
     * \param[in] b1: requires true if one has to include tvector.hxx
     * \param[in] b2: requires true if one has to include vector.hxx
     */
    void requiresTVectorOrVectorIncludes(bool&,
                                         bool&,
                                         const BehaviourData&) const;

    /*!
     * \return true if all mechanical data are specialised
     * This means that the default mechanical data is useless
     */
    bool areAllMechanicalDataSpecialised() const;
    /*!
     * \return true if all mechanical data associated with the given
     * modelling hypothesis are specialised.
     * \param[in] h: modelling hypotheses considered
     */
    bool areAllMechanicalDataSpecialised(const std::set<Hypothesis>&) const;
    /*!
     * \return true if the mechanical data associated with the given
     * modelling hypothesis is specialised.
     * \param[in] h: modelling hypothesis considered
     */
    bool hasSpecialisedMechanicalData(const Hypothesis) const;
    //! \return true if a stress free expansion has been defined
    bool requiresStressFreeExpansionTreatment(const Hypothesis) const;
    //! \return true if thermal expansion coefficient were defined
    bool areThermalExpansionCoefficientsDefined() const;
    //! \return the thermal expansion coefficients
    const std::vector<MaterialProperty>& getThermalExpansionCoefficients()
        const;
    /*!
     * set the behaviour thermal expansion coefficient (isotropic behaviour)
     * \param[in] a: thermal expansion
     */
    void setThermalExpansionCoefficient(MaterialProperty);
    /*!
     * set the behaviour thermal expansions coefficient (orthotropic behaviour)
     * \param[in] a1: thermal expansion in the first direction
     * \param[in] a2: thermal expansion in the second direction
     * \param[in] a3: thermal expansion in the third  direction
     */
    void setThermalExpansionCoefficients(MaterialProperty,
                                         MaterialProperty,
                                         MaterialProperty);
    /*!
     * \return the external names associated with the variables
     * contained in the given container
     * \param[in] h: modelling Hypothesis
     * \param[in] v: variables for which external names are requested
     */
    std::vector<std::string> getExternalNames(const Hypothesis,
                                              const VarContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container
     * \param[out] n: names
     * \param[in]  h: modelling Hypothesis
     * \param[in]  v: variables for which external names are requested
     */
    void getExternalNames(std::vector<std::string>&,
                          const Hypothesis,
                          const VarContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container
     * \param[out] n: names
     * \param[in]  h: modelling Hypothesis
     * \param[in]  v: variables for which external names are requested
     */
    void appendExternalNames(std::vector<std::string>&,
                             const Hypothesis,
                             const VarContainer&) const;
    /*!
     * \return true if the given member is used in a code block
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isMemberUsedInCodeBlocks(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is a parameter name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool isParameterName(const Hypothesis, const std::string&) const;
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] n: parameter name
     * \param[in] v: parameter default value
     */
    void setParameterDefaultValue(const Hypothesis,
                                  const std::string&,
                                  const double);
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] n: parameter name
     * \param[in] i: index
     * \param[in] v: parameter default value
     */
    void setParameterDefaultValue(const Hypothesis,
                                  const std::string&,
                                  const unsigned short i,
                                  const double);
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] n: parameter name
     * \param[in] v: parameter default value
     */
    void setParameterDefaultValue(const Hypothesis,
                                  const std::string&,
                                  const int);
    /*!
     * \param[in] h: modelling hypothesis
     * \param[in] n: parameter name
     * \param[in] v: parameter default value
     */
    void setParameterDefaultValue(const Hypothesis,
                                  const std::string&,
                                  const unsigned short);
    /*!
     * \return the default value of a parameter
     * \param[in] h: modelling hypothesis
     * \param[in] v: parameter default value
     */
    double getFloattingPointParameterDefaultValue(const Hypothesis,
                                                  const std::string&) const;
    /*!
     * \return the default value of a parameter (array case)
     * \param[in] h: modelling hypothesis
     * \param[in] v: parameter default value
     * \param[in] i: index
     */
    double getFloattingPointParameterDefaultValue(const Hypothesis,
                                                  const std::string&,
                                                  const unsigned short) const;
    /*!
     * \return the default value of a parameter
     * \param[in] h: modelling hypothesis
     * \param[in] v: parameter default value
     */
    int getIntegerParameterDefaultValue(const Hypothesis,
                                        const std::string&) const;
    /*!
     * \return the default value of a parameter
     * \param[in] h: modelling hypothesis
     * \param[in] v: parameter default value
     */
    unsigned short getUnsignedShortParameterDefaultValue(
        const Hypothesis, const std::string&) const;
    /*!
     * associate a glossary name to a variable
     * \param[in] h: modelling Hypothesis
     * \param[in] n: variable name
     * \param[in] g: glossary name
     */
    void setGlossaryName(const Hypothesis,
                         const std::string&,
                         const std::string&);
    /*!
     * \return true if the given glossary name is used
     * \param[in] h: modelling Hypothesis
     * \param[in] n: name
     */
    bool isGlossaryNameUsed(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given variable name is associated with a
     * glossary name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool hasGlossaryName(const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given variable name is associated with a
     * entry name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool hasEntryName(const Hypothesis, const std::string&) const;
    /*!
     * associate an entry name to a variable
     * \param[in] h: modelling Hypothesis
     * \param[in] n: variable name
     * \param[in] e: entry name
     */
    void setEntryName(const Hypothesis, const std::string&, const std::string&);
    /*!
     * get the glossary name or the entry name of a variable
     * \param[in] h: modelling Hypothesis
     * \param[in] n: variable name
     */
    std::string getExternalName(const Hypothesis h, const std::string& n) const;
    /*!
     * \return the variable description associated with the given
     * glossary or entry name.
     * \param[in] h: modelling Hypothesis
     * \param[in] n: name
     */
    const VariableDescription& getVariableDescriptionByExternalName(
        const Hypothesis, const std::string&) const;
    /*!
     * \return the name of the variable associated with the given
     * glossary or entry name
     * \param[in] h: modelling Hypothesis
     * \param[in] n: name
     */
    std::string getVariableNameFromGlossaryNameOrEntryName(
        const Hypothesis, const std::string&) const;
    /*!
     * \return true if the given name is an entry name
     * \param[in] h: modelling Hypothesis
     * \param[in] n: name
     */
    bool isUsedAsEntryName(const Hypothesis, const std::string&) const;
    /*!
     * \brief add bounds to a variable
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     * \param[in] b: bounds description
     */
    void setBounds(const Hypothesis,
                   const std::string&,
                   const VariableBoundsDescription&);
    /*!
     * \brief add bounds to a variable
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     * \param[in] i: index (for array of variables)
     * \param[in] b: bounds description
     */
    void setBounds(const Hypothesis,
                   const std::string&,
                   const unsigned short,
                   const VariableBoundsDescription&);
    /*!
     * \brief add physical bounds to a variable
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     * \param[in] b: bounds description
     */
    void setPhysicalBounds(const Hypothesis,
                           const std::string&,
                           const VariableBoundsDescription&);
    /*!
     * \brief add physical bounds to a variable
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     * \param[in] i: index (for array of variables)
     * \param[in] b: bounds description
     */
    void setPhysicalBounds(const Hypothesis,
                           const std::string&,
                           const unsigned short,
                           const VariableBoundsDescription&);
    /*!
     * \brief declares an external state variable to be probably
     * unusable in a purely implicit resolution. This means that its
     * increment is used somewhere.
     *
     * The term "probably" refers to the fact that it may happens that
     * the increment is only used to get the value of the external
     * state variable at a given date. For example, in a standard
     * resolution (non implicit), the value of \(T+\theta\,\Delta\,T\)
     * gives the temperature value at
     * \(t+\theta\,\Delta\,t\). However, this expression is still
     * valid in a purely implicit resolution.
     *
     * \param[in] h: modelling hypothesis
     * \param[in] n: variable name
     */
    void declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
        const Hypothesis, const std::string&);
    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] h: modelling hypothesis
     * \param[in] b: new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void setUsableInPurelyImplicitResolution(const Hypothesis, const bool);
    /*!
     * \brief declare or update a code block
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     * \param[in] c: code
     * \param[in] m: mode
     * \param[in] p: position
     * \param[in] b: if true, the code can be overriden or completed
     */
    void setCode(const Hypothesis,
                 const std::string&,
                 const CodeBlock&,
                 const Mode,
                 const Position,
                 const bool = true);
    /*!
     * \brief return the name of the code blocks defined so far
     * \param[in] h: modelling hypothesis
     */
    std::vector<std::string> getCodeBlockNames(const Hypothesis) const;
    /*!
     * \return the code block with the given name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    const CodeBlock& getCodeBlock(const Hypothesis, const std::string&) const;
    /*!
     * \return the code with the given name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    std::string getCode(const Hypothesis, const std::string&) const;
    /*!
     * \return the code with the given name
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool hasCode(const Hypothesis, const std::string&) const;
    /*!
     * \brief set a mechanical attribute
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     * \param[in] a: attribute
     * \param[in] b: don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const Hypothesis,
                      const std::string&,
                      const BehaviourAttribute&,
                      const bool = false);
    /*!
     * \brief update an attribute
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const Hypothesis,
                         const std::string&,
                         const BehaviourAttribute&);
    /*!
     * \return a mechanical attribute
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    template <typename T>
    const T& getAttribute(const Hypothesis, const std::string&) const;
    /*!
     * \return a mechanical attribute if it exists or the default
     * value
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     * \param[in] v: default value
     */
    template <typename T>
    T getAttribute(const Hypothesis, const std::string&, const T&) const;
    /*!
     * \return true an attribute with the given name has been declared
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool hasAttribute(const Hypothesis, const std::string&) const;
    /*!
     * \return true a parameter with the given name has been declared
     * \param[in] h: modelling hypothesis
     * \param[in] n: name
     */
    bool hasParameter(const Hypothesis, const std::string&) const;
    /*!
     * \return true the given modelling hypothesis has a
     * parameter.
     * \param[in] h: modelling hypothesis \param[in] n:
     * name
     */
    bool hasParameters(const Hypothesis) const;
    /*!
     * \return true if at least one modelling hypothesis has a
     * parameter.
     */
    bool hasParameters() const;
    /*!
     * \brief insert a new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     * \param[in] b: don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const BehaviourAttribute&,
                      const bool);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n: name
     */
    bool hasAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n: name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        T>::type
    getAttribute(const std::string&, const T&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n: name
     */
    const std::map<std::string, BehaviourAttribute>& getAttributes() const;
    /*!
     * reserve the given name
     * \param[in] h: hypothesis
     * \param[in] n: variable name
     */
    void reserveName(const Hypothesis, const std::string&);
    /*!
     * \brief look if a name is reserved
     * \param[in] n: name
     */
    bool isNameReserved(const std::string&) const;
    /*!
     * register the given member name
     * \param[in] h: hypothesis
     * \param[in] n: variable name
     */
    void registerMemberName(const Hypothesis, const std::string&);
    /*!
     * register the given static member name
     * \param[in] h: hypothesis
     * \param[in] n: variable name
     */
    void registerStaticMemberName(const Hypothesis, const std::string&);
    /*!
     * \brief check variable existence
     * \return a pair of iterator. The first part tells if the
     * variable was found in a least one specialisation of the
     * behaviour and the second tells if the variable is available
     * for all distinct modelling hypothesis.
     * The variable could be any sort of variable (local variable,
     * integration variable, static variable, ...)
     * \param[in] v : variable name
     */
    std::pair<bool, bool> checkVariableExistence(const std::string&) const;
    /*!
     * \brief check variable existence for a particular sort of variable.
     * \return a pair of iterator. The first part tells if the
     * variable was found in a least one specialisation of the
     * behaviour and the second tells if the variable is available
     * for all distinct modelling hypothesis.
     * \param[in] v: variable name
     * \param[in] c: variable category
     * \param[in] b: if true, an exception if thrown if the variable is not
     * found
     */
    std::pair<bool, bool> checkVariableExistence(const std::string&,
                                                 const std::string&,
                                                 const bool = true) const;
    /*!
     * \brief check variable glossary name.
     * The variable must exists for all modelling hypothesis and have
     * the given glossary name. If not, an exception is thrown. If
     * another variable has the given glossary name, an exception if
     * thrown.
     * \param[in] v: variable name
     * \param[in] g: glossary name
     */
    void checkVariableGlossaryName(const std::string&,
                                   const std::string&) const;
    /*!
     * \brief check variable entry name.
     * The variable must exists for all modelling hypothesis and have
     * the given entry name. If not, an exception is thrown. If
     * another variable has the given entry name, an exception if
     * thrown.
     * \param[in] v: variable name
     * \param[in] g: entry name
     */
    void checkVariableEntryName(const std::string&, const std::string&) const;
    /*!
     * \brief check a variable position. Throw an exception if this
     * check is not satisfied.
     * \param[in] v: variable name
     * \param[in] c: variable category
     * \param[in] p: variable position
     * \pre The number of variables of the given type must be greater
     * than the position given, otherwise an exception is thrown
     */
    void checkVariablePosition(const std::string&,
                               const std::string&,
                               const size_t);
    //! destructor
    ~BehaviourDescription();

   private:
    //! a simple alias (std::optional is not yet available)
    template <typename T>
    using optional = tfel::utilities::GenType<T>;
    /*!
     * \brief throw an exception saying that no attribute with the
     * given name exists
     */
    TFEL_NORETURN static void throwUndefinedAttribute(const std::string&);
    /*!
     * \return the driving variable with the associated name
     * \param[in] n: name
     */
    Gradient& getGradient(const std::string&);
    /*!
     * \return the thermodynamic force with the associated name
     * \param[in] n: name
     */
    ThermodynamicForce& getThermodynamicForce(const std::string&);
    //! update the class name
    void updateClassName();
    /*!
     * \brief create the mechanical behaviour data associated with the
     * given modelling hypothesis if necessary, and returns it.
     * \param[in] h: modelling hypothesis
     */
    BehaviourData& getBehaviourData2(const ModellingHypothesis::Hypothesis&);
    /*!
     * \call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a: argument given to the behaviour data's method
     */
    template <typename Res, typename Arg1>
    Res getData(const Hypothesis,
                Res (BehaviourData::*)(const Arg1&) const,
                const Arg1&) const;
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a: argument given to the behaviour data's method
     * \param[in] b: propagate to all hypothesis if h is UNDEFINEDHYPOTHESIS
     * \note if h is UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called, unless the last parameter is
     * false.
     */
    template <typename Arg1>
    void callBehaviourData(const Hypothesis,
                           void (BehaviourData::*)(const Arg1&),
                           const Arg1&,
                           const bool);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a1: first  argument given to the behaviour data's method
     * \param[in] a2: second argument given to the behaviour data's method
     * \param[in] b: propagate to all hypothesis if h is UNDEFINEDHYPOTHESIS
     * \note if h is UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called, unless the last parameter is
     * false.
     */
    template <typename Arg1, typename Arg2>
    void callBehaviourData(const Hypothesis,
                           void (BehaviourData::*)(const Arg1&, const Arg2),
                           const Arg1&,
                           const Arg2,
                           const bool);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a1: first  argument given to the behaviour data's method
     * \param[in] a2: second argument given to the behaviour data's method
     * \param[in] b: propagate to all hypothesis if h is UNDEFINEDHYPOTHESIS
     * \note if h is UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called, unless the last parameter is
     * false.
     */
    template <typename Arg1, typename Arg2, typename Arg3>
    void callBehaviourData(const Hypothesis,
                           void (BehaviourData::*)(const Arg1&,
                                                   const Arg2&,
                                                   const Arg3&),
                           const Arg1&,
                           const Arg2&,
                           const Arg3&,
                           const bool);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a1: first  argument given to the behaviour data's method
     * \param[in] a2: second argument given to the behaviour data's method
     * \param[in] b: propagate to all hypothesis if h is UNDEFINEDHYPOTHESIS
     * \note if h is UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called, unless the last parameter is
     * false.
     */
    template <typename Arg1, typename Arg2>
    void callBehaviourData(const Hypothesis,
                           void (BehaviourData::*)(const Arg1&, const Arg2&),
                           const Arg1&,
                           const Arg2&,
                           const bool);
    /*!
     * \brief call the behaviour data associated with the given hypothesis
     * \param[in] h: modelling hypothesis
     * \param[in] m: behaviour data method
     * \param[in] a: argument given to the behaviour data's method
     * \param[in] b: propagate to all hypothesis if h is UNDEFINEDHYPOTHESIS
     * \note if h is UNDEFINEDHYPOTHESIS, the default data
     * and all the specialisations are called, unless the last parameter is
     * false.
     */
    template <typename Arg1>
    void callBehaviourData(const Hypothesis,
                           void (BehaviourData::*)(const Arg1),
                           const Arg1,
                           const bool);
    /*!
     * \brief add variables to the behaviour data
     * \param[in] h: modelling hypothesis
     * \param[in] v: variables to be added
     * \param[in] s: registration status
     * \param[in] m: method retrieving the variable container
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void addVariables(
        const Hypothesis,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus,
        void (BehaviourData::*)(const VariableDescription&,
                                const BehaviourData::RegistrationStatus));
    /*!
     * \brief add a variable to the behaviour data
     * \param[in] h: modelling hypothesis
     * \param[in] v: variable to be added
     * \param[in] s: registration status
     * \param[in] m: method retrieving the variable container
     *
     * \note if h is UNDEFINEDHYPOTHESIS, add the material properties
     * to the default data and to all the specialisations
     */
    void addVariable(
        const Hypothesis,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus,
        void (BehaviourData::*)(const VariableDescription&,
                                const BehaviourData::RegistrationStatus));
    /*!
     * \brief add variables to the behaviour data
     * \param[out] b: behaviour data
     * \param[in]  v: variables to be added
     * \param[in]  s: registration status
     * \param[in]  m: method retrieving the variable container
     */
    void addVariables(
        BehaviourData&,
        const VariableDescriptionContainer&,
        const BehaviourData::RegistrationStatus,
        void (BehaviourData::*)(const VariableDescription&,
                                const BehaviourData::RegistrationStatus));
    /*!
     * \brief add a variable to the behaviour data
     * \param[out] b: behaviour data
     * \param[in]  v: variable to be added
     * \param[in]  s: registration status
     * \param[in]  m: method retrieving the variable container
     */
    void addVariable(
        BehaviourData&,
        const VariableDescription&,
        const BehaviourData::RegistrationStatus,
        void (BehaviourData::*)(const VariableDescription&,
                                const BehaviourData::RegistrationStatus));
    /*!
     * \return the category associated with the given variable
     * \param[in] h: modelling hypothesis
     * \param[in] v: variable name
     */
    MaterialPropertyInput::Category getMaterialPropertyInputCategory(
        const Hypothesis, const std::string&) const;
    /*!
     * \brief check that the given hypothesis is supported
     * \param[in] h: modelling hypothesis
     */
    void checkModellingHypothesis(const Hypothesis&) const;
    //! a simple alias
    typedef std::shared_ptr<BehaviourData> MBDPtr;
    //! behaviour attributes
    std::map<std::string, BehaviourAttribute> attributes;
    //! behaviour name
    std::string behaviour;
    //! dsl name
    std::string dsl;
    //! library name
    std::string library;
    //! material name
    std::string material;
    //! name of the generated class
    std::string className;
    //! included header files
    std::string includes;
    //! specific sources
    std::string sources;
    /*!
     * list of modelling hypotheses for
     * which the behaviour is defined
     */
    mutable std::set<Hypothesis> hypotheses;
    /*!
     * list of modelling hypotheses for
     * which this class returned a mechanical data
     */
    mutable std::set<Hypothesis> requestedHypotheses;
    //! default behaviour data
    BehaviourData d;
    //! specialisations
    std::map<Hypothesis, MBDPtr> sd;
    /*!
     * main variables, association of a driving variable and a
     * thermodynamicforce
     */
    std::vector<std::pair<Gradient, ThermodynamicForce>> mvariables;
    /*!
     * elastic material properties
     * For isotropic   behaviours, only two elastic material properties must be
     * defined.
     * For orthotropic behaviours, two or nine elastic material properties must
     * be defined.
     */
    std::vector<MaterialProperty> elasticMaterialProperties;
    /*!
     * average thermal coefficient
     * For isotropic   behaviours, only one thermal expansion coefficient must
     * be defined.
     * For orthotropic behaviours, one or three thermal expansions coefficients
     * must be defined.
     */
    std::vector<MaterialProperty> thermalExpansionCoefficients;
    /*!
     * elastic material properties
     * For isotropic   behaviours, only two elastic material properties must be
     * defined.
     * For orthotropic behaviours, two or nine elastic material properties must
     * be defined.
     */
    std::vector<ModelDescription> models;
    //! slip systems
    optional<SlipSystemsDescription> gs;
    //! \brief list of all Hill tensors defined
    std::vector<HillTensor> hillTensors;
    //! strain measure
    optional<StrainMeasure> strainMeasure;
    //! use units
    bool use_qt = false;
    //! type of behaviour
    BehaviourType type = GENERALBEHAVIOUR;
    //! symmetry of behaviour (isotropic or orthotropic)
    mutable BehaviourSymmetryType stype = mfront::ISOTROPIC;
    //! orthotropic axes convention
    OrthotropicAxesConvention oac = OrthotropicAxesConvention::DEFAULT;
    //! flag telling if the orthotropic axes convention has been defined
    mutable bool oacIsDefined = false;
    //! flag telling the behaviour symmetry has been defined
    mutable bool stypeIsDefined = false;
    //! symmetry of elastic behaviour (isotropic or orthotropic)
    mutable BehaviourSymmetryType estype = mfront::ISOTROPIC;
    //! flag telling the elastic symmetry has been defined
    mutable bool estypeIsDefined = false;
    //! integration schemes
    IntegrationScheme ischeme = UNDEFINEDINTEGRATIONSCHEME;
    //! list of material laws used
    std::vector<std::string> materialLaws;
    /*!
     * Support for dynamically allocated vectors is not allowed in all
     * dsl's. A dsl may change this value to disable the use of
     * dynamically allocated vectors.
     */
    tfel::utilities::GenType<bool> areDynamicallyAllocatedVectorsAllowed_;
  };  // end of struct BehaviourDescription

  /*!
   * \brief set the elastic symmetry of a material if not already
   * defined. If already defined, check that the symmetry is the same
   * as the one given in argument
   * \param[in,out] bd: behaviour description
   * \param[in]     s:  symmetry type
   */
  MFRONT_VISIBILITY_EXPORT void setElasticSymmetryType(
      BehaviourDescription&, const BehaviourSymmetryType);
  /*!
   * \brief check that the given material property description is strictly
   * positive, when described by a constant material property
   * \param[in] mp: material property description
   */
  MFRONT_VISIBILITY_EXPORT void checkIsStrictlyPositive(
      const BehaviourDescription::MaterialProperty&);
  /*!
   * \brief check that the given material property description is strictly
   * negative,when described by a constant material property
   * \param[in] mp: material property description
   */
  MFRONT_VISIBILITY_EXPORT void checkIsStrictlyNegative(
      const BehaviourDescription::MaterialProperty&);

}  // end of namespace mfront

#include "MFront/BehaviourDescription.ixx"

#endif /* LIB_MFRONT_BEHAVIOURDESCRIPTION_HXX */
