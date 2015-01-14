/*!
 * \file   include/TFEL/Math/Parser/EvaluatorTExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/Parser/Expr.hxx"
#include"TFEL/Math/Parser/LogicalExpr.hxx"
#include"TFEL/Math/Parser/Number.hxx"
#include"TFEL/Math/Parser/BinaryOperator.hxx"
#include"TFEL/Math/Parser/Negation.hxx"
#include"TFEL/Math/Parser/Variable.hxx"
#include"TFEL/Math/Parser/ExternalFunctionExpr.hxx"
#include"TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx"
#include"TFEL/Math/Parser/ConditionalExpr.hxx"

#ifndef _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_
#define _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_ 

namespace tfel
{

  namespace math
  {
    
    struct Evaluator::TExpr
    {
      virtual std::shared_ptr<tfel::math::parser::Expr>
      analyse(void) = 0;
      virtual
      void reduce(void) = 0;
      virtual bool
      isOperator(void) const =0;
      virtual std::string
      getClassName() const = 0;
      virtual ~TExpr();
    }; // end of Evaluator::TExpr
  
    struct Evaluator::TNegation
      : public Evaluator::TExpr
    {
      TNegation(std::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      void
      reduce(void);
      virtual ~TNegation();
    private:
      std::shared_ptr<Evaluator::TExpr> expr;
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
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
    private:
      const std::string type;
    }; // end of struct Evaluator::TOperator

    struct Evaluator::TBinaryOperation
      : public Evaluator::TExpr
    {
      TBinaryOperation(std::shared_ptr<Evaluator::TExpr>,
		       const std::shared_ptr<TOperator>,
		       std::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      virtual ~TBinaryOperation();
    private:
      std::shared_ptr<Evaluator::TExpr> a;
      const std::shared_ptr<TOperator>op;
      std::shared_ptr<Evaluator::TExpr> b;
    }; // end of struct Evaluator::TBinaryOperation

    struct Evaluator::TLogicalExpr
    {
      virtual void
      reduce(void) = 0;
      virtual
      std::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void) = 0;
      virtual ~TLogicalExpr();
    }; // end of struct Evaluator::TLogicalExpr 

    struct Evaluator::TNegLogicalExpr
      : public Evaluator::TLogicalExpr
    {
      TNegLogicalExpr(const std::shared_ptr<Evaluator::TLogicalExpr >);
      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
      virtual ~TNegLogicalExpr();
    private:
      std::shared_ptr<Evaluator::TLogicalExpr > e;
    }; // end of struct Evaluator::TNegLogicalExpr 

    template<typename LogicalOperator>
    struct Evaluator::TLogicalOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalOperation(std::shared_ptr<Evaluator::TExpr>,
			std::shared_ptr<Evaluator::TExpr>);
      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
    };

    template<typename LogicalOperator>
    struct Evaluator::TLogicalBinaryOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalBinaryOperation(std::shared_ptr<Evaluator::TLogicalExpr>,
			      std::shared_ptr<Evaluator::TLogicalExpr>);
      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      std::shared_ptr<Evaluator::TLogicalExpr> a;
      std::shared_ptr<Evaluator::TLogicalExpr> b;
    }; // end of struct Evaluator::TLogicalBinaryOperation

    struct Evaluator::TConditionalExpr
      : public Evaluator::TExpr
    {
      TConditionalExpr(std::shared_ptr<Evaluator::TLogicalExpr>,
		       std::shared_ptr<Evaluator::TExpr>,
		       std::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      virtual ~TConditionalExpr();
    private:
      std::shared_ptr<Evaluator::TLogicalExpr> c;
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
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
      std::shared_ptr<tfel::math::parser::Expr>
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
      add(std::shared_ptr<Evaluator::TExpr>const);      void
      reduce(void);
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      virtual ~TGroup();
    private:
      void
      reduce(const std::string&);
      std::vector<std::shared_ptr<Evaluator::TExpr> > subExpr;
    }; // end of struct Evaluator::TGroup


    struct Evaluator::TFunction
      : public Evaluator::TExpr
    {
      TFunction(Evaluator::FunctionGenerator,
		std::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      virtual ~TFunction();
    private:
      Evaluator::FunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr>arg;
    }; // end of struct Evaluator::TFunction

    struct Evaluator::TBinaryFunction
      : public Evaluator::TExpr
    {
      TBinaryFunction(Evaluator::BinaryFunctionGenerator,
		      std::shared_ptr<Evaluator::TExpr>,
		      std::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      virtual ~TBinaryFunction();
    private:
      Evaluator::BinaryFunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr> arg1;
      std::shared_ptr<Evaluator::TExpr> arg2;
    }; // end of struct Evaluator::TBinaryFunction

    struct Evaluator::TExternalOperator
      : public Evaluator::TExpr
    {
      TExternalOperator(const Evaluator::ExternalFunctionGenerator,
			const std::vector<std::string>&,
			std::vector<std::shared_ptr<Evaluator::TExpr> >&);
      bool
      isOperator(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      virtual ~TExternalOperator();
    private:
      Evaluator::ExternalFunctionGenerator f;
      std::vector<std::string> param;
      std::vector<std::shared_ptr<Evaluator::TExpr> > args;
    }; // end of struct Evaluator::TExternalOperator

    struct Evaluator::TNumber
      : public Evaluator::TExpr
    {
      TNumber(const double v);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
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
			    std::vector<std::shared_ptr<Evaluator::TExpr> >&,
			    std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      std::string name;
      std::vector<std::shared_ptr<Evaluator::TExpr> > args;
      std::shared_ptr<tfel::math::parser::ExternalFunctionManager> manager;
    }; // end of struct Evaluator::TExternalFunctionExpr

    struct Evaluator::TDifferentiatedFunctionExpr
      : public Evaluator::TExpr
    {
      TDifferentiatedFunctionExpr(std::shared_ptr<ExternalFunction>,
				  std::vector<std::shared_ptr<Evaluator::TExpr> >&,
				  const std::vector<std::vector<double>::size_type>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      std::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      std::shared_ptr<ExternalFunction> f;
      std::vector<std::shared_ptr<Evaluator::TExpr> > args;
      std::vector<std::vector<double>::size_type> var;
    }; // end of struct Evaluator::TDifferentiatedFunctionExpr

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/EvaluatorTExpr.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_ */
