/*!
 * \file   mfront/include/GenericBehaviourInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX
#define LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX

#include "MFront/StandardBehaviourInterface.hxx"

namespace mfront {

  /*!
   * \brief a generic behaviour interface
   * See https://github.com/thelfer/MFrontGenericInterfaceSupport
   */
  struct GenericBehaviourInterface : public StandardBehaviourInterface {
    //! \return the interface name
    static std::string getName();
    std::string getInterfaceName() const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;

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
    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;

    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;

    std::string getFunctionNameBasis(const std::string&) const override;

    virtual std::string getLibraryName(const BehaviourDescription&) const;

    //! destructor
    ~GenericBehaviourInterface() override;

   protected:
    /*!
     * \return the name of the function generated for the given hypothesis
     * \param[in] base name for the function to be generated
     */
    virtual std::string getFunctionNameForHypothesis(const std::string&,
                                                     const Hypothesis) const;
    /*!
     * \brief write the generation of an MTest file for the given hypothesis
     * \param[out] out: output stream
     * \param[in]  bd: behaviour description
     * \param[in]  h: modelling hypothesis
     */
    virtual void generateMTestFile(std::ostream&,
                                   const BehaviourDescription&,
                                   const Hypothesis) const;

  };  // end of struct GenericBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX */
