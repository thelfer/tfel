/*!
 * \file   src/Tests/XMLTestOutput.cxx
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
#include <sstream>
#include <stdexcept>

#include <iostream>

#include "TFEL/Raise.hxx"
#include "TFEL/Tests/XMLTestOutput.hxx"

namespace tfel::tests {

  static void replace_all(std::string& s, const char c, const std::string& n) {
    std::string::size_type p = 0u;
    std::string::size_type ns = n.size();
    if ((s.empty()) || (ns == 0)) {
      return;
    }
    while ((p = s.find(c, p)) != std::string::npos) {
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

  XMLTestOutput::XMLTestOutput(const std::string& o) : file(o), testsuite(0u) {
    if (this->file.size() >= 4) {
      if (this->file.substr(this->file.size() - 4) == ".xml") {
        this->file = this->file.substr(0u, this->file.size() - 4);
      }
    }
  }  // end of XMLTestOutput::XMLTestOutput

  void XMLTestOutput::beginTestSuite(const std::string& n) {
    raise_if(this->os.is_open(),
             "XMLTestOutput::beginTestSuite: "
             "output file is not closed");
    std::ostringstream f;
    f << this->file;
    if (this->testsuite != 0u) {
      f << '-' << this->testsuite;
    }
    f << ".xml";
    ++(this->testsuite);
    this->os.open(f.str());
    raise_if(!(this->os),
             "XMLTestOutput::XMLTestOutput: "
             "can't open file '" +
                 f.str() + "'");
    std::string out(n);
    convert_to_xml(out);
    this->os << "<?xml version=\"1.0\" ?>\n"
             << "<testsuite name=\"" + out + "\">\n";
  }  // end of XMLTestOutput::beginTestSuite

  void XMLTestOutput::addTest(const std::string& g,
                              const std::string& n,
                              const TestResult& r) {
    std::string gout(g);
    std::string nout(n);
    convert_to_xml(nout);
    convert_to_xml(gout);
    this->os << "<testcase classname=\"" << gout << "\" name=\"" << nout
             << "\" time=\"" << r.duration() << "\">\n";
    if (r.success()) {
      this->os << "<success>\n";
    } else {
      this->os << "<failure>\n";
    }
    const auto& d = r.getDescription();
    const auto& e = r.getFailureDescription();
    if (!d.empty()) {
      std::string out(d);
      convert_to_xml(out);
      this->os << out << '\n';
    }
    if (!e.empty()) {
      std::string out(e);
      convert_to_xml(out);
      this->os << out << '\n';
    }
    for (const auto& t : r) {
      this->treatTest(t);
    }
    if (r.success()) {
      this->os << "</success>\n";
    } else {
      this->os << "</failure>\n";
    }
    this->os << "</testcase>\n";
  }  // end of XMLTestOutput::addTest

  void XMLTestOutput::treatTest(const TestResult& r) {
    std::string out;
    if (r.success()) {
      out = "SUCCESS : ";
    } else {
      out = "FAILURE : ";
    }
    out += r.getDescription();
    convert_to_xml(out);
    this->os << out << '\n';
    for (const auto& t : r) {
      this->treatTest(t);
    }
  }  // end of XMLTestOutput::treatTest

  void XMLTestOutput::endTestSuite(const TestResult&) {
    this->os << "</testsuite>\n";
    this->os.close();
  }  // end of XMLTestOutput::endTestSuite

  XMLTestOutput::~XMLTestOutput() = default;

}  // end of namespace tfel::tests
