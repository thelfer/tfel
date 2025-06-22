/*!
 * \file  mfront/src/BroydenSolvers.cxx
 * \brief
 * \author Helfer Thomas
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

  void MFrontBroydenSolverBase::endsInputFileProcessing(
      BehaviourDescription& mb) const {
    if (this->usesPowellDogLegAlgorithm()) {
      return PowellDogLegAlgorithmBase::endsInputFileProcessing(mb);
    }
  }  // end of MFrontBroydenSolverBase::endsInputFileProcessing

  void MFrontBroydenSolverBase::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of MFrontBroydenSolverBase::writeSpecificMembers

  void MFrontBroydenSolverBase::writeSpecificInitializeMethodPart(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n;
    // size of linear system
    for (const auto& v : d.getIntegrationVariables()) {
      n += mb.getTypeSize(v.type, v.arraySize);
    }
    if (mb.hasCode(h, BehaviourData::InitializeJacobian)) {
      out << mb.getCode(h, BehaviourData::InitializeJacobian);
    } else {
      out << "// setting jacobian to identity\n";
      out << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));"
             "\n";
      out << "for(unsigned short idx = 0; idx!= " << n << ";++idx){\n";
      out << "this->jacobian(idx,idx)= real(1);\n";
      out << "}\n";
    }
  }  // end of MFrontBroydenSolverBase::writeSpecificInitializeMethodPart

  void MFrontBroydenSolverBase::writeResolutionAlgorithm(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    const auto btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n2;
    for (const auto& v : d.getIntegrationVariables()) {
      n2 += mb.getTypeSize(v.type, v.arraySize);
    }
    out << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n";
    out << "tvector<" << n2 << ",real> fzeros2;\n";
    out << "tvector<" << n2 << ",real> Dzeros;\n";
    out << "real broyden_inv;\n";
    out << "real error;\n";
    out << "bool converged=false;\n";
    out << "this->iter=0;\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : beginning of resolution\" << endl;\n";
    }
    out << "while((converged==false)&&\n";
    out << "(this->iter<" << mb.getClassName() << "::iterMax)){\n";
    out << "++(this->iter);\n";
    out << "fzeros2 = this->fzeros;\n";
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "const bool computeFdF_ok = this->computeFdF();\n";
    out << "if(!computeFdF_ok){\n";
    out << "if(this->iter==1){\n";
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
    out << "const real integrate_one_half = real(1)/real(2);\n";
    out << "this->zeros -= (this->zeros-this->zeros_1)*integrate_one_half;\n";
    out << "}\n";
    out << "} else {\n";
    out << "this->zeros_1  = this->zeros;\n";
    out << "error=norm(this->fzeros);\n";
    out << "converged = ((error)/(real(" << n2 << "))<";
    out << "(this->epsilon));\n";
    if (getDebugMode()) {
      out << "cout << \"" << mb.getClassName()
          << "::integrate() : iteration \" "
          << "<< this->iter << \" : \" << (error)/(real(" << n2
          << ")) << endl;\n";
    }
    out << "if(!converged){\n";
    out << "Dzeros = this->fzeros;\n";
    out << "jacobian2 = this->jacobian;\n";
    out << "try{\n";
    out << "TinyMatrixSolve<" << n2 << ","
        << "real>::exe(jacobian2,Dzeros);\n";
    out << "}\n";
    out << "catch(LUException&){\n";
    if (mb.useQt()) {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n";
    out << "jacobian2 = this->jacobian;\n";
    if (this->usesPowellDogLegAlgorithm()) {
      this->writePowellDogLegStep(out, mb, h, "tjacobian", "tfzeros", "fzeros");
    } else {
      NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h,
                                                              "fzeros");
      out << "this->zeros -= Dzeros;\n";
    }
    out << "if(this->iter>1){\n";
    out << "broyden_inv = (Dzeros|Dzeros);\n";
    out << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n";
    out << "#if (!defined __INTEL_COMPILER)\n";
    out << "this->jacobian += "
        << "(((this->fzeros-fzeros2)-jacobian2*Dzeros)^Dzeros)/broyden_inv;\n";
    out << "#else\n";
    out << "const tvector<" << n2 << ",real> fzeros3 = jacobian2*Dzeros;\n";
    out << "this->jacobian += "
        << "(((this->fzeros-fzeros2)-fzeros3)^Dzeros)/broyden_inv;\n";
    out << "#endif  /* __INTEL_COMPILER */\n";
    out << "}\n";
    out << "}\n";
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out, mb,
                                                                        h);
    NonLinearSystemSolverBase::
        writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(
            out, mb, h);
    out << "}\n";
    out << "}\n";
    out << "}\n";
  }  // end of MFrontBroydenSolverBase::writeResolutionAlgorithm

  MFrontBroydenSolverBase::~MFrontBroydenSolverBase() = default;

  bool MFrontBroydenSolver::usesPowellDogLegAlgorithm() const { return false; }

  MFrontBroydenSolver::~MFrontBroydenSolver() = default;

  bool MFrontPowellDogLegBroydenSolver::usesPowellDogLegAlgorithm() const {
    return true;
  }

  MFrontPowellDogLegBroydenSolver::~MFrontPowellDogLegBroydenSolver() = default;

}  // end of namespace mfront
