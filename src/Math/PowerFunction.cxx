/*!
 * \file   src/Math/PowerFunction.cxx
 * \brief
 * \author Thomas Heler
 * \date   25/04/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include "TFEL/Math/Parser/PowerFunction.hxx"

namespace tfel::math::parser {

  GeneralPowerFunction::GeneralPowerFunction(const std::shared_ptr<Expr> e,
                                             const int nv) noexcept
      : Function(e), n(nv) {}  // end of GeneralPowerFunction

  double GeneralPowerFunction::getValue() const {
    using namespace std;
    const auto arg = this->expr->getValue();
    int old = errno;
    errno = 0;
    const auto res = std::pow(arg, this->n);
    if (errno != 0) {
      int e = errno;
      errno = old;
      FunctionBase::throwInvalidCallException(arg, e);
    }
    errno = old;
    return res;
  }  // end of getValue

  std::string GeneralPowerFunction::getCxxFormula(
      const std::vector<std::string>& m) const {
    const auto a = this->expr->getCxxFormula(m);
    return "std::pow(" + a + "," + std::to_string(this->n) + ")";
  }  // end of getCxxFormula()

  std::shared_ptr<Expr> GeneralPowerFunction::resolveDependencies(
      const std::vector<double>& v) const {
    return std::make_shared<GeneralPowerFunction>(
        this->expr->resolveDependencies(v), this->n);
  }  // end of resolveDependencies

  std::shared_ptr<Expr> GeneralPowerFunction::clone(
      const std::vector<double>& v) const {
    return std::make_shared<GeneralPowerFunction>(this->expr->clone(v),
                                                  this->n);
  }  // end of clone

  std::shared_ptr<Expr>
  GeneralPowerFunction::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& p,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    auto nexpr =
        this->expr->createFunctionByChangingParametersIntoVariables(v, p, pos);
    return std::make_shared<GeneralPowerFunction>(nexpr, this->n);
  }  // end of createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> GeneralPowerFunction::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    auto dp = [this, &v]() -> std::shared_ptr<Expr> {
      auto Nf = std::make_shared<Number>(std::to_string(this->n), this->n);
      if (this->n == 17) {
#ifdef __NVCOMPILER
        auto ptr = new PowerFunction<16>(this->expr->clone(v));
        auto e = std::shared_ptr<Expr>(ptr);
        auto b = std::make_shared<BinaryOperation<OpMult>>(Nf, e);
        return b;
#else /* __NVCOMPILER */
        return std::make_shared<BinaryOperation<OpMult>>(
            Nf, std::make_shared<PowerFunction<16>>(this->expr->clone(v)));

#endif /* __NVCOMPILER */
      }
      return std::make_shared<BinaryOperation<OpMult>>(
          Nf, std::make_shared<GeneralPowerFunction>(this->expr->clone(v),
                                                     this->n - 1));
    }();
    auto de = expr->differentiate(pos, v);
    return applyChainRule(dp, de);
  }  // end of differentiate

  GeneralPowerFunction::~GeneralPowerFunction() = default;

}  // end of namespace tfel::math::parser
