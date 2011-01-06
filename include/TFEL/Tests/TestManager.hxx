/*!
 * \file   TestManager.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 */

#ifndef _LIB_TFEL_TESTS_TESTMANAGER_HXX
#define _LIB_TFEL_TESTS_TESTMANAGER_HXX 1

#include<map>
#include<string>
#include<ostream>
#include<iostream>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestSuite.hxx"
#include"TFEL/Tests/TestResult.hxx"
#include"TFEL/Tests/TestOutput.hxx"
#include"TFEL/Tests/MultipleTestOutputs.hxx"

/*!
 * \def TFEL_TESTS_DEFAULTMAIN
 * \author Helfer Thomas
 * \date   11 Apr 10
 */
#define TFEL_TESTS_DEFAULTMAIN()                          \
  int main(void){                                         \
    using namespace std;                                  \
    using namespace tfel::tests;                          \
    TestManager& manager = TestManager::getTestManager(); \
    manager.addTestOutput(cout);                          \
    TestResult r = manager.execute();                     \
    if(r.success()){                                      \
      return EXIT_SUCCESS;                                \
    }                                                     \
    return EXIT_FAILURE;				  \
  }

namespace tfel
{

  namespace tests
  {
    
    /*!
     * class in charge of managing a collection of tests.
     */
    struct TFEL_VISIBILITY_EXPORT TestManager
    {
      //! a simple alias
      typedef tfel::utilities::shared_ptr<TestOutput> TestOutputPtr;
      //! a simple alias
      typedef tfel::utilities::shared_ptr<Test> TestPtr;
      /*!
       * \return the unique instance of the class
       */
      static TestManager&
      getTestManager();
      /*!
       * add a test output
       * \param o : output to be added
       */
      void
      addTestOutput(const TestOutputPtr&);
      /*!
       * add a test output
       * \param o : output to be added
       * \param b : colourised output
       */
      void
      addTestOutput(std::ostream&,
		    const bool = true);
      /*!
       * add a test output to the specified file 
       * \param f : file name
       */
      void
      addTestOutput(const std::string&);
      /*!
       * add a test output to the specified file 
       * \param f : file name
       */
      void
      addXMLTestOutput(const std::string&);
      /*!
       * add a test output to the specified test suite
       * \param n : name of the test suite
       * \param o : output to be added
       */
      void
      addTestOutput(const std::string&,
		    const TestOutputPtr&);
      /*!
       * add a test output to the specified test suite
       * \param n : name of the test suite
       * \param o : output to be added
       * \param b : colourised output
       */
      void
      addTestOutput(const std::string&,
		    std::ostream&,
		    const bool = true);
      /*!
       * add a test output to the specified file
       * \param n : name of the test suite
       * \param f : file name
       */
      void
      addTestOutput(const std::string&,
		    const std::string&);
      /*!
       * add a test to the specified test suite
       * \param g : group of the test
       * \param n : name of the test suite
       * \param t : test to be added
       */
      void
      addTest(const std::string&,
	      const TestPtr&);
      /*!
       * execute registred tests
       */
      TestResult
      execute(void);
    private:
      //! a simple alias
      typedef tfel::utilities::shared_ptr<MultipleTestOutputs> MultipleTestOutputsPtr;
      //! a simple alias
      typedef tfel::utilities::shared_ptr<TestSuite> TestSuitePtr;
      /*!
       * default constructor declared private to implement the
       * Singleton pattern.
       */
      TFEL_VISIBILITY_LOCAL TestManager();
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL TestManager(const TestManager&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL TestManager&
      operator=(const TestManager&);
      //! outputs used by default
      MultipleTestOutputsPtr default_outputs;
      //! "per test suite" outputs
      std::map<std::string,MultipleTestOutputsPtr> outputs;
      //! registred tests
      std::map<std::string,TestSuitePtr> tests;
    }; // end of struct TestManager
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TESTMANAGER_HXX */
