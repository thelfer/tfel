/*!
 * \file   src/Math/Function.cxx
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

#include <cmath>
#include <numbers>
#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Negation.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/LogicalExpr.hxx"
#include "TFEL/Math/Parser/ConditionalExpr.hxx"
#include "TFEL/Math/Parser/Function.hxx"

#ifndef __SUNPRO_CC
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(X) \
  template <>                                                               \
  std::shared_ptr<Expr> differentiateFunction<std::X>(                      \
      const std::shared_ptr<Expr> expr,                                     \
      const std::vector<double>::size_type pos, const std::vector<double>& v)
#else /* __SUNPRO_CC */
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(X) \
  template <>                                                               \
  std::shared_ptr<Expr> differentiateFunction<X>(                           \
      const std::shared_ptr<Expr> expr,                                     \
      const std::vector<double>::size_type pos, const std::vector<double>& v)
#endif /* __SUNPRO_CC */

namespace tfel::math::parser {

  void FunctionBase::throwUnimplementedDifferentiateFunctionException() {
    raise(
        "FunctionBase::"
        "throwUnimplementedDifferentiateFunctionException : "
        "unimplemented feature");
  }  // end of throwUnimplementedDifferentiateFunctionException

  void FunctionBase::throwInvalidCallException(const double v, const int e) {
    raise(
        "FunctionBase::throwInvalidCallException: "
        "call to function failed for value " +
        std::to_string(v) +
        " "
        "(" +
        std::string(strerror(e)) + ")");
  }  // end of throwInvalidCallException

  std::string FunctionBase::getCxxFormula(const char* const n,
                                          const std::string& e) {
    return std::string(n) + '(' + e + ')';
  }  // end of getCxxFormula

  Function::Function(const std::shared_ptr<Expr> e) noexcept
      : expr(e) {}  // end of Function

  bool Function::isConstant() const { return this->expr->isConstant(); }

  void Function::checkCyclicDependency(std::vector<std::string>& names) const {
    this->expr->checkCyclicDependency(names);
  }  // end of checkCyclicDependency

  void Function::getParametersNames(std::set<std::string>& p) const {
    this->expr->getParametersNames(p);
  }  // end of getParametersNames

  bool Function::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return this->expr->dependsOnVariable(p);
  }  // end of dependsOnVariable

  Function::~Function() = default;

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(exp) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto expe = std::make_shared<StandardFunction<::exp>>("exp", ce);
    return applyChainRule(expe, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sin) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto cose = std::make_shared<StandardFunction<::cos>>("cos", ce);
    return applyChainRule(cose, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cos) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto sine = std::make_shared<StandardFunction<::sin>>("sin", ce);
    return applyChainRule(std::make_shared<Negation>(sine), de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tan) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto tane = std::make_shared<StandardFunction<::tan>>("tan", ce);
    auto tan2e = std::make_shared<BinaryOperation<OpMult>>(tane, tane);
    auto tan2e_ =
        std::make_shared<BinaryOperation<OpPlus>>(Number::one(), tan2e);
    return applyChainRule(tan2e_, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sqrt) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto sqrte = std::make_shared<StandardFunction<::sqrt>>("sqrt", ce);
    auto sqrte_ = std::make_shared<BinaryOperation<OpDiv>>(
        std::make_shared<Number>("0.5", 0.5), sqrte);
    return applyChainRule(sqrte_, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    return std::make_shared<BinaryOperation<OpDiv>>(de, ce);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log10) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    static double ln10 = std::numbers::ln10_v<double>;
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den = std::make_shared<BinaryOperation<OpMult>>(
        std::make_shared<Number>("log(10)", ln10), ce);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(asin) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpMinus>>(Number::one(), den1);
    auto den3 = std::make_shared<StandardFunction<::sqrt>>("sqrt", den2);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den3);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(acos) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto num1 = std::make_shared<Number>("-1", -1.);
    auto num2 = std::make_shared<BinaryOperation<OpMult>>(num1, de);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpMinus>>(Number::one(), den1);
    auto den3 = std::make_shared<StandardFunction<::sqrt>>("sqrt", den2);
    return std::make_shared<BinaryOperation<OpDiv>>(num2, den3);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(atan) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpPlus>>(Number::one(), den1);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den2);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sinh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto coshe = std::make_shared<StandardFunction<::cosh>>("cosh", ce);
    return applyChainRule(coshe, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cosh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto sinhe = std::make_shared<StandardFunction<::sinh>>("sinh", ce);
    return applyChainRule(sinhe, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tanh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto coshe = std::make_shared<StandardFunction<::cosh>>("cosh", ce);
    auto coshe2 = std::make_shared<BinaryOperation<OpMult>>(coshe, coshe);
    return std::make_shared<BinaryOperation<OpDiv>>(de, coshe2);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(exp2) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    static double ln2 = std::numbers::ln2_v<double>;
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto exp2e = std::make_shared<StandardFunction<::exp2>>("exp2", ce);
    auto exp2e_ = std::make_shared<BinaryOperation<OpMult>>(
        std::make_shared<Number>("log(2)", ln2), exp2e);
    return applyChainRule(exp2e_, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(expm1) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto expe = std::make_shared<StandardFunction<::exp>>("exp", ce);
    return applyChainRule(expe, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cbrt) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto cbrte = std::make_shared<StandardFunction<::cbrt>>("cbrt", ce);
    auto cbrte2 = std::make_shared<BinaryOperation<OpMult>>(cbrte, cbrte);
    auto den = std::make_shared<BinaryOperation<OpMult>>(
        std::make_shared<Number>("3", 3.), cbrte2);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(fabs) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    // sign of the argument, consistent with Evaluator::Heavyside
    auto c = std::make_shared<LogicalOperation<OpLesser>>(ce, Number::zero());
    auto signe = std::make_shared<ConditionalExpr>(
        c, std::make_shared<Number>("-1", -1.), Number::one());
    return applyChainRule(signe, de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log2) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    static double ln2 = std::numbers::ln2_v<double>;
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den = std::make_shared<BinaryOperation<OpMult>>(
        std::make_shared<Number>("log(2)", ln2), ce);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log1p) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den = std::make_shared<BinaryOperation<OpPlus>>(Number::one(), ce);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(asinh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpPlus>>(den1, Number::one());
    auto den3 = std::make_shared<StandardFunction<::sqrt>>("sqrt", den2);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den3);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(acosh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpMinus>>(den1, Number::one());
    auto den3 = std::make_shared<StandardFunction<::sqrt>>("sqrt", den2);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den3);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(atanh) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto den2 = std::make_shared<BinaryOperation<OpMinus>>(Number::one(), den1);
    return std::make_shared<BinaryOperation<OpDiv>>(de, den2);
  }  // end of differentiateFunction

  /*!
   * \return the derivative of the error function, 2 / sqrt(pi) * exp(-x * x)
   * \param[in] ce: argument of the error function
   */
  static std::shared_ptr<Expr> buildErrorFunctionDerivative(
      const std::shared_ptr<Expr> ce) {
    static double c = 2 * std::numbers::inv_sqrtpi_v<double>;
    auto ce2 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
    auto expe = std::make_shared<StandardFunction<::exp>>(
        "exp", std::make_shared<Negation>(ce2));
    return std::make_shared<BinaryOperation<OpMult>>(
        std::make_shared<Number>("1.1283791670955126", c), expe);
  }  // end of buildErrorFunctionDerivative

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(erf) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    return applyChainRule(buildErrorFunctionDerivative(ce), de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(erfc) {
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    auto ce = expr->clone(v);
    auto de = expr->differentiate(pos, v);
    return applyChainRule(
        std::make_shared<Negation>(buildErrorFunctionDerivative(ce)), de);
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tgamma) {
    static_cast<void>(v);
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    raise(
        "differentiateFunction: "
        "the derivative of the function 'tgamma' is not implemented");
  }  // end of differentiateFunction

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(lgamma) {
    static_cast<void>(v);
    if (!expr->dependsOnVariable(pos)) {
      return Number::zero();
    }
    raise(
        "differentiateFunction: "
        "the derivative of the function 'lgamma' is not implemented");
  }  // end of differentiateFunction

  template <>
  std::shared_ptr<Expr> differentiateFunction<Evaluator::Heavyside>(
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&) {
    // the Heaviside function is piecewise constant
    return Number::zero();
  }  // end of differentiateFunction

  template <>
  std::string StandardFunction<Evaluator::Heavyside>::getCxxFormula(
      const std::vector<std::string>& m) const {
    // consistent with Evaluator::Heavyside
    return "(((" + this->expr->getCxxFormula(m) + ")<(0.)) ? (0.) : (1.))";
  }  // end of getCxxFormula

}  // end of namespace tfel::math::parser
