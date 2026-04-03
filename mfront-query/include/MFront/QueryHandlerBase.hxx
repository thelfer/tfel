/*!
 * \file  mfront-query/include/MFront/QueryHandlerBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04/03/2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_QUERYHANDLERBASE_HXX
#define LIB_MFRONT_QUERYHANDLERBASE_HXX

#include <memory>
#include <functional>
#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  // forward declarations
  struct AbstractDSL;
  struct VariableDescription;
  struct FileDescription;

  //! \brief base class for queries handler
  struct QueryHandlerBase
      : public tfel::utilities::ArgumentParserBase<QueryHandlerBase>,
        public MFrontBase {
    /*!
     * \brief default constructor
     * \param[in] argc: number of command line arguments
     * \param[in] argv: command line arguments
     */
    QueryHandlerBase(const int, const char *const *const);
    //! \brief destructor
    ~QueryHandlerBase() override;

   protected:
    //! \brief ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<QueryHandlerBase>;
    const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    void treatUnknownArgument() override final;
    std::string getVersionDescription() const override final;
    std::string getUsageDescription() const override final;
    /*!
     * \brief print the description of a variable
     * \param[in] v: variable
     * \param[in] spaces: string placed in front of the description (generally
     * spaces, hence the name)
     */
    static void displayVariable(const mfront::VariableDescription &,
                                const std::string & = std::string{});
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks();
    //! \return the abstract domain specific language
    virtual std::shared_ptr<const AbstractDSL> getDSL() const = 0;
    /*!
     * \return an object handling the `--generated-sources` query
     * \param[in] o: option passed the query
     */
    virtual std::function<void()> generateGeneratedSourcesQuery(
        const std::string &) const;
    /*!
     * \return an object handling the `--specific-target-generated-sources`
     * query.
     * \param[in] n: name of the specific target
     */
    virtual std::function<void()> generateSpecificTargetGeneratedSourcesQuery(
        const std::string &) const;
    /*!
     * \return an object handling the `--all-specific-targets-generated-sources`
     * query.
     */
    virtual std::function<void()>
    generateAllSpecificTargetsGeneratedSourcesQuery() const;
    //! \return an object handling the `--generated-headers` query
    virtual std::function<void()> generateGeneratedHeadersQuery() const;
    //! \return an object handling the `--specific-targets` query
    virtual std::function<void()> generateSpecificTargetsQuery() const;
    //! \return an object handling the `--libraries-dependencies` query
    virtual std::function<void()> generateLibrariesDependenciesQuery() const;
    //! \return an object handling the `--cppflags` query
    virtual std::function<void()> generateCppFlagsQuery() const;
    //! \brief treat the "--generated-sources" query
    virtual void treatGeneratedSources() = 0;
    //! \brief treat the "--specific-target-generated-sources" query
    virtual void treatSpecificTargetGeneratedSources() = 0;
    //! \brief treat the "--all-specific-targets-generated-sources" query
    virtual void treatAllSpecificTargetsGeneratedSources() = 0;
    //! \brief treat the "--cppflags" query
    virtual void treatCppFlags() = 0;
    //! \brief treat the "--generated-headers" query
    virtual void treatGeneratedHeaders() = 0;
    //! \brief treat the "--libraries-dependencies" query
    virtual void treatLibrariesDependencies() = 0;
    //! \brief treat the "--specific-targets" query
    virtual void treatSpecificTargets() = 0;
    //! \brief treat the "--dsl-target" query
    virtual void treatDSLTarget() = 0;
    /*!
     * \brief optional flags stating if sources of library dependencies must be
     * concateneted with the sources of the library
     */
    bool melt_sources = true;
  };  // end of struct QueryHandlerBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_QUERYHANDLERBASE_HXX */
