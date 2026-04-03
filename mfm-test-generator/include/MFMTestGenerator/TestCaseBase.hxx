/*!
 * \file   TestCaseBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX

#include <string>
#include "MFMTestGenerator/Config.hxx"
#include "MFMTestGenerator/Forward.hxx"
#include "MFMTestGenerator/Times.hxx"
#include "MFMTestGenerator/AbstractTestCase.hxx"

namespace mfmtg {

  //! \brief a base class to describe a test case.
  struct MFMTG_VISIBILITY_EXPORT TestCaseBase : AbstractTestCase {
    /*!
     * \brief constructor
     * \param[in] p: parameters
     */
    TestCaseBase(const TestCaseParameters&);
    void addInputFileGenerator(const generator&) override;
    void generate() const override;
    //! \brief destructor
    ~TestCaseBase() override;
    //! name of the test case
    const std::string name;
    //! \brief author (may be empty)
    const std::string author;
    //! \brief date (may be empty)
    const std::string date;
    //! \brief description (may be empty)
    const std::string description;
    //! \brief times
    const Times times;

   private:
    //! list of all registred generators
    std::vector<generator> generators;
  };  // end of struct TestCaseBase

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX */
