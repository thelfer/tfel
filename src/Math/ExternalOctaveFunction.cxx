#include<sstream>
#include<stdexcept>

#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>

#include"TFEL/Math/Parser/ExternalOctaveFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct ExternalOctaveFunction::OctaveInitializer
      {
	OctaveInitializer();
      }; // end of struct ExternalOctaveFunction::OctaveInitializer
      
      ExternalOctaveFunction::OctaveInitializer::OctaveInitializer()
      {
	using namespace std;
	string_vector argv (2);
	argv(0) = "embedded";
	argv(1) = "-q";
	octave_main(2,argv.c_str_vec(), 1);
      } // end of ExternalOctaveFunction::OctaveInitializer::OctaveInitializer()
      
      ExternalOctaveFunction::ExternalOctaveFunction(const std::string& f_,
						     const unsigned short n)
	: f(f_),
	  variables(n)
      {
	static OctaveInitializer init;
      } // end of ExternalOctaveFunction(FunctionPtr f_)

      double
      ExternalOctaveFunction::getValue(void) const
      {
	using namespace std;
	octave_value_list args(static_cast<octave_idx_type>(this->variables.size()),0.);
	octave_value_list res;
	vector<double>::const_iterator p;
	unsigned int i = 0;
	for(p=this->variables.begin();
	    p!=this->variables.end();++p,++i){
	  args(i)=*p;
	}
	res = feval(this->f,args, 1);
	if (error_state || res.length () != 1){
	  string msg("ExternalOctaveFunction::getValue : ");
	  msg += "error while calling function '"+this->f+"'";
	  throw(runtime_error(msg));
	}
	return res(0).scalar_value();
      } // end of getValue(void) const

      void
      ExternalOctaveFunction::setVariableValue(const std::vector<double>::size_type pos,
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
      } // end of ExternalOctaveFunction::setVariableValue

      std::vector<double>::size_type
      ExternalOctaveFunction::getNumberOfVariables(void) const
      {
	return this->variables.size();
      } // end of ExternalOctaveFunction::getNumberOfVariables

      void
      ExternalOctaveFunction::checkCyclicDependency(const std::string&) const
	  throw(std::runtime_error)
      {} // end of ExternalOctaveFunction::checkCyclicDependency

      void
      ExternalOctaveFunction::checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error)
      {} // end of ExternalOctaveFunction::checkCyclicDependency
 
      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalOctaveFunction::resolveDependencies(void) const
      {
	using namespace tfel::utilities;
	return SmartPtr<ExternalFunction>(new ExternalOctaveFunction(this->f,
								     static_cast<unsigned short>(this->variables.size())));
      } // end of ExternalOctaveFunction::resolveDependencies(void) const

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalOctaveFunction::differentiate(const std::vector<double>::size_type) const
      {
	using namespace std;
	string msg("ExternalOctaveFunction::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalOctaveFunction::differentiate

      tfel::utilities::SmartPtr<ExternalFunction>
      ExternalOctaveFunction::differentiate(const std::string&) const
      {
	using namespace std;
	string msg("ExternalOctaveFunction::differentiate : ");
	msg += "can't differentiate external function";
	throw(runtime_error(msg));
      } // end of ExternalOctaveFunction::differentiate

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel
