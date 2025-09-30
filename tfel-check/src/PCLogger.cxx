/*!
 * \file PCLogger.cxx
 * \brief PCLogger implementation
 * \author sb152252
 * \date 31 août 2009
 *
 * $Id$
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include "TFEL/Check/PCILogDriver.hxx"
#include "TFEL/Check/PCLogger.hxx"

namespace tfel::check {

  PCLogger::PCLogger() = default;
  PCLogger::PCLogger(PCLogger&&) = default;
  PCLogger::PCLogger(const PCLogger&) = default;
  PCLogger& PCLogger::operator=(PCLogger&&) = default;
  PCLogger& PCLogger::operator=(const PCLogger&) = default;
  PCLogger::~PCLogger() = default;

  PCLogger::PCLogger(const Driver& logDriver) { this->addDriver(logDriver); }

  void PCLogger::addDriver(Driver logDriver) {
    this->drivers.push_back(logDriver);
  }

  void PCLogger::reportSkippedTest(const std::string& d) {
    for (auto& driver : this->drivers) {
      driver->reportSkippedTest(d);
    }
  }

  void PCLogger::addMessage(const std::string& message) {
    for (auto& driver : this->drivers) {
      driver->addMessage(message);
    }
  }

  void PCLogger::addSimpleTestResult(const std::string& testname,
                                     const bool success,
                                     const std::string& message) {
    this->addTestResult(testname, "", "", 0.0, success, message);
  }  // end of addSimpleTestResult

  void PCLogger::addTestResult(const std::string& testname,
                               const std::string& step,
                               const std::string& command,
                               const float time,
                               const bool success,
                               const std::string& message) {
    for (auto& d : this->drivers) {
      d->addTestResult(testname, step, command, time, success, message);
    }
  }  // end of addTestResult

  void PCLogger::terminate() {
    for (auto& driver : this->drivers) {
      driver->terminate();
    }
  }  // end of terminate

}  // end of namespace tfel::check
