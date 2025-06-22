/*!
 * \file   src/Tests/MultipleTestOutputs.cxx
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

#include <fstream>
#include <stdexcept>

#include "TFEL/Tests/MultipleTestOutputs.hxx"

namespace tfel::tests {

  MultipleTestOutputs::MultipleTestOutputs() = default;

  void MultipleTestOutputs::addTestOutput(const TestOutputPtr& o) {
    this->outputs.push_back(o);
  }  // end of MultipleTestOutputs::addTestOutput

  void MultipleTestOutputs::beginTestSuite(const std::string& n) {
    for (auto& o : this->outputs) {
      o->beginTestSuite(n);
    }
  }  // end of MultipleTestOutputs::beginTestSuite

  void MultipleTestOutputs::addTest(const std::string& g,
                                    const std::string& n,
                                    const TestResult& r) {
    for (auto& o : this->outputs) {
      o->addTest(g, n, r);
    }
  }  // end of MultipleTestOutputs::addTest

  void MultipleTestOutputs::endTestSuite(const TestResult& r) {
    for (auto& o : this->outputs) {
      o->endTestSuite(r);
    }
  }  // end of MultipleTestOutputs::endTestSuite

  MultipleTestOutputs::~MultipleTestOutputs() = default;

}  // end of namespace tfel::tests
