/*! 
 * \file  mfront/src/MFrontSecondBroydenSolver.cxx
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
#include "MFront/MechanicalBehaviourDescription.hxx"
#include "MFront/MFrontPerformanceProfiling.hxx"
#include "MFront/MFrontSecondBroydenSolver.hxx"

namespace mfront{

  std::vector<std::string>
  MFrontSecondBroydenSolver::getReservedNames(void) const
  {
    using namespace std;
    vector<string> n;
    n.push_back("fzeros2");
    n.push_back("Dzeros");
    n.push_back("Dfzeros");
    n.push_back("inv_jacobian");
    n.push_back("inv_jacobian2");
    n.push_back("broyden_inv");
    n.push_back("integrate_one_half");
    n.push_back("computeFdF_ok");
    return n;
  } // end of MFrontSecondBroydenSolver::getReservedNames

  bool
  MFrontSecondBroydenSolver::usesJacobian(void) const
  {
    return false;
  } // end of MFrontSecondBroydenSolver::usesJacobian

  bool
  MFrontSecondBroydenSolver::usesJacobianInvert(void) const
  {
    return true;
  } // end of MFrontSecondBroydenSolver::usesJacobianInvert

  bool
  MFrontSecondBroydenSolver::allowsJacobianInitialisation(void) const
  {
    return false;
  } // end of MFrontSecondBroydenSolver::allowsJacobianInitialisation

  bool
  MFrontSecondBroydenSolver::allowsJacobianInvertInitialisation(void) const
  {
    return true;
  } // end of MFrontSecondBroydenSolver::allowsJacobianInvertInitialisation

  bool
  MFrontSecondBroydenSolver::requiresJacobianToBeReinitialisedToIdentityAtEachIterations(void) const
  {
    return false;
  } // end of MFrontSecondBroydenSolver::allowsJacobianInitialisation

  bool
  MFrontSecondBroydenSolver::requiresNumericalJacobian(void) const
  {
    return false;
  }

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  MFrontSecondBroydenSolver::treatSpecificKeywords(MechanicalBehaviourDescription&,
						   const std::string&,
						   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator p,
						   const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) const
  {
    using namespace std;
    return make_pair(false,p);
  } // end of MFrontLevenbergMarquartSolverBase::treatSpecificKeywords

  void
  MFrontSecondBroydenSolver::endsInputFileProcessing(MechanicalBehaviourDescription&) const
  {} // end of MFrontSecondBroydenSolver::endsInputFileProcessing

  void
  MFrontSecondBroydenSolver::writeSpecificMembers(std::ostream&,
						  const MechanicalBehaviourDescription&,
						  const tfel::material::ModellingHypothesis::Hypothesis) const
  {} // end of MFrontSecondBroydenSolver::writeSpecificMembers

  void
  MFrontSecondBroydenSolver::writeSpecificInitializeMethodPart(std::ostream& out,
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
    if(mb.hasCode(h,MechanicalBehaviourData::InitializeJacobianInvert)){
      out << mb.getCode(h,MechanicalBehaviourData::InitializeJacobianInvert);
    } else {
      out << "// setting the inverse of jacobian to identity" << endl;
      out << "std::fill(this->inv_jacobian.begin(),this->inv_jacobian.end(),real(0));" << endl;
      out << "for(unsigned short idx = 0; idx!= "<< n << ";++idx){" << endl;
      out << "this->inv_jacobian(idx,idx)= real(1);" << endl;
      out << "}" << endl;
    }
  } // end of MFrontSecondBroydenSolver::writeSpecificInitializeMethodPart
  
  void
  MFrontSecondBroydenSolver::writeResolutionAlgorithm(std::ostream& out,
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
    out << "tmatrix<" << n2 << "," << n2 << ",real> inv_jacobian2;\n";
    out << "tvector<" << n2 << ",real> fzeros2;\n";
    out << "tvector<" << n2 << ",real> Dzeros;\n";
    out << "tvector<" << n2 << ",real> Dfzeros;\n";
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
    out << "Dzeros   = -(this->inv_jacobian)*(this->fzeros);\n";
    this->writeLimitsOnIncrementValues(out,mb,h,"Dzeros");
    out << "this->zeros  += Dzeros;\n";
    out << "if(this->iter>1){\n";
    out << "Dfzeros   = (this->fzeros)-fzeros2;\n";
    out << "broyden_inv = Dzeros|((this->inv_jacobian)*Dfzeros);\n";
    out << "if(broyden_inv>100*std::numeric_limits<real>::epsilon()){\n";
    out << "inv_jacobian2 = this->inv_jacobian;\n";
    out << "this->inv_jacobian += "
	<< "((Dzeros-inv_jacobian2*Dfzeros)^(Dzeros*inv_jacobian2))/(broyden_inv);\n";
    out << "}\n";
    out << "}\n";
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnStateVariablesPhysicalBounds(out,mb,h);
    MFrontNonLinearSystemSolverBase::writeLimitsOnIncrementValuesBasedOnIntegrationVariablesIncrementsPhysicalBounds(out,mb,h);
    out << "}\n";
    out << "}\n";
    out << "}\n";
  } // end of MFrontSecondBroydenSolver::writeResolutionAlgorithm

  MFrontSecondBroydenSolver::~MFrontSecondBroydenSolver()
  {} // end of  MFrontSecondBroydenSolver::~MFrontSecondBroydenSolver()
  
} // end of namespace mfront
