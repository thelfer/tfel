/*!
 * \file  tfel-check/src/PCJUnitDriver.cxx
 * \brief JUnit log formater class implemetation.
 * \author sb152252
 * \date 01/09/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Check/PCJUnitDriver.hxx"

namespace tfel::check {

  //! \return the given string with the XML special characters escaped
  static std::string escape(const std::string& s) {
    const auto c1 = tfel::utilities::replace_all(s, "&", "&amp;");
    const auto c2 = tfel::utilities::replace_all(c1, ">", "&gt;");
    const auto c3 = tfel::utilities::replace_all(c2, "<", "&lt;");
    const auto c4 = tfel::utilities::replace_all(c3, "\"", "&quot;");
    return tfel::utilities::replace_all(c4, "'", "&apos;");
  }  // end of escape

  /*!
   * \return the given string without `--`, which is not allowed in an XML
   * comment, nor a trailing `-`
   */
  static std::string escapeComment(const std::string& s) {
    auto r = s;
    while (r.find("--") != std::string::npos) {
      r = tfel::utilities::replace_all(r, "--", "- -");
    }
    if ((!r.empty()) && (r.back() == '-')) {
      r += ' ';
    }
    return r;
  }  // end of escapeComment

  PCJUnitDriver::PCJUnitDriver(const std::string& f) : PCILogDriver(f) {
    this->getStream()
        << "<?xml version=\"1.0\" ?><testsuite name=\"tfel-check\">\n";
  }

  void PCJUnitDriver::addMessage(const std::string& message) {
    this->getStream() << "<!--" << escapeComment(message) << "-->\n";
  }

  void PCJUnitDriver::reportSkippedTest(const std::string& d) {
    this->getStream() << "<!-- test '" << escapeComment(d)
                      << "' has been skipped -->\n";
  }  // end of PCJUnitDriver::reportSkippedTest

  void PCJUnitDriver::addTestResult(const std::string& testname,
                                    const std::string& step,
                                    const std::string& command,
                                    float time,
                                    bool success,
                                    const std::string& message) {
    using namespace std;
    string testcase;
    ostringstream stime;
    stime << time;

    const auto open = (success) ? "  <success>" : "  <failure>";
    const auto close = (success) ? "  </success>" : "  </failure>";

    this->getStream() << "<testcase classname=\"" << escape(testname)
                      << "\" name=\"" << escape(step) << "\" time=\""
                      << stime.str() << "\">\n";
    this->getStream() << open << '\n';
    this->getStream() << "  Command was : " + escape(command) << '\n';
    this->getStream() << escape(message) << '\n';
    this->getStream() << close << '\n';
    this->getStream() << "</testcase>\n";
  }

  void PCJUnitDriver::terminate() { this->getStream() << "</testsuite>\n"; }

  PCJUnitDriver::~PCJUnitDriver() = default;

}  // end of namespace tfel::check
