/*! 
 * \file  MFrontBehaviourTimer.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 mai 2014
 */

#include<string>
#include<stdexcept>

#include "MFront/MFrontBehaviourTimer.hxx"

namespace mfront
{

  static std::string
  getCodeBlockName(const MFrontBehaviourTimer::CodeBlock c)
  {
    using namespace std;
    string n;
    switch(c){
    case MFrontBehaviourTimer::FLOWRULE:
      n = "FlowRule";
      break;
    case MFrontBehaviourTimer::BEFOREINITIALIZELOCALVARIABLES:
      n = "BeforeInitializeLocalVariables";
      break;
    case MFrontBehaviourTimer::INITIALIZELOCALVARIABLES:
      n = "InitializeLocalVariables";
      break;
    case MFrontBehaviourTimer::AFTERINITIALIZELOCALVARIABLES:
      n = "AfterInitializeLocalVariables";
      break;
    case MFrontBehaviourTimer::COMPUTEPREDICTOR:
      n = "ComputePredictor";
      break;
    case MFrontBehaviourTimer::COMPUTESTRESS:
      n = "ComputeStress";
      break;
    case MFrontBehaviourTimer::COMPUTEFINALSTRESS:
      n = "ComputeFinalStress";
      break;
    case MFrontBehaviourTimer::DEFAULTCOMPUTEFINALSTRESS:
      n = "DefaultComputeFinalStress";
      break;
    case MFrontBehaviourTimer::INTEGRATOR:
      n = "Integrator";
      break;
    case MFrontBehaviourTimer::COMPUTEDERIVATIVE:
      n = "ComputeDerivative";
      break;
    case MFrontBehaviourTimer::COMPUTEPREDICTIONOPERATOR:
      n = "ComputePredictionOperator";
      break;
    case MFrontBehaviourTimer::COMPUTETANGENTOPERATOR:
      n = "ComputeTangentOperator";
      break;
    case MFrontBehaviourTimer::UPDATEAUXILIARYSTATEVARIABLES:
      n = "UpdateAuxiliaryStateVariables";
      break;
    case MFrontBehaviourTimer::INITIALIZEJACOBIAN:
      n = "InitializeJacobian";
      break;
    default:
      string msg("getCodeBlockName : no name associated with the given code block");
      throw(runtime_error(msg));
    }
    return n;
  }
  
  
  
} // end of namespace mfront
