/*!
 * \file  mfront/src/DrivingVariable.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 11 mai 2013
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/DrivingVariable.hxx"

namespace mfront {

  DrivingVariable::DrivingVariable(const std::string& t, const std::string& n)
      : VariableDescription(t, n, 1u, 0u) {
  }  // end of DrivingVariable::DrivingVariable

  DrivingVariable::DrivingVariable(DrivingVariable&&) = default;
  DrivingVariable::DrivingVariable(const DrivingVariable&) = default;
  DrivingVariable& DrivingVariable::operator=(DrivingVariable&&) = default;
  DrivingVariable& DrivingVariable::operator=(const DrivingVariable&) = default;

  DrivingVariable::~DrivingVariable() noexcept = default;

}  // end of namespace mfront
