/*!
 * \file   include/TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   13/01/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Config/TFELConfig.hxx"

#ifndef LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX
#define LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX

namespace tfel::math {

  struct IntegerEvaluator::TExpr {
    TExpr() = default;
    TExpr(const TExpr&) = default;
    TExpr(TExpr&&) = default;
    TExpr& operator=(TExpr&&) = default;
    TExpr& operator=(const TExpr&) = default;
    [[nodiscard]] virtual parser::IntegerExprPtr analyse() = 0;
    virtual void reduce() = 0;
    [[nodiscard]] virtual bool isOperator() const = 0;
    virtual ~TExpr();
  };  // end of IntegerEvaluator::TExpr

  struct IntegerEvaluator::TNegation final : public IntegerEvaluator::TExpr {
    //! expression returned
    struct Negation final : public tfel::math::parser::IntegerExpr {
      explicit Negation(const parser::IntegerExprPtr);
      //
      Negation& operator=(const Negation&) = delete;
      Negation& operator=(Negation&&) = delete;
      //
      [[nodiscard]] int getValue() const override;
      [[nodiscard]] parser::IntegerExprPtr clone(
          const std::vector<int>&) const override;
      ~Negation() override;

     private:
      const parser::IntegerExprPtr expr;
    };
    explicit TNegation(std::shared_ptr<IntegerEvaluator::TExpr>);
    [[nodiscard]] bool isOperator() const override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;
    void reduce() override;
    ~TNegation() override;

   private:
    std::shared_ptr<IntegerEvaluator::TExpr> expr;
  };  // end of struct IntegerEvaluator::TNegation

  struct IntegerEvaluator::TBinaryOperation final
      : public IntegerEvaluator::TExpr {
    struct IntegerOpPlus {
      TFEL_VISIBILITY_LOCAL static int apply(const int, const int);
    };  // end of struct IntegerOpPlus
    struct IntegerOpMinus {
      TFEL_VISIBILITY_LOCAL static int apply(const int, const int);
    };  // end of struct IntegerOpMinus
    struct IntegerOpMult {
      TFEL_VISIBILITY_LOCAL static int apply(const int, const int);
    };  // end of struct IntegerOpMult
    struct IntegerOpDiv {
      TFEL_VISIBILITY_LOCAL static int apply(const int, const int);
    };  // end of struct IntegerOpDiv
    /*!
     * Expression generated
     */
    template <typename Op>
    struct TFEL_VISIBILITY_LOCAL BinaryOperation final
        : public tfel::math::parser::IntegerExpr {
      BinaryOperation(const parser::IntegerExprPtr,
                      const parser::IntegerExprPtr) noexcept;
      //
      BinaryOperation& operator=(const BinaryOperation&) = delete;
      BinaryOperation& operator=(BinaryOperation&&) = delete;
      //
      [[nodiscard]] int getValue() const override;
      [[nodiscard]] parser::IntegerExprPtr clone(
          const std::vector<int>&) const override;
      ~BinaryOperation() override;

     private:
      const parser::IntegerExprPtr a;
      const parser::IntegerExprPtr b;
    };  // end of struct BinaryOperation
    /*!
     * Constructor
     */
    TBinaryOperation(std::shared_ptr<IntegerEvaluator::TExpr>,
                     const std::shared_ptr<TOperator>,
                     std::shared_ptr<IntegerEvaluator::TExpr>);
    //
    TBinaryOperation& operator=(const TBinaryOperation&) = delete;
    TBinaryOperation& operator=(TBinaryOperation&&) = delete;
    //
    [[nodiscard]] bool isOperator() const override;
    void reduce() override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;
    ~TBinaryOperation() override;

   private:
    std::shared_ptr<IntegerEvaluator::TExpr> a;
    const std::shared_ptr<TOperator> op;
    std::shared_ptr<IntegerEvaluator::TExpr> b;
  };  // end of struct IntegerEvaluator::TBinaryOperation

  struct IntegerEvaluator::TVariable final : public IntegerEvaluator::TExpr {
    struct Variable final : public tfel::math::parser::IntegerExpr {
      Variable(const std::vector<int>&, const std::vector<int>::size_type);
      //
      Variable& operator=(const Variable&) = delete;
      Variable& operator=(Variable&&) = delete;
      //
      [[nodiscard]] int getValue() const override;
      [[nodiscard]] parser::IntegerExprPtr clone(
          const std::vector<int>&) const override;

     private:
      const std::vector<int>& v;
      const std::vector<int>::size_type pos;
    };  // end of struct Variable
    TVariable(const std::string&, IntegerEvaluator&);
    TVariable(const std::vector<int>::size_type, std::vector<int>&);
    //
    TVariable& operator=(const TVariable&) = delete;
    TVariable& operator=(TVariable&&) noexcept = delete;
    //
    [[nodiscard]] bool isOperator() const override;
    void reduce() override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;

   private:
    std::vector<int>& vars;
    std::vector<int>::size_type pos;
  };

  struct IntegerEvaluator::TOperator final : public IntegerEvaluator::TExpr {
    explicit TOperator(const std::string&);
    TOperator(const TOperator&);
    TOperator(TOperator&&) noexcept;
    //
    TOperator& operator=(const TOperator&) = delete;
    TOperator& operator=(TOperator&&) noexcept = delete;
    //
    [[nodiscard]] virtual std::string getOperatorType() const;
    [[nodiscard]] bool isOperator() const override;
    void reduce() override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;
    ~TOperator() override;

   private:
    const std::string type;
  };  // end of struct IntegerEvaluator::TOperator

  struct IntegerEvaluator::TGroup final : public IntegerEvaluator::TExpr {
    virtual void add(std::shared_ptr<IntegerEvaluator::TExpr> const);
    [[nodiscard]] bool isOperator() const override;
    void reduce() override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;
    ~TGroup() override;

   private:
    void reduce(const std::string&);
    std::vector<std::shared_ptr<IntegerEvaluator::TExpr>> subExpr;
  };  // end of struct IntegerEvaluator::TGroup

  struct IntegerEvaluator::TNumber final : public IntegerEvaluator::TExpr {
    struct Number final : public tfel::math::parser::IntegerExpr {
      explicit Number(const int);
      Number(Number&&) = default;
      Number(const Number&) = default;
      Number& operator=(Number&&) = delete;
      Number& operator=(const Number&) = delete;
      [[nodiscard]] parser::IntegerExprPtr clone(
          const std::vector<int>&) const override;
      [[nodiscard]] int getValue() const override;

     private:
      const int value;
    };  // end of struct Number
    explicit TNumber(const int v);
    //
    TNumber& operator=(const TNumber&) = delete;
    TNumber& operator=(TNumber&&) noexcept = delete;
    //
    [[nodiscard]] bool isOperator() const override;
    [[nodiscard]] parser::IntegerExprPtr analyse() override;
    void reduce() override;

   private:
    const int value;
  };  // end of struct IntegerEvaluator::TNumber

}  // end of namespace tfel::math

#endif /* LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX */
