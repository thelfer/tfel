/*!
 * \file   include/TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx
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

#ifndef LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX
#define LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX

namespace tfel {

  namespace math {

    struct IntegerEvaluator::TExpr {
      TExpr() = default;
      TExpr(const TExpr&) = default;
      TExpr(TExpr&&) = default;
      TExpr& operator=(TExpr&&) = default;
      TExpr& operator=(const TExpr&) = default;
      virtual parser::IntegerExprPtr analyse() = 0;
      virtual void reduce() = 0;
      virtual bool isOperator() const = 0;
      virtual ~TExpr();
    };  // end of IntegerEvaluator::TExpr

    struct IntegerEvaluator::TNegation final : public IntegerEvaluator::TExpr {
      //! expression returned
      struct Negation final : public tfel::math::parser::IntegerExpr {
        Negation(const parser::IntegerExprPtr);
        virtual int getValue() const override;
        virtual parser::IntegerExprPtr clone(
            const std::vector<int>&) const override;
        virtual ~Negation();

       private:
        Negation& operator=(const Negation&) = delete;
        Negation& operator=(Negation&&) = delete;
        const parser::IntegerExprPtr expr;
      };
      TNegation(std::shared_ptr<IntegerEvaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual parser::IntegerExprPtr analyse() override;
      virtual void reduce() override;
      virtual ~TNegation();

     private:
      std::shared_ptr<IntegerEvaluator::TExpr> expr;
    };  // end of struct IntegerEvaluator::TNegation

    struct IntegerEvaluator::TBinaryOperation final
        : public IntegerEvaluator::TExpr {
      struct IntegerOpPlus {
        static int TFEL_VISIBILITY_LOCAL apply(const int, const int);
      };  // end of struct IntegerOpPlus
      struct IntegerOpMinus {
        static int TFEL_VISIBILITY_LOCAL apply(const int, const int);
      };  // end of struct IntegerOpMinus
      struct IntegerOpMult {
        static int TFEL_VISIBILITY_LOCAL apply(const int, const int);
      };  // end of struct IntegerOpMult
      struct IntegerOpDiv {
        static int TFEL_VISIBILITY_LOCAL apply(const int, const int);
      };  // end of struct IntegerOpDiv
      /*!
       * Expression generated
       */
      template <typename Op>
      struct TFEL_VISIBILITY_LOCAL BinaryOperation final
          : public tfel::math::parser::IntegerExpr {
        BinaryOperation(const parser::IntegerExprPtr,
                        const parser::IntegerExprPtr);
        virtual int getValue() const override;
        virtual parser::IntegerExprPtr clone(
            const std::vector<int>&) const override;
        virtual ~BinaryOperation();

       private:
        BinaryOperation& operator=(const BinaryOperation&) = delete;
        BinaryOperation& operator=(BinaryOperation&&) = delete;
        const parser::IntegerExprPtr a;
        const parser::IntegerExprPtr b;
      };  // end of struct BinaryOperation
      /*!
       * Constructor
       */
      TBinaryOperation(std::shared_ptr<IntegerEvaluator::TExpr>,
                       const std::shared_ptr<TOperator>,
                       std::shared_ptr<IntegerEvaluator::TExpr>);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::IntegerExprPtr analyse() override;
      virtual ~TBinaryOperation();

     private:
      TBinaryOperation& operator=(const TBinaryOperation&) = delete;
      TBinaryOperation& operator=(TBinaryOperation&&) = delete;
      std::shared_ptr<IntegerEvaluator::TExpr> a;
      const std::shared_ptr<TOperator> op;
      std::shared_ptr<IntegerEvaluator::TExpr> b;
    };  // end of struct IntegerEvaluator::TBinaryOperation

    struct IntegerEvaluator::TVariable final : public IntegerEvaluator::TExpr {
      struct Variable final : public tfel::math::parser::IntegerExpr {
        Variable(const std::vector<int>&, const std::vector<int>::size_type);
        virtual int getValue() const override;
        virtual parser::IntegerExprPtr clone(
            const std::vector<int>&) const override;

       private:
        Variable& operator=(const Variable&) = delete;
        Variable& operator=(Variable&&) = delete;
        const std::vector<int>& v;
        const std::vector<int>::size_type pos;
      };  // end of struct Variable
      TVariable(const std::string&, IntegerEvaluator&);
      TVariable(const std::vector<int>::size_type, std::vector<int>&);
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::IntegerExprPtr analyse() override;

     private:
      TVariable& operator=(const TVariable&) = delete;
      TVariable& operator=(TVariable&&) = delete;
      std::vector<int>& vars;
      std::vector<int>::size_type pos;
    };

    struct IntegerEvaluator::TOperator final : public IntegerEvaluator::TExpr {
      TOperator(const std::string&);
      TOperator(const TOperator&);
      TOperator(TOperator&&);
      virtual std::string getOperatorType() const;
      virtual bool isOperator() const override;
      virtual void reduce() override;
      virtual parser::IntegerExprPtr analyse() override;
      virtual ~TOperator();

     private:
      TOperator& operator=(const TOperator&) = delete;
      TOperator& operator=(TOperator&&) = delete;
      const std::string type;
    };  // end of struct IntegerEvaluator::TOperator

    struct IntegerEvaluator::TGroup final : public IntegerEvaluator::TExpr {
      virtual bool isOperator() const override;
      virtual void add(std::shared_ptr<IntegerEvaluator::TExpr> const);
      virtual void reduce() override;
      virtual parser::IntegerExprPtr analyse() override;
      virtual ~TGroup();

     private:
      void reduce(const std::string&);
      std::vector<std::shared_ptr<IntegerEvaluator::TExpr>> subExpr;
    };  // end of struct IntegerEvaluator::TGroup

    struct IntegerEvaluator::TNumber final : public IntegerEvaluator::TExpr {
      struct Number final : public tfel::math::parser::IntegerExpr {
        Number(const int);
        Number(Number&&) = default;
        Number(const Number&) = default;
        Number& operator=(Number&&) = delete;
        Number& operator=(const Number&) = delete;
        virtual parser::IntegerExprPtr clone(
            const std::vector<int>&) const override;
        virtual int getValue() const override;
        const int value;
      };  // end of struct Number
      TNumber(const int v);
      virtual bool isOperator() const override;
      virtual parser::IntegerExprPtr analyse() override;
      virtual void reduce() override;

     private:
      TNumber& operator=(const TNumber&);
      TNumber& operator=(TNumber&&);
      const int value;
    };  // end of struct IntegerEvaluator::TNumber

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTEGEREVALUATORTEXPR_HXX */
