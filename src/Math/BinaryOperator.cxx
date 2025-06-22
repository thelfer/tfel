/*!
 * \file   src/Math/BinaryOperator.cxx
 * \brief
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <limits>
#include <stdexcept>
#include <string>
#include <cmath>
#include <sstream>

#include "TFEL/Math/Parser/Function.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      double OpPlus::apply(const double a, const double b) {
        return a + b;
      }  // end of OpPlus::apply

      double OpMinus::apply(const double a, const double b) {
        return a - b;
      }  // end of OpMinus::apply

      double OpMult::apply(const double a, const double b) {
        return a * b;
      }  // end of OpMult::apply

      double OpDiv::apply(const double a, const double b) {
        if (std::abs(b) < std::numeric_limits<double>::min()) {
          throw(
              std::runtime_error("OpDiv::apply : second argument is too small "
                                 "(" +
                                 std::to_string(b) + ")"));
        }
        return a / b;
      }  // end of OpDiv::apply

      double OpPower::apply(const double a, const double b) {
        return std::pow(a, b);
      }  // end of OpDiv::apply

      void
      BinaryOperationBase::throwUnimplementedDifferentiateFunctionException() {
        throw(std::runtime_error(
            "BinaryOperationBase::"
            "throwUnimplementedDifferentiateFunctionException : "
            "unimplemented feature"));
      }  // end of
         // BinaryOperationBase::throwUnimplementedDifferentiateFunctionException()

      template <>
      std::shared_ptr<Expr> differentiateBinaryOperation<OpPlus>(
          const std::shared_ptr<Expr> a,
          const std::shared_ptr<Expr> b,
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) {
        auto da = a->differentiate(pos, v);
        auto db = b->differentiate(pos, v);
        return std::make_shared<BinaryOperation<OpPlus>>(da, db);
      }  // end of differentiateBinaryOperation<OpPlus>

      template <>
      std::shared_ptr<Expr> differentiateBinaryOperation<OpMinus>(
          const std::shared_ptr<Expr> a,
          const std::shared_ptr<Expr> b,
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) {
        return std::make_shared<BinaryOperation<OpMinus>>(
            a->differentiate(pos, v), b->differentiate(pos, v));
      }  // end of differentiateBinaryOperation<OpMinus>

      template <>
      std::shared_ptr<Expr> differentiateBinaryOperation<OpMult>(
          const std::shared_ptr<Expr> a,
          const std::shared_ptr<Expr> b,
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) {
        auto d1 = std::make_shared<BinaryOperation<OpMult>>(
            a->differentiate(pos, v), b->clone(v));
        auto d2 = std::make_shared<BinaryOperation<OpMult>>(
            a->clone(v), b->differentiate(pos, v));
        return std::make_shared<BinaryOperation<OpPlus>>(d1, d2);
      }  // end of differentiateBinaryOperation<OpMult>

      template <>
      std::shared_ptr<Expr> differentiateBinaryOperation<OpDiv>(
          const std::shared_ptr<Expr> a,
          const std::shared_ptr<Expr> b,
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) {
        auto d1 = std::make_shared<BinaryOperation<OpDiv>>(
            a->differentiate(pos, v), b->clone(v));
        auto d2num = std::make_shared<BinaryOperation<OpMult>>(
            a->clone(v), b->differentiate(pos, v));
        auto d2den =
            std::make_shared<BinaryOperation<OpMult>>(b->clone(v), b->clone(v));
        auto d2 = std::make_shared<BinaryOperation<OpDiv>>(d2num, d2den);
        return std::make_shared<BinaryOperation<OpMinus>>(d1, d2);
      }  // end of differentiateBinaryOperation<OpDiv>

      template <>
      std::shared_ptr<Expr> differentiateBinaryOperation<OpPower>(
          const std::shared_ptr<Expr> a,
          const std::shared_ptr<Expr> b,
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) {
        auto ca = a->clone(v);
        auto cb = b->clone(v);
        auto l = std::make_shared<BinaryOperation<OpPower>>(ca, cb);
        auto da = a->differentiate(pos, v);
        auto db = b->differentiate(pos, v);
        auto lna = std::make_shared<StandardFunction<log>>(ca);
        auto r1 = std::make_shared<BinaryOperation<OpMult>>(db, lna);
        auto r21 = std::make_shared<BinaryOperation<OpDiv>>(da, ca);
        auto r2 = std::make_shared<BinaryOperation<OpMult>>(cb, r21);
        auto r = std::make_shared<BinaryOperation<OpPlus>>(r1, r2);
        return std::make_shared<BinaryOperation<OpMult>>(l, r);
      }  // end of differentiateBinaryOperation<OpPower>

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
