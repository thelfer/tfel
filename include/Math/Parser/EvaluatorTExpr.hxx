/*!
 * \file   EvaluatorTExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 */

#include"Math/Parser/Expr.hxx"
#include"Math/Parser/LogicalExpr.hxx"
#include"Math/Parser/Number.hxx"
#include"Math/Parser/BinaryOperator.hxx"
#include"Math/Parser/Negation.hxx"
#include"Math/Parser/Variable.hxx"
#include"Math/Parser/ExternalFunctionExpr.hxx"
#include"Math/Parser/DifferentiatedFunctionExpr.hxx"
#include"Math/Parser/ConditionalExpr.hxx"

#ifndef _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_
#define _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_ 

namespace tfel
{

  namespace math
  {
    
    struct Evaluator::TExpr
    {
      virtual tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void) = 0;
      virtual
      void reduce(void) = 0;
      virtual bool
      isOperator(void) const =0;
      virtual std::string
      getClassName() const = 0;
      virtual ~TExpr(){};
    }; // end of Evaluator::TExpr
  
    struct Evaluator::TNegation
      : public Evaluator::TExpr
    {
      TNegation(tfel::utilities::SmartPtr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      void
      reduce(void);
      ~TNegation();
    private:
      tfel::utilities::SmartPtr<Evaluator::TExpr> expr;
    }; // end of struct Evaluator::TNegation

    struct Evaluator::TOperator
      : public Evaluator::TExpr
    {
      TOperator(const std::string&);
      std::string
      getOperatorType(void) const;
      bool
      isOperator(void) const;
      void
      reduce(void);
      std::string
      getClassName(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
    private:
      const std::string type;
    }; // end of struct Evaluator::TOperator

    struct Evaluator::TBinaryOperation
      : public Evaluator::TExpr
    {
      TBinaryOperation(tfel::utilities::SmartPtr<Evaluator::TExpr>,
		       const tfel::utilities::SmartPtr<TOperator>,
		       tfel::utilities::SmartPtr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      ~TBinaryOperation();
    private:
      tfel::utilities::SmartPtr<Evaluator::TExpr> a;
      const tfel::utilities::SmartPtr<TOperator>op;
      tfel::utilities::SmartPtr<Evaluator::TExpr> b;
    }; // end of struct Evaluator::TBinaryOperation

    struct Evaluator::TLogicalExpr
    {
      virtual void
      reduce(void) = 0;
      virtual
      tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
      analyse(void) = 0;
      virtual ~TLogicalExpr();
    }; // end of struct Evaluator::TLogicalExpr 

    struct Evaluator::TNegLogicalExpr
      : public Evaluator::TLogicalExpr
    {
      TNegLogicalExpr(const tfel::utilities::SmartPtr<Evaluator::TLogicalExpr >);
      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
      analyse(void);
      virtual ~TNegLogicalExpr();
    private:
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr > e;
    }; // end of struct Evaluator::TNegLogicalExpr 

    template<typename LogicalOperator>
    struct Evaluator::TLogicalOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalOperation(tfel::utilities::SmartPtr<Evaluator::TExpr>,
			tfel::utilities::SmartPtr<Evaluator::TExpr>);
      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      tfel::utilities::SmartPtr<Evaluator::TExpr> a;
      tfel::utilities::SmartPtr<Evaluator::TExpr> b;
    };

    template<typename LogicalOperator>
    struct Evaluator::TLogicalBinaryOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalBinaryOperation(tfel::utilities::SmartPtr<Evaluator::TLogicalExpr>,
			      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr>);
      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> a;
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> b;
    }; // end of struct Evaluator::TLogicalBinaryOperation

    struct Evaluator::TConditionalExpr
      : public Evaluator::TExpr
    {
      TConditionalExpr(tfel::utilities::SmartPtr<Evaluator::TLogicalExpr>,
		       tfel::utilities::SmartPtr<Evaluator::TExpr>,
		       tfel::utilities::SmartPtr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      ~TConditionalExpr();
    private:
      tfel::utilities::SmartPtr<Evaluator::TLogicalExpr> c;
      tfel::utilities::SmartPtr<Evaluator::TExpr> a;
      tfel::utilities::SmartPtr<Evaluator::TExpr> b;
    }; // end of struct Evaluator::TLogicalOperation

    struct Evaluator::TVariable
      : public Evaluator::TExpr
    {
      TVariable(const std::string&,
		Evaluator &);
      TVariable(const std::vector<double>::size_type,
		std::vector<double>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
    private:
      std::vector<double>& vars;
      std::vector<double>::size_type pos;
    };

    struct Evaluator::TGroup
      : public Evaluator::TExpr
    {
      bool
      isOperator(void) const;
      void
      add(tfel::utilities::SmartPtr<Evaluator::TExpr>const);      void
      reduce(void);
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      ~TGroup();
    private:
      void
      reduce(const std::string&);
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> > subExpr;
    }; // end of struct Evaluator::TGroup


    struct Evaluator::TFunction
      : public Evaluator::TExpr
    {
      TFunction(Evaluator::FunctionGenerator,
		tfel::utilities::SmartPtr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TFunction();
    private:
      Evaluator::FunctionGenerator f;
      tfel::utilities::SmartPtr<Evaluator::TExpr>arg;
    }; // end of struct Evaluator::TFunction

    struct Evaluator::TBinaryFunction
      : public Evaluator::TExpr
    {
      TBinaryFunction(Evaluator::BinaryFunctionGenerator,
		      tfel::utilities::SmartPtr<Evaluator::TExpr>,
		      tfel::utilities::SmartPtr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TBinaryFunction();
    private:
      Evaluator::BinaryFunctionGenerator f;
      tfel::utilities::SmartPtr<Evaluator::TExpr> arg1;
      tfel::utilities::SmartPtr<Evaluator::TExpr> arg2;
    }; // end of struct Evaluator::TBinaryFunction

    struct Evaluator::TExternalOperator
      : public Evaluator::TExpr
    {
      TExternalOperator(const Evaluator::ExternalFunctionGenerator,
			const std::vector<std::string>&,
			std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >&);
      bool
      isOperator(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TExternalOperator();
    private:
      Evaluator::ExternalFunctionGenerator f;
      std::vector<std::string> param;
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> > args;
    }; // end of struct Evaluator::TExternalOperator

    struct Evaluator::TNumber
      : public Evaluator::TExpr
    {
      TNumber(const double v);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      const double value;
    }; // end of struct Evaluator::TNumber

    struct Evaluator::TExternalFunctionExpr
      : public Evaluator::TExpr
    {
      TExternalFunctionExpr(const std::string&,
			    std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >&,
			    tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      std::string name;
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> > args;
      tfel::utilities::SmartPtr<tfel::math::parser::ExternalFunctionManager> manager;
    }; // end of struct Evaluator::TExternalFunctionExpr

    struct Evaluator::TDifferentiatedFunctionExpr
      : public Evaluator::TExpr
    {
      TDifferentiatedFunctionExpr(tfel::utilities::SmartPtr<ExternalFunction>,
				  std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> >&,
				  const std::vector<std::vector<double>::size_type>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::SmartPtr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      tfel::utilities::SmartPtr<ExternalFunction> f;
      std::vector<tfel::utilities::SmartPtr<Evaluator::TExpr> > args;
      std::vector<std::vector<double>::size_type> var;
    }; // end of struct Evaluator::TDifferentiatedFunctionExpr

  } // end of namespace math

} // end of namespace tfel

#include"Math/Parser/EvaluatorTExpr.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_ */
