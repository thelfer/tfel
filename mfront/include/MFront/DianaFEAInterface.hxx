/*!
 * \file   mfront/include/MFront/DianaFEAInterface.hxx
 * \brief  This file declares the DianaFEAInterface class
 * \author Thomas Helfer
 * \date   08/01/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DIANAFEAINTERFACE_HXX
#define LIB_MFRONT_DIANAFEAINTERFACE_HXX

#include <string>
#include <iosfwd>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief the interface the DianaFEA Standard finite element solver
   */
  struct DianaFEAInterface : public UMATInterfaceBase {
    //! name of finite strain strategy attribute
    static const char *const finiteStrainStrategy;
    //! \return the name of the interface
    static std::string getName();
    /*!
     * \param[in,out] mb: behaviour description
     * \param[in] k  : keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p  : iterator to the current token
     * \param[in] pe : iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription &,
        const std::string &,
        const std::vector<std::string> &,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    void endTreatment(const BehaviourDescription &,
                      const FileDescription &) const override;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    void getTargetsDescription(TargetsDescription &,
                               const BehaviourDescription &) override;
    //! destructor
    ~DianaFEAInterface() override;

   protected:
    std::string getInterfaceName() const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeBehaviourDataGradientSetter(
        std::ostream &,
        const Gradient &,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the initialisation of a driving variables
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    virtual void writeIntegrationDataGradientSetter(
        std::ostream &,
        const Gradient &,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \brief write the instruction of exporting a thermodynamic force in an
     * array
     * \param[out] out : output stream
     * \param[in]  a   : array name
     * \param[in]  f   : thermodynamic force
     * \param[in]  o   : thermodynamic force offset
     */
    virtual void exportThermodynamicForce(
        std::ostream &,
        const std::string &,
        const ThermodynamicForce &,
        const SupportedTypes::TypeSize) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void writeInterfaceSpecificIncludes(
        std::ostream &, const BehaviourDescription &) const override;
    /*!
     * \brief write the initialisation of a thermodynamic force
     * \param[in] os : output file
     * \param[in] v  : variable to be initialised
     * \param[in] o  : variable offsert
     */
    void writeBehaviourDataThermodynamicForceSetter(
        std::ostream &,
        const ThermodynamicForce &,
        const SupportedTypes::TypeSize) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream &) const override;
    std::string getModellingHypothesisTest(const Hypothesis) const override;
    std::map<UMATInterfaceBase::Hypothesis, std::string>
    gatherModellingHypothesesAndTests(
        const BehaviourDescription &) const override;
    std::string getFunctionNameBasis(const std::string &) const override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription &) const override;
    std::string getLibraryName(const BehaviourDescription &) const override;
    void writeGetOutOfBoundsPolicyFunctionImplementation(
        std::ostream &, const std::string &) const override;
    bool areExternalStateVariablesSupported() const override;
    bool isTemperatureIncrementSupported() const override;
    /*!
     * \return the input file example
     * \param[in] mb: behaviour description
     * \param[in] fd: file description
     * \param[in] b: if true, write the example for
     * DianaFEA-Standard. If false, write the example for
     * DianaFEA-Explicit
     */
    virtual void writeInputFileExample(const BehaviourDescription &,
                                       const FileDescription &,
                                       const bool) const;
    /*!
     * \brief write a trait class used for tag dispatching
     * \param[out] os: output stream
     * \param[in] bd: behaviour description
     */
    virtual void writeBehaviourTraits(std::ostream &,
                                      const BehaviourDescription &) const;
  };  // end of DianaFEAInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_DIANAFEAINTERFACE_HXX */
