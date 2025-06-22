/*!
 * \file  mfront/src/SecondBroydenSolver.cxx
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
#include "MFront/SecondBroydenSolver.hxx"

namespace mfront {

  std::vector<std::string> SecondBroydenSolver::getReservedNames() const {
    return {"fzeros2",       "fzeros3",     "fzeros4",
            "Dzeros",        "Dfzeros",     "inv_jacobian",
            "inv_jacobian2", "broyden_inv", "integrate_one_half",
            "computeFdF_ok"};
  }  // end of SecondBroydenSolver::getReservedNames

  bool SecondBroydenSolver::usesJacobian() const {
    return false;
  }  // end of SecondBroydenSolver::usesJacobian

  bool SecondBroydenSolver::usesJacobianInvert() const {
    return true;
  }  // end of SecondBroydenSolver::usesJacobianInvert

  bool SecondBroydenSolver::allowsJacobianInitialisation() const {
    return false;
  }  // end of SecondBroydenSolver::allowsJacobianInitialisation

  bool SecondBroydenSolver::allowsJacobianInvertInitialisation() const {
    return true;
  }  // end of SecondBroydenSolver::allowsJacobianInvertInitialisation

  bool SecondBroydenSolver::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return false;
  }  // end of SecondBroydenSolver::allowsJacobianInitialisation

  bool SecondBroydenSolver::requiresNumericalJacobian() const { return false; }

  std::pair<bool, SecondBroydenSolver::tokens_iterator>
  SecondBroydenSolver::treatSpecificKeywords(BehaviourDescription&,
                                             const std::string&,
                                             const tokens_iterator p,
                                             const tokens_iterator) {
    return {false, p};
  }  // end of SecondBroydenSolver::treatSpecificKeywords

  void SecondBroydenSolver::completeVariableDeclaration(
      BehaviourDescription&) const {
  }  // end of SecondBroydenSolver::completeVariableDeclaration

  void SecondBroydenSolver::writeSpecificMembers(std::ostream&,
                                                 const BehaviourDescription&,
                                                 const Hypothesis) const {
  }  // end of SecondBroydenSolver::writeSpecificMembers

  void SecondBroydenSolver::writeSpecificInitializeMethodPart(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    const auto n = d.getIntegrationVariables().getTypeSize();
    if (mb.hasCode(h, BehaviourData::InitializeJacobianInvert)) {
      out << mb.getCode(h, BehaviourData::InitializeJacobianInvert);
    } else {
      out << "// setting the inverse of jacobian to identity\n"
          << "std::fill(this->inv_jacobian.begin(),this->inv_jacobian.end(),"
             "real(0));\n"
          << "for(unsigned short idx = 0; idx!= " << n << ";++idx){\n"
          << "this->inv_jacobian(idx,idx)= real(1);\n"
          << "}\n";
    }
  }  // end of SecondBroydenSolver::writeSpecificInitializeMethodPart

  void SecondBroydenSolver::writeResolutionAlgorithm(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    const auto n2 = d.getIntegrationVariables().getTypeSize();
    out << "tmatrix<" << n2 << "," << n2 << ",real> inv_jacobian2;\n"
        << "tvector<" << n2 << ",real> fzeros2;\n"
        << "tvector<" << n2 << ",real> Dzeros;\n"
        << "tvector<" << n2 << ",real> Dfzeros;\n"
        << "real broyden_inv;\n"
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
    if (mb.hasCode(h, BehaviourData::ComputeThermodynamicForces)) {
      out << "this->computeThermodynamicForces();\n";
    }
    out << "const auto computeFdF_ok = this->computeFdF(false);\n"
        << "if(computeFdF_ok){\n"
        << "error=norm(this->fzeros);\n"
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
        << "this->updateMaterialPropertiesDependantOnStateVariables();\n"
        << "}\n"
        << "} else {\n"
        << "this->zeros_1  = this->zeros;\n"
        << "error=norm(this->fzeros)/(real(" << n2 << "));\n"
        << "converged = error<this->epsilon;\n"
        << "this->additionalConvergenceChecks(converged, error, smt);\n";
    if (getDebugMode()) {
      out << "cout << \"" << mb.getClassName()
          << "::integrate() : iteration \" "
          << "<< this->iter << \" : \" << error << endl;\n";
    }
    out << "if(!converged){\n"
        << "Dzeros   = -(this->inv_jacobian)*(this->fzeros);\n";
    this->writeLimitsOnIncrementValues(out, mb, h, "Dzeros");
    out << "this->zeros  += Dzeros;\n"
        << "if(this->iter>1){\n"
        << "Dfzeros   = (this->fzeros)-fzeros2;\n"
        << "broyden_inv = Dzeros|((this->inv_jacobian)*Dfzeros);\n"
        << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n"
        << "inv_jacobian2 = this->inv_jacobian;\n"
        << "#if (!defined __INTEL_COMPILER) and (!defined __PGI)\n"
        << "this->inv_jacobian += "
        << "((Dzeros-inv_jacobian2*Dfzeros)^(Dzeros*inv_jacobian2))/"
           "(broyden_inv);\n"
        << "#else\n"
        << "const tvector<" << n2 << ",real> fzeros3 = inv_jacobian2*Dfzeros;\n"
        << "const tvector<" << n2 << ",real> fzeros4 = Dzeros*inv_jacobian2;\n"
        << "this->inv_jacobian += "
        << "((Dzeros-fzeros3)^(fzeros4))/(broyden_inv);\n"
        << "#endif  /* not __INTEL_COMPILER OR __PGI */\n"
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
  }  // end of SecondBroydenSolver::writeResolutionAlgorithm

  SecondBroydenSolver::~SecondBroydenSolver() = default;

}  // end of namespace mfront
