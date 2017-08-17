/*!
 * \file tfel-check/src/PCILogDriver.cxx
 * \brief Implementation of PCLogger Drivers class.
 * \author sb152252
 * \date 31 août 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include <iostream>
#include <stdexcept>
#include <fstream>

#include "TFEL/Check/PCILogDriver.hxx"

namespace tfel{

  namespace check{

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
  
  } // end of namespace check

} // end of namespace tfel
