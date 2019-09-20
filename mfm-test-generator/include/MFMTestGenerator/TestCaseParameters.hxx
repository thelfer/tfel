/*!
 * \file   TestCaseParameters.hxx
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

#ifndef LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX
#define LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX

#include <string>
#include "MFMTestGenerator/Config.hxx"

namespace mfmtg {

  /*!
   * \brief a structure holding all the parameters of a test case.
   */
  struct MFMTG_VISIBILITY_EXPORT TestCaseParameters {
    //! \brief default constructor
    TestCaseParameters();
    //! \brief copy constructor
    TestCaseParameters(const TestCaseParameters&);
    //! \brief move constructor
    TestCaseParameters(TestCaseParameters&&);
    //! \brief standard assignement
    TestCaseParameters& operator=(const TestCaseParameters&);
    //! \brief move assignement
    TestCaseParameters& operator=(TestCaseParameters&&);
    //! \brief destructor
    ~TestCaseParameters();
    //! name of the test case
    std::string name;
  };  // end of TestCaseParameters

}  // end of namespace mfmtg

#endif /* LIB_MFM_TEST_GENERATOR_TESTCASEPARAMETERS_HXX */
