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
          s.write(TerminalColors::Green, sizeof(TerminalColors::Green));
        } else {
          s.write(TerminalColors::Red, sizeof(TerminalColors::Red));
        }
      }
      s << successMsg;
      if (&s == &std::cout) {
        s.write(TerminalColors::Reset, sizeof(TerminalColors::Reset));
      }
      s << '\n';
      if (!command.empty()) {
        s << " Command was : " << command << '\n';
      }
      if (!message.empty()) {
        s << " Message : " << message << '\n';
      }
    }

  }  // end of namespace check

}  // end of namespace tfel
