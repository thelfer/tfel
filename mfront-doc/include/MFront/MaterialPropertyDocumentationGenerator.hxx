/*!
 * \file  MaterialPropertyDocumentationGenerator.hxx
 * \brief
 * \author Maxence Wangermez
 * \date   16 avril 2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_MATERIALPROPERTYDOCUMENTATIONGENERATOR_H
#define LIB_MFRONT_MATERIALPROPERTYDOCUMENTATIONGENERATOR_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "MFront/MaterialPropertyDSL.hxx"
#include "MFront/DocumentationGeneratorBase.hxx"

namespace mfront {

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct MaterialPropertyDocumentationGenerator
      : public DocumentationGeneratorBase {
    /*!
     * build a MaterialPropertyDocumentationGenerator object based on command
     * line arguments \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] d    : behaviour domain specific language
     * \param[in] f    : behaviour domain specific language
     */
    MaterialPropertyDocumentationGenerator(const int,
                                           const char *const *const,
                                           std::shared_ptr<MaterialPropertyDSL>,
                                           const std::string &);
    //! treat the requests
    void exe() const override;
    //! destructor
    ~MaterialPropertyDocumentationGenerator() override;

    struct VariableInformation
        : DocumentationGeneratorBase::VariableInformationBase {
      VariableInformation();
      VariableInformation(VariableInformation &&) noexcept;
      VariableInformation(const VariableInformation &);
      VariableInformation &operator=(VariableInformation &&);
      VariableInformation &operator=(const VariableInformation &);
      ~VariableInformation() noexcept;
    };

   private:
    void writeWebOutput(std::ostream &,
                        const MaterialPropertyDescription &,
                        const FileDescription &) const;
    void writeFullOutput(std::ostream &,
                         const MaterialPropertyDescription &,
                         const FileDescription &) const;
    //! abstract behaviour dsl
    std::shared_ptr<MaterialPropertyDSL> dsl;
  };  // end of struct MaterialPropertyDocumentationGenerator

}  // end of namespace mfront

#endif /* LIB_MFRONT_MATERIALPROPERTYDOCUMENTATIONGENERATOR_H */
