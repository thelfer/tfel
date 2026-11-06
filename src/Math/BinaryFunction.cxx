/*!
 * \file   src/Math/BinaryFunction.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 02/10/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cerrno>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/Negation.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/LogicalExpr.hxx"
#include "TFEL/Math/Parser/ConditionalExpr.hxx"
#include "TFEL/Math/Parser/BinaryFunction.hxx"

namespace tfel::math::parser {

  void StandardBinaryFunctionBase::
      throwUnimplementedDifferentiateFunctionException() {
    raise(
        "StandardBinaryFunctionBase::"
        "throwUnimplementedDifferentiateFunctionException: "
        "unimplemented feature");
  }  // end of
     // StandardBinaryFunctionBase::throwUnimplementedDifferentiateFunctionException

  void StandardBinaryFunctionBase::throwInvalidCallException(const int e) {
    raise(
        "StandardBinaryFunctionBase::"
        "throwInvalidCallException: "
        "call to function failed "
        "(" +
        std::string(strerror(e)) + ")");
  }  // end of struct StandardBinaryFunctionBase::throwInvalidCallException

  std::string StandardBinaryFunctionBase::getCxxFormula(const char* const n,
                                                        const std::string& e1,
                                                        const std::string& e2) {
    return std::string(n) + '(' + e1 + ',' + e2 + ')';
  }  // end of StandardBinaryFunctionBase::getCxxFormula

  BinaryFunction::~BinaryFunction() = default;

  template <>
  std::shared_ptr<Expr> differentiateBinaryFunction<Evaluator::max>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    if ((!a->dependsOnVariable(pos)) && (!b->dependsOnVariable(pos))) {
      return Number::zero();
    }
    // max(a, b) is b if a < b, a otherwise
    auto c =
        std::make_shared<LogicalOperation<OpLesser>>(a->clone(v), b->clone(v));
    return std::make_shared<ConditionalExpr>(c, b->differentiate(pos, v),
                                             a->differentiate(pos, v));
  }  // end of differentiateBinaryFunction<Evaluator::max>

  template <>
  std::shared_ptr<Expr> differentiateBinaryFunction<Evaluator::min>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    if ((!a->dependsOnVariable(pos)) && (!b->dependsOnVariable(pos))) {
      return Number::zero();
    }
    // min(a, b) is b if b < a, a otherwise
    auto c =
        std::make_shared<LogicalOperation<OpLesser>>(b->clone(v), a->clone(v));
    return std::make_shared<ConditionalExpr>(c, b->differentiate(pos, v),
                                             a->differentiate(pos, v));
  }  // end of differentiateBinaryFunction<Evaluator::min>

  template <>
  std::shared_ptr<Expr> differentiateBinaryFunction<std::hypot>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    }
    // d(hypot(a, b)) = (a * da + b * db) / hypot(a, b)
    auto num = [&a, &b, ba, bb, pos, &v]() -> std::shared_ptr<Expr> {
      auto na = [&a, pos, &v] {
        return std::make_shared<BinaryOperation<OpMult>>(
            a->clone(v), a->differentiate(pos, v));
      };
      auto nb = [&b, pos, &v] {
        return std::make_shared<BinaryOperation<OpMult>>(
            b->clone(v), b->differentiate(pos, v));
      };
      if (!bb) {
        return na();
      } else if (!ba) {
        return nb();
      }
      return std::make_shared<BinaryOperation<OpPlus>>(na(), nb());
    }();
    auto den = std::make_shared<StandardBinaryFunction<std::hypot>>(
        "hypot", a->clone(v), b->clone(v));
    return std::make_shared<BinaryOperation<OpDiv>>(num, den);
  }  // end of differentiateBinaryFunction<std::hypot>

  template <>
  std::shared_ptr<Expr> differentiateBinaryFunction<std::atan2>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    }
    // d(atan2(a, b)) = (b * da - a * db) / (a * a + b * b)
    auto num = [&a, &b, ba, bb, pos, &v]() -> std::shared_ptr<Expr> {
      auto na = [&a, &b, pos, &v] {
        return std::make_shared<BinaryOperation<OpMult>>(
            b->clone(v), a->differentiate(pos, v));
      };
      auto nb = [&a, &b, pos, &v] {
        return std::make_shared<BinaryOperation<OpMult>>(
            a->clone(v), b->differentiate(pos, v));
      };
      if (!bb) {
        return na();
      } else if (!ba) {
        return std::make_shared<Negation>(nb());
      }
      return std::make_shared<BinaryOperation<OpMinus>>(na(), nb());
    }();
    auto a2 =
        std::make_shared<BinaryOperation<OpMult>>(a->clone(v), a->clone(v));
    auto b2 =
        std::make_shared<BinaryOperation<OpMult>>(b->clone(v), b->clone(v));
    auto den = std::make_shared<BinaryOperation<OpPlus>>(a2, b2);
    return std::make_shared<BinaryOperation<OpDiv>>(num, den);
  }  // end of differentiateBinaryFunction<std::atan2>

}  // end of namespace tfel::math::parser
