/*!
 * \file   mfront/include/MFront/AsterInterface.hxx
 * \brief  This file declares the AsterInterface class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTASTERINTERFACE_HXX
#define LIB_MFRONTASTERINTERFACE_HXX

#include <string>
#include <iosfwd>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  struct AsterInterface : public UMATInterfaceBase {
    /*!
     * \brief name of the attribute used to known if saving the tangent operator
     * has been requested.
     */
    static const char *const saveTangentOperator;
    /*!
     * \brief name of the attribute used to known which finite strain
     * formulation has been choosen.
     */
    static const char *const finiteStrainFormulation;
    /*!
     * \brief name associated with the SIMO_MIEHE finite strain formulation.
     */
    static const char *const simo_miehe;
    /*!
     * \brief name associated with the GROT_GDEP finite strain formulation
     */
    static const char *const grot_gdep;
    //! \return the name of the interface
    static std::string getName();
    //! constructor
    AsterInterface();
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
    ~AsterInterface() override;

   protected:
    std::string getLibraryName(const BehaviourDescription &) const override;

    std::string getInterfaceName() const override;
    std::string getFunctionNameBasis(const std::string &) const override;
    /*!
     * \param[in] out : output file
     */
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream &) const override;
    /*!
     * \brief write a  specialisation of the AsterTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeAsterBehaviourTraits(std::ostream &,
                                           const BehaviourDescription &,
                                           const Hypothesis) const;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void writeInterfaceSpecificIncludes(
        std::ostream &, const BehaviourDescription &) const override;

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

    bool compareToNumericalTangentOperator = false;

    double strainPerturbationValue = 1.e-6;

    double tangentOperatorComparisonCriterion = 1.e7;

    bool errorReport = false;

  };  // end of AsterInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTASTERINTERFACE_HXX */
