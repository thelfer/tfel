/*!
 * \file   Test.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TEST_HXX
#define _LIB_TFEL_TESTS_TEST_HXX 1

#include<string>

#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {
    
    /*!
     * Base class for all tests
     */
    struct Test
    {
      /*! 
       * return the name of the test
       * \return the name of the test
       */
      virtual std::string
      name(void) const = 0;
      /*! 
       * execute test
       * \return the results of the test
       */
      virtual TestResult
      execute(void) = 0;
      /*! 
       * destructor
       */
      virtual ~Test();
    }; // end of struct Test
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
