/*!
 * \file   mfront/src/VariableDescriptionBase.cxx
 *
 * \brief
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/VariableDescriptionBase.hxx"

namespace mfront {

  VariableDescriptionBase::VariableDescriptionBase() = default;

  VariableDescriptionBase::VariableDescriptionBase(const std::string& t,
                                                   const std::string& n,
                                                   const unsigned short as,
                                                   const size_t l)
      : type(t), name(n), arraySize(as), lineNumber(l) {
    tfel::raise_if(this->arraySize == 0,
                   "VariableDescriptionBase::VariableDescriptionBase: "
                   "invalid array size");
  }  // end of VariableDescriptionBase::VariableDescriptionBase

  VariableDescriptionBase::VariableDescriptionBase(const std::string& t,
                                                   const std::string& s,
                                                   const std::string& n,
                                                   const unsigned short as,
                                                   const size_t l)
      : type(t), symbolic_form(s), name(n), arraySize(as), lineNumber(l) {
    tfel::raise_if(this->arraySize == 0,
                   "VariableDescriptionBase::VariableDescriptionBase: "
                   "invalid array size");
  }  // end of VariableDescriptionBase::VariableDescriptionBase

  VariableDescriptionBase::VariableDescriptionBase(
      const VariableDescriptionBase&) = default;
  VariableDescriptionBase::VariableDescriptionBase(VariableDescriptionBase&&) =
      default;
  VariableDescriptionBase& VariableDescriptionBase::operator=(
      VariableDescriptionBase&&) = default;
  VariableDescriptionBase& VariableDescriptionBase::operator=(
      const VariableDescriptionBase&) = default;

  VariableDescriptionBase::~VariableDescriptionBase() = default;

  const std::string& displayName(const VariableDescriptionBase& v) {
    if ((getUnicodeOutputOption()) && (!v.symbolic_form.empty())) {
      return v.symbolic_form;
    }
    return v.name;
  }  // end of displayName

}  // end of namespace mfront
