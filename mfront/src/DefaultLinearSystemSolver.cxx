/*!
 * \file   mfront/src/DefaultLinearSystemSolver.cxx
 * \brief
 * \author Thomas Helfer
 * \date   05/07/2026
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/AbstractNonLinearSystemSolver.hxx"
#include "MFront/DefaultLinearSystemSolver.hxx"

namespace mfront {

  void DefaultLinearSystemSolver::writeLinearSystemResolution(
      std::ostream& os,
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver& solver,
      const Hypothesis h,
      const LinearSystemVariables& s) const {
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << solver.getExternalAlgorithmClassName(bd, h) << "::solveLinearSystem("
       << s.matrix << ", " << s.rhs << ");\n";
  }  // end of writeLinearSystemResolution

  DefaultLinearSystemSolver::~DefaultLinearSystemSolver() = default;

}  // end of namespace mfront