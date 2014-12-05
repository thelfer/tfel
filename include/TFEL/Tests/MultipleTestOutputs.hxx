 /*!
  * \file   include/TFEL/Tests/MultipleTestOutputs.hxx
  * \author Helfer Thomas
  * \date   10 Apr 10
  * \brief  
  * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX
#define _LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX 1

#include<vector>

#include"TFEL/Config/TFELConfig.hxx"
#include<memory>

#include"TFEL/Tests/TestOutput.hxx"
#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     * Helper class for gathering multiple outputs
     */
    struct TFELTESTS_VISIBILITY_EXPORT MultipleTestOutputs
      : public TestOutput
    {
      //! a simple alias
      typedef std::shared_ptr<TestOutput> TestOutputPtr;
      /*!
       * Constructor
       */
      MultipleTestOutputs();
      /*!
       * add a test output
       * \param o : output to be added
       */
      void
      addTestOutput(const TestOutputPtr&);
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
      virtual ~MultipleTestOutputs();
    private:
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TFEL_VISIBILITY_LOCAL MultipleTestOutputs(const MultipleTestOutputs&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL MultipleTestOutputs&
      operator=(const MultipleTestOutputs&);
      //! registred outputs
      std::vector<TestOutputPtr> outputs;
    }; // end of struct MultipleTestOutputs
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_MULTIPLETESTOUTPUTS_HXX */
