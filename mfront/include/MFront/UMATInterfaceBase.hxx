/*!
 * \file  mfront/include/MFront/UMATInterfaceBase.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTUMATINTERFACEBASE_HXX
#define LIB_MFRONT_MFRONTUMATINTERFACEBASE_HXX

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/SupportedTypes.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct UMATInterfaceBase : public AbstractBehaviourInterface,
                             public SupportedTypes {
    // constructor
    UMATInterfaceBase();

    virtual void exportMechanicalData(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * \param[out] f      : output file
     * \param[out] first  : true if not initializer has been declared yet
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorI(
        std::ostream&,
        bool&,
        const VariableDescriptionContainer&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[out] first  : true if not initializer has been declared yet
     * \param[in]  b      : iterator to the first variable to be initialized
     * \param[in]  e      : iterator past the last variable to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorI(
        std::ostream&,
        bool&,
        const VariableDescriptionContainer::const_iterator&,
        const VariableDescriptionContainer::const_iterator&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in] mb      : behaviour description
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorII(
        std::ostream&,
        const BehaviourDescription&,
        const VariableDescriptionContainer&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  mb     : behaviour description
     * \param[in]  b      : iterator to the first variable to be initialized
     * \param[in]  e      : iterator past the last variable to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     * \param[in]  o      : offset in the array from which the variables are
     * initialized
     */
    virtual void writeVariableInitializersInBehaviourDataConstructorII(
        std::ostream&,
        const BehaviourDescription&,
        const VariableDescriptionContainer::const_iterator&,
        const VariableDescriptionContainer::const_iterator&,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f     : output file
     * \param[in]   mb   : behaviour description
     * \param[in]  v     : variables to be initialized
     * \param[in]  src   : name of the array to which the variables are exported
     */
    virtual void exportResults(std::ostream&,
                               const BehaviourDescription&,
                               const VariableDescriptionContainer&,
                               const std::string&) const;

    virtual void writeResultsArrayResize(
        std::ostream&,
        const std::string&,
        const VariableDescriptionContainer&) const;

    virtual void writeResultsArrayResize(std::ostream&,
                                         const std::string&,
                                         const SupportedTypes::TypeSize&) const;
    /*!
     * \brief write the instruction of exporting a thermodynamic force in an
     * array \param[out] out : output stream \param[in]  a   : array name
     * \param[in]  f   : thermodynamic force
     * \param[in]  o   : thermodynamic force offset
     */
    virtual void exportThermodynamicForce(std::ostream&,
                                          const std::string&,
                                          const ThermodynamicForce&,
                                          const SupportedTypes::TypeSize) const;
    /*!
     * \return the list of additional variables to be passed to the
     * behaviour data and integration data constructors which are
     * specific to the interface.
     *
     * For each item in the list, the first element is the variable
     * name and the second element is the variable description.
     */
    virtual std::vector<std::pair<std::string, std::string>>
    getBehaviourConstructorsAdditionalVariables() const;
    /*!
     * \return the list of additional variables to be passed to the
     * behaviour data constructor which are specific to the interface.
     *
     * For each item in the list, the first element is the variable
     * name and the second element is the variable description.
     */
    virtual std::vector<std::pair<std::string, std::string>>
    getBehaviourDataConstructorAdditionalVariables() const;
    /*!
     * \return the list of additional variables to be passed to the
     * integration data constructor which are specific to the interface.
     *
     * For each item in the list, the first element is the variable
     * name and the second element is the variable description.
     */
    virtual std::vector<std::pair<std::string, std::string>>
    getIntegrationDataConstructorAdditionalVariables() const;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void writeBehaviourConstructor(std::ostream&,
                                           const BehaviourDescription&,
                                           const std::string&) const override;
    /*!
     * \brief write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] h             : modelling hypothesis
     * \param[in] mb            : behaviour description
     */
    virtual void writeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * \brief add interface specific lines at the end of the behaviour
     * data constructor
     * \param[in] behaviourFile : output file
     * \param[in] h             : modelling hypothesis
     * \param[in] mb            : behaviour description
     */
    virtual void completeBehaviourDataConstructor(
        std::ostream&, const Hypothesis, const BehaviourDescription&) const;
    /*!
     * write the initialisation of the driving variables
     * \param[in] os : output file
     * \param[in] mb : behaviour description
     */
    virtual void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const;
    /*!
     * \brief write the initialisation of a thermodynamic force
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataThermodynamicForceSetter(
        std::ostream&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const;

    virtual void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : behaviour description
     */
    virtual void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeIntegrationDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const;
    //! destructor
    virtual ~UMATInterfaceBase();

   protected:
    /*!
     * The umat interface may require additionnal material properties,
     * to compute the stiffness matrix or to compute the thermal
     * expansions. Some additional material properties may also be
     * required by the solver (mass density, orthotropic basis for the
     * Cast3M solver) which are of no interest for the behaviour. On
     * the orher hand, some material properties are highly needed by
     * the behaviour (Young modulus and Poisson ratio for example).
     * This structure allows the computation of an indirection map
     * between the data given by the solver and the ones needed by the
     * interface.
     */
    struct UMATMaterialProperty {
      /*!
       * constructor
       * \param[in] t : type
       * \param[in] n : name
       * \param[in] v : usual variable name
       * \param[in] a : array size
       * \param[in] o : offset
       * \param[in] d : dummy variable
       */
      UMATMaterialProperty(const std::string&,
                           const std::string&,
                           const std::string&,
                           const unsigned short,
                           const SupportedTypes::TypeSize,
                           const bool);
      UMATMaterialProperty(const UMATMaterialProperty&) = default;
      UMATMaterialProperty(UMATMaterialProperty&&) = default;
      UMATMaterialProperty& operator=(const UMATMaterialProperty&) = default;
      UMATMaterialProperty& operator=(UMATMaterialProperty&&) = default;
      ~UMATMaterialProperty();
      /*!
       *  Type of the variable.
       *  If the variable has been declared as an array (see below),
       *  this field holds the type contained by the array.
       */
      std::string type;
      /*!
       * \brief glossary name of the material property
       */
      std::string name;
      /*!
       * \brief variable name of the material property
       */
      std::string var_name;
      //! if the variable has been declared as an array, this field
      //  contains a value greater than 1
      unsigned short arraySize;
      /*!
       * offset of the variable in the indirection map
       */
      SupportedTypes::TypeSize offset;
      /*!
       * If true, this variable is not considered in the computation
       * of the indirection map. For example, the Cast3M solver may
       * require a variable called 'V1X' for the first component of
       * the first direction of the orthotropic basis.  The variable
       * shall not prevent the user of defining a material property
       * named 'V1X'.
       */
      bool dummy;
    };  // end of struct UMATMaterialProperty
    /*!
     * an helper function returning a material property
     * \param[in] mprops : material properties
     * \param[in] n      : glossary name
     * \return the umat material property with the given material
     * property name
     */
    static const UMATMaterialProperty& findUMATMaterialProperty(
        const std::vector<UMATMaterialProperty>&, const std::string&);
    /*!
     * \return a pair. If first is true, the "axial strain" was found
     * and the second contains its offset
     * \param[in] mb : material description
     */
    std::pair<bool, SupportedTypes::TypeSize>
    checkIfAxialStrainIsDefinedAndGetItsOffset(
        const BehaviourDescription&) const;

    /*!
     * \return the name of the generated library
     * \param[in] mb : behaviour description
     */
    virtual std::string getLibraryName(const BehaviourDescription&) const = 0;

    virtual bool readBooleanValue(const std::string&,
                                  tokens_iterator&,
                                  const tokens_iterator) const;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and whose second members is an offset giving the number of
     * imposed material properties.
     * \param[in] mb : behaviour description
     * \param[in] h  : modelling hypothesis
     */
    virtual std::pair<std::vector<UMATMaterialProperty>,
                      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
                                const Hypothesis) const = 0;
    /*!
     * \brief append a variable to material property list
     * \param[out] l : list of material properties
     * \param[in]  t : type of the variable
     * \param[in]  n : name of the variable
     * \param[in]  v : variable name
     * \param[in]  b : if true, this is a dummy variable
     */
    virtual void appendToMaterialPropertiesList(
        std::vector<UMATMaterialProperty>&,
        const std::string&,
        const std::string&,
        const std::string&,
        const bool) const;
    /*!
     * \brief complete the list of material properties.
     * This method shall be called by the buildMaterialPropertiesList
     * after that the mandatory material properties list have been
     * filled.
     * \param[out] mprops : list of material properties
     * \param[in]  mb     : behaviour description
     * \param[in]  h      : modelling hypothesis
     */
    virtual void completeMaterialPropertiesList(
        std::vector<UMATMaterialProperty>&,
        const BehaviourDescription&,
        const Hypothesis) const;
    /*!
     * \return the name of the function generated by the Cyrano interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string getFunctionName(const std::string&) const = 0;
    /*!
     * \return a name used to create symbols for the UMAT++ interface
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     * \param[in] h : modelling hypothesis
     */
    virtual std::string getSymbolName(const std::string&,
                                      const Hypothesis) const;
    /*!
     * \param[out] f      : output file
     * \param[out] first  : boolean stating if the an initializer has already
     * been declared \param[in]  h      : modelling hypothesis \param[in]  mb :
     * behaviour description \param[in]  v      : variables to be initialized
     * \param[in]  iv     : indirection vector
     * \param[in]  o      : offset in the indirection vector
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
        std::ostream&,
        bool& first,
        const Hypothesis,
        const BehaviourDescription&,
        const std::vector<UMATMaterialProperty>&,
        const SupportedTypes::TypeSize,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f      : output file
     * \param[in]  h      : modelling hypothesis
     * \param[in]  mb     : behaviour description
     * \param[in]  v      : variables to be initialized
     * \param[in]  iv     : indirection vector
     * \param[in]  o      : offset in the indirection vector
     * \param[in]  src    : name of the array from which the variables are
     * initialized \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void
    writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&,
        const std::vector<UMATMaterialProperty>&,
        const SupportedTypes::TypeSize,
        const std::string&,
        const std::string&,
        const std::string&) const;
    /*!
     * \param[out] f    : output stream
     * \param[in]  name : name of the umat function
     * \param[in]  h    : modelling hypothesis
     * \param[in]  v    : variables external names and array size
     * \param[in]  t    : class of variable treated used as the name for the
     * generated symbol (for example,  'InternalStateVariables') \param[in]  o
     * : offset
     */
    virtual void writeExternalNames(std::ostream&,
                                    const std::string&,
                                    const Hypothesis&,
                                    const std::vector<std::string>&,
                                    const std::string&) const;

    virtual void checkParametersType(bool&,
                                     bool&,
                                     bool&,
                                     const VariableDescriptionContainer&) const;
    /*!
     * \return the header define
     */
    virtual std::string getHeaderDefine(const BehaviourDescription&) const;

    virtual void getExtraSrcIncludes(std::ostream&,
                                     const BehaviourDescription&) const;

    virtual void writeVisibilityDefines(std::ostream&) const;

    virtual void writeSetOutOfBoundsPolicyFunctionDeclaration(
        std::ostream&, const std::string&) const;

    virtual void writeGetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&, const std::string&) const;

    virtual void writeSetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&, const std::string&) const;

    virtual void writeSetParametersFunctionsDeclarations(
        std::ostream&, const std::string&, const BehaviourDescription&) const;

    virtual void writeSetParametersFunctionsImplementations(
        std::ostream&, const std::string&, const BehaviourDescription&) const;

    /*!
     * write the first part of the code used to generate a MTest file
     */
    virtual void generateMTestFile1(std::ostream&) const;

    /*!
     * write the second part of the code used to generate a MTest file
     */
    virtual void generateMTestFile2(std::ostream&,
                                    const BehaviourDescription::BehaviourType,
                                    const std::string&,
                                    const std::string&,
                                    const BehaviourDescription&) const;

    virtual void writeMTestFileGeneratorSetRotationMatrix(
        std::ostream&, const BehaviourDescription&) const;
    /*!
     * \param[in] out: output file
     * \param[in] n:   name of the entry point
     */
    virtual void writeUMATxxEntryPointSymbol(std::ostream&,
                                             const std::string&) const;
    /*!
     * \param[in] out: output file
     * \param[in] n:   name of the entry point
     */
    virtual void writeUMATxxTFELVersionSymbol(std::ostream&,
                                              const std::string&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxMaterialSymbol(std::ostream&,
                                           const std::string&,
                                           const BehaviourDescription&) const;
    /*!
     * \param[in] out: output file
     * \param[in] n:   name of the entry point
     */
    virtual void writeUMATxxMaterialKnowledgeTypeSymbol(
        std::ostream&, const std::string&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxSpecificSymbols(std::ostream&,
                                            const std::string&,
                                            const BehaviourDescription&,
                                            const FileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void generateUMATxxGeneralSymbols(std::ostream&,
                                              const std::string&,
                                              const BehaviourDescription&,
                                              const FileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void generateUMATxxSymbols(std::ostream&,
                                       const std::string&,
                                       const Hypothesis,
                                       const BehaviourDescription&,
                                       const FileDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxBehaviourTypeSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const = 0;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxBehaviourKinematicSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const = 0;
    /*!
     * \brief write the symbols associated with the source file
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxSourceFileSymbols(std::ostream&,
                                              const std::string&,
                                              const BehaviourDescription&,
                                              const FileDescription&) const;
    /*!
     * \brief write the symbols associated with the interface name
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxInterfaceNameSymbols(std::ostream&,
                                                 const std::string&,
                                                 const BehaviourDescription&,
                                                 const FileDescription&) const;
    /*!
     * \param[in] out   : output file
     * \param[in] name  : name of the behaviour as defined by interface
     *                    (generally taking into account the material
     *                     and the behaviour name)
     * \param[in] h     : modelling hypothesis
     * \param[in] mb    : behaviour description
     */
    virtual void writeUMATxxIsUsableInPurelyImplicitResolutionSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \brief write UMAT++ symbols associated with supported modelling
     * hypothesis \param[in] out : output file \param[in] n   : name of the
     * behaviour as defined by interface (generally taking into account the
     * material and the behaviour name) \param[in] mb  : behaviour description
     */
    virtual void writeUMATxxSupportedModellingHypothesis(
        std::ostream&, const std::string&, const BehaviourDescription&) const;
    /*!
     * \brief write UMAT++ symbols associated with material properties for the
     * given hypothesis \param[in] out : output file \param[in] n   : name of
     * the behaviour as defined by interface (generally taking into account the
     * material and the behaviour name) \param[in] h   : modelling hypothesis
     * \param[in] mb  : behaviour description
     */
    virtual void writeUMATxxMaterialPropertiesSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxStateVariablesSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxExternalStateVariablesSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxParametersSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxParameterDefaultValueSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxBoundsSymbols(std::ostream&,
                                          const std::string&,
                                          const Hypothesis,
                                          const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxPhysicalBoundsSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxRequirementsSymbols(
        std::ostream&,
        const std::string&,
        const Hypothesis,
        const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxSymmetryTypeSymbols(
        std::ostream&, const std::string&, const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : behaviour name
     * \param[in] mb   : behaviour description
     */
    virtual void writeUMATxxElasticSymmetryTypeSymbols(
        std::ostream&, const std::string&, const BehaviourDescription&) const;
    /*!
     * \param[in] out  : output file
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     * \param[in] h    : modelling hypothesis
     * \param[in] mb   : behaviour description
     * \param[in] fd   : file description
     */
    virtual void writeUMATxxAdditionalSymbols(std::ostream&,
                                              const std::string&,
                                              const Hypothesis,
                                              const BehaviourDescription&,
                                              const FileDescription&) const = 0;
    /*!
     * \param[in] out : output file
     */
    virtual void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const = 0;
    /*!
     * \brief associates each distinct modelling hypothesis to appropriate tests
     * \param[in] mb : behaviour description
     */
    virtual std::map<Hypothesis, std::string> gatherModellingHypothesesAndTests(
        const BehaviourDescription&) const;
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : behaviour description
     */
    virtual bool isBehaviourConstructorRequired(
        const Hypothesis, const BehaviourDescription&) const override;
    //! \return true if the interface handles external state variables
    virtual bool areExternalStateVariablesSupported() const;
    //! \return true if the interface handles temperature increment
    virtual bool isTemperatureIncrementSupported() const;
    //! \return a test for the given modelling hypotesis
    virtual std::string getModellingHypothesisTest(const Hypothesis) const = 0;

    bool generateMTestFile = false;

  };  // end of struct UMATInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTUMATINTERFACEBASE_HXX */
