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
#include "MFront/BehaviourAttribute.hxx"

namespace mfront {

  // forward declaration
  struct ModelDescription;

  /*!
   * This structure gathers various behaviour characteristic
   */
  struct MFRONT_VISIBILITY_EXPORT BehaviourData : private SupportedTypes {
    /*
     * normalised code block names
     * \note code block name begins with an upper case
     */
    //! standard code name
    static const char* const FlowRule;
    //! standard code name
    static const char* const BeforeInitializeLocalVariables;
    //! standard code name
    static const char* const InitializeLocalVariables;
    //! standard code name
    static const char* const AfterInitializeLocalVariables;
    //! standard code name
    static const char* const ComputePredictor;
    //! standard code name
    static const char* const ComputeStress;
    //! standard code name
    /* Most behaviours will only rely the @ComputeStress keyword to
     * estimate stresses at the middle of the time step and at the
     * end.  However, some finite strains behaviours must compute a
     * stress measure during the iterations which is not the Cauchy
     * stress. Thus, `@ComputeStress` also defines an
     * ComputeFinalStressCandidate code block which will be used if
     * the user does not provide an alternative through the
     * `@ComputeFinalStress`
     */
    static const char* const ComputeFinalStressCandidate;
    //! standard code name
    static const char* const ComputeFinalStress;
    //! standard code name
    static const char* const DefaultComputeFinalStress;
    //! standard code name
    static const char* const APrioriTimeStepScalingFactor;
    //! standard code name
    static const char* const Integrator;
    //! standard code name
    static const char* const APosterioriTimeStepScalingFactor;
    //! standard code name
    static const char* const ComputeDerivative;
    //! standard code name
    static const char* const ComputePredictionOperator;
    //! standard code name
    static const char* const ComputeTangentOperator;
    //! standard code name
    static const char* const ComputeStressFreeExpansion;
    //! standard code name
    static const char* const UpdateAuxiliaryStateVariables;
    //! standard code name
    static const char* const InitializeJacobian;
    //! standard code name
    static const char* const InitializeJacobianInvert;
    //! standard code name
    static const char* const ComputeInternalEnergy;
    //! standard code name
    static const char* const ComputeDissipatedEnergy;
    //! standard code name
    static const char* const AdditionalConvergenceChecks;
    /*
     * normalised attribute names
     * \note properties name begins with a lower case
     */
    //! if this attribute is true, the behaviour can compute a tangent
    //! operator
    static const char* const hasConsistentTangentOperator;
    //! if this attribute is true, the tangent operator can be
    //  considered symmetric
    static const char* const isConsistentTangentOperatorSymmetric;
    //! if this attribute is true, the behaviour can compute an a
    //! priori estimate of the time step scaling factor after the
    //! initialisation
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
    //! a boolean attribute telling if profiling information shall be collected
    static const char* const profiling;
    //! algorithm used
    static const char* const algorithm;
    //! number of evaluation of the computeDerivative method
    //  of a Runge-Kutta algorithm
    static const char* const numberOfEvaluations;
    /*!
     * The description of a stress free expansion defined through an
     * external state variable
     * \note SFED_ESV: Stress Free Expansion Description _ External
     * State Variable
     */
    struct SFED_ESV {
      //! variable name
      std::string vname;
    };  // end of SFED_ESV
    //! a placeholder for null expansion
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
      //! o volumetric stress free expansion
      StressFreeExpansionHandler sfe;
    };  // end of struct VolumeSwellingStressFreeExpansion
    /*!
     * \brief a simple wrapper around a model describing an axial
     * growth.
     * \f[
     * s = \left(0,0,dlz_lz\,0\,0\,0\right)
     * \f]
     */
    struct AxialGrowth {
      //! description of the axial growth
      StressFreeExpansionHandler sfe;
    };  // end of struct AxialGrowth
    /*!
     * \brief a simple wrapper around a model describing fuel
     * relocation.
     * \f[
     * s = \left(0,0,dlz_lz\,0\,0\,0\right)
     * \f]
     */
    struct Relocation {
      //! description of the axial growth
      StressFreeExpansionHandler sfe;
    };  // end of struct Relocation
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an isotropic swelling given by a linear change in each
     * material geometry:
     * \f[
     * s = \left(dl_l,dl_l,dl_l\,0\,0\,0\right)
     * \f]
     */
    struct IsotropicStressFreeExpansion {
      //! stress free expansion in each direction
      StressFreeExpansionHandler sfe;
    };  // end of struct IsotropicStressFreeExpansion
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an orthotropic swelling
     */
    struct OrthotropicStressFreeExpansion {
      //! stress free expansion in the first material direction
      StressFreeExpansionHandler sfe0;
      //! stress free expansion in the second material direction
      StressFreeExpansionHandler sfe1;
      //! stress free expansion in the third material direction
      StressFreeExpansionHandler sfe2;
    };  // end of struct OrthotropicStressFreeExpansion
    /*!
     * \brief a brief structure describing a stress free expansion du
     * to an orthotropic swelling described by an array of external
     * state variables
     */
    struct OrthotropicStressFreeExpansionII {
      //! stress free expansions
      SFED_ESV esv;
    };  // end of struct OrthotropicStressFreeExpansion
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
    //! description of a stress free expansion
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
    //! constructor
    BehaviourData();
    //! copy constructor
    explicit BehaviourData(const BehaviourData&);
    //! move constructor (disabled)
    BehaviourData(BehaviourData&&) = delete;
    //! assignement operator (disabled)
    BehaviourData& operator=(const BehaviourData&) = delete;
    //! move assignement operator (disabled)
    BehaviourData& operator=(BehaviourData&&) = delete;
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
     * - `ExternalStateVariables`
     * - `Parameters`
     */
    const VariableDescriptionContainer& getVariables(const std::string&) const;
    /*!
     * \return all material properties
     */
    const VariableDescriptionContainer& getMaterialProperties() const;
    /*!
     * \return all persistent variables
     */
    const VariableDescriptionContainer& getPersistentVariables() const;
    /*!
     * \return all integration variables
     */
    const VariableDescriptionContainer& getIntegrationVariables() const;
    /*!
     * \return all state variables
     */
    const VariableDescriptionContainer& getStateVariables() const;
    /*!
     * \return all auxiliary state variables
     */
    const VariableDescriptionContainer& getAuxiliaryStateVariables() const;
    /*!
     * \return all external state variables
     */
    const VariableDescriptionContainer& getExternalStateVariables() const;
    /*!
     * \return all local variables
     */
    const VariableDescriptionContainer& getLocalVariables() const;
    /*!
     * \return all parameter variables
     */
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
     * \return the state variable associated with the given name
     * \param[in] n: name
     */
    const VariableDescription& getAuxiliaryStateVariableDescription(
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

    bool isMaterialPropertyName(const std::string&) const;

    bool isLocalVariableName(const std::string&) const;

    bool isPersistentVariableName(const std::string&) const;

    bool isIntegrationVariableName(const std::string&) const;

    bool isIntegrationVariableIncrementName(const std::string&) const;

    bool isStateVariableName(const std::string&) const;

    bool isStateVariableIncrementName(const std::string&) const;

    bool isAuxiliaryStateVariableName(const std::string&) const;

    bool isExternalStateVariableName(const std::string&) const;

    bool isExternalStateVariableIncrementName(const std::string&) const;

    bool isParameterName(const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] n : name
     */
    bool isStaticVariableName(const std::string& n) const;

    //! \return the static variables defined
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
    /*!
     * \return the material additional members
     */
    std::string getMembers() const;
    /*!
     * \brief append the given code to the members
     * \param[in] s : members
     */
    void appendToPrivateCode(const std::string&);
    //! \return the material private code
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
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void setAttribute(const std::string&,
                      const BehaviourAttribute&,
                      const bool);
    /*!
     * \brief update an existing new attribute
     * \param[in] n: name
     * \param[in] a: attribute
     */
    void updateAttribute(const std::string&, const BehaviourAttribute&);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool hasAttribute(const std::string&) const;
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
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template <typename T>
    typename std::enable_if<
        tfel::meta::TLCountNbrOfT<T, BehaviourAttributeTypes>::value == 1,
        const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string, BehaviourAttribute>& getAttributes() const;
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
     * register a member name
     * \param[in] n : member name
     * \note an exception is thrown is the given name has already been
     * registred
     * \note : this method automatically calls the reserveName method
     */
    void registerMemberName(const std::string&);
    /*!
     * register a static member name
     * \param[in] n : static member name
     * \note an exception is thrown is the given name has already been
     * registred
     * \note : this method automatically calls the reserveName method
     */
    void registerStaticMemberName(const std::string&);
    //! \return all the registred member names
    const std::set<std::string>& getRegistredMembersNames() const;
    //! \return all the registred static member names
    const std::set<std::string>& getRegistredStaticMembersNames() const;
    /*!
     * \return the name of all the variables (material properties,
     * state variables, auxiliary state variables, external state
     * variables, parameters)
     */
    std::set<std::string> getVariablesNames() const;
    //! destructor
    virtual ~BehaviourData();

   private:
    /*!
     * structure used to handle a blocks of code
     */
    struct CodeBlocksAggregator {
      //! a simple alias
      typedef BehaviourData::Position Position;
      //! a simple alias
      typedef BehaviourData::Mode Mode;
      //! constructor
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
      //! \return the code block
      const CodeBlock& get() const;
      //! \return true if the code is mutable
      bool isMutable() const;
      //! destructor
      ~CodeBlocksAggregator();

     private:
      //! \brief update the code block
      void update();
      //! check that the get function was not already called.
      void check() const;
      //! beginnig of the block
      std::string cblock_begin;
      //! block body
      std::string cblock_body;
      //! end of the block
      std::string cblock_end;
      //! beginnig of the doc
      std::string cdoc_begin;
      //! doc body
      std::string cdoc_body;
      //! end of the doc
      std::string cdoc_end;
      //! resulting code block
      CodeBlock cblock;
      //! get already called
      mutable bool is_mutable = true;
    };
    /*!
     * \brief throw an exception saying that no attribute with the given name
     * exists
     */
    TFEL_NORETURN static void throwUndefinedAttribute(const std::string&);
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
    //! registred code blocks
    std::map<std::string, CodeBlocksAggregator> cblocks;
    //! registred material properties
    VariableDescriptionContainer materialProperties;
    //! registred persistent variables
    VariableDescriptionContainer persistentVariables;
    //! registred integration variables
    VariableDescriptionContainer integrationVariables;
    //! registred state variables
    VariableDescriptionContainer stateVariables;
    //! registred auxiliary state variables
    VariableDescriptionContainer auxiliaryStateVariables;
    //! registred external state variables
    VariableDescriptionContainer externalStateVariables;
    //! registred local variables
    VariableDescriptionContainer localVariables;
    //! parameters of the behaviour
    VariableDescriptionContainer parameters;
    //! default value for floatting point paramerters
    std::map<std::string, double> parametersDefaultValues;
    //! default value for signed integer paramerters
    std::map<std::string, int> iParametersDefaultValues;
    //! default value for short unsigned integer paramerters
    std::map<std::string, unsigned short> uParametersDefaultValues;
    //! static variables
    StaticVariableDescriptionContainer staticVariables;
    /*!
     * \brief registred stress fress expansion descriptions
     */
    std::vector<StressFreeExpansionDescription> sfeds;
    //! behaviour attributes
    std::map<std::string, BehaviourAttribute> attributes;
    //! private code
    std::string privateCode;
    //! class members
    std::string members;
    //! list of glossary names used by the behaviour
    std::map<std::string, std::string> glossaryNames;
    //! list of entry names used by the behaviour
    std::map<std::string, std::string> entryNames;
    //! list of reserved names
    std::set<std::string> reservedNames;
    //! list of variables names
    std::set<std::string> membersNames;
    //! list of variables names
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

#include "MFront/BehaviourData.ixx"

#endif /* LIB_MFRONT_BEHAVIOURDATA_HXX */
