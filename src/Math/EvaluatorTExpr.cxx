/*!
 * \file   src/Math/EvaluatorTExpr.cxx
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

#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/Evaluator.hxx"

namespace tfel {

  namespace math {

    Evaluator::TExpr::~TExpr() = default;

    Evaluator::TLogicalExpr::~TLogicalExpr() = default;

    Evaluator::TNegLogicalExpr::TNegLogicalExpr(
        const std::shared_ptr<Evaluator::TLogicalExpr> e_)
        : e(e_) {}  // end of Evaluator::TNegLogicalExpr::TNegLogicalExpr

    void Evaluator::TNegLogicalExpr::reduce() {
      this->e->reduce();
    }  // end of Evaluator::TNegLogicalExpr::reduce

    std::shared_ptr<tfel::math::parser::LogicalExpr>
    Evaluator::TNegLogicalExpr::analyse() {
      using namespace tfel::math::parser;
      return std::shared_ptr<LogicalExpr>(
          new NegLogicalExpression(this->e->analyse()));
    }  // end of struct Evaluator::TNegLogicalExpr

    Evaluator::TNegLogicalExpr::~TNegLogicalExpr() = default;

    Evaluator::TNegation::TNegation(std::shared_ptr<Evaluator::TExpr> e)
        : expr(std::move(e)) {}  // end of Evaluator::TNegation::TNegation

    bool Evaluator::TNegation::isOperator() const { return false; }

    parser::ExprPtr Evaluator::TNegation::analyse() {
      using namespace tfel::math::parser;
      return ExprPtr(new Negation(this->expr->analyse()));
    }

    void Evaluator::TNegation::reduce() { this->expr->reduce(); }

    Evaluator::TNegation::~TNegation() noexcept = default;

    Evaluator::TOperator::TOperator(const std::string& t_)
        : type(t_) {
    }  // end of Evaluator::TOperator::TOperator(const std::string& t_)

    std::string Evaluator::TOperator::getOperatorType() const {
      return this->type;
    }  // end of Evaluator::TOperator::getOperatorType() const

    bool Evaluator::TOperator::isOperator() const {
      return true;
    }  // end of Evaluator::TOperator::isOperator() const

    void Evaluator::TOperator::reduce() {
    }  // end of Evaluator::TOperator::reduce()

    parser::ExprPtr Evaluator::TOperator::analyse() {
      raise("Evaluator::TOperator : invalid call");
    }  // end of Evaluator::TOperator::analyse()

    Evaluator::TOperator::~TOperator() noexcept = default;

    Evaluator::TBinaryOperation::TBinaryOperation(
        std::shared_ptr<Evaluator::TExpr> a_,
        const std::shared_ptr<TOperator> op_,
        std::shared_ptr<Evaluator::TExpr> b_)
        : a(std::move(a_)),
          op(op_),
          b(std::move(b_)) {
    }  // end of Evaluator::TBinaryOperation::TBinaryOperation

    bool Evaluator::TBinaryOperation::isOperator() const {
      return false;
    }  // end of Evaluator::TBinaryOperation::isOperator() const

    void Evaluator::TBinaryOperation::reduce() {
      a->reduce();
      b->reduce();
    }  // end of Evaluator::TBinaryOperation::reduce()

    parser::ExprPtr Evaluator::TBinaryOperation::analyse() {
      using namespace tfel::math::parser;
      if (op->getOperatorType() == "+") {
        return ExprPtr(new BinaryOperation<OpPlus>(a->analyse(), b->analyse()));
      } else if (op->getOperatorType() == "-") {
        return ExprPtr(
            new BinaryOperation<OpMinus>(a->analyse(), b->analyse()));
      } else if (op->getOperatorType() == "*") {
        return ExprPtr(new BinaryOperation<OpMult>(a->analyse(), b->analyse()));
      } else if (op->getOperatorType() == "/") {
        return ExprPtr(new BinaryOperation<OpDiv>(a->analyse(), b->analyse()));
      } else if (op->getOperatorType() == "**") {
        return ExprPtr(
            new BinaryOperation<OpPower>(a->analyse(), b->analyse()));
      }
      raise(
          "Evaluator::TBinaryOperation : "
          "invalid operation type  '" +
          op->getOperatorType() + "'");
    }  // end of Evaluator::TBinaryOperation::analyse()

    Evaluator::TBinaryOperation::~TBinaryOperation() = default;

    Evaluator::TVariable::TVariable(const std::string& name, Evaluator& e)
        : vars(e.variables),
          pos(e.registerVariable(name)) {
    }  // end of Evaluator::TVariable::TVariable

    Evaluator::TVariable::TVariable(const std::vector<double>::size_type ppos,
                                    std::vector<double>& vvars)
        : vars(vvars), pos(ppos) {}

    bool Evaluator::TVariable::isOperator() const { return false; }

    void Evaluator::TVariable::reduce() {}

    parser::ExprPtr Evaluator::TVariable::analyse() {
      using namespace tfel::math::parser;
      return ExprPtr(new Variable(this->vars, this->pos));
    }

    bool Evaluator::TGroup::isOperator() const { return false; }

    void Evaluator::TGroup::add(std::shared_ptr<Evaluator::TExpr> const e) {
      this->subExpr.push_back(e);
    }  // end of Evaluator::TGroup::add

    void Evaluator::TGroup::reduce() {
      using namespace std;
      auto p = this->subExpr.begin();
      auto pe = this->subExpr.end();
      while (p != pe) {
        (*p)->reduce();
        ++p;
      }
      // ok, this is the tricky part
      // operator** has the highest priority
      this->reduce("**");
      // treating operator/
      this->reduce("/");
      // treating operator*
      this->reduce("*");
      // treating operator* -
      this->reduce("-");
      // operator+ has the lowest priority
      this->reduce("+");
    }

    parser::ExprPtr Evaluator::TGroup::analyse() {
      raise_if(this->subExpr.size() != 1u,
               "TGroup::analyse : "
               "tgroup has not been reduced.");
      return (this->subExpr[0])->analyse();
    }

    Evaluator::TGroup::~TGroup() = default;

    void Evaluator::TGroup::reduce(const std::string& op) {
      using namespace tfel::math::parser;
      auto throw_if = [](const bool b, const std::string& m) {
        raise_if(b, "Evaluator::TGroup::reduce: " + m);
      };
      auto p = this->subExpr.begin();
      while (p != this->subExpr.end()) {
        if ((*p)->isOperator()) {
          auto o = std::make_shared<TOperator>(
              static_cast<const TOperator&>(*(p->get())));
          if (o->getOperatorType() == op) {
            auto next = std::next(p);
            if (p == this->subExpr.begin()) {
              throw_if(op != "-",
                       "group began with an operator ('" + op + "')");
              throw_if(next == this->subExpr.end(),
                       "group ends by operator '" + op + "'");
              throw_if((*next)->isOperator(), "group two successive operators");
              *next = std::shared_ptr<Evaluator::TExpr>(new TNegation(*next));
              this->subExpr.erase(p);
              p = this->subExpr.begin();
            } else {
              auto previous = std::prev(p);
              throw_if(next == this->subExpr.end(),
                       "group ends by operator '" + op + "'");
              if ((*previous)->isOperator()) {
                throw_if(op != "-", "group two successive operators");
                const auto po = std::shared_ptr<TOperator>(
                    dynamic_cast<TOperator*>(previous->get()));
                throw_if(po->getOperatorType() != "+",
                         "group two successive operators");
                throw_if((*next)->isOperator(),
                         "group three successive operators");
                *p = std::shared_ptr<Evaluator::TExpr>(new TNegation(*next));
                p = this->subExpr.erase(next);
                --p;
              } else {
                if ((*next)->isOperator()) {
                  throw_if(op == "-", "group two successive operators");
                  const auto no = dynamic_cast<TOperator*>(next->get());
                  throw_if(no->getOperatorType() != "-",
                           "group two successive operators");
                  auto nnext = std::next(next);
                  throw_if(nnext == this->subExpr.end(),
                           "group ends by operator '" + op + "'");
                  throw_if((*nnext)->isOperator(),
                           "group two successive operators");
                  *nnext =
                      std::shared_ptr<Evaluator::TExpr>(new TNegation(*nnext));
                  next = this->subExpr.erase(next);
                  p = next - 1;
                  previous = next - 2;
                }
                *previous = std::shared_ptr<Evaluator::TExpr>(
                    new TBinaryOperation(*previous, o, *next));
                ++next;
                p = this->subExpr.erase(p, next);
                --p;
              }
            }
          }
        }
        ++p;
      }
    }  // end of Evaluator::TGroup::reduce

    Evaluator::TFunction::TFunction(Evaluator::FunctionGenerator f_,
                                    std::shared_ptr<Evaluator::TExpr> g_)
        : f(f_), arg(std::move(g_)) {}

    bool Evaluator::TFunction::isOperator() const { return false; }

    parser::ExprPtr Evaluator::TFunction::analyse() {
      return (this->f)(this->arg->analyse());
    }

    void Evaluator::TFunction::reduce() { this->arg->reduce(); }

    Evaluator::TFunction::~TFunction() = default;

    Evaluator::TBinaryFunction::TBinaryFunction(
        Evaluator::BinaryFunctionGenerator f_,
        std::shared_ptr<Evaluator::TExpr> a1_,
        std::shared_ptr<Evaluator::TExpr> a2_)
        : f(f_), arg1(std::move(a1_)), arg2(std::move(a2_)) {}

    bool Evaluator::TBinaryFunction::isOperator() const { return false; }

    parser::ExprPtr Evaluator::TBinaryFunction::analyse() {
      return (this->f)(this->arg1->analyse(), this->arg2->analyse());
    }

    void Evaluator::TBinaryFunction::reduce() {
      this->arg1->reduce();
      this->arg2->reduce();
    }

    Evaluator::TBinaryFunction::~TBinaryFunction() = default;

    Evaluator::TNumber::TNumber(const double v) : value(v) {}

    bool Evaluator::TNumber::isOperator() const { return false; }

    parser::ExprPtr Evaluator::TNumber::analyse() {
      using namespace tfel::math::parser;
      return ExprPtr(new Number(this->value));
    }

    void Evaluator::TNumber::reduce() {}

    Evaluator::TExternalFunctionExpr::TExternalFunctionExpr(
        const std::string& fname,
        std::vector<std::shared_ptr<Evaluator::TExpr>>& fargs,
        std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
        : name(fname),
          args(fargs),
          manager(m) {}  // end of TExternalFunctionExpr

    bool Evaluator::TExternalFunctionExpr::isOperator() const { return false; }

    parser::ExprPtr Evaluator::TExternalFunctionExpr::analyse() {
      using namespace tfel::math::parser;
      auto r = std::vector<ExprPtr>{};
      for (auto& a : this->args) {
        r.push_back(a->analyse());
      }
      return ExprPtr(new ExternalFunctionExpr(name, r, this->manager));
    }

    void Evaluator::TExternalFunctionExpr::reduce() {
      for (auto& a : this->args) {
        a->reduce();
      }
    }

    Evaluator::TExternalFunctionExpr::~TExternalFunctionExpr() noexcept =
        default;

    Evaluator::TDifferentiatedFunctionExpr::TDifferentiatedFunctionExpr(
        std::shared_ptr<ExternalFunction> ff,
        std::vector<std::shared_ptr<Evaluator::TExpr>>& fargs,
        const std::vector<std::vector<double>::size_type>& fvar)
        : f(std::move(ff)),
          args(fargs),
          var(fvar) {}  // end of TDifferentiatedFunctionExpr

    bool Evaluator::TDifferentiatedFunctionExpr::isOperator() const {
      return false;
    }

    parser::ExprPtr Evaluator::TDifferentiatedFunctionExpr::analyse() {
      using namespace tfel::math::parser;
      auto r = std::vector<ExprPtr>{};
      for (const auto& a : this->args) {
        r.push_back(a->analyse());
      }
      return ExprPtr(new DifferentiatedFunctionExpr(this->f, r, this->var));
    }

    void Evaluator::TDifferentiatedFunctionExpr::reduce() {
      for (auto& a : this->args) {
        a->reduce();
      }
    }

    Evaluator::TConditionalExpr::TConditionalExpr(
        std::shared_ptr<Evaluator::TLogicalExpr> c_,
        std::shared_ptr<Evaluator::TExpr> a_,
        std::shared_ptr<Evaluator::TExpr> b_)
        : c(std::move(c_)),
          a(std::move(a_)),
          b(std::move(b_)) {
    }  // end of Evaluator::TConditionalExpr::TConditionalExpr

    bool Evaluator::TConditionalExpr::isOperator() const {
      return false;
    }  // end of Evaluator::TConditionalExpr::isOperator

    void Evaluator::TConditionalExpr::reduce() {
      this->c->reduce();
      this->a->reduce();
      this->b->reduce();
    }  // end of Evaluator::TConditionalExpr::reduce

    parser::ExprPtr Evaluator::TConditionalExpr::analyse() {
      using namespace tfel::math::parser;
      return ExprPtr(new ConditionalExpr(this->c->analyse(), this->a->analyse(),
                                         this->b->analyse()));
    }  // end of Evaluator::TConditionalExpr::analyse

    Evaluator::TDifferentiatedFunctionExpr::
        ~TDifferentiatedFunctionExpr() noexcept = default;

    Evaluator::TConditionalExpr::~TConditionalExpr() = default;

    Evaluator::TExternalOperator::TExternalOperator(
        const Evaluator::ExternalFunctionGenerator f_,
        const std::vector<std::string>& param_,
        std::vector<std::shared_ptr<Evaluator::TExpr>>& a_)
        : f(f_),
          param(param_),
          args(a_) {}  // end of Evaluator::TExternalOperator::TExternalOperator

    bool Evaluator::TExternalOperator::isOperator() const {
      return false;
    }  // end of Evaluator::TExternalOperator::isOperator() const

    parser::ExprPtr Evaluator::TExternalOperator::analyse() {
      using namespace tfel::math::parser;
      auto r = std::vector<ExprPtr>{};
      for (auto& a : this->args) {
        r.push_back(a->analyse());
      }
      return (*(this->f))(this->param, r);
    }  // end of Evaluator::TExternalOperator::analyse()

    void Evaluator::TExternalOperator::reduce() {
      for (auto& a : this->args) {
        a->reduce();
      }
    }  // end of Evaluator::TExternalOperator::reduce()

    Evaluator::TExternalOperator::~TExternalOperator() = default;

  }  // end of namespace math

}  // end of namespace tfel
