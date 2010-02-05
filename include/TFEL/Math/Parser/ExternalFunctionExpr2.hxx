/*!
 * \file   ExternalFunctionExpr2.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR2_H_
#define _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR2_H_

#include<string>
#include<vector>
#include<stdexcept>

#include"TFEL/Utilities/SmartPtr.hxx"
#include"TFEL/Math/Parser/Expr.hxx"
#include"TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct ExternalFunctionExpr2
	: public Expr
      {
	ExternalFunctionExpr2(tfel::utilities::SmartPtr<ExternalFunction>,
			      std::vector<tfel::utilities::SmartPtr<Expr> >&);
	double
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::SmartPtr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
      private:
	mutable tfel::utilities::SmartPtr<ExternalFunction> f;
	std::vector<tfel::utilities::SmartPtr<Expr> > args;
      }; // end of struct ExternalFunctionExpr2

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif  /* _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR2_H_ */
