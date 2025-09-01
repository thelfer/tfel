/*!
 * \file   include/TFEL/Tests/TestCase.hxx
 * \author Thomas Helfer
 * \date   11 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TESTCASE_HXX
#define LIB_TFEL_TESTS_TESTCASE_HXX 1

#include <string>
#include <string_view>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestResult.hxx"

/*!
 * \def TFEL_TESTS_STATIC_ASSERT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X : code to be evaluated
 */
#define TFEL_TESTS_STATIC_ASSERT(X)                              \
  do {                                                           \
    static_assert((X), "static assertion: '" #X "'");            \
    TestCase::registerResult("static assertion: '" #X "'", (X)); \
  } while (0)

/*!
 * \def TFEL_TESTS_ASSERT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: code to be evaluated
 */
#define TFEL_TESTS_ASSERT(X)                                  \
  do {                                                        \
    try {                                                     \
      TestCase::registerResult("assertion: '" #X "'", (X));   \
    } catch (std::exception & tfel_test_exception) {          \
      TestCase::registerResult("assertion: '" #X "'", false,  \
                               tfel_test_exception.what());   \
    } catch (...) {                                           \
      TestCase::registerResult("assertion: '" #X "'", false); \
    }                                                         \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_THROW
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: code to be evaluated
 * \param Y: exeception execpted to be thrown
 */
//-V:TFEL_TESTS_CHECK_THROW:565
#define TFEL_TESTS_CHECK_THROW(X, Y)                                         \
  do {                                                                       \
    bool tfel_test_has_thrown = false;                                       \
    auto tfel_test_details = std::string{};                                  \
    try {                                                                    \
      X;                                                                     \
    } catch (Y&) {                                                           \
      tfel_test_has_thrown = true;                                           \
    } catch (...) {                                                          \
    }                                                                        \
    TestCase::registerResult("check throw: '" #X "'", tfel_test_has_thrown); \
  } while (0)

/*!
 * \def TFEL_TESTS_CALLMETHOD
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: method to be called
 */
#define TFEL_TESTS_CALLMETHOD(X)                             \
  do {                                                       \
    try {                                                    \
      TestCase::registerResult(#X "()", this->X());          \
    } catch (std::exception & tfel_test_exception) {         \
      TestCase::registerResult("assertion: '" #X "'", false, \
                               tfel_test_exception.what());  \
    } catch (...) {                                          \
      TestCase::registerResult(#X "()", false);              \
    }                                                        \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_EQUAL
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: value to be tested
 * \param Y: expected value
 */
#define TFEL_TESTS_CHECK_EQUAL(X, Y)                              \
  do {                                                            \
    try {                                                         \
      TestCase::registerResult("egal: '" #X "==" #Y "'", X == Y); \
    } catch (std::exception & tfel_test_exception) {              \
      TestCase::registerResult("assertion: '" #X "'", false,      \
                               tfel_test_exception.what());       \
    } catch (...) {                                               \
      TestCase::registerResult("egal: '" #X "==" #Y "'", false);  \
    }                                                             \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_GT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: value to be tested
 * \param Y: expected value
 */
#define TFEL_TESTS_CHECK_GT(X, Y)                                  \
  do {                                                             \
    try {                                                          \
      TestCase::registerResult("greater: '" #X ">" #Y "'", X > Y); \
    } catch (std::exception & tfel_test_exception) {               \
      TestCase::registerResult("assertion: '" #X "'", false,       \
                               tfel_test_exception.what());        \
    } catch (...) {                                                \
      TestCase::registerResult("greater: '" #X ">" #Y "'", false); \
    }                                                              \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_GE
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: value to be tested
 * \param Y: expected value
 */
#define TFEL_TESTS_CHECK_GE(X, Y)                                             \
  do {                                                                        \
    try {                                                                     \
      TestCase::registerResult("greater or equal: '" #X ">=" #Y "'", X >= Y); \
    } catch (std::exception & tfel_test_exception) {                          \
      TestCase::registerResult("assertion: '" #X "'", false,                  \
                               tfel_test_exception.what());                   \
    } catch (...) {                                                           \
      TestCase::registerResult("greater or equal: '" #X ">=" #Y "'", false);  \
    }                                                                         \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_LT
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: value to be tested
 * \param Y: expected value
 */
#define TFEL_TESTS_CHECK_LT(X, Y)                                 \
  do {                                                            \
    try {                                                         \
      TestCase::registerResult("lesser: '" #X "<" #Y "'", X < Y); \
    } catch (std::exception & tfel_test_exception) {              \
      TestCase::registerResult("assertion: '" #X "'", false,      \
                               tfel_test_exception.what());       \
    } catch (...) {                                               \
      TestCase::registerResult("lesser: '" #X "<" #Y "'", false); \
    }                                                             \
  } while (0)

/*!
 * \def TFEL_TESTS_CHECK_LE
 * \brief an helper macro to ease the use of TestCase::registerResult
 * \param X: value to be tested
 * \param Y: expected value
 */
#define TFEL_TESTS_CHECK_LE(X, Y)                                            \
  do {                                                                       \
    try {                                                                    \
      TestCase::registerResult("lesser or equal: '" #X "<=" #Y "'", X <= Y); \
    } catch (std::exception & tfel_test_exception) {                         \
      TestCase::registerResult("assertion: '" #X "'", false,                 \
                               tfel_test_exception.what());                  \
    } catch (...) {                                                          \
      TestCase::registerResult("lesser or equal: '" #X "<" #Y "'", false);   \
    }                                                                        \
  } while (0)

namespace tfel::tests {

  //! \brief class used to describe an single test
  struct TFELTESTS_VISIBILITY_EXPORT TestCase : public Test {
    //! \return the name of the test
    std::string name() const override;
    //! \return the group of the test
    std::string classname() const override;

   protected:
    /*!
     * \brief constructor
     * \param[in] n: name  of the test
     */
    TestCase(const std::string&);
    /*!
     * \brief constructor
     * \param[in] g: group of the test
     * \param[in] n: name  of the test
     */
    TestCase(const std::string&, const std::string&);
    /*!
     * \param[in] n: description of the test
     * \param[in] b: result
     * \param[in] e: reason of the failure if any
     */
    virtual void registerResult(const std::string_view,
                                const bool,
                                const std::string_view = std::string_view{});
    //! destructor
    ~TestCase() override;
    //! result of the test
    TestResult result;

   private:
    //! copy constructor (disabled)
    TestCase(const TestCase&) = delete;
    //! move constructor (disabled)
    TestCase(TestCase&&) = delete;
    //! assignement operator (disabled)
    TestCase& operator=(const TestCase&) = delete;
    //! move assignement operator (disabled)
    TestCase& operator=(TestCase&&) = delete;
    //! group of the test
    const std::string gname;
    //! name of the test
    const std::string tname;
  };  // end of struct TestCase

}  // end of namespace tfel::tests

#endif /* LIB_TFEL_TESTS_TESTCASE_HXX */
