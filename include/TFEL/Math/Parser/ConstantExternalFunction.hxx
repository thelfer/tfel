/*! 
 * \file  include/TFEL/Math/Parser/ConstantExternalFunction.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 18 mars 2013
 */

#ifndef _LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_H_
#define _LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_H_ 

#include<sstream>
#include<vector>
#include<stdexcept>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct TFELMATHPARSER_VISIBILITY_EXPORT ConstantExternalFunction
	: public ExternalFunction
      {
	ConstantExternalFunction(const double);
      	virtual double
	getValue(void) const;
	virtual void
	setVariableValue(const std::vector<double>::size_type,
			 const double);
	virtual std::vector<double>::size_type
	getNumberOfVariables(void) const;
	virtual void
	checkCyclicDependency(const std::string&) const;
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::vector<double>::size_type) const;
	virtual	tfel::utilities::shared_ptr<ExternalFunction>
	differentiate(const std::string&) const;
	virtual	tfel::utilities::shared_ptr<ExternalFunction>
	resolveDependencies(void) const;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(const std::vector<std::string>&) const;
	virtual tfel::utilities::shared_ptr<ExternalFunction>
	createFunctionByChangingParametersIntoVariables(std::vector<std::string>&,
							const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	virtual void
	getParametersNames(std::set<std::string>&) const;
	virtual ~ConstantExternalFunction();
      protected:
	const double value;
      }; // end of struct ConstantExternalFunction
      
    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_PARSER_CONSTANTEXTERNALFUNCTION_H */

