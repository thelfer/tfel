/*!
 * \file   ExternalFunctionExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   02 oct 2007
 */

#ifndef _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR_H_
#define _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR_H_

#include<string>
#include<vector>
#include<stdexcept>

#include"Utilities/SmartPtr.hxx"
#include"Math/Parser/Expr.hxx"
#include"Math/Parser/ExternalFunction.hxx"
#include"Math/Parser/ExternalFunctionManager.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct ExternalFunctionExpr
	: public Expr
      {
	ExternalFunctionExpr(const std::string&,
			     std::vector<tfel::utilities::SmartPtr<Expr> >&,
			     tfel::utilities::SmartPtr<ExternalFunctionManager>&);
	double
	getValue(void) const;
	void
	checkCyclicDependency(const std::vector<std::string>&) const
	  throw(std::runtime_error);
	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
      private:
	std::string name;
	std::vector<tfel::utilities::SmartPtr<Expr> > args;
	mutable tfel::utilities::SmartPtr<ExternalFunctionManager> manager;
      };

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif  /* _TFEL_LIB_MATH_PARSER_EXTERNALFUNCTIONEXPR_H_ */
