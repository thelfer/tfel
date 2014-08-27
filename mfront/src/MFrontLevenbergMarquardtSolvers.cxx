/*! 
 * \file  MFrontLevenbergMarquardtSolvers.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 août 2014
 */

#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MechanicalBehaviourDescription.hxx"
#include "MFront/MFrontPerformanceProfiling.hxx"
#include "MFront/MFrontLevenbergMarquardtSolvers.hxx"

namespace mfront{

  std::vector<std::string>
  MFrontLevenbergMarquartSolverBase::getReservedNames(void) const
  {
    using namespace std;
    vector<string> n;
    n.push_back("levmar_jacobian_1");
    n.push_back("levmar_fzeros_1");
    n.push_back("levmar_m");
    n.push_back("levmar_sm");
    n.push_back("levmar_mu");
    n.push_back("levmar_muF");
    n.push_back("levmar_r");
    n.push_back("levmar_p0");
    n.push_back("levmar_p1");
    n.push_back("levmar_p2");
    n.push_back("levmaor_m");
    return n;
  } // end of MFrontLevenbergMarquartSolverBase::getReservedNames

  void
  MFrontLevenbergMarquartSolverBase::endsInputFileProcessing(MechanicalBehaviourDescription& mb) const
  {
    using namespace tfel::material;
    const ModellingHypothesis::Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if(!mb.hasParameter(h,"levmar_mu0")){
      mb.addParameter(h,VariableDescription("real","levmar_mu0",1u,0u));
      mb.setParameterDefaultValue(h,"levmar_mu0",1.e-6);
    }
    if(!mb.hasParameter(h,"levmar_p0")){
      mb.addParameter(h,VariableDescription("real","levmar_p0",1u,0u));
      mb.setParameterDefaultValue(h,"levmar_p0",1.e-4);
    }
    if(!mb.hasParameter(h,"levmar_p1")){
      mb.addParameter(h,VariableDescription("real","levmar_p1",1u,0u));
      mb.setParameterDefaultValue(h,"levmar_p1",0.25);
    }
    if(!mb.hasParameter(h,"levmar_p2")){
      mb.addParameter(h,VariableDescription("real","levmar_p2",1u,0u));
      mb.setParameterDefaultValue(h,"levmar_p2",0.75);
    }
    if(!mb.hasParameter(h,"levmar_m")){
      mb.addParameter(h,VariableDescription("real","levmar_m",1u,0u));
      mb.setParameterDefaultValue(h,"levmar_m",1.e-8);
    }
  }

  bool
  MFrontLevenbergMarquartSolverBase::usesJacobian(void) const
  {
    return true;
  } // end of MFrontLevenbergMarquartSolverBase::usesJacobian

  bool
  MFrontLevenbergMarquartSolverBase::usesJacobianInvert(void) const
  {
    return false;
  } // end of MFrontLevenbergMarquartSolverBase::usesJacobianInvert

  bool
  MFrontLevenbergMarquartSolverBase::allowsJacobianInitialisation(void) const
  {
    return false;
  } // end of MFrontLevenbergMarquartSolverBase::allowsJacobianInitialisation

  bool
  MFrontLevenbergMarquartSolverBase::allowsJacobianInvertInitialisation(void) const
  {
    return false;
  } // end of MFrontLevenbergMarquartSolverBase::allowsJacobianInvertInitialisation

  bool
  MFrontLevenbergMarquartSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations(void) const
  {
    return !this->requiresNumericalJacobian();
  } // end of MFrontLevenbergMarquartSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontLevenbergMarquartSolverBase::treatSpecificKeywords(MechanicalBehaviourDescription&,
							   const std::string&,
							   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
							   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) const
  {
    using namespace std;
    return make_pair(false,p);
  } // end of MFrontLevenbergMarquartSolverBase::treatSpecificKeywords

  void
  MFrontLevenbergMarquartSolverBase::writeSpecificInitializeMethodPart(std::ostream&,
								       const MechanicalBehaviourDescription&,
								       const tfel::material::ModellingHypothesis::Hypothesis) const
  {} // end of MFrontLevenbergMarquartSolverBase::writeSpecificInitializeMethodPart

  void
  MFrontLevenbergMarquartSolverBase::writeSpecificMembers(std::ostream&,
						      const MechanicalBehaviourDescription&,
						      const tfel::material::ModellingHypothesis::Hypothesis) const
  {} // end of MFrontLevenbergMarquartSolverBase::writeSpecificMembers

  void
  MFrontLevenbergMarquartSolverBase::writeResolutionAlgorithm(std::ostream& out,
							      const MechanicalBehaviourDescription& mb,
							      const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    const string btype = mb.getBehaviourTypeFlag();
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n2;
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n2 += mb.getTypeSize(p->type,p->arraySize);
    }
    out << "// dumping parameter" << endl;
    out << "real levmar_mu = this->levmar_mu0;" << endl;
    out << "real error;" << endl;
    out << "bool converged=false;" << endl;
    out << "this->iter=0;" << endl;
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : beginning of resolution\" << endl;" << endl;
    }
    if(mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      out << "this->computeStress();" << endl;
    }
    out << "if(!this->computeFdF()){" << endl;
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : computFdF returned false on first call, abording...\" << endl;" << endl;
    }
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;" << endl;
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;" << endl;
    }
    out << "}" << endl;
    if(this->requiresNumericalJacobian()){
      out << "this->computeNumericalJacobian(this->jacobian);" << endl;
    }
    out << "error=norm(this->fzeros);" << endl;
    out << "while((converged==false)&&" << endl;
    out << "(this->iter<" << mb.getClassName() << "::iterMax)){" << endl;
    out << "++(this->iter);" << endl;
    out << "this->zeros_1  = this->zeros;" << endl;
    MFrontNonLinearSystemSolverBase::writeComparisonToNumericalJacobian(out,mb,h,"jacobian");
    out << "converged = ((error)/(real(" << n2 << "))<";
    out << "(this->epsilon));" << endl;
    if(getDebugMode()){
      out << "cout << \"" << mb.getClassName()
	  << "::integrate() : iteration \" "
	  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << \", dumping parameter : \" << levmar_mu << endl;" << endl;
    }
    out << "if(!converged){" << endl;
    out << "// matrix containing tJJ+levmar_mu*I" << endl;
    out << "tmatrix<" << n2 << "," << n2 << ",real> levmar_tJJ;" << endl;
    out << "// vector containing tJ*F" << endl;
    out << "tvector<" << n2 << ",real> levmar_sm;" << endl;
    out << "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl
	<< "levmar_sm(idx)=real(0);"
	<< "for(unsigned short idx2=0;idx2!=" << n2 << ";++idx2){" << endl
	<< "levmar_sm(idx)+=(this->jacobian(idx2,idx))*(this->fzeros(idx2));"
	<< "levmar_tJJ(idx,idx2)=real(0);" << endl
	<< "for(unsigned short idx3=0;idx3!=" << n2 << ";++idx3){" << endl
	<< "levmar_tJJ(idx,idx2)+=(this->jacobian(idx3,idx))*(this->jacobian(idx3,idx2));" << endl
	<< "}" << endl
	<< "}" << endl
	<< "}" << endl
	<< "const real levmar_muF = (levmar_mu)*norm(this->fzeros);" << endl
	<< "for(unsigned short idx=0;idx!=" << n2 << ";++idx){" << endl
	<< "levmar_tJJ(idx,idx)+=levmar_muF;" << endl
	<< "}" << endl;
    out << "try{" << endl;
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      writeStandardPerformanceProfilingBegin(out,
					     "TinyMatrixSolve","lu");
    }
    out << "TinyMatrixSolve<" << n2
	<< "," << "real>::exe(levmar_tJJ,levmar_sm);" << endl;
    if(mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      writeStandardPerformanceProfilingEnd(out);
    }
    out << "}" << endl;
    out << "catch(LUException&){" << endl;
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;" << endl;
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;" << endl;
    }
    out << "}" << endl;
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,"levmar_sm");
    // levmar_sm contains the step
    out << "this->zeros -= levmar_sm;" << endl; 
    out << "tvector<" << n2 <<  ",real> levmar_fzeros_1 = this->fzeros;" << endl; 
    out << "tmatrix<" << n2 << "," << n2 <<  ",real> levmar_jacobian_1 = this->jacobian;" << endl; 
    if(mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      out << "this->computeStress();" << endl;
    }
    out << "if(!this->computeFdF()){" << endl;
    out << "// rejecting the step" << endl;
    out << "this->zeros     = this->zeros_1;" << endl; 
    out << "this->fzeros    = levmar_fzeros_1;" << endl; 
    out << "this->jacobian  = levmar_jacobian_1;" << endl; 
    if(mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      out << "this->computeStress();" << endl;
    }
    out << "// updating mu" << endl;
    out << "levmar_mu *= 4;" << endl;
    out << "} else {" << endl;
    out << "const real error_1=error;" << endl;
    out << "const real error_p=norm(levmar_fzeros_1-levmar_jacobian_1*levmar_sm);" << endl;
    out << "error=norm(this->fzeros);" << endl;
    out << "const real levmar_r = (error*error-error_1*error_1)/(error_p*error_p-error_1*error_1);" << endl;
    out << "if(levmar_r<this->levmar_p0){" << endl;
    out << "// rejecting the step" << endl;
    out << "this->zeros     = this->zeros_1;" << endl; 
    out << "this->fzeros    = levmar_fzeros_1;" << endl; 
    out << "this->jacobian  = levmar_jacobian_1;" << endl; 
    out << "error = error_1;" << endl;
    if(mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      out << "this->computeStress();" << endl;
    }
    out << "levmar_mu *= 4;" << endl;
    out << "} else {" << endl;
    out << "// accepting the step and updating mu" << endl;
    if(this->requiresNumericalJacobian()){
      out << "this->computeNumericalJacobian(this->jacobian);" << endl;
    }
    out << "if(levmar_r<this->levmar_p1){" << endl;
    out << "levmar_mu *= 4;" << endl;
    out << "} else if(levmar_r>this->levmar_p2){" << endl;
    out << "levmar_mu  = max(levmar_mu/4,this->levmar_m);" << endl;
    out << "}" << endl;
    out << "}" << endl;
    out << "}" << endl;
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out,mb,h);
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(out,mb,h);
    out << "}" << endl;
    out << "}" << endl;
  } // end of MFrontLevenbergMarquartSolverBase::writeResolutionAlgorithm

  MFrontLevenbergMarquartSolverBase::~MFrontLevenbergMarquartSolverBase()
  {} // end of  MFrontLevenbergMarquartSolverBase::~MFrontLevenbergMarquartSolverBase()


  bool
  MFrontLevenbergMarquartSolver::requiresNumericalJacobian(void) const
  {
    return false;
  }

  MFrontLevenbergMarquartSolver::~MFrontLevenbergMarquartSolver()
  {} // end of MFrontLevenbergMarquartSolver::~MFrontLevenbergMarquartSolver

  bool
  MFrontLevenbergMarquartNumericalJacobianSolver::requiresNumericalJacobian(void) const
  {
    return true;
  }

  MFrontLevenbergMarquartNumericalJacobianSolver::~MFrontLevenbergMarquartNumericalJacobianSolver()
  {} // end of MFrontLevenbergMarquartSolver::~MFrontLevenbergMarquartSolver
  
} // end of namespace mfront

