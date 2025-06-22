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

  std::vector<std::string> MFrontBroydenSolverBase::getReservedNames() const {
    auto n = std::vector<std::string>{};
    if (this->usesPowellDogLegAlgorithm()) {
      const auto& n2 = PowellDogLegAlgorithmBase::getReservedNames();
      n.insert(n.end(), n2.begin(), n2.end());
      n.insert(n.end(), {"tjacobian", "tfzeros", "integrate_one_half"});
    }
    n.insert(n.end(), {"jacobian2", "fzeros2", "Dzeros", "broyden_inv",
                       "integrate_one_half", "computeFdF_ok"});
    return n;
  }  // end of MFrontBroydenSolverBase::getReservedNames

  bool MFrontBroydenSolverBase::usesJacobian() const {
    return true;
  }  // end of MFrontBroydenSolverBase::usesJacobian

  bool MFrontBroydenSolverBase::usesJacobianInvert() const {
    return true;
  }  // end of MFrontBroydenSolverBase::usesJacobianInvert

  bool MFrontBroydenSolverBase::requiresNumericalJacobian() const {
    return false;
  }

  bool MFrontBroydenSolverBase::allowsJacobianInitialisation() const {
    return true;
  }

  bool MFrontBroydenSolverBase::allowsJacobianInvertInitialisation() const {
    return false;
  }

  bool MFrontBroydenSolverBase::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return false;
  }

  std::pair<bool,
            tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontBroydenSolverBase::treatSpecificKeywords(BehaviourDescription& mb,
                                                 const std::string& key,
                                                 const tokens_iterator p,
                                                 const tokens_iterator pe) {
    if (this->usesPowellDogLegAlgorithm()) {
      return PowellDogLegAlgorithmBase::treatSpecificKeywords(mb, key, p, pe);
    }
    return {false, p};
  }  // end of MFrontBroydenSolverBase::treatSpecificKeywords

  void MFrontBroydenSolverBase::completeVariableDeclaration(
      BehaviourDescription& mb) const {
    if (this->usesPowellDogLegAlgorithm()) {
      return PowellDogLegAlgorithmBase::completeVariableDeclaration(mb);
    }
  }  // end of MFrontBroydenSolverBase::completeVariableDeclaration

  void MFrontBroydenSolverBase::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of MFrontBroydenSolverBase::writeSpecificMembers

  void MFrontBroydenSolverBase::writeSpecificInitializeMethodPart(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto n = d.getIntegrationVariables().getTypeSize();
    if (mb.hasCode(h, BehaviourData::InitializeJacobian)) {
      out << mb.getCode(h, BehaviourData::InitializeJacobian);
    } else {
      out << "// setting jacobian to identity\n"
             "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));\n"
             "for(unsigned short idx = 0; idx!= "
          << n
          << ";++idx){\n"
             "this->jacobian(idx,idx)= real(1);\n"
             "}\n";
    }
  }  // end of MFrontBroydenSolverBase::writeSpecificInitializeMethodPart

  void MFrontBroydenSolverBase::writeResolutionAlgorithm(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    const auto n2 = d.getIntegrationVariables().getTypeSize();
    out << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n"
        << "tvector<" << n2 << ",real> fzeros2;\n"
        << "tvector<" << n2 << ",real> Dzeros;\n"
        << "auto broyden_inv = real{};\n"
        << "auto error = real{};\n"
        << "bool converged=false;\n"
        << "this->iter=0;\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : beginning of resolution\\n\";\n";
    }
    out << "while((converged==false)&&\n"
        << "(this->iter<" << mb.getClassName() << "::iterMax)){\n"
        << "++(this->iter);\n"
        << "fzeros2 = this->fzeros;\n";
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "const auto computeFdF_ok = this->computeFdF(false);\n"
        << "if(computeFdF_ok){\n"
        << "error=norm(this->fzeros)/(real(" << n2 << "));\n"
        << "}\n"
        << "if((!computeFdF_ok)||(!ieee754::isfinite(error))){\n"
        << "if(this->iter==1){\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : computFdF returned false on first iteration, "
             "abording...\" << endl;\n";
    }
    if (mb.useQt()) {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "} else {\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : computFdF returned false, dividing increment by "
             "two...\" << endl;\n";
    }
    out << "const real integrate_one_half = real(1)/real(2);\n"
        << "this->zeros -= (this->zeros-this->zeros_1)*integrate_one_half;\n"
        << "}\n"
        << "} else {\n"
        << "this->zeros_1  = this->zeros;\n"
        << "converged = error<this->epsilon;\n"
        << "this->additionalConvergenceChecks(converged,error);\n";
    if (d.hasCode(BehaviourData::AdditionalConvergenceChecks)) {
    }
    if (getDebugMode()) {
      out << "cout << \"" << mb.getClassName()
          << "::integrate() : iteration \" "
          << "<< this->iter << \" : \" << error << endl;\n";
    }
    out << "if(!converged){\n"
        << "Dzeros = this->fzeros;\n"
        << "jacobian2 = this->jacobian;\n"
        << "try{\n"
        << "TinyMatrixSolve<" << n2 << ","
        << "real>::exe(jacobian2,Dzeros);\n"
        << "}\n"
        << "catch(LUException&){\n";
    if (mb.useQt()) {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n"
        << "jacobian2 = this->jacobian;\n";
    if (this->usesPowellDogLegAlgorithm()) {
      this->writePowellDogLegStep(out, mb, h, "tjacobian", "tfzeros", "fzeros");
    } else {
      NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h,
                                                              "fzeros");
      out << "this->zeros -= Dzeros;\n";
    }
    out << "if(this->iter>1){\n"
        << "broyden_inv = (Dzeros|Dzeros);\n"
        << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n"
        << "#if (!defined __INTEL_COMPILER) and (!defined __PGI)\n"
        << "this->jacobian += "
        << "(((this->fzeros-fzeros2)-jacobian2*Dzeros)^Dzeros)/broyden_inv;\n"
        << "#else\n"
        << "const tvector<" << n2 << ",real> fzeros3 = jacobian2*Dzeros;\n"
        << "this->jacobian += "
        << "(((this->fzeros-fzeros2)-fzeros3)^Dzeros)/broyden_inv;\n"
        << "#endif  /* __INTEL_COMPILER OR PGI */\n"
        << "}\n"
        << "}\n";
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out, mb,
                                                                        h);
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(
            out, mb, h);
    out << "this->updateMaterialPropertiesDependantOnStateVariables();\n"
        << "}\n"
        << "}\n"
        << "}\n";
  }  // end of MFrontBroydenSolverBase::writeResolutionAlgorithm

  MFrontBroydenSolverBase::~MFrontBroydenSolverBase() = default;

  bool MFrontBroydenSolver::usesPowellDogLegAlgorithm() const { return false; }

  MFrontBroydenSolver::~MFrontBroydenSolver() = default;

  bool MFrontPowellDogLegBroydenSolver::usesPowellDogLegAlgorithm() const {
    return true;
  }

  MFrontPowellDogLegBroydenSolver::~MFrontPowellDogLegBroydenSolver() = default;

}  // end of namespace mfront
