/*!
 * \file
 * mfm-test-generator/include/MFMTestGenerator/MFMTestGeneratorFileExport.hxx
 * \brief
 * \author Thomas Helfer
 * \date   01/03/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_MFM_TEST_GENERATORFILEEXPORT_HXX
#define LIB_MFM_TEST_GENERATOR_MFM_TEST_GENERATORFILEEXPORT_HXX

#include <string>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  /*!
   * \brief structure describing an mfm-test-generator file to be exported
   */
  struct MFMTG_VISIBILITY_EXPORT TestDescription {
    //! \brief default constructor
    TestDescription();
    //! \brief move constructor
    TestDescription(TestDescription&&);
    //! \brief copy constructor
    TestDescription(const TestDescription&);
    //! \brief move constructor
    TestDescription& operator=(TestDescription&&);
    //! \brief copy constructor
    TestDescription& operator=(const TestDescription&);
    //! \brief destructor
    ~TestDescription() noexcept;
    //! \brief name of the test
    std::string name;
    //! \brief author of the test
    std::string author;
    //! \brief date at which the test has been created
    std::string date;
    //! \brief description of the test
    std::string description;
    //! \brief name of the behaviour to which the test is associated
    std::string behaviour;
    //! \brief name of the material to which the test is associated
    std::string material;
    //! \brief content of the `mfm-test-generator` file
    std::string content;
  };  // end of struct TestDescription

  /*!
   * \brief load the content of an `mfm-test-generator` file.
   *
   * \param[out] d: mfm-test-generator file description
   * \param[in] f: output file path
   */
  MFMTG_VISIBILITY_EXPORT void loadMFMTestGeneratorFileContent(
      TestDescription&, const std::string&);
  /*!
   * \brief write an `mfm-test-generator` test to a file.
   *
   * The file format is deduced from the extension of the file.
   *
   * Currently, only extensions associated with the `madnex` file format
   * (https://github.com/thelfer/madnex) are supported if `TFEL` is compiled
   * with support of this file format. Those extensions are: `mdnx`, `madnex`
   * (deprecated) or `edf` (experimental data file, deprecated). Note that the
   * behaviour member of the metadata must be specified for export in the
   * `madnex` file format.
   *
   * \param[in] d: mfm-test-generator file description
   * \param[in] f: output file path
   */
  MFMTG_VISIBILITY_EXPORT void write(const TestDescription&,
                                     const std::string&);

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_MFM_TEST_GENERATORFILEEXPORT_HXX */
