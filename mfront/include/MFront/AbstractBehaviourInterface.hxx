/*!
 * \file   mfront/include/MFront/AbstractBehaviourInterface.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   16 jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX
#define LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX

#include <map>
#include <set>
#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "MFront/MFrontConfig.hxx"
#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"

namespace mfront {

  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct FileDescription;
  // forward declartion
  struct TargetsDescription;

  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractBehaviourInterface {
    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef ModellingHypothesis::Hypothesis Hypothesis;
    //! a simple alias
    using CxxTokenizer = tfel::utilities::CxxTokenizer;
    //! a simple alias
    using tokens_iterator = CxxTokenizer::TokensContainer::const_iterator;
    //! \return the name of the interface
    virtual std::string getInterfaceName() const = 0;
    /*!
     * \brief treat a keyword
     * \param[in,out] bd: behaviour description
     * \param[in]     key: keyword to be treated
     * \param[in] i:   list of interfaces to which the keyword is restricted
     * \param[in] p:   current position in the file
     * \param[in] pe:  iterator past the end of the file
     * \return a pair. The first entry is true if the keyword was
     * treated by the interface. The second entry is an iterator after
     * the last token treated.
     */
    virtual std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) = 0;
    /*!
     * \return true if the interface will create
     * constructors in the behaviour class.
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : behaviour description
     */
    virtual bool isBehaviourConstructorRequired(
        const Hypothesis, const BehaviourDescription&) const = 0;
    /*!
     * \return the list of modelling hypotheses treated by the interface
     * \param[in] mb : behaviour description
     */
    virtual std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const = 0;

    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : behaviour description
     */
    virtual void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const = 0;

    virtual void exportMechanicalData(std::ostream&,
                                      const Hypothesis,
                                      const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void writeBehaviourConstructor(std::ostream&,
                                           const BehaviourDescription&,
                                           const std::string&) const = 0;

    virtual void writeBehaviourDataConstructor(
        std::ostream&, const Hypothesis, const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : behaviour description
     */
    virtual void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const = 0;

    virtual void writeIntegrationDataConstructor(
        std::ostream&, const Hypothesis, const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : behaviour description
     */
    virtual void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const = 0;
    /*!
     * \brief write output files
     * \param[in] mb : behaviour description
     * \param[in] fd : mfront file description
     */
    virtual void endTreatment(const BehaviourDescription&,
                              const FileDescription&) const = 0;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    virtual void getTargetsDescription(TargetsDescription&,
                                       const BehaviourDescription&) = 0;
    //! destructor
    virtual ~AbstractBehaviourInterface();

  };  // end of AbstractBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX */
