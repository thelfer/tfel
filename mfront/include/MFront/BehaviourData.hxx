/*!
 * \file   mfront/include/MFront/BehaviourData.hxx
 * \brief  This file declares the BehaviourData class
 * \author Thomas Helfer
 * \date   21 Mars 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURDATA_HXX
#define LIB_MFRONT_BEHAVIOURDATA_HXX

#include <set>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "MFront/MFrontConfig.hxx"
#include "MFront/CodeBlock.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/Gradient.hxx"
#include "MFront/ThermodynamicForce.hxx"
#include "MFront/SupportedTypes.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/StaticVariableDescription.hxx"
#include "MFront/MaterialKnowledgeDescription.hxx"

namespace mfront {

  // forward declaration
  struct ModelDescription;

  /*!
   * This structure gathers various behaviour characteristic
   * for a given modelling hypothesis
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourData
      : public MaterialKnowledgeDescription,
        private SupportedTypes {
    /*
     * normalised code block names
     * \note code block name begins with an upper case
     */
    //! \brief standard code name
    static const char* const UserDefinedInitializeCodeBlock;
    //! \brief standard code name
    static const char* const FlowRule;
    //! \brief standard code name
    static const char* const BeforeInitializeLocalVariables;
    //! \brief standard code name
    static const char* const InitializeLocalVariables;
    //! \brief standard code name
    static const char* const AfterInitializeLocalVariables;
    //! \brief standard code name
    static const char* const ComputePredictor;
    //! \brief standard code name
    static const char* const ComputeThermodynamicForces;
    //! \brief standard code name
    /* Most behaviours will only rely the @ComputeThermodynamicForces keyword to
     * estimate thermodynamic forces at the middle of the time step and at the
     * end.  However, some finite strains behaviours must compute a
     * stress measure during the iterations which is not the Cauchy
     * stress. Thus, `@ComputeThermodynamicForces` also defines an
     * ComputeFinalThermodynamicForceCandidate code block which will be used if
     * the user does not provide an alternative through the
     * `@ComputeFinalThermodynamicForces`
     */
    static const char* const ComputeFinalThermodynamicForcesCandidate;
    //! \brief standard code name
    static const char* const ComputeFinalThermodynamicForces;
    //! \brief standard code name
    static const char* const DefaultComputeFinalThermodynamicForce;
    //! \brief standard code name
    static const char* const APrioriTimeStepScalingFactor;
    //! \brief standard code name
    static const char* const Integrator;
    //! \brief standard code name
    static const char* const APosterioriTimeStepScalingFactor;
    //! \brief standard code name
    static const char* const ComputeDerivative;
    //! \brief standard code name
    static const char* const ComputePredictionOperator;
    //! \brief standard code name
    static const char* const ComputeTangentOperator;
    //! \brief standard code name
    static const char* const ComputeStressFreeExpansion;
    //! \brief standard code name
    static const char* const UpdateAuxiliaryStateVariables;
    //! \brief standard code name
    static const char* const InitializeJacobian;
    //! \brief standard code name
    static const char* const InitializeJacobianInvert;
    //! \brief standard code name
    static const char* const ComputeInternalEnergy;
    //! \brief standard code name
    static const char* const ComputeDissipatedEnergy;
    //! \brief standard code name
    static const char* const ComputeSpeedOfSound;
    //! \brief standard code name
    static const char* const AdditionalConvergenceChecks;
    //! \brief standard code name
    static const char* const PostProcessings;
    //! \brief standard code name
    static const char* const ProcessNewCorrection;
    //! \brief standard code name
    static const char* const RejectCurrentCorrection;
    //! \brief standard code name
    static const char* const ProcessNewEstimate;
    /*
     * normalised attribute names
     * \note properties name begins with a lower case
     */
    /*!
     * \brief if this attribute is true, the behaviour can compute a tangent
     * operator.
     */
    static const char* const hasConsistentTangentOperator;
    /*!
     * \brief if this attribute is true, the tangent operator can be
     * considered symmetric.
     */
    static const char* const isConsistentTangentOperatorSymmetric;
    /*!
     * \brief if this attribute is true, the behaviour can compute an a
     * priori estimate of the time step scaling factor after the
     * initialisation
     */
    static const char* const hasAPrioriTimeStepScalingFactor;
    //! if this attribute is true, the behaviour can compute an a
    //! posteriori estimate of the time step scaling factor after the
    //! integration
    static const char* const hasAPosterioriTimeStepScalingFactor;
    //! if this attribute is true, the behaviour can compute a
    //! prediction operator
    static const char* const hasPredictionOperator;
    //! if this attribute is true, the implicit algorithm compares the
    //! analytical jacobian to the numeric one
    static const char* const compareToNumericalJacobian;
    //! list of jacobian blocks that must be computed numerically
    static const char* const numericallyComputedJacobianBlocks;
    /*!
     * a boolean attribute telling if the additionnal variables can be
     * declared. This attribute is set by DSL's when the first code
     * block has been read.
     *
     * \note This attribute applies to all variables except for local
     * variables.
     */
    static const char* const allowsNewUserDefinedVariables;
    //! \brief a boolean attribute telling if profiling information shall be
    //! collected
    static const char* const profiling;
    //! \brief algorithm used
    static const char* const algorithm;
    //! \brief number of evaluation of the computeDerivative method
    //  of a Runge-Kutta algorithm
    static const char* const numberOfEvaluations;
    /*!
     * The description of a stress free expansion defined through an
     * external state variable
     * \note SFED_ESV: Stress Free Expansion Description _ External
     * State Variable
     */
    struct SFED_ESV {
      //! \brief variable name
      std::string vname;
    };  // end of SFED_ESV
    //! \brief a placeholder for null expansion
    struct NullExpansion {};
    /*!
     * a stress free expansion can be computed using by:
     * - an external state variable
     * - a  model
     */
    using StressFreeExpansionHandlerTypes =
        typename tfel::meta::GenerateTypeList<
            SFED_ESV,
            NullExpansion,
            std::shared_ptr<ModelDescription>>::type;
    /*!
     * a stress free expansion handler
     */
    using StressFreeExpansionHandler =
        tfel::utilities::GenTypeBase<StressFreeExpansionHandlerTypes>;
    /*!
     * \brief a simple wrapper around an isotropic swelling model
     * given by a volume change
     * \f[
     * s = \left(dV_V0/3,dV_V0/3,dV_V0/3\,0\,0\,0\right)
     * \f]
     */
    struct VolumeSwellingStressFreeExpansion {
      //! \brief o volumetric stress free expansion
      StressFreeExpansionHandler sfe;
    };
    /*!
     * \brief a simple wrapper around a model describing an axial
     * growth.
     * \f[
     * s = \left(0,0,dlz_lz\,0\,0\,0\right)
     * \f]
     */
    struct AxialGrowth {
      //! \brief description of the axial growth
      StressFreeExpansionHandler sfe;
    };
    /*!
     * \brief a simple wrapper around a model describing fuel
     * relocation.
     * \f[
     * s = \left(0,0,dlz_lz\,0\,0\,0\right)
     * \f]
     */
    struct Relocation {
      //! \brief description of the axial growth
      StressFreeExpansionHandler sfe;
    };
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an isotropic swelling given by a linear change in each
     * material geometry:
     * \f[
     * s = \left(dl_l,dl_l,dl_l\,0\,0\,0\right)
     * \f]
     */
    struct IsotropicStressFreeExpansion {
      //! \brief stress free expansion in each direction
      StressFreeExpansionHandler sfe;
    };
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an orthotropic swelling
     */
    struct OrthotropicStressFreeExpansion {
      //! \brief stress free expansion in the first material direction
      StressFreeExpansionHandler sfe0;
      //! \brief stress free expansion in the second material direction
      StressFreeExpansionHandler sfe1;
      //! \brief stress free expansion in the third material direction
      StressFreeExpansionHandler sfe2;
    };
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an orthotropic swelling described by an array of external
     * state variables
     */
    struct OrthotropicStressFreeExpansionII {
      //! \brief stress free expansions
      SFED_ESV esv;
    };
    /*!
     * \brief list of a stress free expansion descriptions
     */
    using StressFreeExpansionDescriptionTypes =
        typename tfel::meta::GenerateTypeList<
            VolumeSwellingStressFreeExpansion,
            AxialGrowth,
            Relocation,
            IsotropicStressFreeExpansion,
            OrthotropicStressFreeExpansion,
            OrthotropicStressFreeExpansionII>::type;
    //! \brief description of a stress free expansion
    using StressFreeExpansionDescription =
        tfel::utilities::GenTypeBase<StressFreeExpansionDescriptionTypes>;
    /*!
     * mode used when inserting code
     */
    enum Mode {
      CREATE,              /*!< create a code block. Return an error
                             if if fails */
      CREATEORREPLACE,     /*!< create a code block or replace
                            *   it if it already exists. If the
                            *   code is to be replaced, all
                            *   positions are cleard. This applies
                            *   to the whole code block and not only
                            *   to specific positions */
      CREATEORAPPEND,      /*!< create a code block or append
                            *   a code block to an existing one */
      CREATEBUTDONTREPLACE /*!< create a code block but don't replace
                            *   it if it already exists. This applies
                            *   to the whole code block and not only
                            *   to specific positions. This option
                            *   is meant to be used internally and
                            *   is not accessible from a mfront file*/
    };                     // end of enum Mode
    /*!
     * \brief position of the inserted code
     */
    enum Position { AT_BEGINNING, BODY, AT_END };  // end of enum Mode
                                                   /*!
                                                    * Some dsl may want to avoid the user to declare a variable with
                                                    * a given name. To avoid this, they shall reserve the name. Once
                                                    * the variable is finally declare, they shall pass the
                                                    * ALREADYREGISTRED flag to the addXXXVariables methods
                                                    */
    enum RegistrationStatus {
      UNREGISTRED,
      FORCEREGISTRATION,
      ALREADYREGISTRED
    };
    //! \brief constructor
    BehaviourData();
    //! \brief copy constructor
    explicit BehaviourData(const BehaviourData&);
    //! \brief move constructor (disabled)
    BehaviourData(BehaviourData&&) = delete;
    //! \brief assignement operator (disabled)
    BehaviourData& operator=(const BehaviourData&) = delete;
    //! \brief move assignement operator (disabled)
    BehaviourData& operator=(BehaviourData&&) = delete;
    /*!
     * \brief specify that the given variable must be overriden by a parameter.
     * This must be called before the declaration of the variable.
     *
     * \param[in] n: variable name
     * \param[in] v: variable value
     */
    void overrideByAParameter(const std::string&, const double);
    //! \return the list of overriden parameters
    std::map<std::string, double> getOverridenParameters() const;
    /*!
     * \brief check and complete the physical bounds of variables
     * \param[in] s: unit system
     */
    void checkAndCompletePhysicalBoundsDeclaration(const std::string_view);
    /*!
     * This method has been introduced to optimize the mechanical
     * resolution in licos : a purely implicit resolution only
     * evaluate external loadings at the end of the time step. This
     * implies that no increment of an external loading is used in the
     * behaviour law.
     */
    bool isUsableInPurelyImplicitResolution() const;
    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] b : new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void setUsableInPurelyImplicitResolution(const bool);
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
     */
    void declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(
        const std::string&);
    /*!
     * \brief get all the external state variables found to be
     * probably unusable in a purely implicit resolution.
     */
    const std::set<std::string>&
    getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution()
        const;
    /*!
     * \return the variables of the specificed type
     * \param[in] t: type of the variable
     * Currently `t` must have one of the following values:
     * - `MaterialProperties`
     * - `PersistentVariables`
     * - `IntegrationVariables`
     * - `StateVariables`
     * - `AuxiliaryStateVariables`
     * - `InitializeFunctionVariables`
     * - `PostProcessingVariables`
     * - `ExternalStateVariables`
     * - `Parameters`
     */
    const VariableDescriptionContainer& getVariables(const std::string&) const;
    //! \return all material properties
    const VariableDescriptionContainer& getMaterialProperties() const;
    //! \return all persistent variables
    const VariableDescriptionContainer& getPersistentVariables() const;
    //! \return all integration variables
    const VariableDescriptionContainer& getIntegrationVariables() const;
    //! \return all state variables
    const VariableDescriptionContainer& getStateVariables() const;
    //! \return all auxiliary state variables
    const VariableDescriptionContainer& getAuxiliaryStateVariables() const;
    //! \return all initialize variables
    const VariableDescriptionContainer& getInitializeFunctionVariables() const;
    //! \return all post-processing variables
    const VariableDescriptionContainer& getPostProcessingVariables() const;
    //! \return all external state variables
    const VariableDescriptionContainer& getExternalStateVariables() const;
    //! \return all local variables
    const VariableDescriptionContainer& getLocalVariables() const;
    //! \return all parameter variables
    const VariableDescriptionContainer& getParameters() const;
    /*!
     * \return a variable description with the given name.
     * \param[in] n: name
     */
    const VariableDescription& getPersistentVariableDescription(
        const std::string&) const;
    /*!
     * \return a variable description associated with the given
     * external name.
     * \param[in] n: name
     */
    const VariableDescription& getPersistentVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \return a variable description with the given name.
     * \param[in] n: name
     */
    const VariableDescription& getIntegrationVariableDescription(
        const std::string&) const;
    /*!
     * \return a variable description associated with the given
     * external name.
     * \param[in] n: name
     */
    const VariableDescription& getIntegrationVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \return the state variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getStateVariableDescription(
        const std::string&) const;
    /*!
     * \return the state variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getStateVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \return the auxiliary state variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getAuxiliaryStateVariableDescription(
        const std::string&) const;
    /*!
     * \return the initialize variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getInitializeFunctionVariableDescription(
        const std::string&) const;
    /*!
     * \return the post-processing variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getPostProcessingVariableDescription(
        const std::string&) const;
    /*!
     * \param[in] n: name
     */
    const VariableDescription& getExternalStateVariableDescription(
        const std::string&) const;
    /*!
     * \param[in] n: external name
     */
    const VariableDescription&
    getExternalStateVariableDescriptionByExternalName(const std::string&) const;
    /*!
     * \param[in] n: name
     */
    const VariableDescription& getParameterDescription(
        const std::string&) const;
    /*!
     * \param[in] n: external name
     */
    const VariableDescription& getParameterDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \param[in] n: external name
     */
    const VariableDescription&
    getAuxiliaryStateVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \param[in] n: external name
     */
    const VariableDescription&
    getInitializeFunctionVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \param[in] n: external name
     */
    const VariableDescription&
    getPostProcessingVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \return a variable description with the given name.
     * \param[in] n: external name.
     */
    const VariableDescription& getVariableDescription(const std::string&) const;
    /*!
     * \return a variable description associated with the given
     * external name.
     * \param[in] n: external name.
     */
    const VariableDescription& getVariableDescriptionByExternalName(
        const std::string&) const;
    /*!
     * \brief assign an attribute to variable
     * \param[in] v: variable name
     * \param[in] n: attribute name
     * \param[in] a: attribute
     * \param[in] b: don't throw if the attribute already exists.  In
     * this case, the attribute is left unchanged. However the type of
     * the attribute is checked.
     */
    void setVariableAttribute(const std::string&,
                              const std::string&,
                              const VariableAttribute&,
                              const bool);
    /*!
     * \return true if a least one modelling hypothesis is anisotropic
     */
    bool isStressFreeExansionAnisotropic() const;
    /*!
     * \return the registred stress free expansion descriptions
     */
    const std::vector<StressFreeExpansionDescription>&
    getStressFreeExpansionDescriptions() const;
    /*!
     * \return true if the given member is used in a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool isMemberUsedInCodeBlocks(const std::string&) const;
    /*!
     * \return if the given name is the name of a material property
     * \param[in] n: name
     */
    bool isMaterialPropertyName(const std::string&) const;
    /*!
     * \return if the given name is the name of a local variable
     * \param[in] n: name
     */
    bool isLocalVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of a persistent variable
     * \param[in] n: name
     */
    bool isPersistentVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of an integration variable
     * \param[in] n: name
     */
    bool isIntegrationVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of the increment of an integration
     * variable
     * \param[in] n: name
     */
    bool isIntegrationVariableIncrementName(const std::string&) const;
    /*!
     * \return if the given name is the name of a state variable
     * \param[in] n: name
     */
    bool isStateVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of the increment of a state
     * variable
     * \param[in] n: name
     */
    bool isStateVariableIncrementName(const std::string&) const;
    /*!
     * \return if the given name is the name of an auxiliary state variable
     * \param[in] n: name
     */
    bool isAuxiliaryStateVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of an initialize variable
     * \param[in] n: name
     */
    bool isInitializeFunctionVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of a post-processing variable
     * \param[in] n: name
     */
    bool isPostProcessingVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of an external state variable
     * \param[in] n: name
     */
    bool isExternalStateVariableName(const std::string&) const;
    /*!
     * \return if the given name is the name of the increment of an external
     * state variable
     * \param[in] n: name
     */
    bool isExternalStateVariableIncrementName(const std::string&) const;
    /*!
     * \return if the given name is the name of a parameter
     * \param[in] n: name
     */
    bool isParameterName(const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] n : name
     */
    bool isStaticVariableName(const std::string& n) const;

    //! \brief \return the static variables defined
    const StaticVariableDescriptionContainer& getStaticVariables() const;
    /*!
     * \brief add a material property
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addMaterialProperty(const VariableDescription&,
                             const RegistrationStatus);
    /*!
     * \brief add a integration variable
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addIntegrationVariable(const VariableDescription&,
                                const RegistrationStatus);
    /*!
     * \brief add a state variable
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addStateVariable(const VariableDescription&, const RegistrationStatus);
    /*!
     * \brief add an auxiliary state variable
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addAuxiliaryStateVariable(const VariableDescription&,
                                   const RegistrationStatus);
    /*!
     * \brief add an initialize variable
     * \param[in] v : variable description
     */
    void addInitializeFunctionVariable(const VariableDescription&);
    /*!
     * \brief add a post-processing variable
     * \param[in] v : variable description
     */
    void addPostProcessingVariable(const VariableDescription&);
    /*!
     * \brief add an external state variable
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addExternalStateVariable(const VariableDescription&,
                                  const RegistrationStatus);
    /*!
     * \brief add a : variable
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addLocalVariable(const VariableDescription&, const RegistrationStatus);
    /*!
     * \brief add a parameter
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addParameter(const VariableDescription&, const RegistrationStatus);
    /*!
     * \return true if a parameter with the given name exists
     * \param[in] n : name
     */
    bool hasParameter(const std::string&) const;
    /*!
     * \return true if at least one parameter is defined
     */
    bool hasParameters() const;
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void setParameterDefaultValue(const std::string&, const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] i : index
     * \param[in] v : parameter default value
     */
    void setParameterDefaultValue(const std::string&,
                                  const unsigned short i,
                                  const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void setParameterDefaultValue(const std::string&, const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void setParameterDefaultValue(const std::string&, const unsigned short);
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    double getFloattingPointParameterDefaultValue(const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] v: parameter default value
     * \param[in] i: index
     */
    double getFloattingPointParameterDefaultValue(const std::string&,
                                                  const unsigned short) const;
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    int getIntegerParameterDefaultValue(const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    unsigned short getUnsignedShortParameterDefaultValue(
        const std::string&) const;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     * \param[in] s : registration status
     */
    void addStaticVariable(const StaticVariableDescription&,
                           const RegistrationStatus);
    //! \return all the integer constants
    virtual std::map<std::string, int> getIntegerConstants() const;
    /*!
     * \return the value of an integer constant
     * \param[in] n: variable name
     */
    int getIntegerConstant(const std::string&) const;
    /*!
     * \brief add a new stress free expansion description
     * \param[in] sfed: stress free expansion description
     */
    void addStressFreeExpansion(const StressFreeExpansionDescription&);
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToMembers(const std::string&);
    //! \return the material additional members
    std::string getMembers() const;
    /*!
     * \brief add a new initialize function
     * \param[in] n: name of the initialize function
     * \param[in] c: code block associated with the initialize function
     */
    void addInitializeFunction(const std::string&, const CodeBlock&);
    //! \return the registred initialize functions
    const std::map<std::string, CodeBlock>& getInitializeFunctions() const;
    /*!
     * \brief add a new post-processing
     * \param[in] n: name of the post-processing
     * \param[in] c: code block associated with the post-processing
     */
    void addPostProcessing(const std::string&, const CodeBlock&);
    //! \return the registred post-processings
    const std::map<std::string, CodeBlock>& getPostProcessings() const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToPrivateCode(const std::string&);
    //! \brief \return the material private code
    std::string getPrivateCode() const;
    /*!
     * \brief return the name of the code blocks defined so far
     * \param[in] h : modelling hypothesis
     */
    std::vector<std::string> getCodeBlockNames() const;
    /*!
     * \brief declare or update a code block
     * \param[in] n : name
     * \param[in] c : code
     * \param[in] m : mode
     * \param[in] p : position
     * \param[in] b : if true, the code can be overriden or completed
     */
    void setCode(const std::string&,
                 const CodeBlock&,
                 const Mode,
                 const Position,
                 const bool);
    /*!
     * \return the code block associated with the given name
     * \param[in] n : name
     */
    const CodeBlock& getCodeBlock(const std::string&) const;
    /*!
     * \return the code block associated with the given name
     * \param[in] n  : name
     * \param[in] cn : behaviour class name
     * \param[in] b  : add profiling information
     */
    std::string getCode(const std::string&,
                        const std::string&,
                        const bool) const;
    /*!
     * \return true if a code block associated with the given name has
     * been defined
     * \param[in] n : name
     */
    bool hasCode(const std::string&) const;
    /*!
     * \return true a glossary was associated with the given
     * variable.
     * \param[in] v  : variable name
     */
    bool hasGlossaryName(const std::string&) const;
    /*!
     * \return true if an entry name was associated with the given
     * variable.
     * \param[in] v  : variable name
     */
    bool hasEntryName(const std::string&) const;
    /*!
     * \return the glossary or the entry name associated with the
     * given variable. If none was defined, return the variable name.
     * \param[in] v : variable name
     */
    std::string getExternalName(const std::string&) const;
    /*!
     * \return the external names associated with the
     * variables contained in the given container.
     * \see getExternalName
     * \param[in] v  : variables for which glossary names are requested
     */
    std::vector<std::string> getExternalNames(
        const VariableDescriptionContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container.
     * \see getExternalName
     * \param[out] n : names
     * \param[in] v  : variables for which glossary names are requested
     */
    void getExternalNames(std::vector<std::string>&,
                          const VariableDescriptionContainer&) const;
    /*!
     * get the external names associated with the variables
     * contained in the given container.
     * \see getExternalName
     * \param[out] n: names
     * \param[in]  v: variables for which glossary names are requested
     */
    void appendExternalNames(std::vector<std::string>&,
                             const VariableDescriptionContainer&) const;
    /*!
     * \brief associate a glossary name to a variable
     * \param[in] n: variable name
     * \param[in] g: glossary name
     */
    void setGlossaryName(const std::string&, const std::string&);
    /*!
     * Look if the given glossary name is used.
     * \note we explicitely check that the given name is part of the
     * TFEL glossary.
     * \param[in] n : glossary name
     */
    bool isGlossaryNameUsed(const std::string&) const;
    /*!
     * \brief associate an entry name to a variable
     * \param[in] n: variable name
     * \param[in] e: entry name
     */
    void setEntryName(const std::string&, const std::string&);
    /*!
     * look if the given name is used as an entry name
     * \param[in] n : name
     */
    bool isUsedAsEntryName(const std::string&) const;
    /*!
     * \return the name of the variable associated with the given
     * glossary or entry name
     * \param[in] n : name
     */
    std::string getVariableNameFromGlossaryNameOrEntryName(
        const std::string&) const;
    /*!
     * \brief add bounds to a variable
     * \param[in] n: variable name
     * \param[in] b: bounds description
     */
    void setBounds(const std::string&, const VariableBoundsDescription&);
    /*!
     * \brief add bounds to a variable
     * \param[in] n: variable name
     * \param[in] i: index (for array of variables)
     * \param[in] b: bounds description
     */
    void setBounds(const std::string&,
                   const unsigned short,
                   const VariableBoundsDescription&);
    /*!
     * \brief add physical bounds to a variable
     * \param[in] n: variable name
     * \param[in] b: bounds description
     */
    void setPhysicalBounds(const std::string&,
                           const VariableBoundsDescription&);
    /*!
     * \brief add physical bounds to a variable
     * \param[in] n: variable name
     * \param[in] i: index (for array of variables)
     * \param[in] b: bounds description
     */
    void setPhysicalBounds(const std::string&,
                           const unsigned short,
                           const VariableBoundsDescription&);
    /*!
     * reserve the given name
     * \param[in] n : variable name
     * \note an exception is thrown is the given name has already been
     * registred
     */
    void reserveName(const std::string&);
    /*!
     * \brief look if a name is reserved
     * \param[in] n : name
     */
    bool isNameReserved(const std::string&) const;
    /*!
     * \brief register a member name
     * \param[in] n : member name
     * \note an exception is thrown is the given name has already been
     * registred
     * \note : this method automatically calls the reserveName method
     */
    void registerMemberName(const std::string&);
    /*!
     * \brief register a glossary name
     * \param[in] n: variable name
     * \param[in] g: glossary name
     * \note an exception is thrown if:
     * - the variable name has not been reserved
     * - the given glossary name is not a real glossary name
     * - the given glossary name has already been registred
     * \note : this method automatically calls the reserveName method
     */
    void registerGlossaryName(const std::string&, const std::string&);
    /*!
     * \brief register a entry name
     * \param[in] n: variable name
     * \param[in] e: entry name
     * \note an exception is thrown if:
     * - the variable name has not been reserved
     * - the given entry name is a glossary name
     * - the given entry name has already been registred
     * \note : this method automatically calls the reserveName method
     */
    void registerEntryName(const std::string&, const std::string&);
    /*!
     * register a static member name
     * \param[in] n : static member name
     * \note an exception is thrown is the given name has already been
     * registred
     * \note : this method automatically calls the reserveName method
     */
    void registerStaticMemberName(const std::string&);
    //! \brief \return all the registred member names
    const std::set<std::string>& getRegistredMembersNames() const;
    //! \brief \return all the registred static member names
    const std::set<std::string>& getRegistredStaticMembersNames() const;
    /*!
     * \return the name of all the variables (material properties,
     * state variables, auxiliary state variables, external state
     * variables, parameters)
     */
    std::set<std::string> getVariablesNames() const;
    /*!
     * \brief retrieve all the symbols associated with the declared variables.
     * \param[out] symbols: map between symbols and replacement strings
     */
    void getSymbols(std::map<std::string, std::string>&) const;
    //! \return the list of user defined initialize blocks
    std::vector<std::string> getUserDefinedInitializeCodeBlocksNames() const;
    //! \return the list of user defined initialize blocks
    const CodeBlock& getUserDefinedInitializeCodeBlock(
        const std::string&) const;
    /*!
     * \brief method that shall be called when all variables are declared.
     *
     * This method overrides variables by parameters
     */
    void finalizeVariablesDeclaration();
    //! \brief destructor
    ~BehaviourData() override;

   private:
    /*!
     * structure used to handle a blocks of code
     */
    struct CodeBlocksAggregator {
      //! \brief a simple alias
      typedef BehaviourData::Position Position;
      //! \brief a simple alias
      typedef BehaviourData::Mode Mode;
      //! \brief constructor
      CodeBlocksAggregator();
      CodeBlocksAggregator(CodeBlocksAggregator&&) = default;
      CodeBlocksAggregator(const CodeBlocksAggregator&) = default;
      CodeBlocksAggregator& operator=(CodeBlocksAggregator&&) = default;
      CodeBlocksAggregator& operator=(const CodeBlocksAggregator&) = default;
      /*!
       * setter
       * \param[in] c : code
       * \param[in] p : position
       * \param[in] b : if true, the code can be overriden or completed
       */
      void set(const CodeBlock&, const Position, const bool);
      /*!
       * setter
       * \param[in] c : code
       * \param[in] p : position
       * \param[in] b : if true, the code can be overriden or completed
       */
      void replace(const CodeBlock&, const Position, const bool);
      //! \brief \return the code block
      const CodeBlock& get() const;
      //! \brief \return true if the code is mutable
      bool isMutable() const;
      //! \brief destructor
      ~CodeBlocksAggregator();

     private:
      //! \brief \brief update the code block
      void update();
      //! \brief check that the get function was not already called.
      void check() const;
      //! \brief beginnig of the block
      std::string cblock_begin;
      //! \brief block body
      std::string cblock_body;
      //! \brief end of the block
      std::string cblock_end;
      //! \brief beginnig of the doc
      std::string cdoc_begin;
      //! \brief doc body
      std::string cdoc_body;
      //! \brief end of the doc
      std::string cdoc_end;
      //! \brief resulting code block
      CodeBlock cblock;
      //! \brief get already called
      mutable bool is_mutable = true;
    };
    /*!
     * check that the given name has been registred as a variable name
     * \param[in] n : variable name
     * \note an exception is thrown is the given name is not found
     */
    void checkVariableName(const std::string&) const;
    /*!
     * \brief return the variable description associated to the given name
     * \param[in] n: name
     */
    VariableDescription& getVariableDescription(const std::string&);
    /*!
     * \brief add a variable to a container
     * \param[in] c: container
     * \param[in] v: variable to be added
     * \param[in] s: registration status
     * \param[in] bi: if true, register variable increment name
     * \param[in] b: if true, allow the variable to be added even if
     * the allowsNewUserDefinedVariables has been set to false. This
     * shall be true only for local variables which have a specific
     * status.
     */
    void addVariable(VariableDescriptionContainer&,
                     const VariableDescription&,
                     const RegistrationStatus,
                     const bool,
                     const bool = false);
    /*!
     * variables flagged as probably unusable in purely implicit
     * resolutions
     */
    std::set<std::string> pupirv;
    //! \brief registred code blocks
    std::map<std::string, CodeBlocksAggregator> cblocks;
    //! \brief registred initialize functions
    std::map<std::string, CodeBlock> initialize_functions;
    //! \brief registred post-processings
    std::map<std::string, CodeBlock> postprocessings;
    //! \brief registred material properties
    VariableDescriptionContainer materialProperties;
    //! \brief registred persistent variables
    VariableDescriptionContainer persistentVariables;
    //! \brief registred integration variables
    VariableDescriptionContainer integrationVariables;
    //! \brief registred state variables
    VariableDescriptionContainer stateVariables;
    //! \brief registred auxiliary state variables
    VariableDescriptionContainer auxiliaryStateVariables;
    //! \brief registred external state variables
    VariableDescriptionContainer externalStateVariables;
    //! \brief registred local variables
    VariableDescriptionContainer localVariables;
    //! \brief parameters of the behaviour
    VariableDescriptionContainer parameters;
    //! \brief registred initialize variables
    VariableDescriptionContainer initializeVariables;
    //! \brief registred post-processing variables
    VariableDescriptionContainer postProcessingVariables;
    //! \brief default value for floatting point paramerters
    std::map<std::string, double> parametersDefaultValues;
    //! \brief default value for signed integer paramerters
    std::map<std::string, int> iParametersDefaultValues;
    //! \brief default value for short unsigned integer paramerters
    std::map<std::string, unsigned short> uParametersDefaultValues;
    //! \brief static variables
    StaticVariableDescriptionContainer staticVariables;
    /*!
     * \brief registred stress fress expansion descriptions
     */
    std::vector<StressFreeExpansionDescription> sfeds;
    //! \brief overriding parameters
    std::map<std::string, double> overriding_parameters;
    //! \brief private code
    std::string privateCode;
    //! \brief class members
    std::string members;
    //! \brief list of glossary names used by the behaviour
    std::map<std::string, std::string> glossaryNames;
    //! \brief list of entry names used by the behaviour
    std::map<std::string, std::string> entryNames;
    //! \brief list of reserved names
    std::set<std::string> reservedNames;
    //! \brief list of variables names
    std::set<std::string> membersNames;
    //! \brief list of variables names
    std::set<std::string> staticMembersNames;
    /*!
     * boolean stating if this behaviour can be used in a purely
     * implicit resolution.
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    bool usableInPurelyImplicitResolution = false;
  };  // end of struct BehaviourData

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURDATA_HXX */
