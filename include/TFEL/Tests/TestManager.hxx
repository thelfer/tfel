/*!
 * \file   include/TFEL/Tests/TestManager.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TESTMANAGER_HXX
#define LIB_TFEL_TESTS_TESTMANAGER_HXX 1

#include <map>
#include <string>
#include <memory>
#include <iosfwd>

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestSuite.hxx"
#include "TFEL/Tests/TestResult.hxx"
#include "TFEL/Tests/TestOutput.hxx"
#include "TFEL/Tests/MultipleTestOutputs.hxx"

#define TFEL_TESTS_HAS_XML_OUTPUT 1

namespace tfel {

  namespace tests {

    /*!
     * class in charge of managing a collection of tests.
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestManager {
      //! a simple alias
      typedef std::shared_ptr<TestOutput> TestOutputPtr;
      //! a simple alias
      typedef std::shared_ptr<Test> TestPtr;
      /*!
       * \return the unique instance of the class
       */
      static TestManager& getTestManager();
      /*!
       * add a test output
       * \param o : output to be added
       */
      void addTestOutput(TestOutputPtr);
      /*!
       * add a test output
       * \param o : output to be added
       * \param b : colourised output
       */
      void addTestOutput(std::ostream&, const bool = true);
      /*!
       * add a test output to the specified file
       * \param f : file name
       */
      void addTestOutput(const std::string&);
      /*!
       * add a test output to the specified file
       * \param f : file name
       */
      void addXMLTestOutput(const std::string&);
      /*!
       * add a test output to the specified test suite
       * \param n : name of the test suite
       * \param o : output to be added
       */
      void addTestOutput(const std::string&, TestOutputPtr);
      /*!
       * add a test output to the specified test suite
       * \param n : name of the test suite
       * \param o : output to be added
       * \param b : colourised output
       */
      void addTestOutput(const std::string&, std::ostream&, const bool = true);
      /*!
       * add a test output to the specified file
       * \param n : name of the test suite
       * \param f : file name
       */
      void addTestOutput(const std::string&, const std::string&);
      /*!
       * add a test to the specified test suite
       * \param g : group of the test
       * \param t : test to be added
       */
      void addTest(const std::string&, TestPtr);
      /*!
       * execute registred tests
       */
      TestResult execute(void);

     private:
      //! a simple alias
      typedef std::shared_ptr<MultipleTestOutputs> MultipleTestOutputsPtr;
      //! a simple alias
      typedef std::shared_ptr<TestSuite> TestSuitePtr;
      /*!
       * default constructor declared private to implement the
       * Singleton pattern.
       */
      TFEL_VISIBILITY_LOCAL TestManager();
      TestManager(TestManager&&) = delete;
      TestManager(const TestManager&) = delete;
      TestManager& operator=(TestManager&&) = delete;
      TestManager& operator=(const TestManager&) = delete;
      //! outputs used by default
      MultipleTestOutputsPtr default_outputs;
      //! "per test suite" outputs
      std::map<std::string, MultipleTestOutputsPtr> outputs;
      //! registred tests
      std::map<std::string, TestSuitePtr> tests;
    };  // end of struct TestManager

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTMANAGER_HXX */
