/*!
 * \file   include/TFEL/Tests/TestFunctionWrapper.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_HXX
#define _LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_HXX 1

#include<string>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestResult.hxx"

namespace tfel
{

  namespace tests
  {

    /*!
     * A simple wrapper around test functions
     * \param f : function to be wrapped
     */ 
   template<bool (*f)(void)>
   struct TestFunctionWrapper
     : public Test
   {
     /*!
      * Constructor
      * \param g : group of the test
      * \param n : name  of the function
      */
     TestFunctionWrapper(const std::string&,
			 const std::string&);
     /*!
      * Constructor
      * \param n : name of the function
      */
     TestFunctionWrapper(const char* const);
     /*! 
      * return the name of the test function
      * \return the name of the test function
      */
     std::string
     name() const;
     /*! 
      * return the name of the test function
      * \return the name of the test function
      */
     std::string
     classname() const;
     /*! 
      * execute test
      * \return the results of the test
      */
     TestResult
     execute(void);
     /*! 
      * destructor
      */
     ~TestFunctionWrapper();
   private:
     /*!
      * copy constructor (disabled)
      * \param src : object to be copied
      */
     TestFunctionWrapper(const TestFunctionWrapper&);
     /*!
      * assignement operator (disabled)
      * \param src : object to be assigned
      * \return a reference to this object
      */
     TestFunctionWrapper&
     operator=(const TestFunctionWrapper&);
     //! name of test group
     const std::string gname;
     //! name of function
     const std::string fname;
   }; // end of struct TestFunctionWrapper
    
  } // end of namespace tests

} // end of namespace tfel

#include"TFEL/Tests/TestFunctionWrapper.ixx"

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
