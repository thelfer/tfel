/*!
 * \file  BehaviourQuery.hxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_BEHAVIOURQUERY_H
#define LIB_MFRONT_BEHAVIOURQUERY_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MFrontBase.hxx"

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
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct BehaviourQuery final
      : public tfel::utilities::ArgumentParserBase<BehaviourQuery>,
        public MFrontBase {
    /*!
     * build a BehaviourQuery object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    BehaviourQuery(const int,
                   const char* const* const,
                   std::shared_ptr<AbstractBehaviourDSL>,
                   const std::string&);
    //! treat the requests
    virtual void exe(void);
    //! destructor
    virtual ~BehaviourQuery();

   private:
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    //! a simple alias
    using query = std::function<void(
        const FileDescription&, const BehaviourDescription&, const Hypothesis)>;
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<BehaviourQuery>;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks(void);
    //! return the current argument
    virtual const tfel::utilities::Argument& getCurrentCommandLineArgument()
        const override final;
    //! treat a standard query
    virtual void treatStandardQuery(void) final;
    //! treat a standard query (an option to the command line
    //! triggering the query is required)
    virtual void treatStandardQuery2(void) final;
    //! treat the "--generated-sources" query
    virtual void treatGeneratedSources(void) final;
    //! treat the "--cppflags" query
    virtual void treatCppFlags(void) final;
    //! treat the "--generated-headers" query
    virtual void treatGeneratedHeaders(void) final;
    //! treat the "--libraries-dependencies" query
    virtual void treatLibrariesDependencies(void) final;
    //! treat the "--specific-targets" query
    virtual void treatSpecificTargets(void) final;
    //! treat the "--modelling-hypothesis" command line argument
    virtual void treatModellingHypothesis(void) final;
    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! get the version description
    virtual std::string getVersionDescription(void) const override final;
    //! get the usage description
    virtual std::string getUsageDescription(void) const override final;
    //! \return a query that show a list of variables
    template <const VariableDescriptionContainer& (BehaviourData::*m)(void)
                  const>
    query generateVariablesListQuery(void);
    //! all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! abstract behaviour dsl
    std::shared_ptr<AbstractBehaviourDSL> dsl;
    //! file name
    std::string file;
    //! modelling hypothesis
    Hypothesis hypothesis = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
  };  // end of struct BehaviourQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURQUERY_H */
