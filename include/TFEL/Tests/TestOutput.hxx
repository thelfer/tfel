/*!
 * \file   include/TFEL/Tests/TestOutput.hxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TESTOUTPUT_HXX
#define LIB_TFEL_TESTS_TESTOUTPUT_HXX 1

#include <string>

#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    //! \brief base class for tests' outputs
    struct TestOutput {
      /*!
       * \brief begin a new test suite
       * \param[in] n: name of the test suite
       */
      virtual void beginTestSuite(const std::string&) = 0;
      /*!
       * \brief add a new test
       * \param[in] n: group  of the test
       * \param[in] n: name   of the test
       * \param[in] r: result of the test
       */
      virtual void addTest(const std::string&,
                           const std::string&,
                           const TestResult&) = 0;
      /*!
       * \brief end a test suite
       * \param[in] r: (global) result of the test suite
       */
      virtual void endTestSuite(const TestResult&) = 0;
      //! \bried destructor
      virtual ~TestOutput();
    };  // end of struct TestOutput

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTOUTPUT_HXX */
