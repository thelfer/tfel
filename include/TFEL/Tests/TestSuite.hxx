/*!
 * \file   include/TFEL/Tests/TestSuite.hxx
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

#ifndef LIB_TFEL_TESTS_TESTSUITE_HXX
#define LIB_TFEL_TESTS_TESTSUITE_HXX 1

#include <string>
#include <memory>
#include <vector>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestOutput.hxx"

namespace tfel {

  namespace tests {

    /*!
     * \brief a class handling test suites
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestSuite {
      //! a simple alias
      using TestPtr = std::shared_ptr<Test>;
      //! \brief Default constructor
      TestSuite();
      /*!
       * \brief Constructor
       * \param[in] n: name of the function
       */
      TestSuite(const std::string&);
      /*!
       * \brief constructor
       * \param[in] n: name of the function
       */
      TestSuite(const char* const);
      //! \return the name of the test
      std::string name() const;
      /*!
       * \brief add a new test
       * \param t: test to be added
       */
      void add(const TestPtr&);
      /*!
       * \brief execute the test suite
       * \param  o: test output
       * \return tests global results
       */
      TestResult execute(TestOutput&);
      /*!
       * \brief execute the test suite
       * \return tests global results
       */
      TestResult execute();
      //! \brief destructor
      ~TestSuite();

     private:
      //! \brief move constructor (disabled)
      TestSuite(TestSuite&&) = delete;
      //! \brief copy constructor (disabled)
      TestSuite(const TestSuite&) = delete;
      //! \brief move assignement operator (disabled)
      TestSuite& operator=(TestSuite&&) = delete;
      //! \brief assignement operator (disabled)
      TestSuite& operator=(const TestSuite&) = delete;
      //! list of all tests
      std::vector<TestPtr> tests;
      //! name of test suite (may be empty)
      const std::string sname;
    };  // end of struct TestSuite

  }  // namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTSUITE_HXX */
