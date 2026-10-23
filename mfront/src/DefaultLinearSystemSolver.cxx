/*!
 * \file   mfront/src/DefaultLinearSystemSolver.cxx
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
#include "MFront/BehaviourData.hxx"
#include "MFront/BehaviourDescription.hxx"
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

  AbstractLinearSystemSolver::MatrixDecompositionResult
  DefaultLinearSystemSolver::getMatrixDecompositionResults(
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver&,
      const Hypothesis h,
      const std::string& n) const {
    const auto& d = bd.getBehaviourData(h);
    const auto& isvs = d.getIntegrationVariables();
    const auto nivs = mfront::getTypeSize(isvs);
    auto perturbation_type =
        "::tfel::math::TinyPermutation<" + nivs.asString() + ">";
    return {.matrix = n,
            .matrix_size = nivs.asString(),
            .variables = {{.type = perturbation_type,
                           .name = "mfront_jacobian_permutation"}}};
  }  // end of getMatrixDecompositionResults

  AbstractLinearSystemSolver::MatrixDecompositionResult
  DefaultLinearSystemSolver::writeMatrixDecomposition(
      std::ostream& os,
      const BehaviourDescription& bd,
      const AbstractNonLinearSystemSolver& solver,
      const Hypothesis h,
      const MatrixDecompositionVariables& s) const {
    auto results = this->getMatrixDecompositionResults(bd, solver, h, s.matrix);
    if (results.variables.size() != 1) {
      tfel::raise(
          "DefaultLinearSystemSolver::writeMatrixDecomposition: "
          "invalid number of variables resulting from the decomposition");
    }
    for (const auto& v : results.variables) {
      os << "auto " << v.name << " = " << v.type << "{};\n";
    }
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "::tfel::math::TinyMatrixSolve<" << results.matrix_size
       << ", NumericType, false>"
       << "::decomp(" << s.matrix << ", " << results.variables.begin()->name
       << ");\n";
    return results;
  }  // end of writeLinearSystemResolution

  void DefaultLinearSystemSolver::writeLinearSystemSubstitution(
      std::ostream& os,
      const MatrixDecompositionResult& r,
      const LinearSystemSubstitutionVariables& s) const {
    if (r.variables.size() != 1) {
      tfel::raise(
          "invalide number of variables resulting from the matrix "
          "decomposition");
    }
    if (s.returned_value.has_value()) {
      os << *(s.returned_value) << " = ";
    }
    os << "::tfel::math::TinyMatrixSolve<" << r.matrix_size
       << ", NumericType, false>::back_substitute(" << r.matrix << ", "
       << r.variables.begin()->name << ", " << s.rhs << ");\n";
  }  // end of writeLinearSystemSubstitution

  DefaultLinearSystemSolver::~DefaultLinearSystemSolver() = default;

}  // end of namespace mfront