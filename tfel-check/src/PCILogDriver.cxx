/*!
 * \brief Implementation of PCLogger Drivers class.
 *
 * \file PCILogDriver.cpp
 *
 * A longer description.
 *
 * \author sb152252
 * \date 31 août 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <stdexcept>
#include <fstream>

#include "TFELCheck/PCILogDriver.hxx"

namespace tfel_check {

  PCILogDriver::PCILogDriver() = default;
  PCILogDriver::PCILogDriver(PCILogDriver&&) = default;
  PCILogDriver::PCILogDriver(const PCILogDriver&) = default;

  PCILogDriver::PCILogDriver(const std::string& f) {
    this->log = std::make_shared<std::ofstream>(f);
    if (!this->log->good()) {
      throw(
          std::runtime_error("PCILogDriver::PCILogDriver: "
                             "can't open file '" +
                             f + "'"));
    }
  }

  std::ostream& PCILogDriver::getStream() {
    if (this->log != nullptr) {
      return *(this->log);
    }
    return std::cout;
  }

  void PCILogDriver::terminate() {}

  PCILogDriver::~PCILogDriver() = default;

}  // namespace tfel_check
