/*!
 * \file   src/Tests/XMLTestOutput.cxx
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
#include <sstream>
#include <stdexcept>

#include <iostream>

#include "TFEL/Tests/XMLTestOutput.hxx"

namespace tfel {

  namespace tests {

    static void replace_all(std::string& s,
                            const char c,
                            const std::string& n) {
      using namespace std;
      string::size_type p = 0u;
      string::size_type ns = n.size();
      if ((s.empty()) || (ns == 0)) {
        return;
      }
      while ((p = s.find(c, p)) != string::npos) {
        s.replace(p, 1u, n);
        p += ns;
      }
    }  // end of replace_all

    static void convert_to_xml(std::string& s) {
      replace_all(s, '&', "&amp;");
      replace_all(s, '<', "&lt;");
      replace_all(s, '>', "&gt;");
      replace_all(s, '"', "&quot;");
      replace_all(s, '\'', "&apos;");
    }

    XMLTestOutput::XMLTestOutput(const std::string& o)
        : file(o), testsuite(0u) {
      if (this->file.size() >= 4) {
        if (this->file.substr(this->file.size() - 4) == ".xml") {
          this->file = this->file.substr(0u, this->file.size() - 4);
        }
      }
    }  // end of XMLTestOutput::XMLTestOutput

    void XMLTestOutput::beginTestSuite(const std::string& n) {
      using namespace std;
      if (this->os.is_open()) {
        string msg("XMLTestOutput::XMLTestOutput : ");
        msg += "output file is not closed";
        throw(runtime_error(msg));
      }
      ostringstream f;
      f << this->file;
      if (this->testsuite != 0u) {
        f << '-' << this->testsuite;
      }
      f << ".xml";
      ++(this->testsuite);
      this->os.open(f.str().c_str());
      if (!(this->os)) {
        string msg("XMLTestOutput::XMLTestOutput : ");
        msg += "can't open file '" + f.str() + "'";
        throw(runtime_error(msg));
      }
      string out(n);
      convert_to_xml(out);
      this->os << "<?xml version=\"1.0\" ?>" << endl;
      this->os << "<testsuite name=\"" + out + "\">" << endl;
    }  // end of XMLTestOutput::beginTestSuite

    void XMLTestOutput::addTest(const std::string& g,
                                const std::string& n,
                                const TestResult& r) {
      using namespace std;
      TestResult::const_iterator p;
      string gout(g);
      string nout(n);
      convert_to_xml(nout);
      convert_to_xml(gout);
      this->os << "<testcase classname=\"" << gout << "\" name=\"" << nout
               << "\" time=\"" << r.duration() << "\">" << endl;
      if (r.success()) {
        this->os << "<success>" << endl;
      } else {
        this->os << "<failure>" << endl;
      }
      if (!r.details().empty()) {
        string out(r.details());
        convert_to_xml(out);
        this->os << out << endl;
      }
      for (p = r.begin(); p != r.end(); ++p) {
        this->treatTest(*p);
      }
      if (r.success()) {
        this->os << "</success>" << endl;
      } else {
        this->os << "</failure>" << endl;
      }
      this->os << "</testcase>" << endl;
    }  // end of XMLTestOutput::addTest

    void XMLTestOutput::treatTest(const TestResult& r) {
      using namespace std;
      TestResult::const_iterator p;
      string out;
      if (r.success()) {
        out = "SUCCESS : ";
      } else {
        out = "FAILURE : ";
      }
      out += r.details();
      convert_to_xml(out);
      this->os << out << endl;
      for (p = r.begin(); p != r.end(); ++p) {
        this->treatTest(*p);
      }
    }  // end of XMLTestOutput::treatTest

    void XMLTestOutput::endTestSuite(const TestResult&) {
      this->os << "</testsuite>" << std::endl;
      this->os.close();
    }  // end of XMLTestOutput::endTestSuite

    XMLTestOutput::~XMLTestOutput() = default;

  }  // end of namespace tests

}  // end of namespace tfel
