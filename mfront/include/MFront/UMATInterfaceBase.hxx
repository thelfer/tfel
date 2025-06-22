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

#include <string_view>
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/VariableDescription.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"

namespace mfront {

  // forward declaration
  struct VariableDescriptionContainer;
  // forward declaration
  struct Gradient;
  // forward declaration
  struct ThermodynamicForce;

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the UMAT standard or its derivative:
   * - `Abaqus/Standard`
   * - `Abaqus/Explicit`
   * - `Code_Aster`
   * - `CalculiX`
   * - `Cyrano`
   * - `Europlexus`
   * - etc..
   */
  struct MFRONT_VISIBILITY_EXPORT UMATInterfaceBase
      : public StandardBehaviourInterface {
    //! \brief constructor
    UMATInterfaceBase();

    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;
    /*!
     * This method check of the temperature is defined as the first external
     * state variable. If this is not the case, an exception is thrown.
     *
     * \note The temperature must be defined as the first external state
     * variable in many interfaces.
     *
     * \param[in] bd: behaviour description
     */
    void checkIfTemperatureIsDefinedAsTheFirstExternalStateVariable(
        const BehaviourDescription&) const;
    /*!
     * \param[out] f      : output file
     * \param[out] first  : true if not initializer has been declared yet
     * \param[in]  v      : variables to be initialized
     * \param[in]  src    : name of the array from which the variables are
     * initialized
     * \param[in]  prefix : prefix added to variable's names
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
     * initialized
     * \param[in]  prefix : prefix added to variable's names
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
     * initialized
     * \param[in]  prefix : prefix added to variable's names
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
     * initialized
     * \param[in]  prefix : prefix added to variable's names
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
     * array
     * \param[out] out : output stream
     * \param[in]  a   : array name
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
    void writeBehaviourConstructorHeader(std::ostream&,
                                         const BehaviourDescription&,
                                         const Hypothesis,
                                         const std::string&) const override;
    void writeBehaviourConstructorBody(std::ostream&,
                                       const BehaviourDescription&,
                                       const Hypothesis) const override;
    void writeBehaviourDataConstructor(
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

    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataGradientSetter(
        std::ostream&, const Gradient&, const SupportedTypes::TypeSize) const;
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

    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    virtual void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeIntegrationDataGradientSetter(
        std::ostream&, const Gradient&, const SupportedTypes::TypeSize) const;
    //! destructor
    ~UMATInterfaceBase() override;

   protected:
    /*!
     * \return the name of the generated library
     * \param[in] mb : behaviour description
     */
    virtual std::string getLibraryName(const BehaviourDescription&) const = 0;

    /*!
     * \param[out] f      : output file
     * \param[out] first  : boolean stating if the an initializer has already
     * been declared
     * \param[in]  h      : modelling hypothesis
     * \param[in]  mb     : behaviour description
     * \param[in]  v      : variables to be initialized
     * \param[in]  iv     : indirection vector
     * \param[in]  o      : offset in the indirection vector
     * \param[in]  src    : name of the array from which the variables are
     * initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void writeMaterialPropertiesInitializersInBehaviourDataConstructorI(
        std::ostream&,
        bool& first,
        const Hypothesis,
        const BehaviourDescription&,
        const std::vector<BehaviourMaterialProperty>&,
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
     * initialized
     * \param[in]  prefix : prefix added to variable's names
     * \param[in]  suffix : suffix added to variable's names
     */
    virtual void
    writeMaterialPropertiesInitializersInBehaviourDataConstructorII(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&,
        const std::vector<BehaviourMaterialProperty>&,
        const SupportedTypes::TypeSize,
        const std::string&,
        const std::string&,
        const std::string&) const;

    virtual void getExtraSrcIncludes(std::ostream&,
                                     const BehaviourDescription&) const;

    /*!
     * \brief write the first part of the code used to generate a MTest file
     * \param[out] out: output stream
     * \param[in] bd: behaviour description
     */
    virtual void generateMTestFile1(std::ostream&,
                                    const BehaviourDescription&) const;
    /*!
     * \brief write the second part of the code used to generate a MTest file.
     * This method shall be called when only one function is generated for all
     * modelling hypotheses. This methods calls the
     * `gatherModellingHypothesesAndTests` to make an appropriate dispatch among
     * hypotheses. For each hypothesis, the `generateMTestFileForHypothesis` is
     * called.
     * \param[out] out: output stream
     * \param[out] bd: behaviour description
     */
    virtual void generateMTestFile2(std::ostream&,
                                    const BehaviourDescription&,
                                    const BehaviourDescription::BehaviourType,
                                    const std::string&,
                                    const std::string&) const;
    /*!
     * \brief write the code used to generate a MTest file for a given hypotesis
     * \param[out] out: output stream
     * \param[out] bd: behaviour description
     */
    virtual void generateMTestFileForHypothesis(
        std::ostream&,
        const BehaviourDescription&,
        const BehaviourDescription::BehaviourType,
        const std::string&,
        const std::string&,
        const Hypothesis) const;

    virtual void writeMTestFileGeneratorSetRotationMatrix(
        std::ostream&, const BehaviourDescription&) const;
    /*!
     * \param[in] out : output file
     */
    virtual void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const = 0;
    /*!
     * \brief When the interface only provide one entry point (i.e. does not
     * provide an entry point by modelling hypothesis), the generation of
     * `MTest` files, provided by the `generateMTestFile2` method, must have
     * access to tests to distinguish modelling hypotheses.
     * \return a map associating each distinct modelling hypothesis to
     * appropriate tests.
     * This method is called internally by `generateMTestFile2` method.
     * \param[in] mb : behaviour description
     */
    virtual std::map<Hypothesis, std::string> gatherModellingHypothesesAndTests(
        const BehaviourDescription&) const;
    /*!
     * \return a test for the given modelling hypotesis
     * This method is called by the `gatherModellingHypothesesAndTests`.
     * As this method is only meaningfull for interfaces which only provide one
     * entry point (i.e. does not provide an entry point by modelling
     * hypothesis), the default implementation throws an exception.
     */
    virtual std::string getModellingHypothesisTest(const Hypothesis) const;
    //! \return true if the interface handles external state variables
    virtual bool areExternalStateVariablesSupported() const;
    //! \return true if the interface handles temperature increment
    virtual bool isTemperatureIncrementSupported() const;
  };  // end of struct UMATInterfaceBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTUMATINTERFACEBASE_HXX */
