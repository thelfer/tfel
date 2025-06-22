/*!
 * \file  BehaviourQuery.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURQUERY_HXX
#define LIB_MFRONT_BEHAVIOURQUERY_HXX

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/QueryHandlerBase.hxx"

namespace mfront {

  // forward declaration
  struct AbstractBehaviourDSL;
  // forward declaration
  struct FileDescription;
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct BehaviourData;
  // forward declaration
  struct VariableDescriptionContainer;

  /*!
   * \brief class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct BehaviourQuery final : public QueryHandlerBase {
    /*!
     * build a BehaviourQuery object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : input file
     */
    BehaviourQuery(const int,
                   const char* const* const,
                   std::shared_ptr<AbstractBehaviourDSL>,
                   const std::string&);
    //! \brief treat the requests
    virtual void exe();
    //! \brief destructor
    ~BehaviourQuery() override;

   protected:
    //
    std::shared_ptr<const AbstractDSL> getDSL() const override;
    void registerCommandLineCallBacks() override;

   private:
    //! \brief a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! \brief a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! \brief a simple alias
    using query = std::function<void(
        const FileDescription&, const BehaviourDescription&, const Hypothesis)>;
    //! \brief a simple alias
    using query2 = std::function<void(const FileDescription&,
                                      const BehaviourDescription&)>;
    //! \brief treat a standard query
    virtual void treatStandardQuery() final;
    //! \brief treat a standard query (an option to the command line
    //! \brief triggering the query is required)
    virtual void treatStandardQuery2() final;
    //
    void treatGeneratedSources() override final;
    void treatSpecificTargetGeneratedSources() override final;
    void treatAllSpecificTargetsGeneratedSources() override final;
    void treatCppFlags() override final;
    void treatGeneratedHeaders() override final;
    void treatLibrariesDependencies() override final;
    void treatSpecificTargets() override final;
    void treatDSLTarget() override final;
    //! \brief treat the "--modelling-hypothesis" command line argument
    virtual void treatModellingHypothesis() final;
    //! \return a query that show a list of variables
    template <const VariableDescriptionContainer& (BehaviourData::*m)() const>
    query generateVariablesListQuery();
    //! \brief all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! \brief all the registred queries
    std::vector<std::pair<std::string, query2>> queries2;
    //! \brief abstract behaviour dsl
    std::shared_ptr<AbstractBehaviourDSL> dsl;
    //! \brief file name
    std::string file;
    //! \brief modelling hypothesis
    Hypothesis hypothesis = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  };  // end of struct BehaviourQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURQUERY_HXX */
