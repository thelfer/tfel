/*!
 * \file   include/TFEL/Tests/TestCase.hxx
 * \author Helfer Thomas
 * \date   11 Apr 10
 * \brief  
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_TFEL_TESTS_TESTCASE_HXX
#define _LIB_TFEL_TESTS_TESTCASE_HXX 1

#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestResult.hxx"

/*!
 * \def TFEL_TESTS_STATIC_ASSERT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : code to be evaluated
 */
#define TFEL_TESTS_STATIC_ASSERT(X)				\
  {						              	\
    static_assert((X),"static assertion : '"#X"'");		\
    TestCase::registerResult("static assertion : '"#X"'",(X));	\
  }

/*!
 * \def TFEL_TESTS_ASSERT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : code to be evaluated
 */
#define TFEL_TESTS_ASSERT(X)				\
  {							\
    TestCase::registerResult("assertion : '"#X"'",(X));	\
  }

/*!
 * \def TFEL_TESTS_CHECK_THROW
 * \brief an helper macro to ease the use of TestCase::registerResult
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
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : method to be called
 */
#define TFEL_TESTS_CALLMETHOD(X)                                \
  {                                                             \
    TestCase::registerResult(#X"()",this->X());          	\
  }

/*!
 * \def TFEL_TESTS_CHECK_EQUAL
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_EQUAL(X,Y)				\
  {                                                             \
    TestCase::registerResult("egal : '"#X"=="#Y"'",X==Y);       \
  }

/*!
 * \def TFEL_TESTS_CHECK_GT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_GT(X,Y)				\
  {                                                             \
    TestCase::registerResult("greater : '"#X">"#Y"'",X>Y);     \
  }

/*!
 * \def TFEL_TESTS_CHECK_GE
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_GE(X,Y)				      \
  {                                                                   \
    TestCase::registerResult("greater or equal : '"#X">="#Y"'",X>=Y); \
  }

/*!
 * \def TFEL_TESTS_CHECK_LT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_LT(X,Y)				\
  {                                                             \
    TestCase::registerResult("lesser : '"#X"<"#Y"'",X<Y);      \
  }

/*!
 * \def TFEL_TESTS_CHECK_LE
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : value to be tested
 * \param Y : expected value
 */
#define TFEL_TESTS_CHECK_LE(X,Y)				        \
  {                                                                     \
    TestCase::registerResult("lesser or equal : '"#X"<="#Y"'",X<=Y);    \
  }



namespace tfel
{

  namespace tests
  {
    
    /*!
     * Base class for all TestCase
     */
    struct TFELTESTS_VISIBILITY_EXPORT TestCase
      : public Test
    {
      /*! 
       * return the name of the test
       * \return the name of the test
       */
      virtual std::string
      name(void) const;
      /*! 
       * return the group of the test
       * \return the group of the test
       */
      virtual std::string
      classname(void) const;
    protected:
      /*!
       * Constructor
       * \param n : name  of the test
       */
      TestCase(const std::string&);
      /*!
       * Constructor
       * \param g : group of the test
       * \param n : name  of the test
       */
      TestCase(const std::string&,
	       const std::string&);
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
      TFEL_VISIBILITY_LOCAL TestCase(const TestCase&);
      /*!
       * assignement operator (disabled)
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TFEL_VISIBILITY_LOCAL TestCase&
      operator=(const TestCase&);
    protected:
      //! result of the test
      TestResult result;
    private:
      //! group of the test
      const std::string gname;
      //! name of the test
      const std::string tname;
    }; // end of struct TestCase
    
  } // end of namespace test

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TEST_HXX */
