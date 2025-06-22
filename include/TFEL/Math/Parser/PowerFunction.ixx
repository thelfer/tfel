/*!
 * \file   include/TFEL/Math/Parser/Function.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   25/04/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_POWERFUNCTION_IXX
#define LIB_TFEL_MATH_PARSER_POWERFUNCTION_IXX

#include <cmath>
#include <string>
#include <cerrno>
#include <cstring>
#include "TFEL/Math/power.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/Function.hxx"

namespace tfel::math::parser {

  template <int N>
  PowerFunction<N>::PowerFunction(const std::shared_ptr<Expr> e) noexcept
      : Function(e) {}  // end of PowerFunction

  template <int N>
  double PowerFunction<N>::getValue() const {
    if constexpr (N == 0) {
      return 1;
    }
    const auto arg = this->expr->getValue();
    if constexpr (N < 0) {
      if (tfel::math::ieee754::fpclassify(arg) == FP_ZERO) {
        FunctionBase::throwInvalidCallException(arg, EINVAL);
      }
    }
    return tfel::math::power<N>(arg);
  }  // end of getValue

  template <int N>
  std::string PowerFunction<N>::getCxxFormula(
      const std::vector<std::string>& m) const {
    if constexpr (N == 0) {
      return "1";
    }
    const auto a = this->expr->getCxxFormula(m);
    if constexpr (N == 1) {
      return a;
    }
    return "tfel::math::power<" + std::to_string(N) + ">(" + a + ")";
  }  // end of getCxxFormula()

  template <int N>
  std::shared_ptr<Expr> PowerFunction<N>::resolveDependencies(
      const std::vector<double>& v) const {
    return std::make_shared<PowerFunction<N>>(
        this->expr->resolveDependencies(v));
  }  // end of resolveDependencies

  template <int N>
  std::shared_ptr<Expr> PowerFunction<N>::clone(
      const std::vector<double>& v) const {
    return std::make_shared<PowerFunction<N>>(this->expr->clone(v));
  }  // end of clone

  template <int N>
  std::shared_ptr<Expr>
  PowerFunction<N>::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& p,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    auto nexpr =
        this->expr->createFunctionByChangingParametersIntoVariables(v, p, pos);
    return std::make_shared<PowerFunction<N>>(nexpr);
  }  // end of createFunctionByChangingParametersIntoVariables

  template <int N>
  std::shared_ptr<Expr> PowerFunction<N>::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    if constexpr (N == 0) {
      return std::make_shared<Number>("0", 0);
    }
    if constexpr (N == 1) {
      return expr->differentiate(pos, v);
    }
    auto Nf = std::make_shared<Number>(std::to_string(N), N);
    if constexpr (N == -16) {
      auto dp = std::make_shared<BinaryOperation<OpMult>>(
          Nf,
          std::make_shared<GeneralPowerFunction>(this->expr->clone(v), -17));
      auto de = expr->differentiate(pos, v);
      return applyChainRule(dp, de);
    } else {
      auto dp = std::make_shared<BinaryOperation<OpMult>>(
          Nf, std::make_shared<PowerFunction<N - 1>>(this->expr->clone(v)));
      auto de = expr->differentiate(pos, v);
      return applyChainRule(dp, de);
    }
  }  // end of differentiate

  template <int N>
  PowerFunction<N>::~PowerFunction() = default;

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_POWERFUNCTION_IXX */
