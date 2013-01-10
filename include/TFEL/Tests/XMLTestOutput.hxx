 /*!
  * \file   XMLTestOutput.hxx
  * \author Helfer Thomas
  * \date   10 Apr 10
  * \brief  
  */

#ifndef _LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX
#define _LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX 1

#include<string>
#include<fstream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Tests/TestOutput.hxx"
#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     *  Test Output using a standard stream
     */
    struct TFELTESTS_VISIBILITY_EXPORT XMLTestOutput
      : public TestOutput
    {
      /*!
       * Constructor
       * \param o : output file name
       */
      XMLTestOutput(const std::string&);
      /*!
       * Begin a new test suite
       * \param n : name of the test suite
       */
      virtual void
      beginTestSuite(const std::string&);
      /*!
       *  Add a new test
       * \param g : group  of the test
       * \param n : name   of the test
       * \param r : result of the test
       */
      virtual void
      addTest(const std::string&,
	      const std::string&,
	      const TestResult&);
      /*!
       * End a test suite
       * \param r : (global) result of the test suite
       */
      virtual void
      endTestSuite(const TestResult&);
      /*!
       * Destructor
       */
      virtual ~XMLTestOutput();
    private:
      /*!
       * \param r : result to be treated
       */
      TFEL_VISIBILITY_LOCAL void
      treatTest(const TestResult&);
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL XMLTestOutput(const XMLTestOutput&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL XMLTestOutput&
      operator=(const XMLTestOutput&);
      //! output stream
      std::ofstream os;
      //! output file name
      std::string file;
      //! number of test suite treated
      unsigned short testsuite;
    }; // end of struct XMLTestOutput
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_XMLTESTOUTPUT_HXX */
