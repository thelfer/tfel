/*!
 * \file  mfront/src/BroydenSolvers.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/BroydenSolvers.hxx"

namespace mfront {

  bool BroydenSolverBase::requiresNumericalJacobian() const { return false; }

  bool BroydenSolverBase::allowsJacobianInitialisation() const { return true; }

  bool BroydenSolverBase::allowsJacobianInvertInitialisation() const {
    return false;
  }

  bool BroydenSolverBase::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return false;
  }

  void BroydenSolverBase::writeSpecificMembers(std::ostream&,
                                               const BehaviourDescription&,
                                               const Hypothesis) const {
  }  // end of writeSpecificMembers

  void BroydenSolverBase::writeSpecificInitializeMethodPart(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto n = d.getIntegrationVariables().getTypeSize();
    if (mb.hasCode(h, BehaviourData::InitializeJacobian)) {
      out << mb.getCode(h, BehaviourData::InitializeJacobian);
      return;
    }
    out << "// setting jacobian to identity\n"
        << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n"
        << "for(unsigned short idx = 0; idx!= " << n << ";++idx){\n"
        << "this->jacobian(idx,idx)= real(1);\n"
        << "}\n";
  }  // end of writeSpecificInitializeMethodPart

  BroydenSolverBase::~BroydenSolverBase() = default;

  std::vector<std::string> BroydenSolver::getSpecificHeaders() const {
    return {"TFEL/Math/TinyBroydenSolver.hxx"};
  }  // end of getSpecificHeaders

  std::string BroydenSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    return NonLinearSystemSolverBase::buildExternalAlgorithmClassName(
        bd, h, "TinyBroydenSolver");
  }  // end of getExternalAlgorithmClassName

  bool BroydenSolver::usesJacobian() const { return true; }

  bool BroydenSolver::usesJacobianInvert() const { return false; }

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  BroydenSolver::treatSpecificKeywords(BehaviourDescription&,
                                       const std::string&,
                                       const tokens_iterator p,
                                       const tokens_iterator) {
    return {false, p};
  }  // end of treatSpecificKeywords

  void BroydenSolver::completeVariableDeclaration(BehaviourDescription&) const {
  }  // end of completeVariableDeclaration

  BroydenSolver::~BroydenSolver() = default;

  std::vector<std::string> PowellDogLegBroydenSolver::getReservedNames() const {
    return PowellDogLegAlgorithmBase::getReservedNames();
  }  // end of getReservedNames

  std::vector<std::string> PowellDogLegBroydenSolver::getSpecificHeaders()
      const {
    return {"TFEL/Math/TinyPowellDogLegBroydenSolver.hxx"};
  }  // end of getSpecificHeaders

  std::string PowellDogLegBroydenSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    return NonLinearSystemSolverBase::buildExternalAlgorithmClassName(
        bd, h, "TinyPowellDogLegBroydenSolver");
  }  // end of getExternalAlgorithmClassName

  bool PowellDogLegBroydenSolver::usesJacobian() const { return true; }

  bool PowellDogLegBroydenSolver::usesJacobianInvert() const { return false; }

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  PowellDogLegBroydenSolver::treatSpecificKeywords(BehaviourDescription& mb,
                                                   const std::string& key,
                                                   const tokens_iterator p,
                                                   const tokens_iterator pe) {
    return PowellDogLegAlgorithmBase::treatSpecificKeywords(mb, key, p, pe);
  }  // end of treatSpecificKeywords

  void PowellDogLegBroydenSolver::completeVariableDeclaration(
      BehaviourDescription& mb) const {
    PowellDogLegAlgorithmBase::completeVariableDeclaration(mb);
  }  // end of completeVariableDeclaration

  void PowellDogLegBroydenSolver::initializeNumericalParameters(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    const auto sn = this->getExternalAlgorithmClassName(bd, h);
    BroydenSolverBase::initializeNumericalParameters(os, bd, h);
    PowellDogLegAlgorithmBase::initializeNumericalParameters(os, sn);
  }  // end of initializeNumericalParameters

  PowellDogLegBroydenSolver::~PowellDogLegBroydenSolver() = default;

}  // end of namespace mfront
