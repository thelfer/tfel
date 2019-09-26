/*!
 * \file   TestCaseBase.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/09/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX

#include <vector>
#include <string>
#include <functional>
#include "MFMTestGenerator/Config.hxx"
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
    //! \brief destructor
    ~TestCaseBase() override;
    //! name of the test case
    const std::string name;

   private:
    //! list of all registred generators
    std::vector<generator> generators;
  }; // end of struct TestCaseBase 

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEBASE_HXX */
