/*!
 * \file   include/TFEL/Math/Parser/LogicalExpr.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02/11/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_LOGICALEXPR_IXX
#define LIB_TFEL_MATH_PARSER_LOGICALEXPR_IXX

#include <vector>
#include <string>
#include <memory>

namespace tfel::math::parser {

  template <typename Op>
  LogicalOperation<Op>::LogicalOperation(
      const std::shared_ptr<Expr> a_, const std::shared_ptr<Expr> b_) noexcept
      : a(a_), b(b_) {}  // end of LogicalOperation

  template <typename Op>
  bool LogicalOperation<Op>::isConstant() const {
    return (this->a->isConstant()) && (this->b->isConstant());
  }  // end of isConstant

  template <typename Op>
  bool LogicalOperation<Op>::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return this->a->dependsOnVariable(p) || this->b->dependsOnVariable(p);
  }  // end of dependsOnVariable

  template <typename Op>
  bool LogicalOperation<Op>::getValue() const {
    return Op::apply(this->a->getValue(), this->b->getValue());
  }  // end of getValue

  template <typename Op>
  std::string LogicalOperation<Op>::getCxxFormula(
      const std::vector<std::string>& m) const {
    return Op::getCxxFormula(this->a->getCxxFormula(m),
                             this->b->getCxxFormula(m));
  }  // end of getCxxFormula

  template <typename Op>
  void LogicalOperation<Op>::checkCyclicDependency(
      std::vector<std::string>& vars) const {
    std::vector<std::string> a_vars(vars);
    std::vector<std::string> b_vars(vars);
    this->a->checkCyclicDependency(a_vars);
    this->b->checkCyclicDependency(b_vars);
    mergeVariablesNames(vars, a_vars);
    mergeVariablesNames(vars, b_vars);
  }  // end of checkCyclicDependency

  template <typename Op>
  std::shared_ptr<LogicalExpr> LogicalOperation<Op>::resolveDependencies(
      const std::vector<double>& v) const {
    return std::make_shared<LogicalOperation<Op>>(
        this->a->resolveDependencies(v), this->b->resolveDependencies(v));
  }  // end of resolveDependencies() const

  template <typename Op>
  std::shared_ptr<LogicalExpr> LogicalOperation<Op>::clone(
      const std::vector<double>& v) const {
    return std::make_shared<LogicalOperation<Op>>(this->a->clone(v),
                                                  this->b->clone(v));
  }  // end of clone

  template <typename Op>
  std::shared_ptr<LogicalExpr>
  LogicalOperation<Op>::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& p,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    return std::make_shared<LogicalOperation<Op>>(
        this->a->createFunctionByChangingParametersIntoVariables(v, p, pos),
        this->b->createFunctionByChangingParametersIntoVariables(v, p, pos));
  }  // end of createFunctionByChangingParametersIntoVariables

  template <typename Op>
  void LogicalOperation<Op>::getParametersNames(
      std::set<std::string>& p) const {
    this->a->getParametersNames(p);
    this->b->getParametersNames(p);
  }  // end of getParametersNames

  template <typename Op>
  LogicalOperation<Op>::~LogicalOperation() = default;

  template <typename Op>
  LogicalBinaryOperation<Op>::LogicalBinaryOperation(
      const std::shared_ptr<LogicalExpr> a_,
      const std::shared_ptr<LogicalExpr> b_) noexcept
      : a(a_), b(b_) {}  // end of LogicalBinaryOperation

  template <typename Op>
  bool LogicalBinaryOperation<Op>::isConstant() const {
    return this->a->isConstant() && this->b->isConstant();
  }  // end of isConstant

  template <typename Op>
  bool LogicalBinaryOperation<Op>::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return this->a->dependsOnVariable(p) || this->b->dependsOnVariable(p);
  }  // end of dependsOnVariable

  template <typename Op>
  bool LogicalBinaryOperation<Op>::getValue() const {
    return Op::apply(this->a->getValue(), this->b->getValue());
  }  // end of getValue

  template <typename Op>
  std::string LogicalBinaryOperation<Op>::getCxxFormula(
      const std::vector<std::string>& m) const {
    return Op::getCxxFormula(this->a->getCxxFormula(m),
                             this->b->getCxxFormula(m));
  }  // end of getCxxFormula

  template <typename Op>
  void LogicalBinaryOperation<Op>::checkCyclicDependency(
      std::vector<std::string>& vars) const {
    std::vector<std::string> a_vars(vars);
    std::vector<std::string> b_vars(vars);
    this->a->checkCyclicDependency(a_vars);
    this->b->checkCyclicDependency(b_vars);
    mergeVariablesNames(vars, a_vars);
    mergeVariablesNames(vars, b_vars);
  }  // end of checkCyclicDependency

  template <typename Op>
  void LogicalBinaryOperation<Op>::getParametersNames(
      std::set<std::string>& p) const {
    this->a->getParametersNames(p);
    this->b->getParametersNames(p);
  }  // end of getParametersNames

  template <typename Op>
  std::shared_ptr<LogicalExpr> LogicalBinaryOperation<Op>::resolveDependencies(
      const std::vector<double>& v) const {
    return std::make_shared<LogicalBinaryOperation<Op>>(
        this->a->resolveDependencies(v), this->b->resolveDependencies(v));
  }  // end of resolveDependencies

  template <typename Op>
  std::shared_ptr<LogicalExpr> LogicalBinaryOperation<Op>::clone(
      const std::vector<double>& v) const {
    return std::make_shared<LogicalBinaryOperation<Op>>(this->a->clone(v),
                                                        this->b->clone(v));
  }  // end of clone

  template <typename Op>
  std::shared_ptr<LogicalExpr>
  LogicalBinaryOperation<Op>::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& p,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    return std::make_shared<LogicalBinaryOperation<Op>>(
        this->a->createFunctionByChangingParametersIntoVariables(v, p, pos),
        this->b->createFunctionByChangingParametersIntoVariables(v, p, pos));
  }  // end of createFunctionByChangingParametersIntoVariables

  template <typename Op>
  LogicalBinaryOperation<Op>::~LogicalBinaryOperation() = default;

}  // namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_LOGICALEXPR_IXX */
