 /*!
  * \file   include/TFEL/Tests/StdStreamTestOutput.hxx
  * \author Helfer Thomas
  * \date   10 Apr 10
  * \brief  
  */

#ifndef _LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX
#define _LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX 1

#include<string>
#include<ostream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"TFEL/Tests/TestOutput.hxx"
#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     *  Test Output using a standard stream
     */
    struct TFELTESTS_VISIBILITY_EXPORT StdStreamTestOutput
      : public TestOutput
    {
      /*!
       * Constructor
       * \param o : output file name
       */
      StdStreamTestOutput(const std::string&);
      /*!
       * Constructor
       * \param o : output stream
       * \param b : color output
       */
      StdStreamTestOutput(std::ostream&,
			  const bool = true);
      /*!
       * Constructor
       * \param o : output stream
       * \param b : color output
       */
      StdStreamTestOutput(const tfel::utilities::shared_ptr<std::ostream>&,
			  const bool = false);
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
      virtual ~StdStreamTestOutput();
    private:
      /*!
       * \param r : result to be treated
       * \param s : string to be added at beginning of line
       */
      TFEL_VISIBILITY_LOCAL void
      treatTest(const TestResult&,
		const std::string&);
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL StdStreamTestOutput(const StdStreamTestOutput&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL StdStreamTestOutput&
      operator=(const StdStreamTestOutput&);
      /*!
       * pointer used to close stream, if the class has to handle it
       */
      tfel::utilities::shared_ptr<std::ostream> pos;
      //! output stream
      std::ostream& os;
      //! tells if the output shall be colorized
      const bool colorOutput;
    }; // end of struct StdStreamTestOutput
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_STDSTREAMTESTOUTPUT_HXX */
