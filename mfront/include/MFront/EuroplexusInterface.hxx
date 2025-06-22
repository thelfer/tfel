/*!
 * \file   mfront/include/MFront/EuroplexusInterface.hxx
 * \brief  This file declares the EuroplexusInterface class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_EUROPLEXUSINTERFACE_HXX
#define LIB_MFRONT_EUROPLEXUSINTERFACE_HXX

#include <string>
#include <iosfwd>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief the interface the Europlexus Standard finite element solver
   */
  struct EuroplexusInterface : public UMATInterfaceBase {
    //! name of finite strain strategy attribute
    static const char *const finiteStrainStrategy;
    //! return the name of the interface
    static std::string getName();
    /*!
     * \return true if a finite strain strategy has been set up
     * \param[in] bd: behaviour description
     */
    static bool hasFiniteStrainStrategy(const BehaviourDescription &);
    /*!
     * \param[in,out] mb: behaviour description
     * \param[in] k  : keyword treated
     * \param[in] i:  list of interfaces to which the keyword is restricted
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
    ~EuroplexusInterface() override;

   protected:
    std::string getLibraryName(const BehaviourDescription &) const override;
    std::string getInterfaceName() const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream &, const BehaviourDescription &) const override;
    void writeBehaviourDataGradientSetter(
        std::ostream &,
        const Gradient &,
        const SupportedTypes::TypeSize) const override;
    void writeIntegrationDataGradientSetter(
        std::ostream &,
        const Gradient &,
        const SupportedTypes::TypeSize) const override;
    void exportThermodynamicForce(
        std::ostream &,
        const std::string &,
        const ThermodynamicForce &,
        const SupportedTypes::TypeSize) const override;
    std::string getFunctionNameBasis(const std::string &) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream &) const override;
    /*!
     * \brief write a  specialisation of the EuroplexusTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeEuroplexusBehaviourTraits(std::ostream &,
                                                const BehaviourDescription &,
                                                const Hypothesis) const;
    void writeInterfaceSpecificIncludes(
        std::ostream &, const BehaviourDescription &) const override;
    void writeBehaviourDataThermodynamicForceSetter(
        std::ostream &,
        const ThermodynamicForce &,
        const SupportedTypes::TypeSize) const override;
    /*!
     * \param[in] out  : output file
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeFiniteRotationSmallStrainCall(
        std::ostream &,
        const BehaviourDescription &,
        const std::string &) const;
    /*!
     * \param[in] out  : output file
     * \param[in] mb   : behaviour description
     * \param[in] name : name of the behaviour as defined by interface
     *                   (generally taking into account the material
     *                    and the behaviour name)
     */
    virtual void writeLogarithmicStrainCall(std::ostream &,
                                            const BehaviourDescription &,
                                            const std::string &) const;

    virtual std::string getModellingHypothesisTest(
        const Hypothesis) const override;

    virtual std::map<UMATInterfaceBase::Hypothesis, std::string>
    gatherModellingHypothesesAndTests(
        const BehaviourDescription &) const override;
    /*!
     * \return the list of modelling hypotheses treated by the interface
     * \param[in] mb : behaviour description
     */
    virtual std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription &) const override;
  };  // end of EuroplexusInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_EUROPLEXUSINTERFACE_HXX */
