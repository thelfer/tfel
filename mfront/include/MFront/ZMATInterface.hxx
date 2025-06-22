/*!
 * \file  mfront/include/MFront/ZMATInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 23 mai 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MFRONTZMATINTERFACE_HXX
#define LIB_MFRONT_MFRONTZMATINTERFACE_HXX

#include <set>
#include <string>

#include "TFEL/Utilities/CxxTokenizer.hxx"

#include "MFront/SupportedTypes.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on the umat standard
   */
  struct ZMATInterface : public SupportedTypes,
                         public AbstractBehaviourInterface {
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! \return the name of the interface
    static std::string getName();

    ZMATInterface();

    //! \return the name of the interface
    std::string getInterfaceName() const override;
    /*!
     * \param[in,out] mb: behaviour description
     * \param[in] k:   keyword treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p:   iterator to the current token
     * \param[in] pe:  iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : mechanical behaviour description
     */
    bool isBehaviourConstructorRequired(
        const Hypothesis, const BehaviourDescription&) const override;

    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;

    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;
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
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;

    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    /*!
     * write the initialisation of the driving variables
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \brief write output files
     * \param[in] mb        : mechanical behaviour description
     * \param[in] fd        : mfront file description
     */
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    //! destructor
    ~ZMATInterface() override;

   protected:
    /*!
     * \brief write behaviour initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void writeBehaviourInitialisation(std::ostream&,
                                      const BehaviourDescription&,
                                      const ZMATInterface::Hypothesis) const;
    /*!
     * \brief write parameters initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void writeParametersInitialisation(std::ostream&,
                                       const BehaviourDescription&,
                                       const ZMATInterface::Hypothesis) const;
    /*!
     * \brief write material properties initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void writeMaterialPropertiesInitialisation(
        std::ostream&,
        const BehaviourDescription&,
        const ZMATInterface::Hypothesis) const;
    /*!
     * \brief write behaviour initialisation for the given hypothesis
     * \param[out] out : output file
     * \param[in]  mb  : mechancial behaviour description
     * \param[in]  h   : modelling hypothesis
     */
    void writeCallMFrontBehaviour(std::ostream&,
                                  const BehaviourDescription&,
                                  const ZMATInterface::Hypothesis) const;

  };  // end of struct ZMATInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTZMATINTERFACE_HXX */
