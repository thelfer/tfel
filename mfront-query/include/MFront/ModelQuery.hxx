/*!
 * \file   ModelQuery.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MODELQUERY_HXX
#define LIB_MFRONT_MODELQUERY_HXX

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  // forward declaration
  struct ModelDSL;
  // forward declaration
  struct FileDescription;
  // forward declaration
  struct ModelDescription;
  // forward declaration
  struct VariableDescriptionContainer;

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct ModelQuery final
      : public tfel::utilities::ArgumentParserBase<ModelQuery>,
        public MFrontBase {
    /*!
     * build a ModelQuery object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    ModelQuery(const int,
               const char *const *const,
               std::shared_ptr<ModelDSL>,
               const std::string &);
    //! treat the requests
    virtual void exe();
    //! destructor
    virtual ~ModelQuery();

   private:
    //! a simple alias
    using query =
        std::function<void(const FileDescription &, const ModelDescription &)>;
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<ModelQuery>;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks();
    //! return the current argument
    virtual const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    //! treat the "--generated-sources" query
    virtual void treatGeneratedSources() final;
    //! treat the "--cppflags" query
    virtual void treatCppFlags() final;
    //! treat the "--generated-headers" query
    virtual void treatGeneratedHeaders() final;
    //! treat the "--libraries-dependencies" query
    virtual void treatLibrariesDependencies() final;
    //! treat the "--specific-targets" query
    virtual void treatSpecificTargets() final;
    //! treat a standard query
    virtual void treatStandardQuery() final;
    //! treat an unknown argument
    virtual void treatUnknownArgument() final;
    //! get the version description
    virtual std::string getVersionDescription() const override final;
    //! get the usage description
    virtual std::string getUsageDescription() const override final;
    //! all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! abstract behaviour dsl
    std::shared_ptr<ModelDSL> dsl;
    //! file name
    std::string file;
  };  // end of struct ModelQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_MODELQUERY_HXX */
