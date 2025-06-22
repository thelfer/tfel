/*!
 * \file   include/TFEL/Tests/XMLTestOutput.hxx
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
     * \brief Instances of this class reports tests' outputs in an
     * XML file in the JUnit format.
     */
    struct TFELTESTS_VISIBILITY_EXPORT XMLTestOutput final : public TestOutput {
      /*!
       * \brief constructor
       * \param[in] o: output file name
       */
      XMLTestOutput(const std::string&);
      /*!
       * \brief begin a new test suite
       * \param[in] n: name of the test suite
       */
      void beginTestSuite(const std::string&) override;
      /*!
       * \brief add a new test
       * \param[in] g: group  of the test
       * \param[in] n: name   of the test
       * \param[in] r: result of the test
       */
      void addTest(const std::string&,
                   const std::string&,
                   const TestResult&) override;
      /*!
       * End a test suite
       * \param[in] r: (global) result of the test suite
       */
      void endTestSuite(const TestResult&) override;
      //! Destructor
      ~XMLTestOutput() override;

     private:
      /*!
       * \param r: result to be treated
       */
      TFEL_VISIBILITY_LOCAL void treatTest(const TestResult&);
      //! \brief copy constructor (disabled)
      XMLTestOutput(const XMLTestOutput&) = delete;
      //! \brief assignement operator (disabled)
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
