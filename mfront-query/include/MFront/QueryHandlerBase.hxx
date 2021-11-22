/*!
 * \file  mfront-query/include/MFront/QueryHandlerBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_QUERYHANDLERBASE_HXX
#define LIB_MFRONT_QUERYHANDLERBASE_HXX

#include <memory>
#include <functional>

namespace mfront {

  // forward declarations
  struct AbstractDSL;
  struct FileDescription;

  //! \brief base class for queries handler
  struct QueryHandlerBase {
    QueryHandlerBase();
    //! \brief destructor
    virtual ~QueryHandlerBase();

   protected:
    //! \return the abstract domain specific language
    virtual std::shared_ptr<const AbstractDSL> getDSL() const = 0;
    //! \return an object handling the `--generated-sources` query
    virtual std::function<void()> generateGeneratedSourcesQuery(
        const std::string&) const;
    //! \return an object handling the `--generated-headers` query
    virtual std::function<void()> generateGeneratedHeadersQuery() const;
    //! \return an object handling the `--specific-targets` query
    virtual std::function<void()> generateSpecificTargetsQuery() const;
    //! \return an object handling the `--libraries-dependencies` query
    virtual std::function<void()> generateLibrariesDependenciesQuery() const;
    //! \return an object handling the `--cppflags` query
    virtual std::function<void()> generateCppFlagsQuery() const;
  };  // end of struct QueryHandlerBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_QUERYHANDLERBASE_HXX */
