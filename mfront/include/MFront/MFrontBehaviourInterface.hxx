/*!
 * \file   mfront/include/MFront/MFrontBehaviourInterface.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   15/01/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOUR_INTERFACE_HXX
#define LIB_MFRONT_BEHAVIOUR_INTERFACE_HXX

#include "MFront/AbstractBehaviourInterface.hxx"

namespace mfront {

  //! \brief abstract interface of all behaviour interfaces
  struct MFRONT_VISIBILITY_EXPORT MFrontBehaviourInterface
      : AbstractBehaviourInterface {
    //
    static std::string getName();
    std::string getInterfaceName() const override;
    std::string getInterfaceVersion() const override;
    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription &,
        const std::string &,
        const std::vector<std::string> &,
        tokens_iterator,
        const tokens_iterator) override;
    bool isBehaviourConstructorRequired(
        const Hypothesis, const BehaviourDescription &) const override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription &) const override;
    std::pair<std::vector<BehaviourMaterialProperty>, SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription &,
                                const Hypothesis) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream &, const BehaviourDescription &) const override;
    void exportMechanicalData(std::ostream &,
                              const Hypothesis,
                              const BehaviourDescription &) const override;
    void writeBehaviourConstructorHeader(std::ostream &,
                                         const BehaviourDescription &,
                                         const Hypothesis,
                                         const std::string &) const override;
    void writeBehaviourConstructorBody(std::ostream &,
                                       const BehaviourDescription &,
                                       const Hypothesis) const override;
    void writeBehaviourDataConstructor(
        std::ostream &,
        const Hypothesis,
        const BehaviourDescription &) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream &, const BehaviourDescription &) const override;
    void writeIntegrationDataConstructor(
        std::ostream &,
        const Hypothesis,
        const BehaviourDescription &) const override;
    void writeIntegrationDataMainVariablesSetters(
        std::ostream &, const BehaviourDescription &) const override;
    void writeBehaviourInitializeFunctions(std::ostream &,
                                           const BehaviourDescription &,
                                           const Hypothesis) const override;
    void writeBehaviourPostProcessings(std::ostream &,
                                       const BehaviourDescription &,
                                       const Hypothesis) const override;
    void endTreatment(const BehaviourDescription &,
                      const FileDescription &) const override;
    void getTargetsDescription(TargetsDescription &,
                               const BehaviourDescription &) override;
    //! \brief destructor
    ~MFrontBehaviourInterface() override;
  };  // end of AbstractBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX */
