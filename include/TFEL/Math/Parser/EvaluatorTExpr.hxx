/*!
 * \file   include/TFEL/Math/Parser/EvaluatorTExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Config/TFELConfig.hxx"

#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/LogicalExpr.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/Negation.hxx"
#include "TFEL/Math/Parser/Variable.hxx"
#include "TFEL/Math/Parser/ExternalFunctionExpr.hxx"
#include "TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx"
#include "TFEL/Math/Parser/ConditionalExpr.hxx"

#ifndef LIB_TFEL_MATH_EVALUATORTEXPR_HXX
#define LIB_TFEL_MATH_EVALUATORTEXPR_HXX

namespace tfel {

  namespace math {

    struct Evaluator::TExpr {
      TExpr() = default;
      TExpr(TExpr&&) = default;
      TExpr(const TExpr&) = default;
      virtual parser::ExprPtr analyse() = 0;
      virtual void reduce() = 0;
      virtual bool isOperator() const = 0;
      virtual ~TExpr();
    };  // end of Evaluator::TExpr

    struct Evaluator::TNegation final : public Evaluator::TExpr {
      TNegation(std::shared_ptr<Evaluator::TExpr>);
      TNegation(TNegation&&) = default;
      TNegation(const TNegation&) = default;
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TNegation() noexcept;

     private:
      std::shared_ptr<Evaluator::TExpr> expr;
    };  // end of struct Evaluator::TNegation

    struct Evaluator::TOperator final : public Evaluator::TExpr {
      TOperator(const std::string&);
      TOperator(TOperator&&) = default;
      TOperator(const TOperator&) = default;
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::ExprPtr analyse() override;
      std::string getOperatorType() const;
      virtual ~TOperator() noexcept;

     private:
      const std::string type;
    };  // end of struct Evaluator::TOperator

    struct Evaluator::TBinaryOperation final : public Evaluator::TExpr {
      TBinaryOperation(std::shared_ptr<Evaluator::TExpr>,
                       const std::shared_ptr<TOperator>,
                       std::shared_ptr<Evaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::ExprPtr analyse() override;
      virtual ~TBinaryOperation();

     private:
      std::shared_ptr<Evaluator::TExpr> a;
      const std::shared_ptr<TOperator> op;
      std::shared_ptr<Evaluator::TExpr> b;
    };  // end of struct Evaluator::TBinaryOperation

    struct Evaluator::TLogicalExpr {
      virtual void reduce() = 0;
      virtual parser::LogicalExprPtr analyse() = 0;
      virtual ~TLogicalExpr();
    };  // end of struct Evaluator::TLogicalExpr

    struct Evaluator::TNegLogicalExpr final : public Evaluator::TLogicalExpr {
      TNegLogicalExpr(const std::shared_ptr<Evaluator::TLogicalExpr>);
      virtual void reduce() override;
      virtual parser::LogicalExprPtr analyse() override;
      virtual ~TNegLogicalExpr();

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> e;
    };  // end of struct Evaluator::TNegLogicalExpr

    template <typename LogicalOperator>
    struct Evaluator::TLogicalOperation final : public Evaluator::TLogicalExpr {
      TLogicalOperation(std::shared_ptr<Evaluator::TExpr>,
                        std::shared_ptr<Evaluator::TExpr>);
      virtual void reduce() override;
      virtual parser::LogicalExprPtr analyse() override;
      virtual ~TLogicalOperation() noexcept;

     private:
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
    };

    template <typename LogicalOperator>
    struct Evaluator::TLogicalBinaryOperation final
        : public Evaluator::TLogicalExpr {
      TLogicalBinaryOperation(std::shared_ptr<Evaluator::TLogicalExpr>,
                              std::shared_ptr<Evaluator::TLogicalExpr>);
      virtual void reduce() override;
      virtual parser::LogicalExprPtr analyse() override;
      virtual ~TLogicalBinaryOperation() noexcept;

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> a;
      std::shared_ptr<Evaluator::TLogicalExpr> b;
    };  // end of struct Evaluator::TLogicalBinaryOperation

    struct Evaluator::TConditionalExpr final : public Evaluator::TExpr {
      TConditionalExpr(std::shared_ptr<Evaluator::TLogicalExpr>,
                       std::shared_ptr<Evaluator::TExpr>,
                       std::shared_ptr<Evaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::ExprPtr analyse() override;
      virtual ~TConditionalExpr();

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> c;
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
    };  // end of struct Evaluator::TLogicalOperation

    struct Evaluator::TVariable final : public Evaluator::TExpr {
      TVariable(const std::string&, Evaluator&);
      TVariable(const std::vector<double>::size_type, std::vector<double>&);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::ExprPtr analyse() override;

     private:
      std::vector<double>& vars;
      std::vector<double>::size_type pos;
    };

    struct Evaluator::TGroup final : public Evaluator::TExpr {
      void add(std::shared_ptr<Evaluator::TExpr> const);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::ExprPtr analyse() override;
      virtual ~TGroup();

     private:
      void reduce(const std::string&);
      std::vector<std::shared_ptr<Evaluator::TExpr>> subExpr;
    };  // end of struct Evaluator::TGroup

    struct Evaluator::TFunction final : public Evaluator::TExpr {
      TFunction(Evaluator::FunctionGenerator,
                std::shared_ptr<Evaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TFunction();

     private:
      Evaluator::FunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr> arg;
    };  // end of struct Evaluator::TFunction

    struct Evaluator::TBinaryFunction final : public Evaluator::TExpr {
      TBinaryFunction(Evaluator::BinaryFunctionGenerator,
                      std::shared_ptr<Evaluator::TExpr>,
                      std::shared_ptr<Evaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TBinaryFunction();

     private:
      Evaluator::BinaryFunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr> arg1;
      std::shared_ptr<Evaluator::TExpr> arg2;
    };  // end of struct Evaluator::TBinaryFunction

    struct Evaluator::TExternalOperator final : public Evaluator::TExpr {
      TExternalOperator(const Evaluator::ExternalFunctionGenerator,
                        const std::vector<std::string>&,
                        std::vector<std::shared_ptr<Evaluator::TExpr>>&);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TExternalOperator();

     private:
      Evaluator::ExternalFunctionGenerator f;
      std::vector<std::string> param;
      std::vector<std::shared_ptr<Evaluator::TExpr>> args;
    };  // end of struct Evaluator::TExternalOperator

    struct Evaluator::TNumber final : public Evaluator::TExpr {
      TNumber(const double v);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;

     private:
      const double value;
    };  // end of struct Evaluator::TNumber

    struct Evaluator::TExternalFunctionExpr final : public Evaluator::TExpr {
      TExternalFunctionExpr(
          const std::string&,
          std::vector<std::shared_ptr<Evaluator::TExpr>>&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TExternalFunctionExpr() noexcept;

     private:
      std::string name;
      std::vector<std::shared_ptr<Evaluator::TExpr>> args;
      std::shared_ptr<tfel::math::parser::ExternalFunctionManager> manager;
    };  // end of struct Evaluator::TExternalFunctionExpr

    struct Evaluator::TDifferentiatedFunctionExpr final
        : public Evaluator::TExpr {
      TDifferentiatedFunctionExpr(
          std::shared_ptr<ExternalFunction>,
          std::vector<std::shared_ptr<Evaluator::TExpr>>&,
          const std::vector<std::vector<double>::size_type>&);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TDifferentiatedFunctionExpr() noexcept override;

     private:
      std::shared_ptr<ExternalFunction> f;
      std::vector<std::shared_ptr<Evaluator::TExpr>> args;
      std::vector<std::vector<double>::size_type> var;
    };  // end of struct Evaluator::TDifferentiatedFunctionExpr

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Parser/EvaluatorTExpr.ixx"

#endif /* LIB_TFEL_MATH_EVALUATORTEXPR_HXX */
