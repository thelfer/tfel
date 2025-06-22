/*!
 * \file   src/Tests/StdStreamTestOutput.cxx
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

#include "TFEL/Tests/StdStreamTestOutput.hxx"

namespace tfel {

  namespace tests {

    StdStreamTestOutput::StdStreamTestOutput(const std::string& o)
        : pos(new std::ofstream(o.c_str())),
          os(*(pos.get())),
          colorOutput(false) {
      using namespace std;
      if (!(*pos)) {
        string msg("StdStreamTestOutput::StdStreamTestOutput : ");
        msg += "can't open file '" + o + "'";
        throw(runtime_error(msg));
      }
    }  // end of StdStreamTestOutput::StdStreamTestOutput

    StdStreamTestOutput::StdStreamTestOutput(std::ostream& o, const bool b)
        : os(o), colorOutput(b) {
      using namespace std;
      if (!os) {
        string msg("StdStreamTestOutput::StdStreamTestOutput : ");
        msg += "invalid stream";
        throw(runtime_error(msg));
      }
    }  // end of StdStreamTestOutput::StdStreamTestOutput

    StdStreamTestOutput::StdStreamTestOutput(
        const std::shared_ptr<std::ostream>& o, const bool b)
        : pos(o), os(*(pos.get())), colorOutput(b) {
      using namespace std;
      if (!os) {
        string msg("StdStreamTestOutput::StdStreamTestOutput : ");
        msg += "invalid stream";
        throw(runtime_error(msg));
      }
    }  // end of StdStreamTestOutput::StdStreamTestOutput

    void StdStreamTestOutput::beginTestSuite(const std::string& n) {
      using namespace std;
      this->os << endl;
      this->os << "Beginning of test suite '" << n << "'";
      this->os << endl;
    }  // end of StdStreamTestOutput::beginTestSuite

    void StdStreamTestOutput::addTest(const std::string& g,
                                      const std::string& n,
                                      const TestResult& r) {
      using namespace std;
      const char red[5] = {033, '[', '3', '1', 'm'};
      const char reset[4] = {033, '[', '0', 'm'};
      TestResult::const_iterator p;
      string out;
      out += "Result of test '";
      out += n;
      out += "' ";
      if (!g.empty()) {
        out += "of group '" + g + "' ";
      }
      if (out.size() < 65) {
        out.resize(65, ' ');
      } else {
        out = out.substr(0, 65);
      }
      if (r.success()) {
        this->os << out << ": SUCCESS" << endl;
      } else {
        this->os << out << ": ";
        if (this->colorOutput) {
          this->os.write(red, sizeof(red));
        }
        this->os << "FAILED" << endl;
        if (this->colorOutput) {
          this->os.write(reset, sizeof(reset));
        }
      }
      for (p = r.begin(); p != r.end(); ++p) {
        this->treatTest(*p, "* ");
      }
    }  // end of StdStreamTestOutput::addTest

    void StdStreamTestOutput::treatTest(const TestResult& r,
                                        const std::string& s) {
      using namespace std;
      const char red[5] = {033, '[', '3', '1', 'm'};
      const char reset[4] = {033, '[', '0', 'm'};
      TestResult::const_iterator p;
      string out(s);
      out += r.details();
      if (out.size() < 65) {
        out.resize(65, ' ');
      } else {
        out = out.substr(0, 65);
      }
      if (r.success()) {
        this->os << out << ": SUCCESS" << endl;
      } else {
        this->os << out << ": ";
        if (this->colorOutput) {
          this->os.write(red, sizeof(red));
        }
        this->os << "FAILED" << endl;
        this->os << r.details() << endl;
        if (this->colorOutput) {
          this->os.write(reset, sizeof(reset));
        }
      }
      for (p = r.begin(); p != r.end(); ++p) {
        this->treatTest(*p, s + '*');
      }
    }  // end of StdStreamTestOutput::treatTest

    void StdStreamTestOutput::endTestSuite(const TestResult& r) {
      using namespace std;
      const char red[5] = {033, '[', '3', '1', 'm'};
      const char reset[4] = {033, '[', '0', 'm'};
      string out("End of Test Suite ");
      if (out.size() < 65) {
        out.resize(65, ' ');
      } else {
        out = out.substr(0, 65);
      }
      this->os << out << ": ";
      if (r.success()) {
        this->os << "SUCCESS" << endl;
      } else {
        if (this->colorOutput) {
          this->os.write(red, sizeof(red));
        }
        this->os << "FAILED" << endl;
        if (this->colorOutput) {
          this->os.write(reset, sizeof(reset));
        }
      }
    }  // end of StdStreamTestOutput::endTestSuite

    StdStreamTestOutput::~StdStreamTestOutput() = default;

  }  // end of namespace tests

}  // end of namespace tfel
