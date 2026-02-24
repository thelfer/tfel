/*!
 * \file include/TFEL/Math/Parser/EvaluatorTExpr.ixx
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

namespace tfel::math {

  template <typename Op>
  Evaluator::TLogicalOperation<Op>::TLogicalOperation(
      std::shared_ptr<Evaluator::TExpr> a_,
      std::shared_ptr<Evaluator::TExpr> b_) noexcept
      : a(a_),
        b(b_) {}  // end of Evaluator::TLogicalOperation::TLogicalOperation

  template <typename Op>
  void Evaluator::TLogicalOperation<Op>::reduce() {
    this->a->reduce();
    this->b->reduce();
  }  // end of Evaluator::TLogicalOperation::reduce

  template <typename Op>
  std::shared_ptr<tfel::math::parser::LogicalExpr>
  Evaluator::TLogicalOperation<Op>::analyse() {
    using namespace tfel::math::parser;
    return std::make_shared<LogicalOperation<Op>>(this->a->analyse(),
                                                  this->b->analyse());
  }  // end of Evaluator::TLogicalOperation::TLogicalOperation::analyse()

  template <typename Op>
  Evaluator::TLogicalBinaryOperation<Op>::TLogicalBinaryOperation(
      std::shared_ptr<Evaluator::TLogicalExpr> a_,
      std::shared_ptr<Evaluator::TLogicalExpr> b_) noexcept
      : a(a_),
        b(b_) {
  }  // end of Evaluator::TLogicalBinaryOperation<Op>::TLogicalBinaryOperation

  template <typename Op>
  void Evaluator::TLogicalBinaryOperation<Op>::reduce() {
    this->a->reduce();
    this->b->reduce();
  }

  template <typename Op>
  std::shared_ptr<tfel::math::parser::LogicalExpr>
  Evaluator::TLogicalBinaryOperation<Op>::analyse() {
    using namespace tfel::math::parser;
    return std::make_shared<LogicalBinaryOperation<Op>>(this->a->analyse(),
                                                        this->b->analyse());
  }  // end of Evaluator::TLogicalBinaryOperation<Op>::analyse

}  // end of namespace tfel::math
