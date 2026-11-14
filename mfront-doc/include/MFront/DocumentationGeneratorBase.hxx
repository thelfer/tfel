/*!
 * \file  DocumentationGeneratorBase.hxx
 * \brief
 * \author Maxence Wangermez
 * \date 16/04/2024
 * \copyright Copyright (C) 2006-2024 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_DOCUMENTATIONGENERATORBASE_H
#define LIB_MFRONT_DOCUMENTATIONGENERATORBASE_H

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <functional>

#include "TFEL/Utilities/ArgumentParserBase.hxx"
#include "MFront/MFrontBase.hxx"

namespace mfront {

  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct FileDescription;

  /*!
   * Class used by the mfront-query tool to extract information from
   * behaviour implementation
   */
  struct DocumentationGeneratorBase
      : public tfel::utilities::ArgumentParserBase<DocumentationGeneratorBase>,
        public MFrontBase {
    //! \brief type of documentation to be generated
    enum OutputType { FULL, WEB };
    /*!
     * build a DocumentationGeneratorBase object based on command line
     * arguments
     * \param[in] argc : number of command line arguments
     * \param[in] argv : command line arguments
     * \param[in] f    : filename
     */
    DocumentationGeneratorBase(const int,
                               const char* const* const,
                               const std::string&);
    virtual void exe() const = 0;
    //! \brief destructor
    ~DocumentationGeneratorBase() override;

    // Private member data
    //! input file name
    std::string file;
    /*!
     * \brief boolean whose meaning is the following:
     * - if true, the generated documentation is contained in a whole document.
     * - if false, the generated documentation is contained in a section meant
     *   to be included in a bigger document.
     */
    bool standalone = false;

    //! \brief generate output on standard output
    bool std_output = false;
    //! \brief type of ouput
    OutputType otype;
    //! \brief generate latex macros for header of output file
    static void writeStandardLatexMacros(std::ostream&);

    //! \brief data structure describing a variable
    struct VariableInformationBase {
      VariableInformationBase();
      VariableInformationBase(VariableInformationBase&&) noexcept;
      VariableInformationBase(const VariableInformationBase&);
      VariableInformationBase& operator=(VariableInformationBase&&) noexcept;
      VariableInformationBase& operator=(const VariableInformationBase&);
      ~VariableInformationBase() noexcept;
      std::string name;
      std::string type;
      std::string description;
      std::string externalName;
      unsigned short arraySize;
    };

   private:
    //! ArgumentParserBase must be a friend
    friend struct tfel::utilities::ArgumentParserBase<
        DocumentationGeneratorBase>;
    //! \brief treat an unknown argument
    void treatUnknownArgument() final;
    //! \brief get the version description
    [[nodiscard]] std::string getVersionDescription() const final;
    //! \brief get the usage description
    [[nodiscard]] std::string getUsageDescription() const final;
    //! \brief return the current argument
    [[nodiscard]] const tfel::utilities::Argument&
    getCurrentCommandLineArgument() const final;
    //! \brief register call-backs associated with command line arguments
    virtual void registerCommandLineCallBacks();
    //! treat the web argument
    virtual void treatWeb();

  };  // end of struct DocumentationGeneratorBase

}  // end of namespace mfront

#endif /* LIB_MFRONT_DOCUMENTATIONGENERATORBASE_H */
