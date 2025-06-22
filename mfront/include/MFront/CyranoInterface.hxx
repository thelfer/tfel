/*!
 * \file   mfront/include/MFront/CyranoInterface.hxx
 * \brief  This file declares the CyranoInterface class
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTCYRANOINTERFACE_HXX
#define LIB_MFRONTCYRANOINTERFACE_HXX

#include <string>
#include <iosfwd>

#include "MFront/InterfaceBase.hxx"
#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * Interface for behaviours for use in the Cast3M finite element
   * code.
   *
   * The name of this interface comes to the fact that the Cast3M
   * choose to use an interface for user defined behaviours closed to
   * the umat interface of the Abaqus finite element solver. However,
   * conventions of Cast3M makes those two interfaces incompatibles.
   */
  struct CyranoInterface : public UMATInterfaceBase, protected InterfaceBase {
    static std::string getName();

    CyranoInterface();

    void writeGetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&, const std::string&) const override;
    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    /*!
     * \param[in,out] bd: behaviour description
     * \param[in] k:  keyword treated
     * \param[in] i:  list of interfaces to which the keyword is restricted
     * \param[in] p:  iterator to the current token
     * \param[in] pe: iterator past the end of the file
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
    ~CyranoInterface() override;

   protected:
    static int getModellingHypothesisIdentifier(const Hypothesis);

    std::string getLibraryName(const BehaviourDescription&) const override;

    std::string getInterfaceName() const override;

    std::string getFunctionNameBasis(const std::string&) const override;

    std::string getBehaviourName(const std::string&, const std::string&) const;

    virtual std::string getUmatFunctionName(const std::string&,
                                            const std::string&) const;

    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;

    virtual void writeCyranoFunctionDeclaration(std::ostream&,
                                                const std::string&) const;

    /*!
     * \brief write the standard cyrano function
     * \param[in] out : ouptut file
     * \param[in] n   : name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     * \param[in] mb  : behaviour description
     */
    virtual void writeStandardCyranoFunction(std::ostream&,
                                             const std::string&,
                                             const BehaviourDescription&) const;

    /*!
     * \brief write the cyrano function for behaviour based on the Hencky strain
     * measure. The kinematic choosen is the ptest finite strain kinematic
     * (eto-pk1), which is also used by `Cyrano`. \param[in] out : ouptut file
     * \param[in] n   : name of the behaviour as defined by interface
     *                  (generally taking into account the material
     *                   and the behaviour name)
     * \param[in] mb  : behaviour description
     */
    virtual void writeLogarithmicStrainCyranoFunction(
        std::ostream&, const std::string&, const BehaviourDescription&) const;

    /*!
     * \brief write a  specialisation of the CyranoTraits class
     * \param[in] out : ouptut file
     * \param[in] mb  : behaviour description
     * \param[in] h   : modelling hypothesis
     */
    virtual void writeCyranoBehaviourTraits(std::ostream&,
                                            const BehaviourDescription&,
                                            const Hypothesis) const;

    std::string getModellingHypothesisTest(const Hypothesis) const override;
    /*!
     * \return the list of modelling hypotheses treated by the interface
     * \param[in] mb : behaviour description
     */
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;

    bool useTimeSubStepping;

    bool doSubSteppingOnInvalidResults;

    unsigned short maximumSubStepping;

  };  // end of CyranoInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTCYRANOINTERFACE_HXX */
