/*!
 * \file   src/Math/IntegerEvaluatorTExpr.cxx
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
#include "TFEL/Math/IntegerEvaluator.hxx"
#include "TFEL/Math/Parser/IntegerEvaluatorTExpr.hxx"

namespace tfel::math {

  namespace parser {

    IntegerExpr::~IntegerExpr() = default;

  }  // end of namespace parser

  IntegerEvaluator::TNegation::TNegation(
      std::shared_ptr<IntegerEvaluator::TExpr> e)
      : expr(std::move(e)) {}  // end of IntegerEvaluator::TNegation::TNegation

  bool IntegerEvaluator::TNegation::isOperator() const { return false; }

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TNegation::analyse() {
    using namespace tfel::math::parser;
    return std::make_shared<Negation>(this->expr->analyse());
  }

  void IntegerEvaluator::TNegation::reduce() { this->expr->reduce(); }

  IntegerEvaluator::TNegation::~TNegation() = default;

  IntegerEvaluator::TNegation::Negation::Negation(
      const std::shared_ptr<tfel::math::parser::IntegerExpr> e)
      : expr(e) {}  // end of IntegerEvaluator::TNegation::Negation::Negation

  int IntegerEvaluator::TNegation::Negation::getValue() const {
    return -(this->expr->getValue());
  }  // end of IntegerEvaluator::TNegation::Negation::getValue()

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TNegation::Negation::clone(
      const std::vector<int>& v) const {
    using namespace tfel::math::parser;
    return std::make_shared<Negation>(this->expr->clone(v));
  }

  IntegerEvaluator::TNegation::Negation::~Negation() = default;

  IntegerEvaluator::TOperator::TOperator(const std::string& t_)
      : type(t_) {
  }  // end of IntegerEvaluator::TOperator::TOperator(const std::string& t_)

  IntegerEvaluator::TOperator::TOperator(const TOperator&) = default;
  IntegerEvaluator::TOperator::TOperator(TOperator&&) = default;

  std::string IntegerEvaluator::TOperator::getOperatorType() const {
    return this->type;
  }  // end of IntegerEvaluator::TOperator::getOperatorType() const

  bool IntegerEvaluator::TOperator::isOperator() const {
    return true;
  }  // end of IntegerEvaluator::TOperator::isOperator() const

  void IntegerEvaluator::TOperator::reduce() {
  }  // end of IntegerEvaluator::TOperator::reduce()

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TOperator::analyse() {
    raise("IntegerEvaluator::TOperator : invalid call");
  }  // end of IntegerEvaluator::TOperator::analyse()

  IntegerEvaluator::TOperator::~TOperator() = default;

  int IntegerEvaluator::TBinaryOperation::IntegerOpPlus::apply(const int a,
                                                               const int b) {
    return a + b;
  }

  int IntegerEvaluator::TBinaryOperation::IntegerOpMinus::apply(const int a,
                                                                const int b) {
    return a - b;
  }

  int IntegerEvaluator::TBinaryOperation::IntegerOpMult::apply(const int a,
                                                               const int b) {
    return a * b;
  }

  int IntegerEvaluator::TBinaryOperation::IntegerOpDiv::apply(const int a,
                                                              const int b) {
    return a / b;
  }

  template <typename Op>
  IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::BinaryOperation(
      const std::shared_ptr<tfel::math::parser::IntegerExpr> a_,
      const std::shared_ptr<tfel::math::parser::IntegerExpr> b_)
      : a(a_), b(b_) {}  // end of BinaryOperation

  template <typename Op>
  IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::~BinaryOperation() =
      default;

  template <typename Op>
  int IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::getValue()
      const {
    return Op::apply(this->a->getValue(), this->b->getValue());
  }  // end of getValue

  template <typename Op>
  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>::clone(
      const std::vector<int>& v) const {
    using namespace tfel::math::parser;
    return std::shared_ptr<IntegerExpr>(
        new IntegerEvaluator::TBinaryOperation::BinaryOperation<Op>(
            this->a->clone(v), this->b->clone(v)));
  }  // end of clone

  IntegerEvaluator::TBinaryOperation::TBinaryOperation(
      std::shared_ptr<IntegerEvaluator::TExpr> a_,
      const std::shared_ptr<TOperator> op_,
      std::shared_ptr<IntegerEvaluator::TExpr> b_)
      : a(std::move(a_)),
        op(op_),
        b(std::move(b_)) {
  }  // end of IntegerEvaluator::TBinaryOperation::TBinaryOperation

  bool IntegerEvaluator::TBinaryOperation::isOperator() const {
    return false;
  }  // end of IntegerEvaluator::TBinaryOperation::isOperator() const

  void IntegerEvaluator::TBinaryOperation::reduce() {
    a->reduce();
    b->reduce();
  }  // end of IntegerEvaluator::TBinaryOperation::reduce()

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TBinaryOperation::analyse() {
    using namespace tfel::math::parser;
    if (op->getOperatorType() == "+") {
      return std::make_shared<BinaryOperation<IntegerOpPlus>>(a->analyse(),
                                                              b->analyse());
    } else if (op->getOperatorType() == "-") {
      return std::make_shared<BinaryOperation<IntegerOpMinus>>(a->analyse(),
                                                               b->analyse());
    } else if (op->getOperatorType() == "*") {
      return std::make_shared<BinaryOperation<IntegerOpMult>>(a->analyse(),
                                                              b->analyse());
    } else if (op->getOperatorType() == "/") {
      return std::make_shared<BinaryOperation<IntegerOpDiv>>(a->analyse(),
                                                             b->analyse());
    }
    raise(
        "IntegerEvaluator::TBinaryOperation : "
        "invalid operation type  '" +
        op->getOperatorType() + "'");
  }  // end of IntegerEvaluator::TBinaryOperation::analyse()

  IntegerEvaluator::TBinaryOperation::~TBinaryOperation() = default;

  IntegerEvaluator::TVariable::TVariable(const std::string& name,
                                         IntegerEvaluator& e)
      : vars(e.variables),
        pos(e.registerVariable(name)) {
  }  // end of IntegerEvaluator::TVariable::TVariable

  IntegerEvaluator::TVariable::TVariable(const std::vector<int>::size_type ppos,
                                         std::vector<int>& vvars)
      : vars(vvars), pos(ppos) {}

  bool IntegerEvaluator::TVariable::isOperator() const { return false; }

  void IntegerEvaluator::TVariable::reduce() {}

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TVariable::analyse() {
    using namespace tfel::math::parser;
    return std::make_shared<Variable>(this->vars, this->pos);
  }

  IntegerEvaluator::TVariable::Variable::Variable(
      const std::vector<int>& v_, const std::vector<int>::size_type p_)
      : v(v_),
        pos(p_) {}  // end of IntegerEvaluator::TVariable::Variable::Variable

  int IntegerEvaluator::TVariable::Variable::getValue() const {
    return this->v[this->pos];
  }  // end of IntegerEvaluator::TVariable::Variable::getValue

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TVariable::Variable::clone(
      const std::vector<int>& v_) const {
    using namespace tfel::math::parser;
    return std::make_shared<Variable>(v_, this->pos);
  }  // end of IntegerEvaluator::TVariable::Variable::clone

  bool IntegerEvaluator::TGroup::isOperator() const { return false; }

  void IntegerEvaluator::TGroup::add(
      std::shared_ptr<IntegerEvaluator::TExpr> const e) {
    this->subExpr.push_back(e);
  }  // end of IntegerEvaluator::TGroup::add

  void IntegerEvaluator::TGroup::reduce() {
    auto p = this->subExpr.begin();
    auto pe = this->subExpr.end();
    while (p != pe) {
      (*p)->reduce();
      ++p;
    }
    // treating operator/
    this->reduce("/");
    // treating operator*
    this->reduce("*");
    // treating operator* -
    this->reduce("-");
    // operator+ has the lowest priority
    this->reduce("+");
  }

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TGroup::analyse() {
    raise_if(this->subExpr.size() != 1u,
             "TGroup::analyse: tgroup has not been reduced.");
    return (this->subExpr[0])->analyse();
  }

  IntegerEvaluator::TGroup::~TGroup() = default;

  void IntegerEvaluator::TGroup::reduce(const std::string& op) {
    using namespace tfel::math::parser;
    auto throw_if = [](const bool b, const std::string& m) {
      raise_if(b, "IntegerEvaluator::TGroup::reduce: " + m);
    };
    auto p = this->subExpr.begin();
    while (p != this->subExpr.end()) {
      if ((*p)->isOperator()) {
        auto o = std::make_shared<TOperator>(
            static_cast<const TOperator&>(*(p->get())));
        if (o->getOperatorType() == op) {
          auto previous = p - 1;
          auto next = p + 1;
          if (p == this->subExpr.begin()) {
            if (op != "-") {
              throw_if(true, "group began with an operator '" + op + "'");
            } else {
              throw_if(next == this->subExpr.end(),
                       "group ends by operator '" + op + "'");
              throw_if((*next)->isOperator(), "group two successive operators");
              *next = std::make_shared<TNegation>(*next);
              this->subExpr.erase(p);
              p = this->subExpr.begin();
            }
          } else {
            throw_if(next == this->subExpr.end(),
                     "group ends by operator '" + op + "'");
            if ((*previous)->isOperator()) {
              throw_if(op != "-", "group two successive operators");
              auto po = std::shared_ptr<TOperator>(
                  dynamic_cast<TOperator*>(previous->get()));
              throw_if(po->getOperatorType() != "+",
                       "group two successive operators");
              throw_if((*next)->isOperator(),
                       "group three successive operators");
              *p = std::make_shared<TNegation>(*next);
              p = this->subExpr.erase(next);
              --p;
            } else {
              if ((*next)->isOperator()) {
                throw_if(op == "-", "group two successive operators");
                TOperator* const no = dynamic_cast<TOperator*>(next->get());
                throw_if(no->getOperatorType() != "-",
                         "group two successive operators");
                auto nnext = next + 1;
                throw_if(nnext == this->subExpr.end(),
                         "group ends by operator " + op);
                throw_if((*nnext)->isOperator(),
                         "group two successive operators");
                *nnext = std::make_shared<TNegation>(*nnext);
                next = this->subExpr.erase(next);
                p = next - 1;
                previous = next - 2;
              }
              *previous =
                  std::make_shared<TBinaryOperation>(*previous, o, *next);
              ++next;
              p = this->subExpr.erase(p, next);
              --p;
            }
          }
        }
      }
      ++p;
    }
  }  // end of IntegerEvaluator::TGroup::reduce

  IntegerEvaluator::TNumber::TNumber(const int v) : value(v) {}

  bool IntegerEvaluator::TNumber::isOperator() const { return false; }

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TNumber::analyse() {
    return parser::IntegerExprPtr(new Number(value));
  }

  void IntegerEvaluator::TNumber::reduce() {}

  IntegerEvaluator::TNumber::Number::Number(const int v)
      : value(v) {}  // end of IntegerEvaluator::TNumber::Number::Number

  int IntegerEvaluator::TNumber::Number::getValue() const {
    return this->value;
  }  // end of IntegerEvaluator::TNumber::Number::getValue

  std::shared_ptr<tfel::math::parser::IntegerExpr>
  IntegerEvaluator::TNumber::Number::clone(const std::vector<int>&) const {
    using namespace tfel::math::parser;
    return std::make_shared<Number>(this->value);
  }  // end of IntegerEvaluator::TNumber::Number::clone

}  // end of namespace tfel::math
