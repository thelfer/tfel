/*! 
 * \file  mfront/src/LevenbergMarquardtSolvers.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 août 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include "MFront/MFrontDebugMode.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/PerformanceProfiling.hxx"
#include "MFront/LevenbergMarquardtSolvers.hxx"

namespace mfront{

  std::vector<std::string>
  LevenbergMarquartSolverBase::getReservedNames() const
  {
    return {"levmar_jacobian_1","levmar_fzeros_1",
	"levmar_error","levmar_error2","levmar_error_1",
	"levmar_m","levmar_sm",
	"levmar_mu","levmar_mu0","levmar_muF",
	"levmar_r","levmar_p0","levmar_p1","levmar_p2"};
  } // end of LevenbergMarquartSolverBase::getReservedNames

  void
  LevenbergMarquartSolverBase::completeVariableDeclaration(BehaviourDescription& mb) const
  {
    using namespace tfel::material;
    const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!mb.hasParameter(h,"levmar_mu0")){
      mb.addParameter(h,VariableDescription("real","levmar_mu0",1u,0u),
		      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h,"levmar_mu0",1.e-6);
    }
    if(!mb.hasParameter(h,"levmar_p0")){
      mb.addParameter(h,VariableDescription("real","levmar_p0",1u,0u),
		      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h,"levmar_p0",1.e-4);
    }
    if(!mb.hasParameter(h,"levmar_p1")){
      mb.addParameter(h,VariableDescription("real","levmar_p1",1u,0u),
		      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h,"levmar_p1",0.25);
    }
    if(!mb.hasParameter(h,"levmar_p2")){
      mb.addParameter(h,VariableDescription("real","levmar_p2",1u,0u),
		      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h,"levmar_p2",0.75);
    }
    if(!mb.hasParameter(h,"levmar_m")){
      mb.addParameter(h,VariableDescription("real","levmar_m",1u,0u),
		      BehaviourData::ALREADYREGISTRED);
      mb.setParameterDefaultValue(h,"levmar_m",1.e-8);
    }
  }

  bool
  LevenbergMarquartSolverBase::usesJacobian() const
  {
    return true;
  } // end of LevenbergMarquartSolverBase::usesJacobian

  bool
  LevenbergMarquartSolverBase::usesJacobianInvert() const
  {
    return false;
  } // end of LevenbergMarquartSolverBase::usesJacobianInvert

  bool
  LevenbergMarquartSolverBase::allowsJacobianInitialisation() const
  {
    return false;
  } // end of LevenbergMarquartSolverBase::allowsJacobianInitialisation

  bool
  LevenbergMarquartSolverBase::allowsJacobianInvertInitialisation() const
  {
    return false;
  } // end of LevenbergMarquartSolverBase::allowsJacobianInvertInitialisation

  bool
  LevenbergMarquartSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const
  {
    return !this->requiresNumericalJacobian();
  } // end of LevenbergMarquartSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool,LevenbergMarquartSolverBase::tokens_iterator>
  LevenbergMarquartSolverBase::treatSpecificKeywords(BehaviourDescription&,
						     const std::string&,
						     const tokens_iterator p,
						     const tokens_iterator)
  {
    return {false,p};
  } // end of LevenbergMarquartSolverBase::treatSpecificKeywords

  void
  LevenbergMarquartSolverBase::writeSpecificInitializeMethodPart(std::ostream&,
								 const BehaviourDescription&,
								 const Hypothesis) const
  {} // end of LevenbergMarquartSolverBase::writeSpecificInitializeMethodPart

  void
  LevenbergMarquartSolverBase::writeSpecificMembers(std::ostream&,
						    const BehaviourDescription&,
						    const Hypothesis) const
  {} // end of LevenbergMarquartSolverBase::writeSpecificMembers

  void
  LevenbergMarquartSolverBase::writeResolutionAlgorithm(std::ostream& out,
							const BehaviourDescription& mb,
							const Hypothesis h) const
  {
    const auto btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    const auto n2 = d.getIntegrationVariables().getTypeSize();
    out << "// dumping parameter\n";
    out << "real levmar_mu = this->levmar_mu0;\n";
    out << "auto error    = real{};\n";
    out << "auto levmar_error = real{};\n";
    out << "bool converged=false;\n";
    out << "this->iter=0;\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : beginning of resolution\\n\";\n";
    }
    if(mb.hasCode(h,BehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "if(!this->computeFdF()){\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : computFdF returned false on first call, abording...\\n\";\n";
    }
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n";
    if(this->requiresNumericalJacobian()){
      out << "this->computeNumericalJacobian(this->jacobian);\n";
    }
    out << "levmar_error=norm(this->fzeros);\n"
	<< "error=levmar_error/(real(" << n2 << "));\n"
	<< "while((converged==false)&&\n"
	<< "(this->iter<" << mb.getClassName() << "::iterMax)){\n"
	<< "++(this->iter);\n"
	<< "this->zeros_1  = this->zeros;\n";
    NonLinearSystemSolverBase::writeEvaluateNumericallyComputedBlocks(out,mb,h);
    NonLinearSystemSolverBase::writeComparisonToNumericalJacobian(out,mb,h,"jacobian");
    out << "converged = error<this->epsilon;\n"
	<< "this->additionalConvergenceChecks(converged,error);\n";
    if(getDebugMode()){
      out << "cout << \"" << mb.getClassName()
	  << "::integrate() : iteration \" "
	  << "<< this->iter << \" : \" << error << \", dumping parameter : \" << levmar_mu << endl;\n";
    }
    out << "if(!converged){\n"
	<< "// matrix containing tJJ+levmar_mu*I\n"
	<< "tmatrix<" << n2 << "," << n2 << ",real> levmar_tJJ;\n"
	<< "// vector containing tJ*F\n"
	<< "tvector<" << n2 << ",real> levmar_sm;\n"
	<< "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n"
	<< "levmar_sm(idx)=real(0);"
	<< "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){\n"
	<< "levmar_sm(idx)+=(this->jacobian(idx2,idx))*(this->fzeros(idx2));"
	<< "levmar_tJJ(idx,idx2)=real(0);\n"
	<< "for(unsigned short idx3=0;idx3!=" << n2 << ";++idx3){\n"
	<< "levmar_tJJ(idx,idx2)+=(this->jacobian(idx3,idx))*(this->jacobian(idx3,idx2));\n"
	<< "}\n"
	<< "}\n"
	<< "}\n"
	<< "const real levmar_muF = (levmar_mu)*norm(this->fzeros);\n"
	<< "for(unsigned short idx=0;idx!=" << n2 << ";++idx){\n"
	<< "levmar_tJJ(idx,idx)+=levmar_muF;\n"
	<< "}\n"
	<< "try{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingBegin(out,mb.getClassName(),
					     "TinyMatrixSolve","lu");
    }
    out << "TinyMatrixSolve<" << n2
	<< "," << "real>::exe(levmar_tJJ,levmar_sm);\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingEnd(out);
    }
    out << "}\n"
	<< "catch(LUException&){\n";
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n";
    NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,"levmar_sm");
    // levmar_sm contains the step
    out << "this->zeros -= levmar_sm;\n"; 
    out << "tvector<" << n2 <<  ",real> levmar_fzeros_1 = this->fzeros;\n"; 
    out << "tmatrix<" << n2 << "," << n2 <<  ",real> levmar_jacobian_1 = this->jacobian;\n"; 
    if(mb.hasCode(h,BehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "if(!this->computeFdF()){\n"
	<< "// rejecting the step\n"
	<< "this->zeros     = this->zeros_1;\n"; 
    out << "this->fzeros    = levmar_fzeros_1;\n"; 
    out << "this->jacobian  = levmar_jacobian_1;\n"; 
    if(mb.hasCode(h,BehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "// updating mu\n"
	<< "levmar_mu *= 4;\n"
	<< "} else {\n"
	<< "const real levmar_error_1=levmar_error;\n"
	<< "#if (!defined __INTEL_COMPILER)\n"
	<< "const real error_p=norm(levmar_fzeros_1-levmar_jacobian_1*levmar_sm);\n"
	<< "#else\n"
	<< "tvector<" << n2 <<  ",real> levmar_error2 = levmar_jacobian_1*levmar_sm\n;"
	<< "const real error_p=norm(levmar_fzeros_1-levmar_error2);\n"
	<< "#endif  /* __INTEL_COMPILER */\n"
	<< "levmar_error=norm(this->fzeros);\n"
	<< "const real levmar_r = (levmar_error*levmar_error-levmar_error_1*levmar_error_1)/(error_p*error_p-levmar_error_1*levmar_error_1);\n"
	<< "if(levmar_r<this->levmar_p0){\n"
	<< "// rejecting the step\n"
	<< "this->zeros     = this->zeros_1;\n"
	<< "this->fzeros    = levmar_fzeros_1;\n"
	<< "this->jacobian  = levmar_jacobian_1;\n"
	<< "levmar_error = levmar_error_1;\n";
    if(mb.hasCode(h,BehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "levmar_mu *= 4;\n"
	<< "} else {\n"
	<< "// accepting the step and updating mu\n"
	<< "error=levmar_error/(real(" << n2 << "));\n";
    if(this->requiresNumericalJacobian()){
      out << "this->computeNumericalJacobian(this->jacobian);\n";
    }
    out << "if(levmar_r<this->levmar_p1){\n"
	<< "levmar_mu *= 4;\n"
	<< "} else if(levmar_r>this->levmar_p2){\n"
	<< "levmar_mu  = max(levmar_mu/4,this->levmar_m);\n"
	<< "}\n"
	<< "}\n"
	<< "}\n";
    NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out,mb,h);
    NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(out,mb,h);
    out << "}\n"
	<< "}\n";
  } // end of LevenbergMarquartSolverBase::writeResolutionAlgorithm

  LevenbergMarquartSolverBase::~LevenbergMarquartSolverBase() = default;

  bool LevenbergMarquartSolver::requiresNumericalJacobian() const
  {
    return false;
  }

  LevenbergMarquartSolver::~LevenbergMarquartSolver() = default;

  bool LevenbergMarquartNumericalJacobianSolver::requiresNumericalJacobian() const
  {
    return true;
  }

  LevenbergMarquartNumericalJacobianSolver::~LevenbergMarquartNumericalJacobianSolver() = default;
  
} // end of namespace mfront

