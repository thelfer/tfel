/*!
 * \file   include/TFEL/Tests/TestSuite.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTSUITE_HXX
#define _LIB_TFEL_TESTS_TESTSUITE_HXX 1

#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestOutput.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     * Test Suites
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestSuite
    {
      //! a simple alias
      typedef tfel::utilities::shared_ptr<Test> TestPtr;
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
      TestSuite(const char * const);
      /*! 
       * return the name of the test
       * \return the name of the test
       */
      std::string
      name(void) const;
      /*!
       * add a new test
       * \param t : test to be added
       */
      void
      add(const TestPtr&);
      /*! 
       * execute the test suite
       * \param  o : test output
       * \return tests global results
       */
      TestResult
      execute(TestOutput&);
      /*! 
       * execute the test suite
       * \return tests global results
       */
      TestResult
      execute(void);
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
      TFEL_VISIBILITY_LOCAL TestSuite&
      operator=(const TestSuite&);
      //! list of all tests
      std::vector<TestPtr> tests;
      //! name of test suite (may be empty)
      const std::string sname;
    }; // end of struct TestSuite
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
