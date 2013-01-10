/*!
 * \file   ExternalFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _LIB_TFEL_MATH_PARSER_EXTERNALFUNCTION_HXX_
#define _LIB_TFEL_MATH_PARSER_EXTERNALFUNCTION_HXX_ 

#include<set>
#include<map>
#include<vector>
#include<string>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct TFELMATHPARSER_VISIBILITY_EXPORT ExternalFunction
      {
	virtual double
	getValue(void) const = 0;
	virtual void
	setVariableValue(const std::vector<double>::size_type,
			 const double) = 0;
	virtual std::vector<double>::size_type
	getNumberOfVariables(void) const = 0;
	virtual void
	checkCyclicDependency(const std::string&) const = 0;
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const = 0;
	virtual	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const = 0;
	virtual	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const = 0;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const = 0;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
							const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const = 0;
	virtual void
	getParametersNames(std::set<std::string>&) const = 0;
	virtual ~ExternalFunction();
      }; // end of struct ExternalFunction
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_EVALUATOR_HXX */
