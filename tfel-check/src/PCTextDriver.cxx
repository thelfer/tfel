/*!
 * \brief ASCII text log formater class implementation.
 *
 * \file PCTextDriver.cxx
 *
 * \author sb152252
 * \date 1 sept. 2009
 *
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <iomanip>

#include "TFEL/Utilities/TerminalColors.hxx"
#include "TFEL/Check/PCTextDriver.hxx"

namespace tfel {

  namespace check {

    PCTextDriver::PCTextDriver() = default;

    PCTextDriver::PCTextDriver(const std::string& f) : PCILogDriver(f) {}

    PCTextDriver::~PCTextDriver() = default;

    void PCTextDriver::addMessage(const std::string& message) {
      this->getStream() << message << std::endl;
    }

    static std::string formatMessage(const std::string& t,
                                     const std::string& s) {
      auto msg = t;
      if (!s.empty()) {
        msg += ":" + s;
      }
      if (msg.size() >= 70) {
        msg.resize(67);
        msg += "...";
      }
      return msg;
    }  // end of formatMessage

    template <size_t N>
    static void setStreamColor(std::ostream& os, const char (&c)[N]) {
      if (&os == &std::cout) {
        os.write(c, sizeof(c));
      }
    }  // end of setStreamColor

    void PCTextDriver::reportSkippedTest(const std::string& d) {
      using tfel::utilities::TerminalColors;
      const auto msg = formatMessage(d, "");
      // format with 80 char width
      auto& os = this->getStream();
      os << std::setw(70) << std::left << std::setfill(' ') << msg
         << std::setfill(' ') << std::right << std::setw(10);
      setStreamColor(os, TerminalColors::Yellow);
      os << "[SKIPPED]";
      setStreamColor(os, TerminalColors::Reset);
      os << '\n';
    }  // end of PCTextDriver::reportSkippedTest

    void PCTextDriver::addTestResult(const std::string& testname,
                                     const std::string& step,
                                     const std::string& command,
                                     const float,
                                     const bool success,
                                     const std::string& message) {
      using tfel::utilities::TerminalColors;
      const auto successMsg = (success) ? "[SUCCESS]" : "[ FAILED]";
      const auto msg = formatMessage(testname, step);
      // format with 80 char width
      auto& os = this->getStream();
      os << std::setw(70) << std::left << std::setfill(' ') << msg
         << std::setfill(' ') << std::right << std::setw(10);
      if (success) {
        setStreamColor(os, TerminalColors::Green);
      } else {
        setStreamColor(os, TerminalColors::Red);
      }
      os << successMsg;
      setStreamColor(os, TerminalColors::Reset);
      os << '\n';
      if (!command.empty()) {
        os << " Command was : " << command << '\n';
      }
      if (!message.empty()) {
        os << " Message : " << message << '\n';
      }
    }

  }  // end of namespace check

}  // end of namespace tfel
