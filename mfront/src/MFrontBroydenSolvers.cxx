/*! 
 * \file  mfront/src/MFrontBroydenSolvers.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 22 août 2014
 */

#include "MFront/MFrontDebugMode.hxx"
#include "MFront/MechanicalBehaviourDescription.hxx"
#include "MFront/MFrontPerformanceProfiling.hxx"
#include "MFront/MFrontBroydenSolvers.hxx"

namespace mfront{

  std::vector<std::string>
  MFrontBroydenSolverBase::getReservedNames(void) const
  {
    using namespace std;
    vector<string> n;
    if(this->usesPowellDogLegAlgorithm()){
      const vector<string>& n2 =
	MFrontPowellDogLegAlgorithmBase::getReservedNames();
      n.insert(n.end(),n2.begin(),n2.end());
      n.push_back("tjacobian");
      n.push_back("tfzeros");
      n.push_back("integrate_one_half");
    }
    n.push_back("jacobian2");
    n.push_back("fzeros2");
    n.push_back("Dzeros");
    n.push_back("broyden_inv");
    n.push_back("integrate_one_half");
    n.push_back("computeFdF_ok");
    return n;
  } // end of MFrontBroydenSolverBase::getReservedNames

  bool
  MFrontBroydenSolverBase::usesJacobian(void) const
  {
    return true;
  } // end of MFrontBroydenSolverBase::usesJacobian

  bool
  MFrontBroydenSolverBase::usesJacobianInvert(void) const
  {
    return true;
  } // end of MFrontBroydenSolverBase::usesJacobianInvert

  bool
  MFrontBroydenSolverBase::requiresNumericalJacobian(void) const
  {
    return false;
  }

  bool
  MFrontBroydenSolverBase::allowsJacobianInitialisation(void) const
  {
    return true;
  }

  bool
  MFrontBroydenSolverBase::allowsJacobianInvertInitialisation(void) const
  {
    return false;
  }

  bool
  MFrontBroydenSolverBase::requiresJacobianToBeReinitialisedToIdentityAtEachIterations(void) const
  {
    return false;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontBroydenSolverBase::treatSpecificKeywords(MechanicalBehaviourDescription& mb,
						 const std::string& key,
						 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
						 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator pe) const
  {
    using namespace std;
    if(this->usesPowellDogLegAlgorithm()){
      return MFrontPowellDogLegAlgorithmBase::treatSpecificKeywords(mb,key,p,pe);
    }
    return make_pair(false,p);
  } // end of MFrontBroydenSolverBase::treatSpecificKeywords

  void
  MFrontBroydenSolverBase::endsInputFileProcessing(MechanicalBehaviourDescription& mb) const
  {
    using namespace std;
    if(this->usesPowellDogLegAlgorithm()){
      return MFrontPowellDogLegAlgorithmBase::endsInputFileProcessing(mb);
    }
  } // end of MFrontBroydenSolverBase::endsInputFileProcessing

  void
  MFrontBroydenSolverBase::writeSpecificMembers(std::ostream&,
						const MechanicalBehaviourDescription&,
						const tfel::material::ModellingHypothesis::Hypothesis) const
  {} // end of MFrontBroydenSolverBase::writeSpecificMembers

  void
  MFrontBroydenSolverBase::writeSpecificInitializeMethodPart(std::ostream& out,
							     const MechanicalBehaviourDescription& mb,
							     const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    const MechanicalBehaviourData& d = mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    SupportedTypes::TypeSize n;
    // size of linear system
    for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
      n += mb.getTypeSize(p->type,p->arraySize);
    }
    if(mb.hasCode(h,MechanicalBehaviourData::InitializeJacobian)){
      out << mb.getCode(h,MechanicalBehaviourData::InitializeJacobian);
    } else {
      out << "// setting jacobian to identity" << endl;
      out << "std::fill(this->jacobian.begin(),this->jacobian.end(),real(0));" << endl;
      out << "for(unsigned short idx = 0; idx!= "<< n << ";++idx){" << endl;
      out << "this->jacobian(idx,idx)= real(1);" << endl;
      out << "}" << endl;
    }
  } // end of MFrontBroydenSolverBase::writeSpecificInitializeMethodPart

  void
  MFrontBroydenSolverBase::writeResolutionAlgorithm(std::ostream& out,
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
    out << "tmatrix<" << n2 << "," << n2 << ",real> jacobian2;\n";
    out << "tvector<" << n2 << ",real> fzeros2;\n";
    out << "tvector<" << n2 << ",real> Dzeros;\n";
    out << "real broyden_inv;\n";
    out << "real error;\n";
    out << "bool converged=false;\n";
    out << "this->iter=0;\n";
    if(getDebugMode()){
      out << "cout << endl << \"" << mb.getClassName()
	  << "::integrate() : beginning of resolution\" << endl;\n";
    }
    out << "while((converged==false)&&\n";
    out << "(this->iter<" << mb.getClassName() << "::iterMax)){\n";
    out << "++(this->iter);\n";
    out << "fzeros2 = this->fzeros;\n";
    if(mb.hasCode(h,MechanicalBehaviourData::ComputeStress)){
      out << "this->computeStress();\n";
    }
    out << "const bool computeFdF_ok = this->computeFdF();\n";
    out << "if(!computeFdF_ok){\n";
    out << "if(this->iter==1){\n";
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
    out << "const real integrate_one_half = real(1)/real(2);\n";
    out << "this->zeros -= (this->zeros-this->zeros_1)*integrate_one_half;\n";
    out << "}\n";
    out << "} else {\n";
    out << "this->zeros_1  = this->zeros;\n";
    out << "error=norm(this->fzeros);\n";
    out << "converged = ((error)/(real(" << n2 << "))<";
    out << "(this->epsilon));\n";
    if(getDebugMode()){
      out << "cout << \"" << mb.getClassName()
	  << "::integrate() : iteration \" "
	  << "<< this->iter << \" : \" << (error)/(real(" << n2 << ")) << endl;\n";
    }
    out << "if(!converged){\n";
    out << "Dzeros = this->fzeros;\n";
    out << "jacobian2 = this->jacobian;\n";
    out << "try{" << endl;
    out << "TinyMatrixSolve<" << n2
	<< "," << "real>::exe(jacobian2,Dzeros);\n";
    out << "}" << endl;
    out << "catch(LUException&){" << endl;
    if(mb.useQt()){        
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
    } else {
      out << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
    }
    out << "}" << endl;
    out << "jacobian2 = this->jacobian;\n";
    if(this->usesPowellDogLegAlgorithm()){
      this->writePowellDogLegStep(out,mb,h,"tjacobian","tfzeros","fzeros");
    } else {
      MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValues(out,mb,h,"fzeros");
      out << "this->zeros -= Dzeros;\n";
    }
    out << "if(this->iter>1){\n";
    out << "broyden_inv = (Dzeros|Dzeros);\n";
    out << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n";
    out << "this->jacobian += "
	<< "(((this->fzeros-fzeros2)-(jacobian2)*(Dzeros))^Dzeros)/broyden_inv;\n";
    out << "}\n";
    out << "}\n";
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out,mb,h);
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(out,mb,h);
    out << "}\n";
    out << "}\n";
    out << "}\n";
  } // end of MFrontBroydenSolverBase::writeResolutionAlgorithm

  MFrontBroydenSolverBase::~MFrontBroydenSolverBase()
  {} // end of  MFrontBroydenSolverBase::~MFrontBroydenSolverBase()

  bool
  MFrontBroydenSolver::usesPowellDogLegAlgorithm(void) const
  {
    return false;
  }

  MFrontBroydenSolver::~MFrontBroydenSolver()
  {} // end of MFrontBroydenSolver::~MFrontBroydenSolver

  bool
  MFrontPowellDogLegBroydenSolver::usesPowellDogLegAlgorithm(void) const
  {
    return true;
  }

  MFrontPowellDogLegBroydenSolver::~MFrontPowellDogLegBroydenSolver()
  {} // end of MFrontBroydenSolver::~MFrontPowellDogLegBroydenSolver
  
} // end of namespace mfront
