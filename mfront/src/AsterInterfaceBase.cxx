/*!
 * \file   AsterInterfaceBase.cxx
 * \brief  This file implements the AsterInterfaceBase class.
 * \author Helfer Thomas
 * \date   12/12/2011
 */

#include<cstdlib>
#include<sstream>
#include<iostream>

#include "MFront/Aster/AsterInterfaceBase.hxx"

namespace aster{

  void
  AsterInterfaceBase::throwUnMatchedNumberOfMaterialProperties(const std::string& b,
							       const unsigned short n1,
							       const AsterInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "AsterInterfaceBase::throwUnMatchedNumberOfMaterialProperties : "
	<< "the number of material properties does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " material properties, and "
	<< n2 << " material properties where declared";
    throw(AsterException(msg.str()));
  } // end of throwUnMatchedNumberOfMaterialProperties
    
  void
  AsterInterfaceBase::throwUnMatchedNumberOfStateVariables(const std::string& b,
							  const unsigned short n1,
							  const AsterInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "AsterInterfaceBase::throwUnMatchedNumberOfStateVariables : "
	<< "the number of internal state variables does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " state variables, and "
	<< n2 << " state variables where declared";
    throw(AsterException(msg.str()));
  } // end of throwUnMatchedNumberOfStateVariables

  void
  AsterInterfaceBase::treatAsterException(const std::string& b,
					  const AsterException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an AsterException : "
	 << e.what() << endl;
  } // end of treatUmatException

  void
  AsterInterfaceBase::treatMaterialException(const std::string& b,
					    const tfel::material::MaterialException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an MaterialException : "
	 << e.what() << endl;
  } // end of treatMaterialException
    
  void
  AsterInterfaceBase::treatTFELException(const std::string& b,
					const tfel::exception::TFELException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic tfel exception : "
	 << e.what() << endl;
  } // end of treatTFELException

  void
  AsterInterfaceBase::treatStandardException(const std::string& b,
					    const std::exception& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic standard exception : "
	 << e.what() << endl;
  } // end of treatStandardException

  void
  AsterInterfaceBase::treatUnknownException(const std::string& b)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an unknown exception" << endl;
  } // end of treatUnknownException
  
  void
  AsterInterfaceBase::throwNegativeTimeStepException(const std::string& b)
  {
    using namespace std;
    string msg("AsterInterfaceBase::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of AsterInterfaceBase::throwNegativeTimeStepException
  
  void
  AsterInterfaceBase::throwBehaviourIntegrationFailedException(const std::string& b)
  {
    using namespace std;
    string msg("AsterInterfaceBase::throwBehaviourIntegrationFailedException : ");
    msg += "integration failed for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of AsterInterfaceBase::throwBehaviourIntegrationFailedException
  
  void
  AsterInterfaceBase::throwConsistantTangentOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("AsterInterfaceBase::throwConsistantTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a consistant tangent operator";
    throw(runtime_error(msg));
  } // end of AsterInterfaceBase::throwBehaviourIntegrationFailedException

  void
  AsterInterfaceBase::displayInvalidNTENSValueErrorMessage()
  {
    using namespace std;
    cout << "AsterInterfaceBase::displayInvalidNTENSValueErrorMessage : "
	 << "invalide value for the NTENS paramater" << endl;    
  }

} // end of namespace aster 
