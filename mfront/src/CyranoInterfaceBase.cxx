/*!
 * \file   CyranoInterfaceBase.cxx
 * \brief  This file implements the CyranoInterfaceBase class.
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#include<cstdlib>
#include<sstream>
#include<iostream>

#include "TFEL/Utilities/ToString.hxx"
#include "MFront/Cyrano/CyranoInterfaceBase.hxx"

namespace cyrano{

  void
  CyranoInterfaceBase::throwUnMatchedNumberOfMaterialProperties(const std::string& b,
							      const unsigned short n1,
							      const CyranoInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceBase::throwUnMatchedNumberOfMaterialProperties : "
	<< "the number of material properties does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " material properties, and "
	<< n2 << " material properties were declared";
    throw(CyranoException(msg.str()));
  } // end of throwUnMatchedNumberOfMaterialProperties
    
  void
  CyranoInterfaceBase::throwUnMatchedNumberOfStateVariables(const std::string& b,
							  const unsigned short n1,
							  const CyranoInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceBase::throwUnMatchedNumberOfStateVariables : "
	<< "the number of internal state variables does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " state variables, and "
	<< n2 << " state variables were declared";
    throw(CyranoException(msg.str()));
  } // end of throwUnMatchedNumberOfStateVariables

  void
  CyranoInterfaceBase::treatCyranoException(const std::string& b,
					    const CyranoException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an CyranoException : "
	 << e.what() << endl;
  } // end of treatCyranoException

  void
  CyranoInterfaceBase::treatMaterialException(const std::string& b,
					    const tfel::material::MaterialException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an MaterialException : "
	 << e.what() << endl;
  } // end of treatMaterialException
    
  void
  CyranoInterfaceBase::treatTFELException(const std::string& b,
					const tfel::exception::TFELException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic tfel exception : "
	 << e.what() << endl;
  } // end of treatTFELException

  void
  CyranoInterfaceBase::treatStandardException(const std::string& b,
					    const std::exception& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic standard exception : "
	 << e.what() << endl;
  } // end of treatStandardException

  void
  CyranoInterfaceBase::treatUnknownException(const std::string& b)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an unknown exception" << endl;
  } // end of treatUnknownException
  
  void
  CyranoInterfaceBase::throwNegativeTimeStepException(const std::string& b)
  {
    using namespace std;
    string msg("CyranoInterfaceBase::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of CyranoInterfaceBase::throwNegativeTimeStepException
  
  void
  CyranoInterfaceBase::throwBehaviourIntegrationFailedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "integration failed for behaviour '"+b+"'";
    throw(CyranoIntegrationFailed(msg));
  } // end of CyranoInterfaceBase::throwBehaviourIntegrationFailedException
  
  void
  CyranoInterfaceBase::throwMaximumNumberOfSubSteppingReachedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "maximum number of sub stepping reached failed for behaviour '"+b+"'";
    throw(CyranoIntegrationFailed(msg));    
  } // end of CyranoInterfaceBase::throwMaximumNumberOfSubSteppingReachedException
  
  void
  CyranoInterfaceBase::throwPlaneStressMaximumNumberOfIterationsReachedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "maximum number of iterations of the plane stress algorithm ";
    msg += "reached failed for behaviour '"+b+"'";
    throw(CyranoIntegrationFailed(msg));  
  } // end of CyranoInterfaceBase::throwPlaneStressMaximumNumberOfIterationsReachedException

  void
  CyranoInterfaceBase::checkNTENSValue(const CyranoInt NTENS,
				     const unsigned short s)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(NTENS!=s){
      string msg("CyranoInterfaceBase::checkNTENSValue : "
		 "invalid value for the NTENS parameter "
		 "(got '"+ToString(NTENS)+"', "
		 "expected '"+ToString(s)+"')");
      throw(runtime_error(msg));  
    }
  } // end of CyranoInterfaceBase::checkNTENSValue

  void
  CyranoInterfaceBase::displayInvalidModellingHypothesisErrorMessage(void)
  {
    using namespace std;
    cout << "CyranoInterfaceBase::displayInvalidModellingHypothesisErrorMessage : "
	 << "invalid value for the NDI parameter" << endl;    
  }

  void
  CyranoInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis(const std::string& h)
  {
    using namespace std;
    string msg("CyranoInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis : "
	       "behaviour can't be used in '"+h+"'");
    throw(runtime_error(msg));  
  } // end of CyranoInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis

  void
  CyranoInterfaceBase::throwPredictionComputationFailedException(const std::string& b)
  {
    using namespace std;
    string msg("CyranoInterfaceBase::throwPredictionComputationFailedException : ");
    msg += "prediction computation failed for behaviour '"+b+"'";
    throw(runtime_error(msg));
  }

  void
  CyranoInterfaceBase::throwPredictionOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("CyranoInterfaceBase::throwPredictionOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a prediction operator";
    throw(runtime_error(msg));
  } // end of CyranoInterfaceBase::throwBehaviourIntegrationFailedException

  void
  CyranoInterfaceBase::throwConsistantTangentOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("CyranoInterfaceBase::throwConsistantTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a consistant tangent operator";
    throw(runtime_error(msg));
  } // end of CyranoInterfaceBase::throwBehaviourIntegrationFailedException

  void
  CyranoInterfaceBase::throwInvalidDDSOEException(const std::string& b,
						 const CyranoReal v)
  {
    using namespace std;
    ostringstream msg;
    msg << "CyranoInterfaceBase::throwInvalidDDSOEException : "
	<< "an invalid value for the DDSOE parameter has been given ('" << v << "')"
	<< " to the behaviour '" << b << "'.\n"
	<< "The following values are accepted:\n"
	<< "-3 : compute the prediction tangent operator, no behaviour integration\n"
	<< "-2 : compute the prediction secant  operator, no behaviour integration\n"
	<< "-1 : compute the prediction elastic operator, no behaviour integration\n"
	<< " 0 : integrate the behaviour over the time step, no stiffness requested\n"
	<< " 1 : integrate the behaviour over the time step, elastic stiffness requested\n"
	<< " 2 : integrate the behaviour over the time step, secant  operator  requested\n"
	<< " 3 : integrate the behaviour over the time step, tagent  operator  requested\n"
	<< " 4 : integrate the behaviour over the time step, consistant tagent operator requested\n";
    throw(runtime_error(msg.str()));
  } // end of CyranoInterfaceBase::throwInvalidDDSOEException

  void
  CyranoUnSupportedCaseHandler::exe(const CyranoReal *const ,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  const CyranoInt  *const,
				  const CyranoReal *const,
				  const CyranoReal *const,
				  CyranoReal *const,
				  const CyranoInt  *const,
				  CyranoReal *const)
  {
    using namespace std;
    string msg("CyranoUnSupportedCaseHandler::exe : "
	       "we fall in a case that the cyrano interface "
	       "is not able to handle.");
    throw(runtime_error(msg));  
  } // end of exe

} // end of namespace cyrano 
