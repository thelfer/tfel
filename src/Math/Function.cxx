/*!
 * \file   src/Math/Function.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>

#include "TFEL/Math/Parser/Negation.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
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

namespace tfel {
  namespace math {

    namespace parser {

      Function::~Function() = default;

      void
      StandardFunctionBase::throwUnimplementedDifferentiateFunctionException() {
        throw(std::runtime_error(
            "StandardFunctionBase::"
            "throwUnimplementedDifferentiateFunctionException : "
            "unimplemented feature"));
      }  // end of
         // StandardFunctionBase::throwUnimplementedDifferentiateFunctionException()

      void StandardFunctionBase::throwInvalidCallException(const double v,
                                                           const int e) {
        throw(std::runtime_error(
            "StandardFunctionBase::throwInvalidCallException: "
            "call to function failed for value " +
            std::to_string(v) + " (" + std::string(strerror(e)) + ")"));
      }  // end of struct StandardFunctionBase::throwInvalidCallException()

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(exp) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto expe = std::make_shared<StandardFunction<::exp>>(ce);
        return std::make_shared<BinaryOperation<OpMult>>(expe, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sin) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto cose = std::make_shared<StandardFunction<::cos>>(ce);
        return std::make_shared<BinaryOperation<OpMult>>(cose, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cos) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto sine = std::make_shared<StandardFunction<::sin>>(ce);
        return std::make_shared<BinaryOperation<OpMult>>(
            std::make_shared<Negation>(sine), de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tan) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto tane = std::make_shared<StandardFunction<::tan>>(ce);
        auto tan2e = std::make_shared<BinaryOperation<OpMult>>(tane, tane);
        auto tan2e_ = std::make_shared<BinaryOperation<OpPlus>>(
            std::make_shared<Number>(1.), tan2e);
        return std::make_shared<BinaryOperation<OpMult>>(tan2e_, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sqrt) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto sqrte = std::make_shared<StandardFunction<::sqrt>>(ce);
        auto sqrte_ = std::make_shared<BinaryOperation<OpDiv>>(
            std::make_shared<Number>(0.5), sqrte);
        return std::make_shared<BinaryOperation<OpMult>>(sqrte_, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        return std::make_shared<BinaryOperation<OpDiv>>(de, ce);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(log10) {
        static double ln10 = std::log(10.);
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto de_ = std::make_shared<BinaryOperation<OpMult>>(
            std::make_shared<Number>(ln10), de);
        return std::make_shared<BinaryOperation<OpDiv>>(de_, ce);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(asin) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
        auto den2 = std::make_shared<BinaryOperation<OpMinus>>(
            std::make_shared<Number>(1.), den1);
        auto den3 = std::make_shared<StandardFunction<::sqrt>>(den2);
        return std::make_shared<BinaryOperation<OpDiv>>(de, den3);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(acos) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto num1 = std::make_shared<Number>(-1.);
        auto num2 = std::make_shared<BinaryOperation<OpMult>>(num1, de);
        auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
        auto den2 = std::make_shared<BinaryOperation<OpMinus>>(
            std::make_shared<Number>(1.), den1);
        auto den3 = std::make_shared<StandardFunction<::sqrt>>(den2);
        return std::make_shared<BinaryOperation<OpDiv>>(num2, den3);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(atan) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto den1 = std::make_shared<BinaryOperation<OpMult>>(ce, ce);
        auto den2 = std::make_shared<BinaryOperation<OpPlus>>(
            std::make_shared<Number>(1.), den1);
        return std::make_shared<BinaryOperation<OpDiv>>(de, den2);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(sinh) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto coshe = std::make_shared<StandardFunction<::cosh>>(ce);
        return std::make_shared<BinaryOperation<OpMult>>(coshe, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(cosh) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto sinhe = std::make_shared<StandardFunction<::sinh>>(ce);
        return std::make_shared<BinaryOperation<OpMult>>(sinhe, de);
      }  // end of differentiateFunction

      TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DEFINITION(tanh) {
        auto ce = expr->clone(v);
        auto de = expr->differentiate(pos, v);
        auto coshe = std::make_shared<StandardFunction<::cosh>>(ce);
        auto coshe2 = std::make_shared<BinaryOperation<OpMult>>(coshe, coshe);
        return std::make_shared<BinaryOperation<OpDiv>>(de, coshe2);
      }  // end of differentiateFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
