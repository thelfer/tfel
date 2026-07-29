/*!
 * \file   mfront/include/ComsolInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/07/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_COMSOLINTERFACE_HXX
#define LIB_MFRONT_COMSOLINTERFACE_HXX

#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  //! \brief the interface to Comsol
  struct ComsolInterface : public UMATInterfaceBase {
    //! \return the name of the interface
    [[nodiscard]] static std::string getName();
    //
    [[nodiscard]] std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    [[nodiscard]] std::string getInterfaceName() const override;
    //! \brief destructor
    ~ComsolInterface() override;

   protected:
    [[nodiscard]] std::string getFunctionNameBasis(
        const std::string&) const override;
    [[nodiscard]] std::string getLibraryName(
        const BehaviourDescription&) const override;
    [[nodiscard]] bool areExternalStateVariablesSupported() const override;
    [[nodiscard]] bool isTemperatureIncrementSupported() const override;
    [[nodiscard]] std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;
  };  // end of struct ComsolInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_COMSOLINTERFACE_HXX */
