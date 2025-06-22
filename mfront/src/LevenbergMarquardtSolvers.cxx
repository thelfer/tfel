/*!
 * \file  mfront/src/LevenbergMarquardtSolvers.cxx
 * \brief
 * \author Helfer Thomas
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

  std::vector<std::string> MFrontLevenbergMarquartSolverBase::getReservedNames()
      const {
    return {"levmar_jacobian_1", "levmar_fzeros_1", "levmar_error",
            "levmar_m",          "levmar_sm",       "levmar_mu",
            "levmar_mu0",        "levmar_muF",      "levmar_r",
            "levmar_p0",         "levmar_p1",       "levmar_p2"};
  }  // end of MFrontLevenbergMarquartSolverBase::getReservedNames

  void MFrontLevenbergMarquartSolverBase::endsInputFileProcessing(
      BehaviourDescription& mb) const {
    using namespace tfel::material;
    const ModellingHypothesis::Hypothesis h =
        ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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

  bool MFrontLevenbergMarquartSolverBase::usesJacobian() const {
    return true;
  }  // end of MFrontLevenbergMarquartSolverBase::usesJacobian

  bool MFrontLevenbergMarquartSolverBase::usesJacobianInvert() const {
    return false;
  }  // end of MFrontLevenbergMarquartSolverBase::usesJacobianInvert

  bool MFrontLevenbergMarquartSolverBase::allowsJacobianInitialisation() const {
    return false;
  }  // end of MFrontLevenbergMarquartSolverBase::allowsJacobianInitialisation

  bool MFrontLevenbergMarquartSolverBase::allowsJacobianInvertInitialisation()
      const {
    return false;
  }  // end of
     // MFrontLevenbergMarquartSolverBase::allowsJacobianInvertInitialisation

  bool MFrontLevenbergMarquartSolverBase::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return !this->requiresNumericalJacobian();
  }  // end of
     // MFrontLevenbergMarquartSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool, MFrontLevenbergMarquartSolverBase::tokens_iterator>
  MFrontLevenbergMarquartSolverBase::treatSpecificKeywords(
      BehaviourDescription&,
      const std::string&,
      const tokens_iterator p,
      const tokens_iterator) {
    return {false, p};
  }  // end of MFrontLevenbergMarquartSolverBase::treatSpecificKeywords

  void MFrontLevenbergMarquartSolverBase::writeSpecificInitializeMethodPart(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of
     // MFrontLevenbergMarquartSolverBase::writeSpecificInitializeMethodPart

  void MFrontLevenbergMarquartSolverBase::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of MFrontLevenbergMarquartSolverBase::writeSpecificMembers

  void MFrontLevenbergMarquartSolverBase::writeResolutionAlgorithm(
      std::ostream& out,
      const BehaviourDescription& mb,
      const Hypothesis h) const {
    using namespace std;
    const string btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n2;
    for (p = d.getIntegrationVariables().begin();
         p != d.getIntegrationVariables().end(); ++p) {
      n2 += mb.getTypeSize(p->type, p->arraySize);
    }
    out << "// dumping parameter\n";
    out << "real levmar_mu = this->levmar_mu0;\n";
    out << "real error;\n";
    out << "bool converged=false;\n";
    out << "this->iter=0;\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : beginning of resolution\"\n;\n";
    }
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "if(!this->computeFdF()){\n";
    if (getDebugMode()) {
      out << "cout << endl << \"" << mb.getClassName()
          << "::integrate() : computFdF returned false on first call, "
             "abording...\\n\";\n";
    }
    if (mb.useQt()) {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype
          << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n";
    if (this->requiresNumericalJacobian()) {
      out << "this->computeNumericalJacobian(this->jacobian);\n";
    }
    out << "error=norm(this->fzeros);\n";
    out << "while((converged==false)&&\n";
    out << "(this->iter<" << mb.getClassName() << "::iterMax)){\n";
    out << "++(this->iter);\n";
    out << "this->zeros_1  = this->zeros;\n";
    NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(out, mb, h,
                                                                  "jacobian");
    out << "converged = ((error)/(real(" << n2 << "))<";
    out << "(this->epsilon));\n";
    if (getDebugMode()) {
      out << "cout << \"" << mb.getClassName()
          << "::integrate() : iteration \" "
          << "<< this->iter << \" : \" << (error)/(real(" << n2
          << ")) << \", dumping parameter : \" << levmar_mu << endl;\n";
    }
    out << "if(!converged){\n";
    out << "// matrix containing tJJ+levmar_mu*I\n";
    out << "tmatrix<" << n2 << "," << n2 << ",real> levmar_tJJ;\n";
    out << "// vector containing tJ*F\n";
    out << "tvector<" << n2 << ",real> levmar_sm;\n";
    out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n"
        << "levmar_sm(idx)=real(0);"
        << "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){\n"
        << "levmar_sm(idx)+=(this->jacobian(idx2,idx))*(this->fzeros(idx2));"
        << "levmar_tJJ(idx,idx2)=real(0);\n"
        << "for(unsigned short idx3=0;idx3!=" << n2 << ";++idx3){\n"
        << "levmar_tJJ(idx,idx2)+=(this->jacobian(idx3,idx))*(this->jacobian("
           "idx3,idx2));\n"
        << "}\n"
        << "}\n"
        << "}\n"
        << "const real levmar_muF = (levmar_mu)*norm(this->fzeros);\n"
        << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n"
        << "levmar_tJJ(idx,idx)+=levmar_muF;\n"
        << "}\n";
    out << "try{\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingBegin(out, mb.getClassName(),
                                             "TinyMatrixSolve", "lu");
    }
    out << "TinyMatrixSolve<" << n2 << ","
        << "real>::exe(levmar_tJJ,levmar_sm);\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      writeStandardPerformanceProfilingEnd(out);
    }
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
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out, mb, h,
                                                            "levmar_sm");
    // levmar_sm contains the step
    out << "this->zeros -= levmar_sm;\n";
    out << "tvector<" << n2 << ",real> levmar_fzeros_1 = this->fzeros;\n";
    out << "tmatrix<" << n2 << "," << n2
        << ",real> levmar_jacobian_1 = this->jacobian;\n";
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "if(!this->computeFdF()){\n";
    out << "// rejecting the step\n";
    out << "this->zeros     = this->zeros_1;\n";
    out << "this->fzeros    = levmar_fzeros_1;\n";
    out << "this->jacobian  = levmar_jacobian_1;\n";
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "// updating mu\n";
    out << "levmar_mu *= 4;\n";
    out << "} else {\n";
    out << "const real error_1=error;\n";
    out << "#if (!defined __INTEL_COMPILER)\n";
    out << "const real "
           "error_p=norm(levmar_fzeros_1-levmar_jacobian_1*levmar_sm);\n";
    out << "#else\n";
    out << "tvector<" << n2
        << ",real> levmar_error = "
           "levmar_jacobian_1*levmar_sm\n;";
    out << "const real error_p=norm(levmar_fzeros_1-levmar_error);\n";
    out << "#endif  /* __INTEL_COMPILER */\n";
    out << "error=norm(this->fzeros);\n";
    out << "const real levmar_r = "
           "(error*error-error_1*error_1)/(error_p*error_p-error_1*error_1);\n";
    out << "if(levmar_r<this->levmar_p0){\n";
    out << "// rejecting the step\n";
    out << "this->zeros     = this->zeros_1;\n";
    out << "this->fzeros    = levmar_fzeros_1;\n";
    out << "this->jacobian  = levmar_jacobian_1;\n";
    out << "error = error_1;\n";
    if (mb.hasCode(h, BehaviourData::ComputeStress)) {
      out << "this->computeStress();\n";
    }
    out << "levmar_mu *= 4;\n";
    out << "} else {\n";
    out << "// accepting the step and updating mu\n";
    if (this->requiresNumericalJacobian()) {
      out << "this->computeNumericalJacobian(this->jacobian);\n";
    }
    out << "if(levmar_r<this->levmar_p1){\n";
    out << "levmar_mu *= 4;\n";
    out << "} else if(levmar_r>this->levmar_p2){\n";
    out << "levmar_mu  = max(levmar_mu/4,this->levmar_m);\n";
    out << "}\n";
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
  }  // end of MFrontLevenbergMarquartSolverBase::writeResolutionAlgorithm

  MFrontLevenbergMarquartSolverBase::~MFrontLevenbergMarquartSolverBase() =
      default;

  bool MFrontLevenbergMarquartSolver::requiresNumericalJacobian() const {
    return false;
  }

  MFrontLevenbergMarquartSolver::~MFrontLevenbergMarquartSolver() = default;

  bool
  MFrontLevenbergMarquartNumericalJacobianSolver::requiresNumericalJacobian()
      const {
    return true;
  }

  MFrontLevenbergMarquartNumericalJacobianSolver::
      ~MFrontLevenbergMarquartNumericalJacobianSolver() = default;

}  // end of namespace mfront
