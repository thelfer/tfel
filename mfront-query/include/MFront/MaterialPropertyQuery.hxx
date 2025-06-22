/*!
 * \file   MaterialPropertyQuery.hxx
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

#ifndef LIB_MFRONT_MATERIALPROPERTYQUERY_HXX
#define LIB_MFRONT_MATERIALPROPERTYQUERY_HXX

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>
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
  struct MaterialPropertyQuery final : public QueryHandlerBase {
    /*!
     * build a MaterialPropertyQuery object based on command line arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : specific language
     * \param[in] f    : mfront file
     */
    MaterialPropertyQuery(const int,
                          const char *const *const,
                          std::shared_ptr<MaterialPropertyDSL>,
                          const std::string &);
    //! \brief treat the requests
    virtual void exe();
    //! \brief destructor
    ~MaterialPropertyQuery() override;

   protected:
    //
    std::shared_ptr<const AbstractDSL> getDSL() const override;
    void registerCommandLineCallBacks() override;

   private:
    //! \brief a simple alias
    using query = std::function<void(const FileDescription &,
                                     const MaterialPropertyDescription &)>;
    //
    void treatGeneratedSources() override final;
    void treatSpecificTargetGeneratedSources() override final;
    void treatAllSpecificTargetsGeneratedSources() override final;
    void treatCppFlags() override final;
    void treatGeneratedHeaders() override final;
    void treatLibrariesDependencies() override final;
    void treatSpecificTargets() override final;
    void treatDSLTarget() override final;
    //! \brief treat the "--parameter-default-value" query
    virtual void treatParameterDefaultValue() final;
    //! \brief treat a standard query
    virtual void treatStandardQuery() final;
    //! \brief all the registred queries
    std::vector<std::pair<std::string, query>> queries;
    //! \brief abstract behaviour dsl
    std::shared_ptr<MaterialPropertyDSL> dsl;
    //! \brief file name
    std::string file;
  };  // end of struct MaterialPropertyQuery

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYQUERY_HXX */
