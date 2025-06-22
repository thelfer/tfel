/*!
 * \file   include/TFEL/Tests/XMLTestOutput.hxx
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

#ifndef LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX
#define LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX 1

#include <string>
#include <fstream>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Tests/TestOutput.hxx"
#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    /*!
     *  Test Output using a standard stream
     */
    struct TFELTESTS_VISIBILITY_EXPORT XMLTestOutput final : public TestOutput {
      /*!
       * Constructor
       * \param o : output file name
       */
      XMLTestOutput(const std::string&);
      /*!
       * Begin a new test suite
       * \param n : name of the test suite
       */
      virtual void beginTestSuite(const std::string&) override;
      /*!
       *  Add a new test
       * \param g : group  of the test
       * \param n : name   of the test
       * \param r : result of the test
       */
      virtual void addTest(const std::string&,
                           const std::string&,
                           const TestResult&) override;
      /*!
       * End a test suite
       * \param r : (global) result of the test suite
       */
      virtual void endTestSuite(const TestResult&) override;
      //! Destructor
      virtual ~XMLTestOutput();

     private:
      /*!
       * \param r : result to be treated
       */
      TFEL_VISIBILITY_LOCAL void treatTest(const TestResult&);
      //! copy constructor (disabled)
      XMLTestOutput(const XMLTestOutput&) = delete;
      //! assignement operator (disabled)
      XMLTestOutput& operator=(const XMLTestOutput&) = delete;
      //! output stream
      std::ofstream os;
      //! output file name
      std::string file;
      //! number of test suite treated
      unsigned short testsuite;
    };  // end of struct XMLTestOutput

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX */
