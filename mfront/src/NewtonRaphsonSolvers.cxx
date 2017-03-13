/*! 
 * \file  mfront/src/NewtonRaphsonSolvers.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
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
#include "MFront/NewtonRaphsonSolvers.hxx"

namespace mfront{

  MFrontNewtonRaphsonSolverBase::MFrontNewtonRaphsonSolverBase()
    : jacobianUpdatePeriod(-1)
  {} // end of MFrontNewtonRaphsonSolverBase::MFrontNewtonRaphsonSolverBase()

  std::vector<std::string>
  MFrontNewtonRaphsonSolverBase::getReservedNames() const
  {
    auto n = std::vector<std::string>{};
    if(this->usesPowellDogLegAlgorithm()){
      const auto& n2 = PowellDogLegAlgorithmBase::getReservedNames();
      n.insert(n.end(),n2.begin(),n2.end());
      n.insert(n.end(),{"tjacobian","tfzeros"});
    }
    n.insert(n.end(),{"permutation_vector",
	  "integrate_one_half","computeFdF_ok"});
    return n;
  } // end of MFrontNewtonRaphsonSolverBase::getReservedNames

  bool MFrontNewtonRaphsonSolverBase::usesJacobian() const
  {
    return true;
  } // end of MFrontNewtonRaphsonSolverBase::usesJacobian

  bool MFrontNewtonRaphsonSolverBase::usesJacobianInvert() const
  {
    return false;
  } // end of MFrontNewtonRaphsonSolverBase::usesJacobianInvert

  bool MFrontNewtonRaphsonSolverBase::allowsJacobianInitialisation() const
  {
    return false;
  } // end of MFrontNewtonRaphsonSolverBase::allowsJacobianInitialisation

  bool MFrontNewtonRaphsonSolverBase::allowsJacobianInvertInitialisation() const
  {
    return false;
  } // end of MFrontNewtonRaphsonSolverBase::allowsJacobianInvertInitialisation

  bool
  MFrontNewtonRaphsonSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const
  {
    return !this->requiresNumericalJacobian();
  } // end of MFrontNewtonRaphsonSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontNewtonRaphsonSolverBase::treatSpecificKeywords(BehaviourDescription& mb,
						       const std::string& key,
						       const tokens_iterator p,
						       const tokens_iterator pe)
  {
    using namespace std;
    using tfel::utilities::CxxTokenizer;
    const auto r = PowellDogLegAlgorithmBase::treatSpecificKeywords(mb,key,p,pe);
    if(r.first){
      return r;
    }
    if(key=="@JacobianUpdatePeriod"){
      if(!this->requiresNumericalJacobian()){
	throw(runtime_error("MFrontNewtonRaphsonSolverBase::treatSpecificKeywords : "
			    "jacobian update period is only defined "
			    "for algorithms using a numerical jacobian"));
      }
      if(this->jacobianUpdatePeriod!=-1){
	throw(runtime_error("MFrontNewtonRaphsonSolverBase::treatSpecificKeywords : "
			    "jacobian update period already defined"));
      }
      auto c = p;
      this->jacobianUpdatePeriod=CxxTokenizer::readInt(c,pe);
      if(this->jacobianUpdatePeriod<=0){
	throw(runtime_error("MFrontNewtonRaphsonSolverBase::treatSpecificKeywords : "
			    "invalid value for jacobian update period "
			    "(read '"+p->value+"')"));
      }
      CxxTokenizer::readSpecifiedToken("MFrontNewtonRaphsonSolverBase::treatSpecificKeywords : ",
				       ";",c,pe);
      return {true,c};
    }
    return {false,p};
  } // end of MFrontNewtonRaphsonSolverBase::treatSpecificKeywords

  void MFrontNewtonRaphsonSolverBase::endsInputFileProcessing(BehaviourDescription& mb) const
  {
    if(this->usesPowellDogLegAlgorithm()){
      return PowellDogLegAlgorithmBase::endsInputFileProcessing(mb);
    }
  } // end of MFrontNewtonRaphsonSolverBase::endsInputFileProcessing

  void MFrontNewtonRaphsonSolverBase::writeSpecificInitializeMethodPart(std::ostream&,
									const BehaviourDescription&,
									const Hypothesis) const
  {} // end of MFrontNewtonRaphsonSolverBase::writeSpecificMembers

  void MFrontNewtonRaphsonSolverBase::writeSpecificMembers(std::ostream&,
							   const BehaviourDescription&,
							   const Hypothesis) const
  {} // end of MFrontNewtonRaphsonSolverBase::writeSpecificMembers

  void MFrontNewtonRaphsonSolverBase::writeResolutionAlgorithm(std::ostream& out,
							       const BehaviourDescription& mb,
							       const Hypothesis h) const
  {
    
    using namespace std;
    const auto  btype = mb.getBehaviourTypeFlag();
    const auto& d = mb.getBehaviourData(h);
    SupportedTypes::TypeSize n2;
    for(const auto& v : d.getIntegrationVariables()){
      n2 += mb.getTypeSize(v.type,v.arraySize);
    }
    if(mb.hasAttribute(h,BehaviourData::compareToNumericalJacobian)){
      out << "tmatrix<" << n2 << "," << n2 << ",real> njacobian;\n";
    }
    if((this->requiresNumericalJacobian())&&(this->jacobianUpdatePeriod!=-1)&&(n2.getValueForDimension(1u)>3)){
      out << "TinyPermutation<" << n2 << "> permutation_vector;\n";
    }
    out << "real error;\n"
	<< "bool converged=false;\n"
	<< "this->iter=0;\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName() << "::integrate() : beginning of resolution\" << endl;\n";
    }
    out << "while((converged==false)&&\n"
	<< "(this->iter<" << mb.getClassName() << "::iterMax)){\n"
	<< "++(this->iter);\n";
    if(mb.hasCode(h,BehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "const bool computeFdF_ok = this->computeFdF();\n"
	<< "if(computeFdF_ok){\n"
	<< "error=norm(this->fzeros);\n"
	<< "}\n"
	<< "if((!computeFdF_ok)||(!ieee754::isfinite(error))){\n"
	<< "if(this->iter==1){\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : computFdF returned false on first iteration, abording...\" << endl;\n";
    }
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "} else {\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : computFdF returned false, dividing increment by two...\" << endl;\n";
    }
    out << "const real integrate_one_half = real(1)/real(2);\n"
	<< "this->zeros -= (this->zeros-this->zeros_1)*integrate_one_half;\n"
	<< "}\n"
	<< "} else {\n"
	<< "this->zeros_1  = this->zeros;\n";
    if(!this->requiresNumericalJacobian()){
      this->writeComparisonToNumericalJacobian(out,mb,h,"njacobian");
    }
    out << "converged = ((error)/(real(" << n2 << "))<"
	<< "(this->epsilon));\n";
    if(this->requiresNumericalJacobian()){
      // We compute the numerical jacobian even if we converged since
      // most of the time, this tangent operator will be computed
      // using the partial jacobian invert. We consider very unlikely
      // that a user may use a numerical jacobian and provide an
      // analytic definition of the tangent operator
      out << "if((!converged)||(smt!=NOSTIFFNESSREQUESTED)){\n";
      if(this->jacobianUpdatePeriod!=-1){
	out << "if(converged){\n"
	    << "this->computeNumericalJacobian(this->jacobian);\n"
	    << "} else {\n"
	    << "if(this->iter%" << jacobianUpdatePeriod << "){\n";
	if(getDebugMode()){
	  out << "cout << \"" << mb.getClassName()
	      << "::integrate() : updating jacobian\" << endl;\n";
	}
	out << "this->computeNumericalJacobian(this->jacobian);\n"
	    << "}\n"
	    << "}\n";
      } else {
	out << "this->computeNumericalJacobian(this->jacobian);\n";
      }	
      out << "}\n";
    }
    if(getDebugMode()){
      out << "cout << \"" << mb.getClassName()
	  << "::integrate() : iteration \" "
	  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << endl;\n";
    }
    out << "if(!converged){\n";
    if(this->usesPowellDogLegAlgorithm()){
      out << "tmatrix<" << n2 << "," << n2 << ",real> tjacobian(this->jacobian);\n";
      out << "tvector<" << n2 << ",real> tfzeros(this->fzeros);\n";
    }
    out << "try{\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingBegin(out,mb.getClassName(),"TinyMatrixSolve","lu");
    }
    if((this->requiresNumericalJacobian())&&(this->jacobianUpdatePeriod!=-1)&&(n2.getValueForDimension(1u)>3)){
      out << "if(this->iter%" << jacobianUpdatePeriod << "){\n"
	  << "TinyMatrixSolve<" << n2
	  << "," << "real>::decomp(this->jacobian,permutation_vector);\n"
	  << "}\n"
	  << "TinyMatrixSolve<" << n2
	  << "," << "real>::back_substitute(this->jacobian,permutation_vector,this->fzeros);\n";
    } else {
      out << "TinyMatrixSolve<" << n2
	  << "," << "real>::exe(this->jacobian,this->fzeros);\n";
    }
    if(mb.getAttribute(BehaviourData::profiling,false)){
      writeStandardPerformanceProfilingEnd(out);
    }
    out << "}\n"
	<< "catch(LUException&){\n";
    if(getDebugMode()){
      out << "cout << \"" << mb.getClassName()
	  << "::integrate(): jacobian inversion failed\" << endl << endl;\n";
    }
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}\n";
    if(this->usesPowellDogLegAlgorithm()){
      this->writePowellDogLegStep(out,mb,h,"tjacobian","tfzeros","fzeros");
    } else {
      NonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,"fzeros");
      out << "this->zeros -= this->fzeros;\n";
    }
    NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out,mb,h);
    NonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(out,mb,h);
    out << "}\n"
	<< "}\n"
	<< "}\n";
  } // end of MFrontNewtonRaphsonSolverBase::writeResolutionAlgorithm

  MFrontNewtonRaphsonSolverBase::~MFrontNewtonRaphsonSolverBase() = default;

  bool MFrontNewtonRaphsonSolver::requiresNumericalJacobian() const
  {
    return false;
  }

  bool MFrontNewtonRaphsonSolver::usesPowellDogLegAlgorithm() const
  {
    return false;
  }

  MFrontNewtonRaphsonSolver::~MFrontNewtonRaphsonSolver() = default;

  bool MFrontNewtonRaphsonNumericalJacobianSolver::requiresNumericalJacobian() const
  {
    return true;
  }

  bool MFrontNewtonRaphsonNumericalJacobianSolver::usesPowellDogLegAlgorithm() const
  {
    return false;
  }

  MFrontNewtonRaphsonNumericalJacobianSolver::~MFrontNewtonRaphsonNumericalJacobianSolver() = default;

  bool MFrontPowellDogLegNewtonRaphsonSolver::requiresNumericalJacobian() const
  {
    return false;
  }

  bool MFrontPowellDogLegNewtonRaphsonSolver::usesPowellDogLegAlgorithm() const
  {
    return true;
  }

  MFrontPowellDogLegNewtonRaphsonSolver::~MFrontPowellDogLegNewtonRaphsonSolver() = default;

  bool MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver::requiresNumericalJacobian() const
  {
    return true;
  }

  bool MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver::usesPowellDogLegAlgorithm() const
  {
    return true;
  }

  MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver::~MFrontPowellDogLegNewtonRaphsonNumericalJacobianSolver() = default;
  
} // end of namespace mfront
