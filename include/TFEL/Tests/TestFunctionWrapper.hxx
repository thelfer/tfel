/*!
 * \file   include/TFEL/Tests/TestFunctionWrapper.hxx
 * \author Thomas Helfer
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_HXX
#define LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_HXX 1

#include <string>

#include "TFEL/Tests/Test.hxx"
#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    /*!
     * \brief A simple wrapper around test functions.
     * \param f : function to be wrapped
     */
    template <bool (*f)()>
    struct TestFunctionWrapper final : public Test {
      /*!
       * \brief constructor
       * \param[in] g: group of the test
       * \param[in] n: name  of the function
       */
      TestFunctionWrapper(const std::string&, const std::string&);
      /*!
       * \brief constructor
       * \param[in] n: name of the function
       */
      TestFunctionWrapper(const char* const);
      //! \return the name of the test function
      std::string name() const override;
      //! \return the name of the test function
      std::string classname() const override;
      /*!
       * \brief execute test
       * \return the results of the test
       */
      TestResult execute() override;
      //! destructor
      ~TestFunctionWrapper() override;

     private:
      //! copy constructor (disabled)
      TestFunctionWrapper(const TestFunctionWrapper&) = delete;
      //! assignement operator (disabled)
      TestFunctionWrapper& operator=(const TestFunctionWrapper&) = delete;
      //! name of test group
      const std::string gname;
      //! name of function
      const std::string fname;
    };  // end of struct TestFunctionWrapper

  }  // end of namespace tests

}  // end of namespace tfel

#include "TFEL/Tests/TestFunctionWrapper.ixx"

#endif /* LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_HXX */
