/*!
 * \file   src/Tests/TestCase.cxx
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

#include "TFEL/Tests/TestCase.hxx"

namespace tfel {

  namespace tests {

    TestCase::TestCase(const std::string& n)
        : gname("UnitTest"), tname(n) {}  // end of TestCase::~TestCase

    TestCase::TestCase(const std::string& g, const std::string& n)
        : gname(g), tname(n) {}  // end of TestCase::~TestCase

    std::string TestCase::classname() const {
      return this->gname;
    }  // end of TestCase::classname

    std::string TestCase::name() const {
      return this->tname;
    }  // end of TestCase::~TestCase

    void TestCase::registerResult(const std::string& n, const bool b) {
      this->result.append(TestResult(b, n));
    }  // end of TestCase::registerResult

    TestCase::~TestCase() = default;

  }  // end of namespace tests

}  // end of namespace tfel
