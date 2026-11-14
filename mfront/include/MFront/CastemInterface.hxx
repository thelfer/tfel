/*!
 * \file   mfront/include/MFront/CastemInterface.hxx
 * \brief  This file declares the CastemInterface class
 * \author Thomas Helfer
 * \date 17/01/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_CASTEMINTERFACE_HXX
#define LIB_MFRONT_CASTEMINTERFACE_HXX

#include <map>
#include <string>
#include <vector>
#include <iosfwd>
#include <cstddef>

#include "MFront/InterfaceBase.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief Interface for behaviours for use in the Cast3M finite element
   * code.
   *
   * The name of this interface comes to the fact that the Cast3M
   * choose to use an interface for user defined behaviours closed to
   * the umat interface of the Abaqus finite element solver. However,
   * conventions of Cast3M makes those two interfaces incompatibles.
   */
  struct CastemInterface : public UMATInterfaceBase, protected InterfaceBase {
    static const char* const finiteStrainStrategies;

    static const char* const useTimeSubStepping;

    static const char* const doSubSteppingOnInvalidResults;

    static const char* const maximumSubStepping;
    //! \return the name of the interface
    static std::string getName();
    /*!
     * \return true if the interface shall support plane stress
     * hypothesis through a generic algorithm.
     * \return false if the behaviour handles the plane stress
     * hypothesis or if the behaviour don't handle the generalised
     * plane strain hypothesis.
     */
    static bool usesGenericPlaneStressAlgorithm(const BehaviourDescription&);
    /*!
     * \brief write the arguments of the `UMAT` function.
     * \note This function is meant to be used for declaring `UMAT` functions as
     * the variables of the function are not named
     * \param[in] os: output stream
     */
    static void writeUMATFunctionArguments(std::ostream&);
    /*!
     *\brief write the arguments of the `UMAT` function
     * \param[in] os: output stream
     * \param[in] t: behaviour type
     */
    static void writeUMATFunctionArguments(
        std::ostream&, const BehaviourDescription::BehaviourType&);
    //! \brief default constructor
    CastemInterface();
    //
    [[nodiscard]] std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    //! \brief destructor
    ~CastemInterface() override;

   protected:
    //
    struct UniqueCast3MIdentifiersGenerator {
      UniqueCast3MIdentifiersGenerator();
      //
      [[nodiscard]] std::string treatScalar(const std::string&,
                                            const std::string&);
      [[nodiscard]] std::string treatScalar(const std::string&,
                                            const std::string&,
                                            const unsigned short);
      [[nodiscard]] std::string treatTVector(const Hypothesis,
                                             const std::string&,
                                             const std::string&);
      [[nodiscard]] std::string treatTVector(const Hypothesis,
                                             const std::string&,
                                             const std::string&,
                                             const unsigned short);
      [[nodiscard]] std::string treatStensor(const Hypothesis,
                                             const std::string&,
                                             const std::string&);
      [[nodiscard]] std::string treatStensor(const Hypothesis,
                                             const std::string&,
                                             const std::string&,
                                             const unsigned short);
      [[nodiscard]] std::string treatTensor(const Hypothesis,
                                            const std::string&,
                                            const std::string&);
      [[nodiscard]] std::string treatTensor(const Hypothesis,
                                            const std::string&,
                                            const std::string&,
                                            const unsigned short);

     private:
      [[nodiscard]] bool isIdAlreadyUsed(const std::string&) const;
      [[nodiscard]] std::string getUniqueId(const std::string&,
                                            const std::string&,
                                            const std::string&);
      [[nodiscard]] std::string getQuotedId(const std::string&,
                                            const std::string&);
      [[nodiscard]] std::string getQuotedId(const std::string&,
                                            const std::string&,
                                            const unsigned short);
      [[nodiscard]] std::string getQuotedIds(const std::string&,
                                             const std::string&,
                                             const std::vector<std::string>&);
      [[nodiscard]] std::string getQuotedIds(const std::string&,
                                             const std::string&,
                                             const unsigned short,
                                             const std::vector<std::string>&);
      /*!
       * \brief a map giving the generated Cast3M identifier for a given
       * external name (including array index and component)
       */
      std::map<std::string, std::string> ids;
      //! \brief counter used to generate unique ids
      std::size_t count = std::size_t{};
    };
    //
    [[nodiscard]] std::string getLibraryName(
        const BehaviourDescription&) const override;
    [[nodiscard]] std::string getInterfaceName() const override;
    [[nodiscard]] std::string getFunctionNameBasis(
        const std::string&) const override;
    [[nodiscard]] std::string getBehaviourName(
        const BehaviourDescription&) const;
    [[nodiscard]] std::string getModellingHypothesisTest(
        const Hypothesis) const override;
    [[nodiscard]] std::map<UMATInterfaceBase::Hypothesis, std::string>
    gatherModellingHypothesesAndTests(
        const BehaviourDescription&) const override;
    [[nodiscard]] std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    [[nodiscard]] std::pair<std::vector<BehaviourMaterialProperty>,
                            SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription&,
                                const Hypothesis) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;
    void writeGetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&) const override;
    //
    [[nodiscard]] virtual std::string getUmatFunctionName(
        const BehaviourDescription&) const;
    /*!
     * \return the list of material properties required by the `Cast3M` finite
     * element solver.
     * \param[in] mb: mechanical behaviour description
     * \param[in] h: modelling hypothesis
     */
    [[nodiscard]] virtual std::vector<BehaviourMaterialProperty>
    getDefaultMaterialPropertiesList(const BehaviourDescription&,
                                     const Hypothesis) const;

    /*!
     * \brief write the set out of bounds policy function for an alias
     * behaviour \param[out] out   : output stream \param[out] name  : name of
     * the alias \param[out] name2 : name of the original behaviour
     */
    virtual void writeSetOutOfBoundsPolicyFunctionImplementation2(
        std::ostream&, const std::string&, const std::string&) const;
    /*!
     * \brief generate an example of an input file
     * \param[in] bd: behaviour description
     * \param[in] h: modelling hypothesis
     */
    virtual void generateInputFileExample(const BehaviourDescription&,
                                          const FileDescription&) const;
    /*!
     * \brief generate an example of how to use the behaviour for the given
     * hypothesis. This method is called internally by the
     * `generateInputFileExample` method.
     * \param[out] out: output stream
     * \param[in] bd: behaviour description
     * \param[in] h: modelling hypothesis
     */
    virtual void generateInputFileExampleForHypothesis(
        std::ostream&, const BehaviourDescription&, const Hypothesis) const;

    virtual void writeVariableDescriptionContainerToGibiane(
        std::ostream&,
        UniqueCast3MIdentifiersGenerator&,
        const Hypothesis,
        const VariableDescriptionContainer&) const;

    virtual void writeVariableDescriptionsToGibiane(
        std::ostream&,
        UniqueCast3MIdentifiersGenerator&,
        const Hypothesis,
        const VariableDescriptionContainer::const_iterator,
        const VariableDescriptionContainer::const_iterator) const;

    virtual void writeGibianeMappingComments(
        std::ostream&,
        UniqueCast3MIdentifiersGenerator&,
        const Hypothesis,
        const VariableDescriptionContainer&) const;

    virtual void writeGibianeMappingComments(
        std::ostream&,
        UniqueCast3MIdentifiersGenerator&,
        const std::pair<std::vector<BehaviourMaterialProperty>,
                        SupportedTypes::TypeSize>&) const;

    virtual void writeGibianeInstruction(std::ostream&,
                                         const std::string&) const;

    virtual void writeCastemFunctionDeclaration(std::ostream&,
                                                const std::string&) const;
    /*!
     * \brief an helper function
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviour description
     * \param[in]  m       : méthod called to write the function
     */
    virtual void writeCastemFunction(
        std::ostream&,
        const std::string&,
        const std::string&,
        const BehaviourDescription&,
        void (CastemInterface::*)(std::ostream&,
                                  const std::string&,
                                  const std::string&,
                                  const std::string&,
                                  const BehaviourDescription&) const) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  fname   : function name
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void writeFiniteRotationSmallStrainCastemFunction(
        std::ostream&,
        const std::string&,
        const std::string&,
        const std::string&,
        const BehaviourDescription&) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  fname   : function name
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void writeMieheApelLambrechtLogarithmicStrainCastemFunction(
        std::ostream&,
        const std::string&,
        const std::string&,
        const std::string&,
        const BehaviourDescription&) const;
    /*!
     * \brief write the implementation of the umat function
     * corresponding to the LOGARITHMICSTRAIN1D finite strain strategy
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  fname   : function name
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviourd description
     */
    virtual void writeLogarithmicStrain1DCastemFunction(
        std::ostream&,
        const std::string&,
        const std::string&,
        const std::string&,
        const BehaviourDescription&) const;
    /*!
     * \param[out] out     : output file
     * \param[in]  name    : name of law
     * \param[in]  fname   : function name
     * \param[in]  suffix  : suffix library
     * \param[in]  mb      : mechanical behaviour description
     */
    virtual void writeStandardCastemFunction(std::ostream&,
                                             const std::string&,
                                             const std::string&,
                                             const std::string&,
                                             const BehaviourDescription&) const;
    /*!
     * \brief return a string used to initialize the offset of the material
     * properties when declaring the `CastemTraits` class.
     * \param[in]  mb: mechanical behaviour description
     */
    [[nodiscard]] virtual std::string
    getMaterialPropertiesOffsetForBehaviourTraits(
        const BehaviourDescription&) const;
    /*!
     * \brief write a  specialisation of the `CastemTraits` class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeCastemBehaviourTraits(std::ostream&,
                                            const BehaviourDescription&,
                                            const Hypothesis) const;
    /*!
     * \brief plane stress handling requires to have access to the
     * axial strain, but it is not obvious nor always possible to get it...
     * This method handles all the cases.
     * \return false if the axial strain can't be initialized.
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] c: '0' (beginning of time step) or '1' (end of time
     * step)
     */
    [[nodiscard]] virtual bool writeInitializeAxialStrain(
        std::ostream&, const BehaviourDescription&, const char) const;
    /*!
     * \brief plane stress handling requires to have access to the
     * axial strain, but it is not obvious nor always possible to get it...
     * This method handles all the cases.
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] c   : beginning of the call
     * \param[in] c2  : code computing the axial deformation gradient from the
     * axial strain
     * \param[in] c3: '0' (beginning of time step) or '1' (end of time
     * step)
     */
    virtual void writeFiniteStrainStrategiesPlaneStressSpecificCall(
        std::ostream&,
        const BehaviourDescription&,
        const std::string&,
        const std::string&,
        const char) const;
    /*!
     * \return true if the interface supports the given modelling
     * hypothesis.
     *
     * \note the Cast3M interface may support modelling hypotheses
     * not intrinsically supported by the behaviour (plane stress case
     * for strain based behaviours).
     *
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : behaviour description
     */
    [[nodiscard]] virtual bool isModellingHypothesisSupported(
        const Hypothesis, const BehaviourDescription&) const;
  };  // end of CastemInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_CASTEMINTERFACE_HXX */
