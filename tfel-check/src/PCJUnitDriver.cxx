/*!
 * \brief JUnit log formater class implemetation.
 *
 * \file PCJUnitDriver.cpp
 *
 * \author sb152252
 * \date 1 sept. 2009
 *
 * $Id$
 */

#include <sstream>
#include "TFELCheck/PCJUnitDriver.hxx"

namespace tfel_check {

  PCJUnitDriver::PCJUnitDriver(const std::string& f)
    : PCILogDriver(f)
  {
    this->getStream() << "<?xml version=\"1.0\" ?><testsuite name=\"tfel-check\">\n";
  }

  PCJUnitDriver::~PCJUnitDriver() = default;

  void PCJUnitDriver::addMessage(const std::string & message) {
    this->getStream() << "<!--" << message << "-->\n";
  }

  void PCJUnitDriver::addTestResult(const std::string& testname,
				    const std::string& step,
				    const std::string& command, float time,
				    bool success, const std::string& message) {
    using namespace std;
    string testcase;
    ostringstream stime;
    stime << time;

    const auto open = (success) ? "  <success>" : "  <failure>";
    const auto close = (success) ? "  </success>" : "  </failure>";

    this->getStream() << "<testcase classname=\"" << testname << "\" name=\""
		      << step << "\" time=\"" << stime.str() << "\">\n";
    this->getStream() << open << '\n';
    this->getStream() << "  Command was : " + command << '\n';
    this->getStream() << message << '\n';
    this->getStream() << close << '\n';
    this->getStream() << "</testcase>\n";
  }

  void PCJUnitDriver::terminate() {
    this->getStream() << "</testsuite>\n";
  }
}  // namespace tfel_check
