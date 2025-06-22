/*!
 * \file  mfront/src/LevenbergMarquardtSolvers.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 24 août 2014
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
#include "MFront/LevenbergMarquardtSolvers.hxx"

namespace mfront {

  std::vector<std::string> LevenbergMarquardtSolverBase::getSpecificHeaders()
      const {
    return {"TFEL/Math/TinyLevenbergMarquardtSolver.hxx"};
  }  // end of getSpecificHeaders

  std::string LevenbergMarquardtSolverBase::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    return NonLinearSystemSolverBase::buildExternalAlgorithmClassName(
        bd, h, "TinyLevenbergMarquardtSolver");
  }  // end of getExternalAlgorithmClassName

  std::vector<std::string> LevenbergMarquardtSolverBase::getReservedNames()
      const {
    return {"levmar_jacobian_1", "levmar_fzeros_1", "levmar_error",
            "levmar_error2",     "levmar_error_1",  "levmar_m",
            "levmar_sm",         "levmar_mu",       "levmar_mu0",
            "levmar_muF",        "levmar_r",        "levmar_p0",
            "levmar_p1",         "levmar_p2"};
  }  // end of LevenbergMarquardtSolverBase::getReservedNames

  void LevenbergMarquardtSolverBase::completeVariableDeclaration(
      BehaviourDescription& mb) const {
    using namespace tfel::material;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (!mb.hasParameter(h, "levmar_mu0")) {
      mb.addParameter(h, VariableDescription("real", "levmar_mu0", 1u, 0u),
                      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h, "levmar_mu0", 1.e-6);
    }
    if (!mb.hasParameter(h, "levmar_p0")) {
      mb.addParameter(h, VariableDescription("real", "levmar_p0", 1u, 0u),
                      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h, "levmar_p0", 1.e-4);
    }
    if (!mb.hasParameter(h, "levmar_p1")) {
      mb.addParameter(h, VariableDescription("real", "levmar_p1", 1u, 0u),
                      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h, "levmar_p1", 0.25);
    }
    if (!mb.hasParameter(h, "levmar_p2")) {
      mb.addParameter(h, VariableDescription("real", "levmar_p2", 1u, 0u),
                      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h, "levmar_p2", 0.75);
    }
    if (!mb.hasParameter(h, "levmar_m")) {
      mb.addParameter(h, VariableDescription("real", "levmar_m", 1u, 0u),
                      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h, "levmar_m", 1.e-8);
    }
  }

  void LevenbergMarquardtSolverBase::initializeNumericalParameters(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    NonLinearSystemSolverBase::initializeNumericalParameters(os, bd, h);
    const auto sn = this->getExternalAlgorithmClassName(bd, h);
    for (const auto p : {"levmar_mu0", "levmar_p0", "levmar_p1",  //
                         "levmar_p2", "levmar_m"}) {
      os << sn << "::" << p << " = this->" << p << ";\n";
    }
  }  // end of initializeNumericalParameters

  bool LevenbergMarquardtSolverBase::usesJacobian() const {
    return true;
  }  // end of LevenbergMarquardtSolverBase::usesJacobian

  bool LevenbergMarquardtSolverBase::usesJacobianInvert() const {
    return false;
  }  // end of LevenbergMarquardtSolverBase::usesJacobianInvert

  bool LevenbergMarquardtSolverBase::allowsJacobianInitialisation() const {
    return false;
  }  // end of LevenbergMarquardtSolverBase::allowsJacobianInitialisation

  bool LevenbergMarquardtSolverBase::allowsJacobianInvertInitialisation()
      const {
    return false;
  }  // end of LevenbergMarquardtSolverBase::allowsJacobianInvertInitialisation

  bool LevenbergMarquardtSolverBase::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return !this->requiresNumericalJacobian();
  }  // end of
     // LevenbergMarquardtSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool, LevenbergMarquardtSolverBase::tokens_iterator>
  LevenbergMarquardtSolverBase::treatSpecificKeywords(BehaviourDescription&,
                                                      const std::string&,
                                                      const tokens_iterator p,
                                                      const tokens_iterator) {
    return {false, p};
  }  // end of LevenbergMarquardtSolverBase::treatSpecificKeywords

  void LevenbergMarquardtSolverBase::writeSpecificInitializeMethodPart(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of LevenbergMarquardtSolverBase::writeSpecificInitializeMethodPart

  void LevenbergMarquardtSolverBase::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of LevenbergMarquardtSolverBase::writeSpecificMembers

  LevenbergMarquardtSolverBase::~LevenbergMarquardtSolverBase() = default;

  bool LevenbergMarquardtSolver::requiresNumericalJacobian() const {
    return false;
  }

  LevenbergMarquardtSolver::~LevenbergMarquardtSolver() = default;

  bool LevenbergMarquardtNumericalJacobianSolver::requiresNumericalJacobian()
      const {
    return true;
  }

  LevenbergMarquardtNumericalJacobianSolver::
      ~LevenbergMarquardtNumericalJacobianSolver() = default;

}  // end of namespace mfront
