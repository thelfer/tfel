/*!
 * \file   include/TFEL/Tests/TestOutput.hxx
 * \author Helfer Thomas
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

    /*!
     * Base class for Test Outputs
     */
    struct TestOutput {
      /*!
       * Begin a new test suite
       * \param n : name of the test suite
       */
      virtual void beginTestSuite(const std::string&) = 0;
      /*!
       *  Add a new test
       * \param n : group  of the test
       * \param n : name   of the test
       * \param r : result of the test
       */
      virtual void addTest(const std::string&,
                           const std::string&,
                           const TestResult&) = 0;
      /*!
       * End a test suite
       * \param r : (global) result of the test suite
       */
      virtual void endTestSuite(const TestResult&) = 0;
      /*!
       * Destructor
       */
      virtual ~TestOutput();
    };  // end of struct TestOutput

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTOUTPUT_HXX */
