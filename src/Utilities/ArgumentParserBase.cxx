/*!
 * \file  ArgumentParserBase.cxx
 * \brief
 * \author Helfer Thomas
 * \date   04 mars 2015
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Utilities/ArgumentParserBase.hxx"

namespace tfel {

  namespace utilities {

    Argument::Argument(std::string s) : name(std::forward<std::string>(s)) {}

    Argument::Argument(const char* const s) : name(s) {}

    Argument::Argument(Argument&&) = default;
    Argument::Argument(const Argument&) = default;
    Argument& Argument::operator=(Argument&&) = default;
    Argument& Argument::operator=(const Argument&) = default;

    std::string& Argument::as_string() noexcept { return this->name; }

    const std::string& Argument::as_string() const noexcept {
      return this->name;
    }

    Argument::operator const std::string&() const noexcept {
      return this->as_string();
    }

    bool Argument::hasOption(void) const noexcept { return this->isOptionSet; }

    void Argument::setOption(const std::string& o) {
      this->isOptionSet = true;
      this->option = o;
    }

    const std::string& Argument::getOption() const noexcept { return option; }

    Argument::~Argument() noexcept {}

  }  // end of namespace utilities

}  // end of namespace tfel
