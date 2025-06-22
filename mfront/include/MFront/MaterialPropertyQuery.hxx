/*!
 * \file   MaterialPropertyQuery.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   08 juin 2016
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
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
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct MaterialPropertyQuery final
      : public tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>,
        public MFrontBase {
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
    //! treat the requests
    virtual void exe(void);
    //! destructor
    virtual ~MaterialPropertyQuery();

   private:
    //! a simple alias
    using query = std::function<void(const FileDescription &,
                                     const MaterialPropertyDescription &)>;
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<MaterialPropertyQuery>;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks(void);
    //! return the current argument
    virtual const tfel::utilities::Argument &getCurrentCommandLineArgument()
        const override final;
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
    //! treat a standard query
    virtual void treatStandardQuery(void) final;
    //! treat an unknown argument
    virtual void treatUnknownArgument(void) final;
    //! get the version description
    virtual std::string getVersionDescription(void) const override final;
    //! get the usage description
    virtual std::string getUsageDescription(void) const override final;
    //! all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! abstract behaviour dsl
    std::shared_ptr<MaterialPropertyDSL> dsl;
    //! file name
    std::string file;
  };  // end of struct MaterialPropertyQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYQUERY_HXX_ */
