/*!
 * \file  tfel-check/src/PCJUnitDriver.cxx
 * \brief JUnit log formater class implemetation.
 * \author sb152252
 * \date 1 sept. 2009
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

namespace tfel {

  namespace check {

    PCJUnitDriver::PCJUnitDriver(const std::string& f) : PCILogDriver(f) {
      this->getStream()
          << "<?xml version=\"1.0\" ?><testsuite name=\"tfel-check\">\n";
    }

    void PCJUnitDriver::addMessage(const std::string& message) {
      this->getStream() << "<!--" << message << "-->\n";
    }

    void PCJUnitDriver::reportSkippedTest(const std::string& d) {
      this->getStream() << "<!-- test '" << d << "' has been skipped -->\n";
    }  // end of PCJUnitDriver::reportSkippedTest

    void PCJUnitDriver::addTestResult(const std::string& testname,
                                      const std::string& step,
                                      const std::string& command,
                                      float time,
                                      bool success,
                                      const std::string& message) {
      using namespace std;
      const auto c1 = tfel::utilities::replace_all(command, "&", "&amp;");
      const auto c2 = tfel::utilities::replace_all(c1, ">", "&gt;");
      const auto c3 = tfel::utilities::replace_all(c2, "<", "&lt;");
      const auto c4 = tfel::utilities::replace_all(c3, "\"", "&quot;");
      const auto c5 = tfel::utilities::replace_all(c4, "'", "&apos;");
      string testcase;
      ostringstream stime;
      stime << time;

      const auto open = (success) ? "  <success>" : "  <failure>";
      const auto close = (success) ? "  </success>" : "  </failure>";

      this->getStream() << "<testcase classname=\"" << testname << "\" name=\""
                        << step << "\" time=\"" << stime.str() << "\">\n";
      this->getStream() << open << '\n';
      this->getStream() << "  Command was : " + c5 << '\n';
      this->getStream() << message << '\n';
      this->getStream() << close << '\n';
      this->getStream() << "</testcase>\n";
    }

    void PCJUnitDriver::terminate() { this->getStream() << "</testsuite>\n"; }

    PCJUnitDriver::~PCJUnitDriver() = default;

  }  // end of namespace check

}  // end of namespace tfel
