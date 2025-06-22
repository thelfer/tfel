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
#include "TFEL/Check/PCLogger.hxx"

namespace tfel {

  namespace check {

    PCLogger::PCLogger() = default;
    PCLogger::PCLogger(PCLogger&&) = default;
    PCLogger::PCLogger(const PCLogger&) = default;
    PCLogger& PCLogger::operator=(PCLogger&&) = default;
    PCLogger& PCLogger::operator=(const PCLogger&) = default;
    PCLogger::~PCLogger() = default;

    PCLogger::PCLogger(const driver& logDriver) { this->addDriver(logDriver); }

    void PCLogger::addDriver(driver logDriver) {
      this->drivers.push_back(logDriver);
    }

    void PCLogger::addMessage(const std::string& message) {
      for (auto& d : this->drivers) {
        d->addMessage(message);
      }
    }

    void PCLogger::addSimpleTestResult(const std::string& testname,
                                       bool success,
                                       const std::string& message) {
      this->addTestResult(testname, "", "", 0.0, success, message);
    }

    void PCLogger::addTestResult(const std::string& testname,
                                 const std::string& step,
                                 const std::string& command,
                                 float time,
                                 bool success,
                                 const std::string& message) {
      for (auto& d : this->drivers) {
        d->addTestResult(testname, step, command, time, success, message);
      }
    }

    void PCLogger::terminate() {
      for (auto& d : this->drivers) {
        d->terminate();
      }
    }

  }  // end of namespace check

}  // end of namespace tfel
