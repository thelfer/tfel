#include<sstream>
#include<stdexcept>
#include"TFEL/Math/Parser/ExternalCastemFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      ExternalCastemFunction::ExternalCastemFunction(const tfel::system::CastemFunctionPtr f_,
						     const unsigned short n)
	: variables(n),
	  f(f_)
      {} // end of ExternalCastemFunction

      double
      ExternalCastemFunction::getValue(void) const
      {
	return (*f)(&(this->variables[0]));
      } // end of getValue(void) const

      void
      ExternalCastemFunction::setVariableValue(const std::vector<double>::size_type pos,
					       const double value)
      {
	using namespace std;
	if(pos>=this->variables.size()){
	  ostringstream msg;
	  msg << "ExternalCFunctionBase::setVariableValue : "
	      << "invalid index " << pos 
	      << " (function has only " << this->variables.size() << " variables).";
	  throw(runtime_error(msg.str()));
	}
	this->variables[pos] = value;
      } // end of ExternalCastemFunction::setVariableValue

      std::vector<double>::size_type
      ExternalCastemFunction::getNumberOfVariables(void) const
      {
	return this->variables.size();
      } // end of ExternalCastemFunction::getNumberOfVariables

      void
      ExternalCastemFunction::checkCyclicDependency(const std::string&) const
	  throw(std::runtime_error)
      {} // end of ExternalCastemFunction::checkCyclicDependency

      void
      ExternalCastemFunction::checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error)
      {} // end of ExternalCastemFunction::checkCyclicDependency
 
      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCastemFunction::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalCastemFunction(this->f,
								     static_cast<unsigned short>(this->variables.size())));
      } // end of ExternalCastemFunction::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCastemFunction::differentiate(const std::vector<double>::size_type) const
      {
	using namespace std;
	string msg("ExternalCastemFunction::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCastemFunction::differentiate

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalCastemFunction::differentiate(const std::string&) const
      {
	using namespace std;
	string msg("ExternalCastemFunction::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalCastemFunction::differentiate

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
