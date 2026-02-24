/*!
 * \file   src/Math/BinaryOperator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include <string>
#include <sstream>
#include <stdexcept>
#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/Negation.hxx"
#include "TFEL/Math/Parser/Function.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"

namespace tfel::math::parser {

  double OpPlus::apply(const double a, const double b) {
    return a + b;
  }  // end of OpPlus::apply

  std::string OpPlus::getCxxFormula(const std::string& a,
                                    const std::string& b) {
    return '(' + a + ")+(" + b + ')';
  }  // end of OpPlus::getCxxFormula

  double OpMinus::apply(const double a, const double b) {
    return a - b;
  }  // end of OpMinus::apply

  std::string OpMinus::getCxxFormula(const std::string& a,
                                     const std::string& b) {
    return '(' + a + ")-(" + b + ')';
  }  // end of OpMinus::getCxxFormula

  double OpMult::apply(const double a, const double b) {
    return a * b;
  }  // end of OpMult::apply

  std::string OpMult::getCxxFormula(const std::string& a,
                                    const std::string& b) {
    return '(' + a + ")*(" + b + ')';
  }  // end of OpMult::getCxxFormula

  double OpDiv::apply(const double a, const double b) {
    raise_if(std::abs(b) < std::numeric_limits<double>::min(),
             "OpDiv::apply : second argument is too small "
             "(" +
                 std::to_string(b) + ")");
    return a / b;
  }  // end of OpDiv::apply

  std::string OpDiv::getCxxFormula(const std::string& a, const std::string& b) {
    return '(' + a + ")/(" + b + ')';
  }  // end of OpDiv::getCxxFormula

  double OpPower::apply(const double a, const double b) {
    return std::pow(a, b);
  }  // end of OpDiv::apply

  std::string OpPower::getCxxFormula(const std::string& a,
                                     const std::string& b) {
    return "std::pow(" + a + ',' + b + ')';
  }  // end of OpPower::getCxxFormula

  void BinaryOperationBase::throwUnimplementedDifferentiateFunctionException() {
    raise(
        "BinaryOperationBase::"
        "throwUnimplementedDifferentiateFunctionException : "
        "unimplemented feature");
  }  // end of
     // BinaryOperationBase::throwUnimplementedDifferentiateFunctionException()

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpPlus>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    } else if (ba && (!bb)) {
      return a->differentiate(pos, v);
    } else if (!(ba) && bb) {
      return b->differentiate(pos, v);
    }
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
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    } else if (ba && (!bb)) {
      return a->differentiate(pos, v);
    } else if (!(ba) && bb) {
      return std::make_shared<Negation>(b->differentiate(pos, v));
    }
    return std::make_shared<BinaryOperation<OpMinus>>(a->differentiate(pos, v),
                                                      b->differentiate(pos, v));
  }  // end of differentiateBinaryOperation<OpMinus>

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpMult>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    } else if (ba && (!bb)) {
      return std::make_shared<BinaryOperation<OpMult>>(a->differentiate(pos, v),
                                                       b->clone(v));
    } else if (!(ba) && bb) {
      return std::make_shared<BinaryOperation<OpMult>>(
          a->clone(v), b->differentiate(pos, v));
    }
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
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    } else if (ba && (!bb)) {
      return std::make_shared<BinaryOperation<OpDiv>>(a->differentiate(pos, v),
                                                      b->clone(v));
    } else if (!(ba) && bb) {
      auto d2num = std::make_shared<BinaryOperation<OpMult>>(
          a->clone(v), b->differentiate(pos, v));
      auto d2den =
          std::make_shared<BinaryOperation<OpMult>>(b->clone(v), b->clone(v));
      auto d2 = std::make_shared<BinaryOperation<OpDiv>>(d2num, d2den);
      return std::make_shared<Negation>(d2);
    }
    auto d1 = std::make_shared<BinaryOperation<OpDiv>>(a->differentiate(pos, v),
                                                       b->clone(v));
    auto d2num = std::make_shared<BinaryOperation<OpMult>>(
        a->clone(v), b->differentiate(pos, v));
    auto d2den =
        std::make_shared<BinaryOperation<OpMult>>(b->clone(v), b->clone(v));
    auto d2 = std::make_shared<BinaryOperation<OpDiv>>(d2num, d2den);
    return std::make_shared<BinaryOperation<OpMinus>>(d1, d2);
  }  // end of differentiateBinaryOperation<OpDiv>

  /*!
   * \brief expression describing the derivative of a^b with respect to b.
   */
  struct ExponentDerivative final : public Expr {
    /*!
     * \brief constructor
     * \param[in] e_a: expression associated to a
     * \param[in] e_b: expression associated to b
     * \param[in] e_db: expression associated with the differential of b
     */
    ExponentDerivative(const std::shared_ptr<Expr>& e_a,
                       const std::shared_ptr<Expr>& e_b,
                       const std::shared_ptr<Expr>& e_db)
        : a(e_a), b(e_b), db(e_db) {
      auto tmp1 = std::make_shared<StandardFunction<log>>("log", this->a);
      auto tmp2 = std::make_shared<BinaryOperation<OpPower>>(this->a, this->b);
      auto tmp3 = std::make_shared<BinaryOperation<OpMult>>(tmp1, tmp2);
      this->derivative = applyChainRule(tmp3, this->db);
    }  // end of ExponentDerivative
    bool dependsOnVariable(
        const std::vector<double>::size_type pos) const override {
      return this->derivative->dependsOnVariable(pos);
    }  // end of dependsOnVariable
    bool isConstant() const override {
      return this->derivative->isConstant();
    }  // end of isConstant
    double getValue() const override {
      const auto va = this->a->getValue();
      const auto vb = this->b->getValue();
      if ((tfel::math::ieee754::fpclassify(va) == FP_ZERO) && (vb > 0)) {
        return 0.;
      }
      return this->derivative->getValue();
    }  // end of getValue
    void checkCyclicDependency(
        std::vector<std::string>& variables) const override {
      this->derivative->checkCyclicDependency(variables);
    }  // end of checkCyclicDependency
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>& values) const override {
      return this->derivative->resolveDependencies(values);
    }  // end of resolveDependencies
    std::shared_ptr<Expr> clone(
        const std::vector<double>& values) const override {
      return this->derivative->clone(values);
    }  // end of clone
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type pos,
        const std::vector<double>& values) const override {
      return this->derivative->differentiate(pos, values);
    }  // end of differentiate
    void getParametersNames(std::set<std::string>& parameters) const override {
      this->derivative->getParametersNames(parameters);
    }  // end of getParametersNames
    std::string getCxxFormula(
        const std::vector<std::string>& variables) const override {
      const auto df = this->derivative->getCxxFormula(variables);
      if (this->a->isConstant()) {
        return df;
      }
      const auto af = this->a->getCxxFormula(variables);
      const auto bf = this->b->getCxxFormula(variables);
      return "(((tfel::math::ieee754::fpclassify(" + af + ")==FP_ZERO)&&(" +
             bf + " > 0)) ? 0 : " + df + ")";
    }  // end of getCxxFormula
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>& values,
        const std::vector<std::string>& variables,
        const std::map<std::string, std::vector<double>::size_type>& mapping)
        const override {
      return this->derivative->createFunctionByChangingParametersIntoVariables(
          values, variables, mapping);
    }  // end of createFunctionByChangingParametersIntoVariables
    //! \brief destructor
    ~ExponentDerivative() override = default;

   private:
    //! \brief expression associated with a
    std::shared_ptr<Expr> a;
    //! \brief expression associated with b
    std::shared_ptr<Expr> b;
    //! \brief expression associated with the derivative of b
    std::shared_ptr<Expr> db;
    //! \brief expression of the derivative
    std::shared_ptr<Expr> derivative;
  };  // end of ExponentDerivative

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpPower>(
      const std::shared_ptr<Expr> a,
      const std::shared_ptr<Expr> b,
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) {
    const auto ba = a->dependsOnVariable(pos);
    const auto bb = b->dependsOnVariable(pos);
    if ((!ba) && (!bb)) {
      return Number::zero();
    }
    auto ca = a->clone(v);
    auto cb = b->clone(v);
    auto derivative_wrt_a = [&a, &ca, &cb, pos, &v] {
      auto e = [&cb]() -> std::shared_ptr<Expr> {
        if (cb->isConstant()) {
          const auto ev = cb->getValue() - 1;
          return std::make_shared<Number>(std::to_string(ev), ev);
        }
        auto one = Number::one();
        return std::make_shared<BinaryOperation<OpMinus>>(cb, one);
      }();
      auto tmp1 = std::make_shared<BinaryOperation<OpPower>>(ca, e);
      auto tmp2 = std::make_shared<BinaryOperation<OpMult>>(cb, tmp1);
      auto da = a->differentiate(pos, v);
      return applyChainRule(tmp2, da);
    };
    auto derivative_wrt_b = [&b, &ca, &cb, pos, &v] {
      auto db = b->differentiate(pos, v);
      return std::make_shared<ExponentDerivative>(ca, cb, db);
    };
    if (ba && (!bb)) {
      return derivative_wrt_a();
    } else if (!(ba) && bb) {
      return derivative_wrt_b();
    }
    const auto d1 = derivative_wrt_a();
    const auto d2 = derivative_wrt_b();
    return std::make_shared<BinaryOperation<OpPlus>>(d1, d2);
  }  // end of differentiateBinaryOperation<OpPower>

}  // end of namespace tfel::math::parser
