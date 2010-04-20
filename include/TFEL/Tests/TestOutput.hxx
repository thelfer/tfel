/*!
 * \file   TestOutput.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTOUTPUT_HXX
#define _LIB_TFEL_TESTS_TESTOUTPUT_HXX 1

#include<string>

#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     * Base class for Test Outputs
     */
    struct TestOutput
    {
      /*!
       * Begin a new test suite
       * \param n : name of the test suite
       */
      virtual void
      beginTestSuite(const std::string& n) = 0;
      /*!
       *  Add a new test
       * \param n : name   of the test
       * \param r : result of the test
       */
      virtual void
      addTest(const std::string& n,
	      const TestResult&  r) = 0;
      /*!
       * End a test suite
       * \param r : (global) result of the test suite
       */
      virtual void
      endTestSuite(const TestResult& r) = 0;
      /*!
       * Destructor
       */
      virtual ~TestOutput();
    }; // end of struct TestOutput
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TESTOUTPUT_HXX */
