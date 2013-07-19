/*!
 * \file   UMATInterfaceBase.cxx
 * \brief  This file implements the UMATInterfaceBase class.
 * \author Helfer Thomas
 * \date   12/12/2011
 */

#include<cstdlib>
#include<sstream>
#include<iostream>

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
				     const unsigned short N)
  {
    using namespace std;
    bool ok = false;
    if(NTENS==3){
      ok = (N==1u);
    } else if (NTENS==4){
      ok = (N==2u);
    } else if (NTENS==6){
      ok = (N==3u);
    }
    if(!ok){
      string msg("UMATInterfaceBase::checkNTENSValue : ");
      msg += "invalid value for the NTENS parameter";
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

} // end of namespace umat 
