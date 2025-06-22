/*!
 * \file   include/TFEL/Tests/TestFunctionWrapper.ixx
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

#ifndef LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX
#define LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX 1

namespace tfel::tests {

  template <bool (*f)()>
  TestFunctionWrapper<f>::TestFunctionWrapper(const std::string& g,
                                              const std::string& n)
      : gname(g),
        fname(n) {}  // end of TestFunctionWrapper<f>::TestFunctionWrapper

  template <bool (*f)()>
  TestFunctionWrapper<f>::TestFunctionWrapper(const char* const n)
      : fname(n) {}  // end of TestFunctionWrapper<f>::TestFunctionWrapper

  template <bool (*f)()>
  std::string TestFunctionWrapper<f>::name() const {
    return this->fname;
  }  // end of TestFunctionWrapper<f>::name

  template <bool (*f)()>
  std::string TestFunctionWrapper<f>::classname() const {
    return this->gname;
  }  // end of TestFunctionWrapper<f>::classname

  template <bool (*f)()>
  TestResult TestFunctionWrapper<f>::execute() {
    const bool b = (*f)();
    return TestResult(b, this->fname);
  }

  template <bool (*f)()>
  TestFunctionWrapper<f>::~TestFunctionWrapper() {
  }  // end of TestFunctionWrapper<f>::~TestFunctionWrapper

}  // end of namespace tfel::tests

#endif /* LIB_TFEL_TESTS_TESTFUNCTIONWRAPPER_IXX */
