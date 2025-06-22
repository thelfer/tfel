/*!
 * \file   src/Math/ExternalFunctionExpr.cxx
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
#include <algorithm>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/Variable.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/ExternalFunctionExpr.hxx"
#include "TFEL/Math/Parser/ExternalFunctionExpr2.hxx"

namespace tfel::math::parser {

  ExternalFunctionExpr::ExternalFunctionExpr(
      const std::string& fname,
      std::vector<std::shared_ptr<Expr>>& fargs,
      std::shared_ptr<tfel::math::parser::ExternalFunctionManager>& m)
      : name(fname), args(fargs), manager(m) {}  // end of ExternalFunctionExpr

  bool ExternalFunctionExpr::isConstant() const {
    for (const auto& a : this->args) {
      if (!a->isConstant()) {
        return false;
      }
    }
    return true;
  }  // end of isConstant

  bool ExternalFunctionExpr::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    for (const auto& a : this->args) {
      if (a->dependsOnVariable(p)) {
        return true;
      }
    }
    return false;
  }  // end of dependsOnVariable

  double ExternalFunctionExpr::getValue() const {
    using namespace tfel::math::parser;
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::size_type i;
    auto p2 = this->manager->find(this->name);
    raise_if(p2 == this->manager->end(),
             "ExternalFunctionExpr::getValue: "
             "unknown function '" +
                 this->name + "'");
    raise_if(p2->second->getNumberOfVariables() != this->args.size(),
             "ExternalFunctionExpr::getValue: "
             "invalid number of arguments for function '" +
                 this->name +
                 "' "
                 "(" +
                 std::to_string(this->args.size()) + " given, " +
                 std::to_string(p2->second->getNumberOfVariables()) +
                 " required)");
    for (p = this->args.begin(), i = 0u; p != this->args.end(); ++p, ++i) {
      const auto val = (*p)->getValue();
      p2->second->setVariableValue(i, val);
    }
    return p2->second->getValue();
  }  // end of getValue

  std::string ExternalFunctionExpr::getCxxFormula(
      const std::vector<std::string>&) const {
    tfel::raise(
        "ExternalFunctionExpr::getCxxFormula: "
        "unimplemented feature");
  }  // end of getCxxFormula

  void ExternalFunctionExpr::checkCyclicDependency(
      std::vector<std::string>& names) const {
    std::vector<std::string> pnames;
    raise_if(std::find(names.begin(), names.end(), this->name) != names.end(),
             "ExternalFunctionExpr::checkCyclicDependency: "
             "cyclic dependency found on function '" +
                 this->name + "'");
    const auto p2 = this->manager->find(this->name);
    raise_if(p2 == this->manager->end(),
             "ExternalFunctionExpr::checkCyclicDependency: "
             "unknown function '" +
                 this->name + "'");
    pnames = names;
    names.push_back(this->name);
    p2->second->checkCyclicDependency(names);
    for (const auto& a : this->args) {
      std::vector<std::string> n(pnames);
      a->checkCyclicDependency(n);
      mergeVariablesNames(names, n);
    }
  }  // end of checkCyclicDependency

  std::shared_ptr<Expr> ExternalFunctionExpr::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    auto p = this->args.begin();
    std::vector<std::shared_ptr<Expr>>::const_iterator p3;
    std::vector<std::shared_ptr<Expr>>::iterator p4;
    unsigned short i = 0;
    if (args.empty()) {
      return Number::zero();
    }
    const auto p2 = this->manager->find(this->name);
    raise_if(p2 == this->manager->end(),
             "ExternalFunctionExpr::differentiate: "
             "unknown function '" +
                 this->name + "'");
    raise_if(p2->second->getNumberOfVariables() != this->args.size(),
             "ExternalFunctionExpr::getValue: "
             "invalid number of arguments for function "
             "'" +
                 this->name +
                 "' "
                 "(" +
                 std::to_string(this->args.size()) + " given, " +
                 std::to_string(p2->second->getNumberOfVariables()) +
                 " required)");
    for (p3 = this->args.begin(), p4 = nargs.begin(); p3 != this->args.end();
         ++p3, ++p4) {
      *p4 = (*p3)->clone(v);
    }
    auto df = std::shared_ptr<Expr>{};
    auto df_ = std::make_shared<ExternalFunctionExpr2>(
        p2->second->differentiate(i), nargs);
    df = std::make_shared<BinaryOperation<OpMult>>(df_,
                                                   (*p)->differentiate(pos, v));
    ++p;
    ++i;
    while (p != this->args.end()) {
      df_ = std::make_shared<ExternalFunctionExpr2>(
          p2->second->differentiate(i), nargs);
      auto df2 = std::make_shared<BinaryOperation<OpMult>>(
          df_, (*p)->differentiate(pos, v));
      df = std::make_shared<BinaryOperation<OpPlus>>(df, df2);
      ++p;
      ++i;
    }
    return df;
  }  // end of differentiate

  std::shared_ptr<Expr> ExternalFunctionExpr::clone(
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::iterator p2;
    for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
         ++p, ++p2) {
      *p2 = (*p)->clone(v);
    }
    return std::make_shared<ExternalFunctionExpr>(this->name, nargs,
                                                  this->manager);
  }  // end of clone

  std::shared_ptr<Expr>
  ExternalFunctionExpr::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& params,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    std::vector<std::shared_ptr<Expr>>::const_iterator p;
    std::vector<std::shared_ptr<Expr>>::iterator p2;
    std::vector<std::shared_ptr<Expr>> nargs;
    std::vector<std::string> vnames;
    std::vector<std::string>::size_type i;
    if (this->args.empty()) {
      if (find(params.begin(), params.end(), this->name) != params.end()) {
        auto p3 = pos.find(this->name);
        raise_if(p3 == pos.end(),
                 "ExternalFunctionExpr::"
                 "createFunctionByChangingParametersIntoVariables: "
                 "internal error (no position found for "
                 "parameter '" +
                     this->name + "')");
        return std::shared_ptr<Expr>(new Variable(v, p3->second));
      }
    }
    const auto pf = this->manager->find(this->name);
    raise_if(pf == this->manager->end(),
             "ExternalFunctionExpr::"
             "createFunctionByChangingParametersIntoVariables: "
             "no function '" +
                 this->name + "' declared");
    raise_if(pf->second->getNumberOfVariables() != this->args.size(),
             "ExternalFunctionExpr::getValue: "
             "invalid number of arguments for function "
             "'" +
                 this->name +
                 "' "
                 "(" +
                 std::to_string(this->args.size()) + " given, " +
                 std::to_string(pf->second->getNumberOfVariables()) +
                 " required)");
    auto nf = pf->second->createFunctionByChangingParametersIntoVariables(
        vnames, v, params, pos);
    raise_if(nf->getNumberOfVariables() < this->args.size(),
             "ExternalFunctionExpr::getValue : "
             "internal error (function as less variable after "
             "'createFunctionByChangingParametersIntoVariables' "
             "than before");
    nargs.resize(nf->getNumberOfVariables());
    for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
         ++p, ++p2) {
      *p2 =
          (*p)->createFunctionByChangingParametersIntoVariables(v, params, pos);
    }
    for (i = 0; i != vnames.size(); ++i) {
      const auto p3 = pos.find(vnames[i]);
      raise_if(p3 == pos.end(),
               "ExternalFunctionExpr::"
               "createFunctionByChangingParametersIntoVariables: "
               "internal error (no position found for "
               "parameter '" +
                   vnames[i] + "')");
      nargs[args.size() + i] =
          std::shared_ptr<Expr>(new Variable(v, p3->second));
    }
    return std::shared_ptr<Expr>(new ExternalFunctionExpr2(nf, nargs));
  }  // end of createFunctionByChangingParametersIntoVariables

  void ExternalFunctionExpr::getParametersNames(
      std::set<std::string>& p) const {
    if (this->args.empty()) {
      p.insert(this->name);
    } else {
      auto pf = this->manager->find(this->name);
      raise_if(pf == this->manager->end(),
               "ExternalFunctionExpr::getParametersNames: "
               "no function '" +
                   this->name + "' declared");
      pf->second->getParametersNames(p);
      for (const auto& a : this->args) {
        a->getParametersNames(p);
      }
    }
  }  // end of getParametersNames

  std::shared_ptr<Expr> ExternalFunctionExpr::resolveDependencies(
      const std::vector<double>& v) const {
    std::vector<std::shared_ptr<Expr>> nargs(this->args.size());
    auto p = this->args.begin();
    auto p2 = nargs.begin();
    for (; p != this->args.end(); ++p, ++p2) {
      *p2 = (*p)->resolveDependencies(v);
    }
    auto p3 = this->manager->find(this->name);
    raise_if(p3 == this->manager->end(),
             "ExternalFunctionExpr::getValue: "
             "unknown function '" +
                 this->name + "'");
    raise_if(p3->second->getNumberOfVariables() != this->args.size(),
             "ExternalFunctionExpr::getValue: "
             "invalid number of arguments for function "
             "'" +
                 this->name +
                 "' "
                 "(" +
                 std::to_string(this->args.size()) + " given, " +
                 std::to_string(p3->second->getNumberOfVariables()) +
                 " required)");
    return std::make_shared<ExternalFunctionExpr2>(
        p3->second->resolveDependencies(), nargs);
  }  // end of resolveDependencies

  ExternalFunctionExpr::~ExternalFunctionExpr() = default;

}  // end of namespace tfel::math::parser
