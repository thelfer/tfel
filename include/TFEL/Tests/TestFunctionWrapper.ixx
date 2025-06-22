/*!
 * \file   include/TFEL/Tests/TestFunctionWrapper.ixx
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

#ifndef LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX
#define LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX 1

namespace tfel {

  namespace tests {

    template <bool (*f)(void)>
    TestFunctionWrapper<f>::TestFunctionWrapper(const std::string& g,
                                                const std::string& n)
        : gname(g),
          fname(n) {}  // end of TestFunctionWrapper<f>::TestFunctionWrapper

    template <bool (*f)(void)>
    TestFunctionWrapper<f>::TestFunctionWrapper(const char* const n)
        : fname(n) {}  // end of TestFunctionWrapper<f>::TestFunctionWrapper

    template <bool (*f)(void)>
    std::string TestFunctionWrapper<f>::name() const {
      return this->fname;
    }  // end of TestFunctionWrapper<f>::name

    template <bool (*f)(void)>
    std::string TestFunctionWrapper<f>::classname() const {
      return this->gname;
    }  // end of TestFunctionWrapper<f>::classname

    template <bool (*f)(void)>
    TestResult TestFunctionWrapper<f>::execute() {
      const bool b = (*f)();
      return TestResult(b, this->fname);
    }

    template <bool (*f)(void)>
    TestFunctionWrapper<f>::~TestFunctionWrapper() {
    }  // end of TestFunctionWrapper<f>::~TestFunctionWrapper

  }  // end of namespace tests

}  // end of namespace tfel

#endif /* LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX */
