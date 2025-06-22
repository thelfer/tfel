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
      bool isOperator() const override;
      parser::ExprPtr analyse() override;
      void reduce() override;
      ~TNegation() noexcept override;

     private:
      std::shared_ptr<Evaluator::TExpr> expr;
    };  // end of struct Evaluator::TNegation

    struct Evaluator::TOperator final : public Evaluator::TExpr {
      TOperator(const std::string&);
      TOperator(TOperator&&) = default;
      TOperator(const TOperator&) = default;
      bool isOperator() const override;
      void reduce() override;
      parser::ExprPtr analyse() override;
      std::string getOperatorType() const;
      ~TOperator() noexcept override;

     private:
      const std::string type;
    };  // end of struct Evaluator::TOperator

    struct Evaluator::TBinaryOperation final : public Evaluator::TExpr {
      TBinaryOperation(std::shared_ptr<Evaluator::TExpr>,
                       const std::shared_ptr<TOperator>,
                       std::shared_ptr<Evaluator::TExpr>);
      bool isOperator() const override;
      void reduce() override;
      parser::ExprPtr analyse() override;
      ~TBinaryOperation() override;

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
      void reduce() override;
      parser::LogicalExprPtr analyse() override;
      ~TNegLogicalExpr() override;

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> e;
    };  // end of struct Evaluator::TNegLogicalExpr

    template <typename LogicalOperator>
    struct Evaluator::TLogicalOperation final : public Evaluator::TLogicalExpr {
      TLogicalOperation(std::shared_ptr<Evaluator::TExpr>,
                        std::shared_ptr<Evaluator::TExpr>);
      void reduce() override;
      parser::LogicalExprPtr analyse() override;
      ~TLogicalOperation() noexcept override;

     private:
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
    };

    template <typename LogicalOperator>
    struct Evaluator::TLogicalBinaryOperation final
        : public Evaluator::TLogicalExpr {
      TLogicalBinaryOperation(std::shared_ptr<Evaluator::TLogicalExpr>,
                              std::shared_ptr<Evaluator::TLogicalExpr>);
      void reduce() override;
      parser::LogicalExprPtr analyse() override;
      ~TLogicalBinaryOperation() noexcept override;

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> a;
      std::shared_ptr<Evaluator::TLogicalExpr> b;
    };  // end of struct Evaluator::TLogicalBinaryOperation

    struct Evaluator::TConditionalExpr final : public Evaluator::TExpr {
      TConditionalExpr(std::shared_ptr<Evaluator::TLogicalExpr>,
                       std::shared_ptr<Evaluator::TExpr>,
                       std::shared_ptr<Evaluator::TExpr>);
      bool isOperator() const override;
      void reduce() override;
      parser::ExprPtr analyse() override;
      ~TConditionalExpr() override;

     private:
      std::shared_ptr<Evaluator::TLogicalExpr> c;
      std::shared_ptr<Evaluator::TExpr> a;
      std::shared_ptr<Evaluator::TExpr> b;
    };  // end of struct Evaluator::TLogicalOperation

    struct Evaluator::TVariable final : public Evaluator::TExpr {
      TVariable(const std::string&, Evaluator&);
      TVariable(const std::vector<double>::size_type, std::vector<double>&);
      bool isOperator() const override;
      void reduce() override;
      parser::ExprPtr analyse() override;
      ~TVariable() override;

     private:
      std::vector<double>& vars;
      std::vector<double>::size_type pos;
    };

    struct Evaluator::TGroup final : public Evaluator::TExpr {
      void add(std::shared_ptr<Evaluator::TExpr> const);
      bool isOperator() const override;
      void reduce() override;
      parser::ExprPtr analyse() override;
      ~TGroup() override;

     private:
      void reduce(const std::string&);
      std::vector<std::shared_ptr<Evaluator::TExpr>> subExpr;
    };  // end of struct Evaluator::TGroup

    struct Evaluator::TFunction final : public Evaluator::TExpr {
      TFunction(Evaluator::FunctionGenerator,
                std::shared_ptr<Evaluator::TExpr>);
      bool isOperator() const override;
      parser::ExprPtr analyse() override;
      void reduce() override;
      ~TFunction() override;

     private:
      Evaluator::FunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr> arg;
    };  // end of struct Evaluator::TFunction

    struct Evaluator::TBinaryFunction final : public Evaluator::TExpr {
      TBinaryFunction(Evaluator::BinaryFunctionGenerator,
                      std::shared_ptr<Evaluator::TExpr>,
                      std::shared_ptr<Evaluator::TExpr>);
      bool isOperator() const override;
      parser::ExprPtr analyse() override;
      void reduce() override;
      ~TBinaryFunction() override;

     private:
      Evaluator::BinaryFunctionGenerator f;
      std::shared_ptr<Evaluator::TExpr> arg1;
      std::shared_ptr<Evaluator::TExpr> arg2;
    };  // end of struct Evaluator::TBinaryFunction

    struct Evaluator::TExternalOperator final : public Evaluator::TExpr {
      TExternalOperator(const Evaluator::ExternalFunctionGenerator,
                        const std::vector<std::string>&,
                        std::vector<std::shared_ptr<Evaluator::TExpr>>&);
      bool isOperator() const override;
      parser::ExprPtr analyse() override;
      void reduce() override;
      ~TExternalOperator() override;

     private:
      Evaluator::ExternalFunctionGenerator f;
      std::vector<std::string> param;
      std::vector<std::shared_ptr<Evaluator::TExpr>> args;
    };  // end of struct Evaluator::TExternalOperator

    struct Evaluator::TNumber final : public Evaluator::TExpr {
      TNumber(const std::string&, const double);
      virtual bool isOperator() const override;
      virtual parser::ExprPtr analyse() override;
      virtual void reduce() override;

     private:
      const std::string str;
      const double value;
    };  // end of struct Evaluator::TNumber

    struct Evaluator::TExternalFunctionExpr final : public Evaluator::TExpr {
      TExternalFunctionExpr(
          const std::string&,
          std::vector<std::shared_ptr<Evaluator::TExpr>>&,
          std::shared_ptr<tfel::math::parser::ExternalFunctionManager>&);
      bool isOperator() const override;
      parser::ExprPtr analyse() override;
      void reduce() override;
      ~TExternalFunctionExpr() noexcept override;

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
