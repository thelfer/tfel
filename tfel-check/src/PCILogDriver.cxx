/*!
 * \brief Implementation of PCLogger Drivers class.
 *
 * \file PCILogDriver.cpp
 *
 * A longer description.
 *
 * \author sb152252
 * \date 31 août 2009
 *
 * $Id$
 */

#include <iostream>
#include <stdexcept>
#include <fstream>

#include "TFELCheck/PCILogDriver.hxx"

namespace tfel_check {

  PCILogDriver::PCILogDriver() = default;
  PCILogDriver::PCILogDriver(PCILogDriver&&) = default;
  PCILogDriver::PCILogDriver(const PCILogDriver&) = default;

  PCILogDriver::PCILogDriver(const std::string& f)
  {
    this->log = std::make_shared<std::ofstream>(f);
    if (!this->log->good()){
      throw(std::runtime_error("PCILogDriver::PCILogDriver: "
			       "can't open file '"+f+"'"));
    }
  }

  std::ostream&
  PCILogDriver::getStream() {
    if(this->log!=nullptr){
      return *(this->log);
    }
    return std::cout;
  }

  void PCILogDriver::terminate()
  {}

  PCILogDriver::~PCILogDriver() = default;

}  // namespace tfel_check

