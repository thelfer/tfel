/*!
 * \file   mfront/include/ComsolInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   30/07/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_COMSOLINTERFACE_HXX
#define LIB_MFRONT_COMSOLINTERFACE_HXX

#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  //! \brief the interface to Comsol
  struct ComsolInterface : public UMATInterfaceBase {
    //! \return the name of the interface
    static std::string getName();

    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription &,
        const std::string &,
        const std::vector<std::string> &,
        tokens_iterator,
        const tokens_iterator) override;

    void endTreatment(const BehaviourDescription &,
                      const FileDescription &) const override;

    void getTargetsDescription(TargetsDescription &,
                               const BehaviourDescription &) override;

    std::string getInterfaceName() const override;

    //! destructor
    ~ComsolInterface() override;

   protected:
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription &) const override;

    void writeInterfaceSpecificIncludes(
        std::ostream &, const BehaviourDescription &) const override;

    std::string getFunctionNameBasis(const std::string &) const override;

    std::string getLibraryName(const BehaviourDescription &) const override;

    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream &) const override;

    bool areExternalStateVariablesSupported() const override;

    bool isTemperatureIncrementSupported() const override;

    /*!
     * \brief write the implementation of the `init` function
     * \param[in, out] out: output stream
     * \param[in] bd: behaviour description
     */
    virtual void writeInitFunctionImplementation(
        std::ostream &, const BehaviourDescription &) const;
    /*!
     * \brief write the implementation of the `eval` function
     * \param[in, out] out: output stream
     * \param[in] bd: behaviour description
     * \param[in] name: name of behaviour's implementation
     */
    virtual void writeEvalFunctionImplementation(std::ostream &,
                                                 const BehaviourDescription &,
                                                 const std::string &) const;
    /*!
     * \brief write the implementation of the `cleanup` function
     * \param[in, out] out: output stream
     * \param[in] bd: behaviour description
     * \param[in] name: name of behaviour's implementation
     */
    virtual void writeCleanUpFunctionImplementation(
        std::ostream &, const BehaviourDescription &) const;

  };  // end of struct ComsolInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_COMSOLINTERFACE_HXX */
