/*!
 * \file   ConditionalExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#ifndef _LIB_TFEL_CONDITIONALEXPR_HXX_
#define _LIB_TFEL_CONDITIONALEXPR_HXX_ 

#include<vector>
#include<string>

#include"TFEL/Math/Parser/Expr.hxx"
#include"TFEL/Math/Parser/LogicalExpr.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

      struct ConditionalExpr
	: public Expr
      {
	ConditionalExpr(const tfel::utilities::shared_ptr<LogicalExpr>,
			const tfel::utilities::shared_ptr<Expr>,
			const tfel::utilities::shared_ptr<Expr>);
	double
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const;
 	tfel::utilities::shared_ptr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::shared_ptr<Expr>
	clone(const std::vector<double>&) const;
	void
	getParametersNames(std::set<std::string>&) const;
	tfel::utilities::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const;
	~ConditionalExpr();
      private:
	const tfel::utilities::shared_ptr<LogicalExpr> c;
	const tfel::utilities::shared_ptr<Expr> a;
	const tfel::utilities::shared_ptr<Expr> b;
      }; // end of struct BinaryOperation

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_CONDITIONALEXPR_HXX_ */
