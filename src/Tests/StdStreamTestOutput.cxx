/*!
 * \file   src/Tests/StdStreamTestOutput.cxx
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
#include "TFEL/Raise.hxx"
#include "TFEL/Tests/StdStreamTestOutput.hxx"

namespace tfel::tests {

  StdStreamTestOutput::StdStreamTestOutput(const std::string& o)
      : pos(new std::ofstream(o.c_str())),
        os(*(pos.get())),
        colorOutput(false) {
    raise_if(!(*pos),
             "StdStreamTestOutput::StdStreamTestOutput: "
             "can't open file '" +
                 o + "'");
  }  // end of StdStreamTestOutput::StdStreamTestOutput

  StdStreamTestOutput::StdStreamTestOutput(std::ostream& o, const bool b)
      : os(o), colorOutput(b) {
    raise_if(!os,
             "StdStreamTestOutput::StdStreamTestOutput: "
             "invalid stream");
  }  // end of StdStreamTestOutput::StdStreamTestOutput

  StdStreamTestOutput::StdStreamTestOutput(
      const std::shared_ptr<std::ostream>& o, const bool b)
      : pos(o), os(*(pos.get())), colorOutput(b) {
    raise_if(!os,
             "StdStreamTestOutput::StdStreamTestOutput: "
             "invalid stream");
  }  // end of StdStreamTestOutput::StdStreamTestOutput

  void StdStreamTestOutput::beginTestSuite(const std::string& n) {
    this->os << "\nBeginning of test suite '" << n << "'\n";
  }  // end of StdStreamTestOutput::beginTestSuite

  void StdStreamTestOutput::addTest(const std::string& g,
                                    const std::string& n,
                                    const TestResult& r) {
    const char red[5] = {033, '[', '3', '1', 'm'};
    const char reset[4] = {033, '[', '0', 'm'};
    auto out = "Result of test '" + n + "'";
    if (!g.empty()) {
      out += " of group '" + g + "'";
    }
    if (out.size() < 65) {
      out.resize(65, ' ');
    } else {
      out = out.substr(0, 65);
    }
    if (r.success()) {
      this->os << out << ": SUCCESS\n";
    } else {
      this->os << out << ": ";
      if (this->colorOutput) {
        this->os.write(red, sizeof(red));
      }
      this->os << "FAILED\n";
      if (this->colorOutput) {
        this->os.write(reset, sizeof(reset));
      }
    }
    for (const auto& t : r) {
      this->treatTest(t, "* ");
    }
  }  // end of StdStreamTestOutput::addTest

  void StdStreamTestOutput::treatTest(const TestResult& r,
                                      const std::string& s) {
    const char red[5] = {033, '[', '3', '1', 'm'};
    const char reset[4] = {033, '[', '0', 'm'};
    auto out = s;
    out += r.getDescription();
    if (out.size() < 65) {
      out.resize(65, ' ');
    } else {
      out = out.substr(0, 65);
    }
    if (r.success()) {
      this->os << out << ": SUCCESS\n";
    } else {
      this->os << out << ": ";
      if (this->colorOutput) {
        this->os.write(red, sizeof(red));
      }
      this->os << "FAILED\n";
      const auto& error = r.getFailureDescription();
      if (!error.empty()) {
        this->os << error << '\n';
      }
      if (this->colorOutput) {
        this->os.write(reset, sizeof(reset));
      }
    }
    for (const auto& t : r) {
      this->treatTest(t, s + '*');
    }
  }  // end of StdStreamTestOutput::treatTest

  void StdStreamTestOutput::endTestSuite(const TestResult& r) {
    const char red[5] = {033, '[', '3', '1', 'm'};
    const char reset[4] = {033, '[', '0', 'm'};
    auto out = std::string("End of Test Suite");
    out.resize(65, ' ');
    this->os << out << ": ";
    if (r.success()) {
      this->os << "SUCCESS\n";
    } else {
      if (this->colorOutput) {
        this->os.write(red, sizeof(red));
      }
      this->os << "FAILED\n";
      if (this->colorOutput) {
        this->os.write(reset, sizeof(reset));
      }
    }
  }  // end of StdStreamTestOutput::endTestSuite

  StdStreamTestOutput::~StdStreamTestOutput() = default;

}  // end of namespace tfel::tests
