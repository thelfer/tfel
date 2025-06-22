/*!
 * \file   include/TFEL/Tests/Test.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_TESTS_TEST_HXX
#define LIB_TFEL_TESTS_TEST_HXX 1

#include <string>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Tests/TestResult.hxx"

namespace tfel {

  namespace tests {

    /*!
     * Base class for all tests
     */
    struct TFELTESTS_VISIBILITY_EXPORT Test {
      /*!
       * return the name of the test
       * \return the name of the test
       */
      virtual std::string name() const = 0;
      /*!
       * return the group of the test
       * \return the group of the test
       */
      virtual std::string classname() const = 0;
      /*!
       * execute test
       * \return the results of the test
       */
      virtual TestResult execute() = 0;
      /*!
       * destructor
       */
      virtual ~Test();
    };  // end of struct Test

  }  // end of namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TEST_HXX */
