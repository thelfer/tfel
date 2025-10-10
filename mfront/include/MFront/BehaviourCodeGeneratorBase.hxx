/*!
 * \file   mfront/include/MFront/BehaviourCodeGeneratorBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   03/08/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURCODEGENERATORBASE_HXX
#define LIB_MFRONT_BEHAVIOURCODEGENERATORBASE_HXX

#include <map>
#include <string>
#include <memory>
#include <iosfwd>
#include <string_view>
#include "MFront/AbstractBehaviourCodeGenerator.hxx"

namespace mfront {

  // forward declaration
  struct FileDescription;
  struct BehaviourDescription;
  struct AbstractBehaviourInterface;

  //! \brief base class for all behaviour's code generator
  struct BehaviourCodeGeneratorBase : AbstractBehaviourCodeGenerator {
    // \brief a simple alias
    using BehaviourInterfaceMap =
        std::map<std::string, std::shared_ptr<AbstractBehaviourInterface>>;
    /*!
     * \brief constructor
     * \param[in] f: file description
     * \param[in] d: behaviour description
     * \param[in] bim: behaviour interfaces
     */
    BehaviourCodeGeneratorBase(const FileDescription&,
                               const BehaviourDescription&,
                               const BehaviourInterfaceMap&);
    //
    void generateOutputFiles(const std::set<Hypothesis>&) override;
    bool isSrcFileRequired() const override;
    //! \brief destructor
    ~BehaviourCodeGeneratorBase() override;

   protected:
    //! \brief a simple alias
    using OrthotropicAxesConvention = tfel::material::OrthotropicAxesConvention;

    /*!
     * \brief a method checking if the given gradient and the thermodynamic
     * forces have the same array size. \param[in] m: name of the calling method
     * \param[in] mv: main variable
     */
    static void checkArraySizeOfMainVariables(
        const std::string_view, const std::pair<Gradient, ThermodynamicForce>&);
    /*!
     * \brief throw an std::runtime_error
     * \param[in] m: calling method name
     * \param[in] e: error message
     */
    [[noreturn]] void throwRuntimeError(const std::string_view,
                                        const std::string_view) const;
    /*!
     * \brief return if the code must generate the time derivatives of the
     * state variables
     */
    virtual bool usesStateVariableTimeDerivative() const;
    /*!
     * write the given variable declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variable to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the
     * variables
     */
    virtual void writeVariableDeclaration(std::ostream&,
                                          const VariableDescription&,
                                          const std::string&,
                                          const std::string&,
                                          const std::string&,
                                          const bool) const;
    /*!
     * write the given variables declaration
     * \param[out] f                 : output file
     * \param[in]  v                 : variables to be declared
     * \param[in]  prefix            : prefix added to variable's names
     * \param[in]  suffix            : suffix added to variable's names
     * \param[in]  useTimeDerivative : declare time derivative of the
     * variables
     */
    virtual void writeVariablesDeclarations(std::ostream&,
                                            const VariableDescriptionContainer&,
                                            const std::string&,
                                            const std::string&,
                                            const std::string&,
                                            const bool) const;

    virtual void writeIncludes(std::ostream&) const;

    virtual void writeHeaderGuardBegin(std::ostream&,
                                       const std::string&,
                                       const bool) const;

    virtual void writeHeaderGuardEnd(std::ostream&,
                                     const std::string&,
                                     const bool) const;

    virtual void writeNamespaceBegin(std::ostream&) const;

    virtual void writeNamespaceEnd(std::ostream&) const;

    virtual void writeTypeAliases(std::ostream&) const;

    virtual void checkBehaviourDataFile(std::ostream&) const;

    virtual void writeBehaviourDataTypeAliases(std::ostream&) const;

    virtual void writeBehaviourDataStandardTFELIncludes(std::ostream&) const;

    virtual void writeBehaviourDataFileHeader(std::ostream&) const;

    virtual void writeBehaviourDataFileHeaderBegin(std::ostream&) const;

    virtual void writeBehaviourDataFileHeaderEnd(std::ostream&) const;

    virtual void writeBehaviourDataClassHeader(std::ostream&) const;

    virtual void writeBehaviourDataDisabledConstructors(std::ostream&) const;

    virtual void writeBehaviourDataConstructors(std::ostream&,
                                                const Hypothesis) const;
    /*!
     * write interface's setters for the main variables
     */
    virtual void writeBehaviourDataMainVariablesSetters(std::ostream&) const;

    virtual void writeBehaviourDataClassBegin(std::ostream&,
                                              const Hypothesis) const;

    virtual void writeBehaviourDataClassEnd(std::ostream&) const;

    virtual void writeBehaviourDataDefaultMembers(std::ostream&) const;

    virtual void writeBehaviourDataMaterialProperties(std::ostream&,
                                                      const Hypothesis) const;

    virtual void writeBehaviourDataStateVariables(std::ostream&,
                                                  const Hypothesis) const;

    virtual void writeBehaviourDataAssignementOperator(std::ostream&,
                                                       const Hypothesis) const;

    virtual void writeBehaviourDataOutputOperator(std::ostream&,
                                                  const Hypothesis) const;

    virtual void writeBehaviourDataInitializeMethods(std::ostream&,
                                                     const Hypothesis) const;

    virtual void writeBehaviourDataExport(std::ostream&,
                                          const Hypothesis) const;

    virtual void writeBehaviourDataPublicMembers(std::ostream&) const;

    virtual void writeBehaviourDataFileBegin(std::ostream&) const;

    virtual void writeBehaviourDataFileEnd(std::ostream&) const;

    virtual void writeBehaviourDataClass(std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourDataForwardDeclarations(std::ostream&) const;

    virtual void checkIntegrationDataFile(std::ostream&) const;

    virtual void writeIntegrationDataTypeAliases(std::ostream&) const;

    virtual void writeIntegrationDataStandardTFELIncludes(std::ostream&) const;

    virtual void writeIntegrationDataFileHeader(std::ostream&) const;

    virtual void writeIntegrationDataFileHeaderBegin(std::ostream&) const;

    virtual void writeIntegrationDataFileHeaderEnd(std::ostream&) const;

    virtual void writeIntegrationDataClassHeader(std::ostream&) const;

    virtual void writeIntegrationDataDisabledConstructors(std::ostream&) const;

    virtual void writeIntegrationDataConstructors(std::ostream&,
                                                  const Hypothesis) const;
    /*!
     * write interface's setters for the main variables
     */
    virtual void writeIntegrationDataMainVariablesSetters(std::ostream&) const;

    virtual void writeIntegrationDataScaleOperators(std::ostream&,
                                                    const Hypothesis) const;

    virtual void writeIntegrationDataUpdateDrivingVariablesMethod(
        std::ostream&) const;

    virtual void writeIntegrationDataClassBegin(std::ostream&,
                                                const Hypothesis) const;

    virtual void writeIntegrationDataClassEnd(std::ostream&) const;

    virtual void writeIntegrationDataDefaultMembers(std::ostream&) const;

    virtual void writeIntegrationDataExternalStateVariables(
        std::ostream&, const Hypothesis) const;

    virtual void writeIntegrationDataFileBegin(std::ostream&) const;

    virtual void writeIntegrationDataFileEnd(std::ostream&) const;

    virtual void writeIntegrationDataClass(std::ostream&,
                                           const Hypothesis) const;

    virtual void writeIntegrationDataForwardDeclarations(std::ostream&) const;

    virtual void writeIntegrationDataOutputOperator(std::ostream&,
                                                    const Hypothesis) const;

    virtual void checkBehaviourFile(std::ostream&) const;

    virtual void writeBehaviourTypeAliases(std::ostream&) const;

    virtual void writeBehaviourFileHeader(std::ostream&) const;

    virtual void writeBehaviourFileHeaderBegin(std::ostream&) const;

    virtual void writeBehaviourFileHeaderEnd(std::ostream&) const;

    virtual void writeBehaviourFileBegin(std::ostream&) const;

    virtual void writeBehaviourFileEnd(std::ostream&) const;

    virtual void writeBehaviourClass(std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourForwardDeclarations(std::ostream&) const;

    virtual void writeBehaviourProfiler(std::ostream&) const;

    virtual void writeBehaviourParserSpecificInheritanceRelationship(
        std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourParserSpecificTypedefs(std::ostream&) const;

    virtual void writeBehaviourParserSpecificMembers(std::ostream&,
                                                     const Hypothesis) const;

    virtual void writeBehaviourParserSpecificIncludes(std::ostream&) const;

    virtual void writeBehaviourClassBegin(std::ostream&,
                                          const Hypothesis) const;

    virtual void writeBehaviourFriends(std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourGetModellingHypothesis(std::ostream&) const;

    virtual void writeBehaviourClassEnd(std::ostream&) const;

    virtual void writeBehaviourOutOfBoundsPolicyVariable(std::ostream&) const;

    virtual void writeBehaviourSetOutOfBoundsPolicy(std::ostream&) const;

    virtual void writeBehaviourCheckBounds(std::ostream&,
                                           const Hypothesis) const;
    /*!
     * \brief write the checks associated to a bound
     * \param[out] os: output stream
     * \param[in]  v:  variable description
     * \param[in]  b:  if true, checks are written also on the variable
     * updated with its increment \note if the variable has no bounds, nothing
     * is done
     */
    virtual void writeBoundsChecks(std::ostream&,
                                   const VariableDescription&,
                                   const bool) const;
    /*!
     * \brief write the checks associated to a physical bound
     * \param[out] os: output stream
     * \param[in]  v:  variable description
     * \param[in]  b:  if true, checks are written also on the variable
     * updated with its increment \note if the variable has no physical
     * bounds, nothing is done
     */
    virtual void writePhysicalBoundsChecks(std::ostream&,
                                           const VariableDescription&,
                                           const bool) const;
    /*!
     * \brief write the disabled constructors
     * \param[in] os: output stream
     */
    virtual void writeBehaviourDisabledConstructors(std::ostream&) const;
    /*!
     * \brief write the assignement operators
     * \param[in] os: output stream
     */
    virtual void writeBehaviourDisabledAssignementOperators(
        std::ostream&) const;

    virtual void writeBehaviourConstructors(std::ostream&,
                                            const Hypothesis) const;

    //! \return behaviour constructor initializers.
    virtual std::string getBehaviourConstructorsInitializers(
        const Hypothesis) const;
    //! \return local variables initalizers.
    virtual std::string getLocalVariablesInitializers(const Hypothesis) const;
    /*!
     * \brief write the arguments of a material property, including
     * the the surrounding parenthesis. Those arguments are used to
     * evaluate the material property and/or check its bounds.
     * \param[out] out: output stream
     * \param[in]  m:   material property description
     * \param[in]  f:   function converting input variable name.
     * The function f can be used to specify how evaluate a variable value.
     * For example, if we want to evaluate the variable name 'V' at
     * the end of the time step, we could make f return V+dV
     */
    virtual void writeExternalMFrontMaterialPropertyArguments(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&) const;
    /*!
     * \brief write the bounds checks to a material property
     * \param[out] out: output stream
     * \param[in]  m:   material property description
     * \param[in]  f:   function converting input variable name.
     * The function f can be used to specify how evaluate a variable value.
     * For example, if we want to evaluate the variable name 'V' at
     * the end of the time step, we could make f return V+dV
     */
    virtual void writeMaterialPropertyCheckBoundsEvaluation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&) const;
    void writeMaterialPropertyEvaluation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        std::function<std::string(const MaterialPropertyInput&)>&)
        const override;
    /*!
     * \brief write the evoluation of a thermal expansion coefficient
     * \param[out] out: output stream
     * \param[in]  mpd: material property
     * \param[in] T:    temperature at which the thermal expansion
     * coefficient is computed
     * \param[in] idx:  index
     * \param[in] s:    suffix
     */
    virtual void writeThermalExpansionCoefficientComputation(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    virtual void writeThermalExpansionCoefficientsComputations(
        std::ostream&,
        const BehaviourDescription::MaterialProperty&,
        const std::string& = "") const;
    virtual void writeThermalExpansionComputation(
        std::ostream&,
        const std::string&,
        const std::string&,
        const std::string& = "") const;
    /*!
     * \brief write the behaviour's computeStressFreeExpansion method, if
     * mandatory.
     * \param[in] h : modelling hypothesis
     */
    virtual void writeBehaviourComputeStressFreeExpansion(
        std::ostream&, const Hypothesis) const;
    /*!
     * \brief write the stiffness tensor computation evaluation
     * from the elastic material properties.
     * \param[out] out: output stream
     * \param[in]  D:   name of the stiffness tensor variable
     * \param[in]  f:   function used to handle the variables of the material
     * properties.
     */
    virtual void writeStiffnessTensorComputation(
        std::ostream&,
        const std::string&,
        std::function<std::string(const MaterialPropertyInput&)>&) const;
    /*!
     * \brief write the Hill tensor computation evaluation
     * from the elastic material properties.
     * \param[out] out: output stream
     * \param[in]  H:   name of the Hill tensor variable to be computed
     * \param[in]  h:   Hill tensor definition
     * \param[in]  f:   function used to handle the variables of the material
     * properties.
     */
    virtual void writeHillTensorComputation(
        std::ostream&,
        const std::string&,
        const BehaviourDescription::HillTensor&,
        std::function<std::string(const MaterialPropertyInput&)>&) const;
    /*!
     * \brief write the initalize methods.
     *
     * It always create an `initialize` method which contains `.
     * This method is called after that the main variables were set.
     */
    virtual void writeBehaviourInitializeMethods(std::ostream&,
                                                 const Hypothesis) const;
    /*!
     * write part of the constructor specific to the parser
     * \param[in] h : modelling hypothesis
     */
    virtual void writeBehaviourParserSpecificInitializeMethodPart(
        std::ostream&, const Hypothesis) const;

    /*!
     * \param[in] h: modelling hypothesis
     */
    virtual std::string getIntegrationVariablesIncrementsInitializers(
        const Hypothesis) const;

    virtual void writeBehaviourIntegrationVariablesIncrements(
        std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourLocalVariables(std::ostream&,
                                              const Hypothesis) const;

    virtual void writeBehaviourIntegrationVariables(std::ostream&,
                                                    const Hypothesis) const;

    virtual void writeBehaviourParameters(std::ostream&,
                                          const Hypothesis) const;

    virtual void writeBehaviourStaticVariables(std::ostream&,
                                               const Hypothesis) const;

    virtual void writeBehaviourAdditionalMembers(std::ostream&,
                                                 const Hypothesis) const;

    virtual void writeBehaviourPrivate(std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourUpdateIntegrationVariables(
        std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourUpdateStateVariables(std::ostream&,
                                                    const Hypothesis) const;

    virtual void writeBehaviourUpdateAuxiliaryStateVariables(
        std::ostream&, const Hypothesis) const;
    /*!
     * \brief write the computeInternalEnergy method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeInternalEnergy(std::ostream&,
                                                     const Hypothesis) const;
    /*!
     * \brief write the computeDissipatedEnergy method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeDissipatedEnergy(std::ostream&,
                                                       const Hypothesis) const;
    /*!
     * \brief write the computeSpeedOfSound method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeSpeedOfSound(std::ostream&,
                                                   const Hypothesis) const;
    //! \brief write the getTimeStepScalingFactor method
    virtual void writeBehaviourGetTimeStepScalingFactor(std::ostream&) const;
    //! \brief write the integrate method
    virtual void writeBehaviourIntegrator(std::ostream&,
                                          const Hypothesis) const;
    /*!
     * \brief write the computeAPrioriTimeStepsScalingFactor method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeAPrioriTimeStepScalingFactor(
        std::ostream&) const;
    /*!
     * \brief write the computeAPrioriTimeStepsScalingFactorII method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeAPrioriTimeStepScalingFactorII(
        std::ostream&, const Hypothesis) const;
    /*!
     * \brief write the computeAPosterioriTimeStepsScalingFactor method
     * \param[in] os: output stream
     */
    virtual void writeBehaviourComputeAPosterioriTimeStepScalingFactor(
        std::ostream&) const;
    /*!
     * \brief write the computeAPosterioriTimeStepsScalingFactorII method
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourComputeAPosterioriTimeStepScalingFactorII(
        std::ostream&, const Hypothesis) const;
    /*!
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourUpdateExternalStateVariables(
        std::ostream&, const Hypothesis) const;
    /*!
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourInitializeFunctions(std::ostream&,
                                                   const Hypothesis) const;
    /*!
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourPostProcessings(std::ostream&,
                                               const Hypothesis) const;
    /*!
     * \brief write the methods associated with the behaviour variables
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourBehaviourVariablesMethods(
        std::ostream&, const Hypothesis) const;
    /*!
     * \param[in] os: output stream
     * \param[in] h: modelling hypothesis
     */
    virtual void writeBehaviourOutputOperator(std::ostream&,
                                              const Hypothesis) const;
    /*!
     * \param[in] os: output stream
     */
    virtual void writeBehaviourDestructor(std::ostream&) const;
    /*!
     * \param[in] os: output stream
     */
    virtual void writeBehaviourTraits(std::ostream&) const;

    /*!
     * \param[out] out: os file stream
     * \param[in]  h:   modelling hypothesis
     * \param[in]  b    true if the behaviour is defined for the given
     * modelling hypothesis
     */
    virtual void writeBehaviourTraitsSpecialisation(std::ostream&,
                                                    const Hypothesis,
                                                    const bool) const;

    virtual void writeBehaviourIncludes(std::ostream&) const;

    virtual void writeBehaviourLocalVariablesInitialisation(
        std::ostream&, const Hypothesis) const;

    virtual void writeBehaviourParameterInitialisation(std::ostream&,
                                                       const Hypothesis) const;

    virtual void writeBehaviourParametersInitializers(std::ostream&) const;

    virtual void writeBehaviourParametersInitializer(std::ostream&,
                                                     const Hypothesis) const;

    virtual void checkSrcFile(std::ostream&) const;

    virtual void writeSrcFileHeader(std::ostream&) const;

    virtual void writeSrcFileUserDefinedCode(std::ostream&) const;

    virtual void writeSrcFileBehaviourProfiler(std::ostream&) const;

    virtual void writeSrcFileParametersInitializers(std::ostream&) const;

    virtual void writeSrcFileParametersInitializer(std::ostream&,
                                                   const Hypothesis) const;

    /*!
     * \brief write the source file
     */
    virtual void writeSrcFile(std::ostream&) const;

    virtual void writeBehaviourComputePredictionOperator(
        std::ostream&, const Hypothesis) const;

    /*!
     * \brief write the methods associated with the computation of the tangent
     * operator
     * \param[in,out] os: output file stream
     * \param[in] h: modelling hypothesis
     * \note this method calls `writeBehaviourComputeTangentOperatorBody` to
     * write the body of the method. The body of the method follows a preamble
     * containing some `using` statements (for `std` and `tfel::math`
     * namespaces) and the declaration of material laws.
     */
    virtual void writeBehaviourComputeTangentOperator(std::ostream&,
                                                      const Hypothesis) const;
    /*!
     * \brief write the body of a method computing the tangent operator
     * \param[in,out] os: output file stream
     * \param[in] h: modelling hypothesis
     * \param[in] n: code block name
     */
    virtual void writeBehaviourComputeTangentOperatorBody(
        std::ostream&, const Hypothesis, const std::string&) const;

    //! \brief write the code returning the tangent operator
    virtual void writeBehaviourGetTangentOperator(std::ostream&) const;
    /*!
     * \brief write the code declaring with the tangent operator and its
     * blocks, if required.
     */
    virtual void writeBehaviourTangentOperator(std::ostream&) const;
    /*!
     * \brief write the call to a model
     * \param[out] out: output stream
     * \param[in,out] tmpnames: temporary names
     * \param[in]  h:   hypothesis
     * \param[in]  md:  model description
     * \param[in]  outputVariables: names of the variables containing the
     * results \param[in]  inputVariables: names of the variables containing
     * the values of the variables modified by the model at the beginning of
     * the time step \param[in]  bn:  base name for temporary variable
     *
     * \pre the size of the input variables must be equal to the size of the
     * output variables
     */
    virtual void writeModelCall(std::ostream&,
                                std::vector<std::string>&,
                                const Hypothesis,
                                const ModelDescription&,
                                const std::vector<std::string>&,
                                const std::vector<std::string>&,
                                const std::string&) const;
    //! \brief write the header files declaring the slip systems
    virtual void generateSlipSystemsFiles();
    //! \brief file description
    const FileDescription& fd;
    //! \brief behaviour description
    const BehaviourDescription& bd;
    //! \brief list of registred interfaces, indexed by their name
    const BehaviourInterfaceMap& interfaces;
  };  // end of BehaviourCodeGeneratorBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURCODEGENERATORBASE_HXX */
