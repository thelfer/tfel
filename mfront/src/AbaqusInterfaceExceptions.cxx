/*!
 * \file   mfront/src/AbaqusInterfaceExceptions.cxx
 * \brief  This file implements the AbaqusInterfaceExceptions class.
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

#include "MFront/Abaqus/AbaqusInterfaceExceptions.hxx"

namespace abaqus{

  

  void
  AbaqusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties(const std::string& b,
								     const unsigned short n1,
								     const AbaqusInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "AbaqusInterfaceExceptions::throwUnMatchedNumberOfMaterialProperties : "
	<< "the number of material properties does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " material properties, and "
	<< n2 << " material properties were declared";
    throw(AbaqusException(msg.str()));
  } // end of throwUnMatchedNumberOfMaterialProperties
    
  void
  AbaqusInterfaceExceptions::throwUnMatchedNumberOfStateVariables(const std::string& b,
								 const unsigned short n1,
								 const AbaqusInt n2)
  {
    using namespace std;
    ostringstream msg;
    msg << "AbaqusInterfaceExceptions::throwUnMatchedNumberOfStateVariables : "
	<< "the number of internal state variables does not match. The behaviour '" 
	<< b  << "' requires " << n1 << " state variables, and "
	<< n2 << " state variables were declared";
    throw(AbaqusException(msg.str()));
  } // end of throwUnMatchedNumberOfStateVariables

  void
  AbaqusInterfaceExceptions::treatAbaqusException(const std::string& b,
						const AbaqusException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an AbaqusException : "
	 << e.what() << endl;
  } // end of AbaqusInterfaceExceptions::treatAbaqusException

  void
  AbaqusInterfaceExceptions::treatMaterialException(const std::string& b,
						   const tfel::material::MaterialException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an MaterialException : "
	 << e.what() << endl;
  } // end of treatMaterialException
    
  void
  AbaqusInterfaceExceptions::treatTFELException(const std::string& b,
					       const tfel::exception::TFELException& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic tfel exception : "
	 << e.what() << endl;
  } // end of treatTFELException

  void
  AbaqusInterfaceExceptions::treatStandardException(const std::string& b,
						   const std::exception& e)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown a generic standard exception : "
	 << e.what() << endl;
  } // end of treatStandardException

  void
  AbaqusInterfaceExceptions::treatUnknownException(const std::string& b)
  {
    using namespace std;
    cout << "The behaviour '" << b 
	 << "' has thrown an unknown exception" << endl;
  } // end of treatUnknownException
  
  void
  AbaqusInterfaceExceptions::throwNegativeTimeStepException(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwNegativeTimeStepException : ");
    msg += "negative time step detected for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of AbaqusInterfaceExceptions::throwNegativeTimeStepException
  
  void
  AbaqusInterfaceExceptions::throwInvalidDDSOEException(const std::string& b,
						       const AbaqusReal v)
  {
    using namespace std;
    ostringstream msg;
    msg << "AbaqusInterfaceExceptions::throwInvalidDDSOEException : "
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
  } // end of AbaqusInterfaceExceptions::throwInvalidDDSOEException

  void
  AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException : ");
    msg += "integration failed for behaviour '"+b+"'";
    throw(runtime_error(msg));
  } // end of AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException
  
  void
  AbaqusInterfaceExceptions::throwPredictionComputationFailedException(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwPredictionComputationFailedException : ");
    msg += "prediction computation failed for behaviour '"+b+"'";
    throw(runtime_error(msg));
  }

  void
  AbaqusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwPredictionOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a prediction operator";
    throw(runtime_error(msg));
  } // end of AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  AbaqusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwConsistentTangentOperatorIsNotAvalaible : ");
    msg += "behaviour '"+b+"' can't compute a consistent tangent operator";
    throw(runtime_error(msg));
  } // end of AbaqusInterfaceExceptions::throwBehaviourIntegrationFailedException

  void
  AbaqusInterfaceExceptions::throwUnsupportedStressFreeExpansionException(const std::string& b)
  {
    using namespace std;
    string msg("AbaqusInterfaceExceptions::throwUnsupportedStressFreeExpansionException : ");
    msg += "behaviour '"+b+"' can handle stress-free expansion but the Abaqus interface can't";
    throw(runtime_error(msg));
  } // end of AbaqusInterfaceExceptions::throwUnsupportedStressFreeExpansionException

  void
  AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage()
  {
    using namespace std;
    cout << "AbaqusInterfaceExceptions::displayUnsupportedHypothesisMessage : "
	 << "unsupported hypothesis" << endl;
  }


  void
  AbaqusReduceTangentOperator<1u>::exe(AbaqusReal * const DDSOE,
				      const AbaqusReal* const K)
  {
    DDSOE[0] = K[0];
    DDSOE[1] = K[1];
    DDSOE[2] = K[2];
    DDSOE[3] = K[6];
    DDSOE[4] = K[7];
    DDSOE[5] = K[8];
    DDSOE[6] = K[12];
    DDSOE[7] = K[13];
    DDSOE[8] = K[14];
  } // end of AbaqusReduceTangentOperator<1u>::exe

  void
  AbaqusReduceTangentOperator<2u>::exe(AbaqusReal * const DDSOE,
				      const AbaqusReal* const K)
  {
    DDSOE[0] = K[0];
    DDSOE[1] = K[1];
    DDSOE[2] = K[2];
    DDSOE[3] = K[3];
    DDSOE[4] = K[6];
    DDSOE[5] = K[7];
    DDSOE[6] = K[8];
    DDSOE[7] = K[9];
    DDSOE[8]  = K[12];
    DDSOE[9]  = K[13];
    DDSOE[10] = K[14];
    DDSOE[11] = K[15];
    DDSOE[12] = K[18];
    DDSOE[13] = K[19];
    DDSOE[14] = K[20];
    DDSOE[15] = K[21];
  } // end of AbaqusReduceTangentOperator<1u>::exe

  void
  AbaqusUnSupportedCaseHandler::exe(const AbaqusReal *const,
				   const AbaqusReal *const,
				   AbaqusReal *const,
				   const AbaqusReal *const,
				   const AbaqusReal *const,
				   const AbaqusReal *const,
				   const AbaqusReal *const,
				   const AbaqusReal *const,
				   const AbaqusInt  *const,
				   const AbaqusReal *const,
				   const AbaqusReal *const,
				   AbaqusReal *const,
				   const AbaqusInt  *const,
				   AbaqusReal *const,
				   const StressFreeExpansionHandler&)
  {
    using namespace std;
    string msg("AbaqusUnSupportedCaseHandler::exe : "
	       "we fall in a case that the abaqus interface "
	       "is not able to handle.");
    throw(runtime_error(msg));  
  } // end of exe

} // end of namespace abaqus 
