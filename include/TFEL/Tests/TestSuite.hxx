/*!
 * \file   include/TFEL/Tests/TestSuite.hxx
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

#ifndef LIB_TFEL_TESTS_TESTSUITE_HXX
#define LIB_TFEL_TESTS_TESTSUITE_HXX 1

#include <string>
#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestOutput.hxx"

namespace tfel {

  namespace tests {

    /*!
     * Test Suites
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestSuite {
      //! a simple alias
      typedef std::shared_ptr<Test> TestPtr;
      /*!
       * Default constructor
       */
      TestSuite();
      /*!
       * Constructor
       * \param n : name of the function
       */
      TestSuite(const std::string&);
      /*!
       * Constructor
       * \param n : name of the function
       */
      TestSuite(const char* const);
      /*!
       * return the name of the test
       * \return the name of the test
       */
      std::string name(void) const;
      /*!
       * add a new test
       * \param t : test to be added
       */
      void add(const TestPtr&);
      /*!
       * execute the test suite
       * \param  o : test output
       * \return tests global results
       */
      TestResult execute(TestOutput&);
      /*!
       * execute the test suite
       * \return tests global results
       */
      TestResult execute(void);
      /*!
       * destructor
       */
      ~TestSuite();

     private:
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL
      TestSuite(const TestSuite&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL TestSuite& operator=(const TestSuite&);
      //! list of all tests
      std::vector<TestPtr> tests;
      //! name of test suite (may be empty)
      const std::string sname;
    };  // end of struct TestSuite

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTSUITE_HXX */
