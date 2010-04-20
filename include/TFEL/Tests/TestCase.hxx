/*!
 * \file   TestCase.hxx
 * \author Helfer Thomas
 * \date   11 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTCASE_HXX
#define _LIB_TFEL_TESTS_TESTCASE_HXX 1

#include<string>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestResult.hxx"

/*!
 * \def TFEL_TESTS_ASSERT
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : code to be evaluated
 */
#define TFEL_TESTS_ASSERT(X)				\
  {							\
    TestCase::registerResult("assertion : '"#X"'",(X));	\
  }

/*!
 * \def TFEL_TESTS_CHECK_THROW
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : code to be evaluated
 * \param Y : exeception execpted to be thrown
 */
#define TFEL_TESTS_CHECK_THROW(X,Y)				\
  {                                                             \
    bool hasThrown = false;                                     \
    try{                                                        \
      X;                                                        \
    }                                                           \
    catch(Y&){                                                  \
      hasThrown = true;                                         \
    }                                                           \
    catch(...){}						\
    TestCase::registerResult("check throw : '"#X"'",hasThrown);	\
  }

/*!
 * \def TFEL_TESTS_CALLMETHOD
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : method to be called
 */
#define TFEL_TESTS_CALLMETHOD(X)                                \
  {                                                             \
    TestCase::registerResult(#X"()",this->X());          	\
  }

/*!
 * \def TFEL_TESTS_CHECK_EQUAL
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_EQUAL(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X==Y);       \
  }

/*!
 * \def TFEL_TESTS_CHECK_GT
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_GT(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X>Y);        \
  }

/*!
 * \def TFEL_TESTS_CHECK_GE
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_GE(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X>=Y);       \
  }

/*!
 * \def TFEL_TESTS_CHECK_LT
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_LT(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X<Y);        \
  }

/*!
 * \def TFEL_TESTS_CHECK_LE
 * \brief an helper macro to ease the use of TestCase::help
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_LE(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X<=Y);       \
  }



namespace tfel
{

  namespace tests
  {
    
    /*!
     * Base class for all TestCase
     */
    struct TestCase
      : public Test
    {
      /*! 
       * return the name of the test
       * \return the name of the test
       */
      virtual std::string
      name(void) const;
    protected:
      /*!
       * Constructor
       * \param n : name of the test
       */
      TestCase(const std::string&);
      /*!
       * Destructor
       */
      virtual ~TestCase();
      /*!
       * \param n : description of the test
       * \param b : result
       */
      void
      registerResult(const std::string&,
		     const bool);
    private:
      /*!
       * copy constructor (disabled)
       * \param src : object to be copied
       */
      TestCase(const TestCase&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TestCase&
      operator=(const TestCase&);
    protected:
      //! result of the test
      TestResult result;
    private:
      //! name of test
      const std::string tname;
    }; // end of struct TestCase
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
