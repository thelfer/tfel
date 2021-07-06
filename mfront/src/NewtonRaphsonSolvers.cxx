/*!
 * \file  mfront/src/NewtonRaphsonSolvers.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/NewtonRaphsonSolvers.hxx"

namespace mfront {

  NewtonRaphsonSolverBase::NewtonRaphsonSolverBase() = default;

  bool NewtonRaphsonSolverBase::usesExternalAlgorithm() const {
    return true;
  }  // end of usesExternalAlgorithm

  std::vector<std::string> NewtonRaphsonSolverBase::getReservedNames()
      const {
    return {};
  }  // end of getReservedNames

  bool NewtonRaphsonSolverBase::usesJacobian() const {
    return true;
  }  // end of usesJacobian

  bool NewtonRaphsonSolverBase::usesJacobianInvert() const {
    return false;
  }  // end of usesJacobianInvert

  bool NewtonRaphsonSolverBase::allowsJacobianInitialisation() const {
    return false;
  }  // end of allowsJacobianInitialisation

  bool NewtonRaphsonSolverBase::allowsJacobianInvertInitialisation()
      const {
    return false;
  }  // end of allowsJacobianInvertInitialisation

  bool NewtonRaphsonSolverBase::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return !this->requiresNumericalJacobian();
  }  // end of requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool, NewtonRaphsonSolverBase::tokens_iterator>
  NewtonRaphsonSolverBase::treatSpecificKeywords(
      BehaviourDescription&,
      const std::string& key,
      const tokens_iterator p,
      const tokens_iterator pe) {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(
          c, "NewtonRaphsonSolverBase::treatSpecificKeywords: " + m);
    };
    if (key == "@JacobianUpdatePeriod") {
      throw_if(!this->requiresNumericalJacobian(),
               "jacobian update period is only defined "
               "for algorithms using a numerical jacobian");
      throw_if(this->jacobianUpdatePeriod != -1,
               "jacobian update period already defined");
      auto c = p;
      this->jacobianUpdatePeriod = CxxTokenizer::readInt(c, pe);
      throw_if(this->jacobianUpdatePeriod <= 0,
               "invalid value for jacobian update period "
               "(read '" +
                   p->value + "')");
      CxxTokenizer::readSpecifiedToken(
          "NewtonRaphsonSolverBase::treatSpecificKeywords", ";", c, pe);
      return {true, c};
    }
    return {false, p};
  }  // end of treatSpecificKeywords

  void NewtonRaphsonSolverBase::completeVariableDeclaration(
      BehaviourDescription&) const {}  // end of completeVariableDeclaration

  void NewtonRaphsonSolverBase::writeSpecificInitializeMethodPart(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeSpecificMembers

  void NewtonRaphsonSolverBase::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeSpecificMembers

  void NewtonRaphsonSolverBase::writeResolutionAlgorithm(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeResolutionAlgorithm

  NewtonRaphsonSolverBase::~NewtonRaphsonSolverBase() = default;

  std::vector<std::string> NewtonRaphsonSolver::getSpecificHeaders()
      const {
    return {"TFEL/Math/TinyNewtonRaphsonSolver.hxx"};
  }  // end of NewtonRaphsonSolverBase::getSpecificHeaders

  std::string NewtonRaphsonSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    const auto hn = [&h]() -> std::string {
      if (h == tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        return "hypothesis";
      }
      return "ModellingHypothesis::" +
             tfel::material::ModellingHypothesis::toUpperCaseString(h);
    }();
    const auto n =
        mfront::getTypeSize(bd.getBehaviourData(h).getIntegrationVariables())
            .getValue({"ModellingHypothesisToSpaceDimension<" + hn + ">::value",
                       "ModellingHypothesisToStensorSize<" + hn + ">::value",
                       "ModellingHypothesisToTensorSize<" + hn + ">::value"});
    const auto cn =
        bd.useQt() ? bd.getClassName() + "<" + hn + ", NumericType, true>"
                   : bd.getClassName() + "<" + hn + ", NumericType, false>";
    return "tfel::math::TinyNewtonRaphsonSolver<" +  //
           n + ", NumericType, " + cn + ">";
  }  // end of getExternalAlgorithmClassName

  bool NewtonRaphsonSolver::requiresNumericalJacobian() const {
    return false;
  }

  NewtonRaphsonSolver::~NewtonRaphsonSolver() = default;

  std::vector<std::string>
  NewtonRaphsonNumericalJacobianSolver::getSpecificHeaders() const {
    return {"TFEL/Math/TinyNewtonRaphsonSolver.hxx"};
  }  // end of NewtonRaphsonSolverBase::getSpecificHeaders

  std::string
  NewtonRaphsonNumericalJacobianSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    const auto hn = [&h]() -> std::string {
      if (h == tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        return "hypothesis";
      }
      return "ModellingHypothesis::" +
             tfel::material::ModellingHypothesis::toUpperCaseString(h);
    }();
    const auto n =
        mfront::getTypeSize(bd.getBehaviourData(h).getIntegrationVariables())
            .getValue({"ModellingHypothesisToSpaceDimension<" + hn + ">::value",
                       "ModellingHypothesisToStensorSize<" + hn + ">::value",
                       "ModellingHypothesisToTensorSize<" + hn + ">::value"});
    const auto cn =
        bd.useQt() ? bd.getClassName() + "<" + hn + ", NumericType, true>"
                   : bd.getClassName() + "<" + hn + ", NumericType, false>";
    return "tfel::math::TinyNewtonRaphsonSolver<" +  //
           n + ", NumericType, " + cn + ">";
  }  // end of getExternalAlgorithmClassName

  bool NewtonRaphsonNumericalJacobianSolver::requiresNumericalJacobian()
      const {
    return true;
  }

  NewtonRaphsonNumericalJacobianSolver::
      ~NewtonRaphsonNumericalJacobianSolver() = default;

  std::vector<std::string>
  PowellDogLegNewtonRaphsonSolver::getSpecificHeaders() const {
    return {"TFEL/Math/TinyPowellDogLegNewtonRaphsonSolver.hxx"};
  }  // end of NewtonRaphsonSolverBase::getSpecificHeaders

  std::string
  PowellDogLegNewtonRaphsonSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    const auto hn = [&h]() -> std::string {
      if (h == tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        return "hypothesis";
      }
      return "ModellingHypothesis::" +
             tfel::material::ModellingHypothesis::toUpperCaseString(h);
    }();
    const auto n =
        mfront::getTypeSize(bd.getBehaviourData(h).getIntegrationVariables())
            .getValue({"ModellingHypothesisToSpaceDimension<" + hn + ">::value",
                       "ModellingHypothesisToStensorSize<" + hn + ">::value",
                       "ModellingHypothesisToTensorSize<" + hn + ">::value"});
    const auto cn =
        bd.useQt() ? bd.getClassName() + "<" + hn + ", NumericType, true>"
                   : bd.getClassName() + "<" + hn + ", NumericType, false>";
    return "tfel::math::TinyPowellDogLegNewtonRaphsonSolver<" +  //
           n + ", NumericType, " + cn + ">";
  }  // end of getExternalAlgorithmClassName

  bool PowellDogLegNewtonRaphsonSolver::requiresNumericalJacobian()
      const {
    return false;
  }

  std::pair<bool, NewtonRaphsonSolverBase::tokens_iterator>
  PowellDogLegNewtonRaphsonSolver::treatSpecificKeywords(
      BehaviourDescription& mb,
      const std::string& key,
      const tokens_iterator p,
      const tokens_iterator pe) {
    const auto r =
        PowellDogLegAlgorithmBase::treatSpecificKeywords(mb, key, p, pe);
    if (r.first) {
      return r;
    }
    return NewtonRaphsonSolverBase::treatSpecificKeywords(mb, key, p, pe);
  }  // end of treatSpecificKeywords

  void PowellDogLegNewtonRaphsonSolver::initializeNumericalParameters(
      std::ostream& os,
      const BehaviourDescription& bd,
      const Hypothesis h) const {
    const auto sn = this->getExternalAlgorithmClassName(bd, h);
    NewtonRaphsonSolverBase::initializeNumericalParameters(os, bd, h);
    PowellDogLegAlgorithmBase::initializeNumericalParameters(os, sn);
  }  // end of initializeNumericalParameters

  void PowellDogLegNewtonRaphsonSolver::completeVariableDeclaration(
      BehaviourDescription& bd) const {
    PowellDogLegAlgorithmBase::completeVariableDeclaration(bd);
  }

  PowellDogLegNewtonRaphsonSolver::
      ~PowellDogLegNewtonRaphsonSolver() = default;

  std::vector<std::string>
  PowellDogLegNewtonRaphsonNumericalJacobianSolver::getSpecificHeaders()
      const {
    return {"TFEL/Math/TinyPowellDogLegNewtonRaphsonSolver.hxx"};
  }  // end of NewtonRaphsonSolverBase::getSpecificHeaders

  std::string PowellDogLegNewtonRaphsonNumericalJacobianSolver::
      getExternalAlgorithmClassName(const BehaviourDescription& bd,
                                    const Hypothesis h) const {
    const auto hn = [&h]() -> std::string {
      if (h == tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS) {
        return "hypothesis";
      }
      return "ModellingHypothesis::" +
             tfel::material::ModellingHypothesis::toUpperCaseString(h);
    }();
    const auto n =
        mfront::getTypeSize(bd.getBehaviourData(h).getIntegrationVariables())
            .getValue({"ModellingHypothesisToSpaceDimension<" + hn + ">::value",
                       "ModellingHypothesisToStensorSize<" + hn + ">::value",
                       "ModellingHypothesisToTensorSize<" + hn + ">::value"});
    const auto cn =
        bd.useQt() ? bd.getClassName() + "<" + hn + ", NumericType, true>"
                   : bd.getClassName() + "<" + hn + ", NumericType, false>";
    return "tfel::math::TinyPowellDogLegNewtonRaphsonSolver<" +  //
           n + ", NumericType, " + cn + ">";
  }  // end of getExternalAlgorithmClassName

  void PowellDogLegNewtonRaphsonNumericalJacobianSolver::
      initializeNumericalParameters(std::ostream& os,
                                    const BehaviourDescription& bd,
                                    const Hypothesis h) const {
    const auto sn = this->getExternalAlgorithmClassName(bd, h);
    NewtonRaphsonSolverBase::initializeNumericalParameters(os, bd, h);
    PowellDogLegAlgorithmBase::initializeNumericalParameters(os, sn);
  }  // end of initializeNumericalParameters

  bool PowellDogLegNewtonRaphsonNumericalJacobianSolver::
      requiresNumericalJacobian() const {
    return true;
  }

  void PowellDogLegNewtonRaphsonNumericalJacobianSolver::
      completeVariableDeclaration(BehaviourDescription& bd) const {
    PowellDogLegAlgorithmBase::completeVariableDeclaration(bd);
  }

  std::pair<bool, NewtonRaphsonSolverBase::tokens_iterator>
  PowellDogLegNewtonRaphsonNumericalJacobianSolver::treatSpecificKeywords(
      BehaviourDescription& mb,
      const std::string& key,
      const tokens_iterator p,
      const tokens_iterator pe) {
    const auto r =
        PowellDogLegAlgorithmBase::treatSpecificKeywords(mb, key, p, pe);
    if (r.first) {
      return r;
    }
    return NewtonRaphsonSolverBase::treatSpecificKeywords(mb, key, p, pe);
  }  // end of treatSpecificKeywords

  PowellDogLegNewtonRaphsonNumericalJacobianSolver::
      ~PowellDogLegNewtonRaphsonNumericalJacobianSolver() = default;

}  // end of namespace mfront
