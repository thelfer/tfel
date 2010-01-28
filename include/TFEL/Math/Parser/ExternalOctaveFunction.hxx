/*!
 * \file   ExternalOctaveFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALOCTAVEFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALOCTAVEFUNCTION_HXX_ 

#include<sstream>
#include<vector>
#include<stdexcept>

#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct ExternalOctaveFunction
	: public ExternalFunction
      {
	ExternalOctaveFunction(const std::string&,
			       const unsigned short);
	void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	std::vector<double>::size_type
	getNumberOfVariables(void) const;
	double getValue(void) const;
	void
	checkCyclicDependency(const std::string&) const
	  throw(std::runtime_error);
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	differentiate(const std::string&) const;
	tfel::utilities::SmartPtr<ExternalFunction>
	resolveDependencies(void) const;
      private:
	struct OctaveInitializer;
	std::string f;
	std::vector<double> variables;
      };// end of struct ExternalOctaveFunction
 
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EXTERNALOCTAVEFUNCTION_HXX_ */
