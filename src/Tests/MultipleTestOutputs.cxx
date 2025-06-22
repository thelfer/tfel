/*!
 * \file   src/Tests/MultipleTestOutputs.cxx
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

#include <fstream>
#include <stdexcept>

#include "TFEL/Tests/MultipleTestOutputs.hxx"

namespace tfel {

  namespace tests {

    MultipleTestOutputs::MultipleTestOutputs() {
    }  // end of MultipleTestOutputs::MultipleTestOutputs

    void MultipleTestOutputs::addTestOutput(const TestOutputPtr& o) {
      this->outputs.push_back(o);
    }  // end of MultipleTestOutputs::addTestOutput

    void MultipleTestOutputs::beginTestSuite(const std::string& n) {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for (p = this->outputs.begin(); p != this->outputs.end(); ++p) {
        (*p)->beginTestSuite(n);
      }
    }  // end of MultipleTestOutputs::beginTestSuite

    void MultipleTestOutputs::addTest(const std::string& g,
                                      const std::string& n,
                                      const TestResult& r) {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for (p = this->outputs.begin(); p != this->outputs.end(); ++p) {
        (*p)->addTest(g, n, r);
      }
    }  // end of MultipleTestOutputs::addTest

    void MultipleTestOutputs::endTestSuite(const TestResult& r) {
      using namespace std;
      vector<TestOutputPtr>::iterator p;
      for (p = this->outputs.begin(); p != this->outputs.end(); ++p) {
        (*p)->endTestSuite(r);
      }
    }  // end of MultipleTestOutputs::endTestSuite

    MultipleTestOutputs::~MultipleTestOutputs() = default;

  }  // end of namespace tests

}  // end of namespace tfel
