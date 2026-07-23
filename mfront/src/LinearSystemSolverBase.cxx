/*!
 * \file  mfront/src/LinearSystemSolverBase.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 5/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/LinearSystemSolverBase.hxx"

namespace mfront {

  LinearSystemSolverBase::LinearSystemSolverBase() = default;

  std::vector<std::string> LinearSystemSolverBase::getSpecificHeaders() const {
    return {};
  }  // end of getSpecificHeaders

  std::vector<std::string> LinearSystemSolverBase::getReservedNames() const {
    return {};
  }  // end of getReservedNames

  void LinearSystemSolverBase::writeSpecificMembers(std::ostream&,
                                                    const BehaviourDescription&,
                                                    const Hypothesis) const {
  }  // end of writeSpecificMembers

  LinearSystemSolverBase::~LinearSystemSolverBase() = default;

}  // end of namespace mfront
