/*!
 * \file  mfront/src/LinearSystemSolverBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date 05/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
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

  bool LinearSystemSolverBase::usesSimultaneousSubstitutions() const {
    return this->simultaneous_substitutions;
  }  // end of usesSimultaneousSubstitutions

  void LinearSystemSolverBase::writeIdentityColumnsSubstitution(
      std::ostream& os,
      const MatrixDecompositionResult& r,
      const LinearSystemSubstitutionVariables& s) const {
    os << "std::fill(" << s.rhs << ".begin(), " << s.rhs
       << ".end(), NumericType(0));\n"
       << "for(ushort mfront_idx = 0; mfront_idx != " << s.rhs_column_size
       << "; ++mfront_idx){\n"
       << s.rhs << "(mfront_idx, mfront_idx) = NumericType(1);\n"
       << "}\n";
    this->writeLinearSystemSubstitution(os, r, s);
  }  // end of writeIdentityColumnsSubstitution

  LinearSystemSolverBase::~LinearSystemSolverBase() = default;

}  // end of namespace mfront
