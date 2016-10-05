/*!
 * \brief ASCII text log formater class implementation.
 *
 * \file PCTextDriver.cxx
 *
 * \author sb152252
 * \date 1 sept. 2009
 *
 * $Id$
 */

#include <iostream>
#include <iomanip>

#include"TFEL/Utilities/TerminalColors.hxx"
#include "TFELCheck/PCTextDriver.hxx"

namespace tfel_check {

  PCTextDriver::PCTextDriver() = default;
  
  PCTextDriver::PCTextDriver(const std::string& f)
    : PCILogDriver(f)
  {}

  PCTextDriver::~PCTextDriver() = default;

  void PCTextDriver::addMessage(const std::string& message) {
    this->getStream() << message << std::endl;
  }

  void PCTextDriver::addTestResult(const std::string& testname,
				   const std::string& step,
				   const std::string& command,
				   const float,
				   const bool success,
				   const std::string& message) {
    using tfel::utilities::TerminalColors;
    const auto successMsg = (success) ? "[SUCCESS]" : "[ FAILED]";
    auto localtestname = testname;
    if (!step.empty()) {
      localtestname += ":" + step;
    }
    if (localtestname.size() >= 70) {
      localtestname.resize(67);
      localtestname += "...";
    }
    // format with 80 char width
    auto& s = this->getStream();
    s << std::setw(70) << std::left << std::setfill(' ') << localtestname
      << std::setfill(' ') << std::right << std::setw(10);
    if (&s == &std::cout) {
      if (success) {
	s.write(TerminalColors::Green,
		sizeof(TerminalColors::Green));
      } else {
	s.write(TerminalColors::Red,
		sizeof(TerminalColors::Red));
      }
    }
    s << successMsg;
    if (&s==&std::cout) {
      s.write(TerminalColors::Reset,
	      sizeof(TerminalColors::Reset));
    }
    s << '\n';
    if (!command.empty()) {
      s << " Command was : " << command << '\n';
    }
    if (!message.empty()) {
      s << " Message : " << message << '\n';
    }
  }
  
}  // namespace tfel_check

