/*!
 * \file   mfront/src/UMATInterfaceBase.cxx
 * \brief  This file implements the UMATInterfaceBase class.
 * \author Helfer Thomas
 * \date   12/12/2011
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<cstdlib>
#include<sstream>
#include<iostream>

#include "TFEL/Utilities/ToString.hxx"
#include "MFront/UMAT/UMATInterfaceBase.hxx"

namespace umat{

  void
  UMATInterfaceBase::throwUnMatchedNumberOfMaterialProperties(const std::string& b,
							      const unsigned short n1,
							      const UMATInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "UMATInterfaceBase::throwUnMatchedNumberOfMaterialProperties : "
	<< "the number of material properties does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " material properties, and "
	<< n2 << " material properties were declared";
    throw(UMATException(msg.str()));
  } // end of throwUnMatchedNumberOfMaterialProperties
    
  void
  UMATInterfaceBase::throwUnMatchedNumberOfStateVariables(const std::string& b,
							  const unsigned short n1,
							  const UMATInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "UMATInterfaceBase::throwUnMatchedNumberOfStateVariables : "
	<< "the number of internal state variables does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " state variables, and "
	<< n2 << " state variables were declared";
    throw(UMATException(msg.str()));
  } // end of throwUnMatchedNumberOfStateVariables

  void
  UMATInterfaceBase::treatUmatException(const std::string& b,
					const UMATException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an UMATException : "
	 << e.what() << endl;
  } // end of treatUmatException

  void
  UMATInterfaceBase::treatMaterialException(const std::string& b,
					    const tfel::material::MaterialException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an MaterialException : "
	 << e.what() << endl;
  } // end of treatMaterialException
    
  void
  UMATInterfaceBase::treatTFELException(const std::string& b,
					const tfel::exception::TFELException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic tfel exception : "
	 << e.what() << endl;
  } // end of treatTFELException

  void
  UMATInterfaceBase::treatStandardException(const std::string& b,
					    const std::exception& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic standard exception : "
	 << e.what() << endl;
  } // end of treatStandardException

  void
  UMATInterfaceBase::treatUnknownException(const std::string& b)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an unknown exception" << endl;
  } // end of treatUnknownException
  
  void
  UMATInterfaceBase::throwNegativeTimeStepException(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of UMATInterfaceBase::throwNegativeTimeStepException
  
  void
  UMATInterfaceBase::throwBehaviourIntegrationFailedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "integration failed for behaviour '"+b+"'";
    throw(UMATIntegrationFailed(msg));
  } // end of UMATInterfaceBase::throwBehaviourIntegrationFailedException
  
  void
  UMATInterfaceBase::throwMaximumNumberOfSubSteppingReachedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "maximum number of sub stepping reached failed for behaviour '"+b+"'";
    throw(UMATIntegrationFailed(msg));    
  } // end of UMATInterfaceBase::throwMaximumNumberOfSubSteppingReachedException
  
  void
  UMATInterfaceBase::throwPlaneStressMaximumNumberOfIterationsReachedException(const std::string& b)
  {
    using namespace std;
    string msg;
    msg += "maximum number of iterations of the plane stress algorithm ";
    msg += "reached failed for behaviour '"+b+"'";
    throw(UMATIntegrationFailed(msg));  
  } // end of UMATInterfaceBase::throwPlaneStressMaximumNumberOfIterationsReachedException

  void
  UMATInterfaceBase::checkNTENSValue(const UMATInt NTENS,
				     const unsigned short s)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(NTENS!=s){
      string msg("UMATInterfaceBase::checkNTENSValue : "
		 "invalid value for the NTENS parameter "
		 "(got '"+ToString(NTENS)+"', "
		 "expected '"+ToString(s)+"')");
      throw(runtime_error(msg));  
    }
  } // end of UMATInterfaceBase::checkNTENSValue

  void
  UMATInterfaceBase::displayInvalidModellingHypothesisErrorMessage(void)
  {
    using namespace std;
    cout << "UMATInterfaceBase::displayInvalidModellingHypothesisErrorMessage : "
	 << "invalid value for the NDI parameter" << endl;    
  }

  void
  UMATInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis(const UMATBehaviourType,
								     const std::string& h)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis : "
	       "behaviour can't be used in '"+h+"'");
    throw(runtime_error(msg));  
  } // end of UMATInterfaceBase::throwInvalidBehaviourTypeAndModellingHypothesis

  void
  UMATInterfaceBase::throwPredictionComputationFailedException(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwPredictionComputationFailedException : ");
    msg += "prediction computation failed for behaviour '"+b+"'";
    throw(runtime_error(msg));
  }

  void
  UMATInterfaceBase::throwPredictionOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwPredictionOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a prediction operator";
    throw(runtime_error(msg));
  } // end of UMATInterfaceBase::throwBehaviourIntegrationFailedException

  void
  UMATInterfaceBase::throwConsistentTangentOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwConsistentTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a consistent tangent operator";
    throw(runtime_error(msg));
  } // end of UMATInterfaceBase::throwBehaviourIntegrationFailedException

  void
  UMATInterfaceBase::throwUnsupportedStressFreeExpansionException(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwUnsupportedStressFreeExpansionException : ");
    msg += "behaviour '"+b+"' can handle stress-free expansion but the UMAT interface can't";
    throw(runtime_error(msg));
  } // end of UMATInterfaceBase::throwUnsupportedStressFreeExpansionException

  void
  UMATInterfaceBase::throwThermalExpansionCoefficientShallBeNull(const std::string& b)
  {
    using namespace std;
    string msg("UMATInterfaceBase::throwThermalExpansionCoefficientShallBeNull : ");
    msg += "behaviour '"+b+"' handle thermal expansion, so the thermal expansion "
      "declared in UMAT must be null.";
    throw(runtime_error(msg));
  }

  void
  UMATInterfaceBase::throwInvalidDDSOEException(const std::string& b,
						 const UMATReal v)
  {
    using namespace std;
    ostringstream msg;
    msg << "UMATInterfaceBase::throwInvalidDDSOEException : "
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
	<< " 4 : integrate the behaviour over the time step, consistent tagent operator requested\n";
    throw(runtime_error(msg.str()));
  } // end of UMATInterfaceBase::throwInvalidDDSOEException

  void
  UMATUnSupportedCaseHandler::exe(const UMATReal *const,
				  const UMATReal *const,
				  UMATReal *const,
				  const UMATReal *const,
				  const UMATReal *const,
				  const UMATReal *const,
				  const UMATReal *const,
				  const UMATReal *const,
				  const UMATInt  *const,
				  const UMATReal *const,
				  const UMATReal *const,
				  UMATReal *const,
				  const UMATInt  *const,
				  UMATReal *const,
				  const StressFreeExpansionHandler&)
  {
    using namespace std;
    string msg("UMATUnSupportedCaseHandler::exe : "
	       "we fall in a case that the umat interface "
	       "is not able to handle.");
    throw(runtime_error(msg));  
  } // end of exe

} // end of namespace umat 
