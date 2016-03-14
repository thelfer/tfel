/*!
 * \file   mfront/src/EuroplexusInterfaceExceptions.cxx
 * \brief  This file implements the EuroplexusInterfaceExceptions class.
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

#include "MFront/Europlexus/EuroplexusInterfaceExceptions.hxx"

namespace epx{

  void
  EuroplexusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(const std::string& b,
									  const unsigned short n1,
									  const EuroplexusInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "EuroplexusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties : "
	<< "the number of material properties does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " material properties, and "
	<< n2 << " material properties were declared";
    throw(EuroplexusException(msg.str()));
  } // end of throwUnMatchedNumberOfMaterialProperties
    
  void
  EuroplexusInterfaceExceptions::throwUnMatchedNumberOfStateVariables(const std::string& b,
								      const unsigned short n1,
								      const EuroplexusInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "EuroplexusInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
	<< "the number of internal state variables does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " state variables, and "
	<< n2 << " state variables were declared";
    throw(EuroplexusException(msg.str()));
  } // end of throwUnMatchedNumberOfStateVariables

  void
  EuroplexusInterfaceExceptions::treatEuroplexusException(const std::string& b,
							  const EuroplexusException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an EuroplexusException : "
	 << e.what() << endl;
  } // end of EuroplexusInterfaceExceptions::treatEuroplexusException

  void
  EuroplexusInterfaceExceptions::treatMaterialException(const std::string& b,
							const tfel::material::MaterialException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an MaterialException : "
	 << e.what() << endl;
  } // end of treatMaterialException
    
  void
  EuroplexusInterfaceExceptions::treatTFELException(const std::string& b,
						    const tfel::exception::TFELException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic tfel exception : "
	 << e.what() << endl;
  } // end of treatTFELException

  void
  EuroplexusInterfaceExceptions::treatStandardException(const std::string& b,
							const std::exception& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic standard exception : "
	 << e.what() << endl;
  } // end of treatStandardException

  void
  EuroplexusInterfaceExceptions::treatUnknownException(const std::string& b)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an unknown exception" << endl;
  } // end of treatUnknownException
  
  void
  EuroplexusInterfaceExceptions::throwNegativeTimeStepException(const std::string& b)
  {
    using namespace std;
    string msg("EuroplexusInterfaceExceptions::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of EuroplexusInterfaceExceptions::throwNegativeTimeStepException

  void
  EuroplexusInterfaceExceptions::throwPredictionComputationFailedException(const std::string& b)
  {
    throw(std::runtime_error("EuroplexusInterfaceExceptions::throwPredictionComputationFailedException: "
			     "prediction computation failed for behaviour '"+b+"'"));
  }

  void
  EuroplexusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(const std::string& b)
  {
    throw(std::runtime_error("EuroplexusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible: "
			     "behaviour '"+b+"' can't compute a prediction operator"));
  } // end of EuroplexusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  EuroplexusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(const std::string& b)
  {
    throw(std::runtime_error("EuroplexusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible: "
			     "behaviour '"+b+"' can't compute a consistent tangent operator"));
  } // end of EuroplexusInterfaceExceptions::throwBehaviourIntegrationFailedException
  
  void
  EuroplexusInterfaceExceptions::throwUnsupportedStressFreeExpansionException(const std::string& b)
  {
    throw(std::runtime_error("EuroplexusInterfaceExceptions::throwUnsupportedStressFreeExpansionException: "
			     "behaviour '"+b+"' can handle stress-free expansion but the Europlexus interface can't"));
  } // end of EuroplexusInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void
  EuroplexusInterfaceExceptions::displayUnsupportedHypothesisMessage()
  {
    using namespace std;
    cout << "EuroplexusInterfaceExceptions::displayUnsupportedHypothesisMessage : "
	 << "unsupported hypothesis" << endl;
  }

  void
  EuroplexusUnSupportedCaseHandler::exe(const EuroplexusReal *const,
					const EuroplexusReal *const,
					const EuroplexusReal *const,
					const EuroplexusReal *const,
					const EuroplexusReal *const,
					const EuroplexusReal *const,
					const EuroplexusInt  *const,
					const EuroplexusReal *const,
					const EuroplexusReal *const,
					EuroplexusReal *const,
					const EuroplexusInt  *const,
					EuroplexusReal *const,
					const StressFreeExpansionHandler&)
  {
    throw(std::runtime_error("EuroplexusUnSupportedCaseHandler::exe : "
			     "we fall in a case that the Europlexus interface "
			     "is not able to handle."));  
  } // end of exe

} // end of namespace epx 
