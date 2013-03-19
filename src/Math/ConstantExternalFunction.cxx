/*! 
 * \file  ConstantExternalFunction.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mars 2013
 */

#include<sstream>
#include<stdexcept>

#include"TFEL/Math/Parser/ConstantExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {
    
      ConstantExternalFunction::ConstantExternalFunction(const double v)
	: value(v)
      {} // end of ConstantExternalFunction::ConstantExternalFunction

      double
      ConstantExternalFunction::getValue(void) const
      {
	return this->value;
      } // end of ConstantExternalFunction::getValue
      
      void
      ConstantExternalFunction::setVariableValue(const std::vector<double>::size_type pos,
						 const double)
      {
	using namespace std;
	ostringstream msg;
	msg << "ConstantExternalFunction::setVariableValue : "
	    << "invalid index " << pos 
	    << " (function has no  variable).";
	throw(runtime_error(msg.str()));
      }

      std::vector<double>::size_type
      ConstantExternalFunction::getNumberOfVariables(void) const
      {
	return 0u;
      }

      void
      ConstantExternalFunction::checkCyclicDependency(const std::string&) const
      {}

      void
      ConstantExternalFunction::checkCyclicDependency(std::vector<std::string>&) const
      {}

      tfel::utilities::shared_ptr<ExternalFunction>
      ConstantExternalFunction::differentiate(const std::vector<double>::size_type) const
      {
	using namespace tfel::utilities;
	return shared_ptr<ExternalFunction>(new ConstantExternalFunction(0.));
      } // end of ConstantExternalFunction:: differentiate

      tfel::utilities::shared_ptr<ExternalFunction>
      ConstantExternalFunction::differentiate(const std::string&) const
      {
	using namespace tfel::utilities;
	return shared_ptr<ExternalFunction>(new ConstantExternalFunction(0.));
      } // end of ConstantExternalFunction:: differentiate

      tfel::utilities::shared_ptr<ExternalFunction>
      ConstantExternalFunction::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return shared_ptr<ExternalFunction>(new ConstantExternalFunction(this->value));
      }

      tfel::utilities::shared_ptr<ExternalFunction>
      ConstantExternalFunction::createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const
      {
	using namespace std;
	using namespace tfel::utilities;
	string msg("ConstantExternalFunction::createFunctionByChangingParametersIntoVariables : ");
	msg += "invalid call";
	throw(runtime_error(msg));
	return shared_ptr<ExternalFunction>(static_cast<ExternalFunction*>(0));
      } // end of ConstantExternalFunction::createFunctionByChangingParametersIntoVariables

      
      tfel::utilities::shared_ptr<ExternalFunction>
      ConstantExternalFunction::createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
										const std::vector<double>&,
										const std::vector<std::string>&,
										const std::map<std::string,
											       std::vector<double>::size_type>&) const
      {
	return this->resolveDependencies();
      } // end of ConstantExternalFunction::createFunctionByChangingParametersIntoVariables

      void
      ConstantExternalFunction::getParametersNames(std::set<std::string>&) const
      {} // endo of ConstantExternalFunction::getParametersNames

      ConstantExternalFunction::~ConstantExternalFunction()
      {} // end of ConstantExternalFunction::~ConstantExternalFunction()
      
    } // end of namespace parser

  } // end of namespace math
  
} // end of namespace tfel

