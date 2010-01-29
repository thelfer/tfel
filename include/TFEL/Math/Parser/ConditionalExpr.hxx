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
	ConditionalExpr(const tfel::utilities::SmartPtr<LogicalExpr>,
			const tfel::utilities::SmartPtr<Expr>,
			const tfel::utilities::SmartPtr<Expr>);
	double
	getValue(void) const;
	void
	checkCyclicDependency(std::vector<std::string>&) const;
	tfel::utilities::SmartPtr<Expr>
	resolveDependencies(void) const;
 	tfel::utilities::SmartPtr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const;
	tfel::utilities::SmartPtr<Expr>
	clone(const std::vector<double>&) const;
	~ConditionalExpr();
      private:
	const tfel::utilities::SmartPtr<LogicalExpr> c;
	const tfel::utilities::SmartPtr<Expr> a;
	const tfel::utilities::SmartPtr<Expr> b;
      }; // end of struct BinaryOperation

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_CONDITIONALEXPR_HXX_ */
