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
      virtual tfel::utilities::shared_ptr<tfel::math::parser::Expr>
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
      TNegation(tfel::utilities::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      void
      reduce(void);
      ~TNegation();
    private:
      tfel::utilities::shared_ptr<Evaluator::TExpr> expr;
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
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
    private:
      const std::string type;
    }; // end of struct Evaluator::TOperator

    struct Evaluator::TBinaryOperation
      : public Evaluator::TExpr
    {
      TBinaryOperation(tfel::utilities::shared_ptr<Evaluator::TExpr>,
		       const tfel::utilities::shared_ptr<TOperator>,
		       tfel::utilities::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      ~TBinaryOperation();
    private:
      tfel::utilities::shared_ptr<Evaluator::TExpr> a;
      const tfel::utilities::shared_ptr<TOperator>op;
      tfel::utilities::shared_ptr<Evaluator::TExpr> b;
    }; // end of struct Evaluator::TBinaryOperation

    struct Evaluator::TLogicalExpr
    {
      virtual void
      reduce(void) = 0;
      virtual
      tfel::utilities::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void) = 0;
      virtual ~TLogicalExpr();
    }; // end of struct Evaluator::TLogicalExpr 

    struct Evaluator::TNegLogicalExpr
      : public Evaluator::TLogicalExpr
    {
      TNegLogicalExpr(const tfel::utilities::shared_ptr<Evaluator::TLogicalExpr >);
      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
      virtual ~TNegLogicalExpr();
    private:
      tfel::utilities::shared_ptr<Evaluator::TLogicalExpr > e;
    }; // end of struct Evaluator::TNegLogicalExpr 

    template<typename LogicalOperator>
    struct Evaluator::TLogicalOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalOperation(tfel::utilities::shared_ptr<Evaluator::TExpr>,
			tfel::utilities::shared_ptr<Evaluator::TExpr>);
      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      tfel::utilities::shared_ptr<Evaluator::TExpr> a;
      tfel::utilities::shared_ptr<Evaluator::TExpr> b;
    };

    template<typename LogicalOperator>
    struct Evaluator::TLogicalBinaryOperation
      : public Evaluator::TLogicalExpr
    {
      TLogicalBinaryOperation(tfel::utilities::shared_ptr<Evaluator::TLogicalExpr>,
			      tfel::utilities::shared_ptr<Evaluator::TLogicalExpr>);
      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::LogicalExpr>
      analyse(void);
    private:
      tfel::utilities::shared_ptr<Evaluator::TLogicalExpr> a;
      tfel::utilities::shared_ptr<Evaluator::TLogicalExpr> b;
    }; // end of struct Evaluator::TLogicalBinaryOperation

    struct Evaluator::TConditionalExpr
      : public Evaluator::TExpr
    {
      TConditionalExpr(tfel::utilities::shared_ptr<Evaluator::TLogicalExpr>,
		       tfel::utilities::shared_ptr<Evaluator::TExpr>,
		       tfel::utilities::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      ~TConditionalExpr();
    private:
      tfel::utilities::shared_ptr<Evaluator::TLogicalExpr> c;
      tfel::utilities::shared_ptr<Evaluator::TExpr> a;
      tfel::utilities::shared_ptr<Evaluator::TExpr> b;
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
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
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
      add(tfel::utilities::shared_ptr<Evaluator::TExpr>const);      void
      reduce(void);
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      std::string
      getClassName(void) const;
      ~TGroup();
    private:
      void
      reduce(const std::string&);
      std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> > subExpr;
    }; // end of struct Evaluator::TGroup


    struct Evaluator::TFunction
      : public Evaluator::TExpr
    {
      TFunction(Evaluator::FunctionGenerator,
		tfel::utilities::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TFunction();
    private:
      Evaluator::FunctionGenerator f;
      tfel::utilities::shared_ptr<Evaluator::TExpr>arg;
    }; // end of struct Evaluator::TFunction

    struct Evaluator::TBinaryFunction
      : public Evaluator::TExpr
    {
      TBinaryFunction(Evaluator::BinaryFunctionGenerator,
		      tfel::utilities::shared_ptr<Evaluator::TExpr>,
		      tfel::utilities::shared_ptr<Evaluator::TExpr>);
      bool
      isOperator(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TBinaryFunction();
    private:
      Evaluator::BinaryFunctionGenerator f;
      tfel::utilities::shared_ptr<Evaluator::TExpr> arg1;
      tfel::utilities::shared_ptr<Evaluator::TExpr> arg2;
    }; // end of struct Evaluator::TBinaryFunction

    struct Evaluator::TExternalOperator
      : public Evaluator::TExpr
    {
      TExternalOperator(const Evaluator::ExternalFunctionGenerator,
			const std::vector<std::string>&,
			std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> >&);
      bool
      isOperator(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
      std::string
      getClassName(void) const;
      ~TExternalOperator();
    private:
      Evaluator::ExternalFunctionGenerator f;
      std::vector<std::string> param;
      std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> > args;
    }; // end of struct Evaluator::TExternalOperator

    struct Evaluator::TNumber
      : public Evaluator::TExpr
    {
      TNumber(const double v);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
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
			    std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> >&,
			    tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      std::string name;
      std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> > args;
      tfel::utilities::shared_ptr<tfel::math::parser::ExternalFunctionManager> manager;
    }; // end of struct Evaluator::TExternalFunctionExpr

    struct Evaluator::TDifferentiatedFunctionExpr
      : public Evaluator::TExpr
    {
      TDifferentiatedFunctionExpr(tfel::utilities::shared_ptr<ExternalFunction>,
				  std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> >&,
				  const std::vector<std::vector<double>::size_type>&);
      bool
      isOperator(void) const;
      std::string
      getClassName(void) const;
      tfel::utilities::shared_ptr<tfel::math::parser::Expr>
      analyse(void);
      void
      reduce(void);
    private:
      tfel::utilities::shared_ptr<ExternalFunction> f;
      std::vector<tfel::utilities::shared_ptr<Evaluator::TExpr> > args;
      std::vector<std::vector<double>::size_type> var;
    }; // end of struct Evaluator::TDifferentiatedFunctionExpr

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/EvaluatorTExpr.ixx"

#endif /* _LIB_TFEL_MATH_EVALUATORTEXPR_HXX_ */
