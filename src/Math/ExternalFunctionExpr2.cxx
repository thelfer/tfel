/*!
 * \file   src/Math/ExternalFunctionExpr2.cxx
 * \brief
 *
 * \author Thomas Helfer
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

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel::math::parser {

  ExternalFunctionExpr2::ExternalFunctionExpr2(
      std::shared_ptr<ExternalFunction> ff,
      std::vector<std::shared_ptr<Expr>>& fargs)
      : f(std::move(ff)), args(fargs) {
    raise_if(f->getNumberOfVariables() != args.size(),
             "ExternalFunctionExpr2::ExternalFunctionExpr2: "
             "invalid number of arguments for function "
             "(" +
                 std::to_string(this->args.size()) + " given, " +
                 std::to_string(f->getNumberOfVariables()) + " required)");
  }  // end of ExternalFunctionExpr2

  bool ExternalFunctionExpr2::isConstant() const {
    for (const auto& a : this->args) {
      if (!a->isConstant()) {
        return false;
      }
    }
    return true;
  }  // end of isConstant

  bool ExternalFunctionExpr2::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    for (const auto& a : this->args) {
      if (a->dependsOnVariable(p)) {
        return true;
      }
    }
    return false;
  }  // end of dependsOnVariable

  double ExternalFunctionExpr2::getValue() const {
    using namespace tfel::math::parser;
    auto p = this->args.begin();
    auto i = std::vector<std::shared_ptr<Expr>>::size_type{};
    for (; p != this->args.end(); ++p, ++i) {
      const auto val = (*p)->getValue();
      this->f->setVariableValue(i, val);
    }
    return this->f->getValue();
  }  // end of getValue

  std::string ExternalFunctionExpr2::getCxxFormula(
      const std::vector<std::string>&) const {
    tfel::raise(
        "ExternalFunctionExpr2::getCxxFormula: "
        "unimplemented feature");
  }  // end of getCxxFormula

  void ExternalFunctionExpr2::checkCyclicDependency(
      std::vector<std::string>& names) const {
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::string> a;
    for (p = this->args.begin(); p != this->args.end(); ++p) {
      std::vector<std::string> n(a);
      (*p)->checkCyclicDependency(n);
      mergeVariablesNames(names, n);
    }
  }  // end of checkCyclicDependency

  void ExternalFunctionExpr2::getParametersNames(
      std::set<std::string>& p) const {
    for (const auto& a : this->args) {
      a->getParametersNames(p);
    }
  }  // end of getParametersNames

  std::shared_ptr<Expr> ExternalFunctionExpr2::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    auto p = this->args.begin();
    unsigned short i = 0;
    if (this->args.empty()) {
      return Number::zero();
    }
    auto p3 = this->args.begin();
    auto p4 = nargs.begin();
    for (; p3 != this->args.end(); ++p3, ++p4) {
      *p4 = (*p3)->clone(v);
    }
    auto df_ = std::make_shared<ExternalFunctionExpr2>(
        this->f->differentiate(i), nargs);
    std::shared_ptr<Expr> df = std::make_shared<BinaryOperation<OpMult>>(
        df_, (*p)->differentiate(pos, v));
    ++p;
    ++i;
    while (p != this->args.end()) {
      df_ = std::make_shared<ExternalFunctionExpr2>(this->f->differentiate(i),
                                                    nargs);
      std::shared_ptr<Expr> df2 = std::make_shared<BinaryOperation<OpMult>>(
          df_, (*p)->differentiate(pos, v));
      df = std::make_shared<BinaryOperation<OpPlus>>(df, df2);
      ++p;
      ++i;
    }
    return df;
  }  // end of differentiate

  std::shared_ptr<Expr>
  ExternalFunctionExpr2::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& params,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::iterator p2;
    for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
         ++p, ++p2) {
      *p2 =
          (*p)->createFunctionByChangingParametersIntoVariables(v, params, pos);
    }
    return std::make_shared<ExternalFunctionExpr2>(this->f, nargs);
  }  // end of createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> ExternalFunctionExpr2::clone(
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::iterator p2;
    for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
         ++p, ++p2) {
      *p2 = (*p)->clone(v);
    }
    return std::make_shared<ExternalFunctionExpr2>(this->f, nargs);
  }  // end of clone

  std::shared_ptr<Expr> ExternalFunctionExpr2::resolveDependencies(
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::iterator p2;
    for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
         ++p, ++p2) {
      *p2 = (*p)->resolveDependencies(v);
    }
    return std::make_shared<ExternalFunctionExpr2>(
        this->f->resolveDependencies(), nargs);
  }  // end of resolveDependencies

  ExternalFunctionExpr2::~ExternalFunctionExpr2() = default;

}  // end of namespace tfel::math::parser
