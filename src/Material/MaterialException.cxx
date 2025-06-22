/*!
 * \file   src/Material/MaterialException.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   14 fév 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <iostream>
#include <utility>
#include "TFEL/Material/MaterialException.hxx"

namespace tfel {

  namespace material {

    MaterialException::~MaterialException() noexcept = default;

    MaterialException::MaterialException(const char* const msg) {
      std::cerr << "MaterialException::MaterialException: " << msg << std::endl;
    }

    MaterialException::MaterialException(const std::string& msg) {
      std::cerr << "MaterialException::MaterialException: " << msg << std::endl;
    }

    DivergenceException::DivergenceException(const char* const msg) {
      std::cerr << "DivergenceException::DivergenceException: " << msg
                << std::endl;
    }

    DivergenceException::DivergenceException(const std::string& msg) {
      std::cerr << "DivergenceException::DivergenceException: " << msg
                << std::endl;
    }

    const char* DivergenceException::what() const noexcept {
      return "DivergenceException";
    }  // end of DivergenceException::what

    DivergenceException::~DivergenceException() noexcept = default;

    OutOfBoundsException::OutOfBoundsException(std::string m)
        : msg(std::forward<std::string>(m)) {}

    const char* OutOfBoundsException::what() const noexcept {
      return this->msg.c_str();
    }  // end of DivergenceException::~DivergenceException

    OutOfBoundsException::~OutOfBoundsException() noexcept = default;

  }  // end of namespace material

}  // end of namespace tfel
