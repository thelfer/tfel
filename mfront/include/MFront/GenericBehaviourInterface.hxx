/*!
 * \file   mfront/include/GenericBehaviourInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/06/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX
#define LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX

#include "UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * \brief a generic behaviour interface
   * See https://github.com/thelfer/MFrontGenericInterfaceSupport
   */
  struct GenericBehaviourInterface : public UMATInterfaceBase {
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
    void endTreatment(
        const BehaviourDescription&,
        const FileDescription&) const override;
    void getTargetsDescription(
        TargetsDescription&,
        const BehaviourDescription&) override;
    //! destructor
    ~GenericBehaviourInterface() override;

   protected:
    std::string getLibraryName(const BehaviourDescription&) const override;
    std::string getFunctionNameBasis(const std::string&) const override;
    /*!
     * \return the name of the function generated for the given hypothesis
     * \param[in] base name for the function to be generated
     */
    virtual std::string getFunctionNameForHypothesis(const std::string&,
                                             const Hypothesis) const;
    void writeUMATxxBehaviourTypeSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const override;
    void writeUMATxxBehaviourKinematicSymbols(
        std::ostream&,
        const std::string&,
        const BehaviourDescription&) const override;
    void writeUMATxxAdditionalSymbols(std::ostream&,
                                      const std::string&,
                                      Hypothesis,
                                      const BehaviourDescription&,
                                      const FileDescription&) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;
  };  // end of struct GenericBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOURINTERFACE_HXX */
