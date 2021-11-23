/*!
 * \file   MaterialPropertyQuery.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALPROPERTYQUERY_HXX
#define LIB_MFRONT_MATERIALPROPERTYQUERY_HXX

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontBase.hxx"
#include "MFront/QueryHandlerBase.hxx"

namespace mfront {

  // forward declaration
  struct MaterialPropertyDSL;
  // forward declaration
  struct FileDescription;
  // forward declaration
  struct MaterialPropertyDescription;
  // forward declaration
  struct VariableDescriptionContainer;

  /*!
   * \brief class used by the mfront-query tool to extract information from
   * the implementation of a material property
   */
  struct MaterialPropertyQuery final
      : public tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>,
        public MFrontBase,
        public QueryHandlerBase {
    /*!
     * build a MaterialPropertyQuery object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    MaterialPropertyQuery(const int,
                          const char *const *const,
                          std::shared_ptr<MaterialPropertyDSL>,
                          const std::string &);
    //! \brief treat the requests
    virtual void exe();
    //! \brief destructor
    ~MaterialPropertyQuery() override;

   private:
    //! \brief a simple alias
    using query = std::function<void(const FileDescription &,
                                     const MaterialPropertyDescription &)>;
    //! \brief ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>;
    //
    std::shared_ptr<const AbstractDSL> getDSL() const override;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks();
    //! \brief return the current argument
    const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
    //! \brief treat the "--generated-sources" query
    virtual void treatGeneratedSources() final;
    //! \brief treat the "--cppflags" query
    virtual void treatCppFlags() final;
    //! \brief treat the "--generated-headers" query
    virtual void treatGeneratedHeaders() final;
    //! \brief treat the "--libraries-dependencies" query
    virtual void treatLibrariesDependencies() final;
    //! \brief treat the "--specific-targets" query
    virtual void treatSpecificTargets() final;
    //! \brief treat the "--parameter-default-value" query
    virtual void treatParameterDefaultValue() final;
    //! \brief treat a standard query
    virtual void treatStandardQuery() final;
    //! \brief treat an unknown argument
    void treatUnknownArgument() override final;
    //! \brief get the version description
    std::string getVersionDescription() const override final;
    //! \brief get the usage description
    std::string getUsageDescription() const override final;
    //! \brief all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! \brief abstract behaviour dsl
    std::shared_ptr<MaterialPropertyDSL> dsl;
    //! \brief file name
    std::string file;
  };  // end of struct MaterialPropertyQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYQUERY_HXX */
