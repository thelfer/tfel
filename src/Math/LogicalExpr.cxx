/*!
 * \file   src/Math/LogicalExpr.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <limits>
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Math/Parser/LogicalExpr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      bool OpEqual::apply(const double a, const double b) {
        return tfel::math::ieee754::fpclassify(std::abs(a - b)) == FP_ZERO;
      }  // end of OpEqual::apply

      std::string OpEqual::getCxxFormula(const std::string& a,
                                         const std::string& b) {
        return '(' + a + ")==(" + b + ')';
      }  // end of OpEqual::getCxxFormula

      bool OpGreater::apply(const double a, const double b) {
        return a > b;
      }  // end of OpGreater::apply

      std::string OpGreater::getCxxFormula(const std::string& a,
                                           const std::string& b) {
        return '(' + a + ")>(" + b + ')';
      }  // end of OpGreater::getCxxFormula

      bool OpGreaterOrEqual::apply(const double a, const double b) {
        return a >= b;
      }  // end of OpGreaterOrEqual::apply

      std::string OpGreaterOrEqual::getCxxFormula(const std::string& a,
                                                  const std::string& b) {
        return '(' + a + ")>=(" + b + ')';
      }  // end of OpGreaterOrEqual::getCxxFormula

      bool OpLesser::apply(const double a, const double b) {
        return a < b;
      }  // end of OpLesser::apply

      std::string OpLesser::getCxxFormula(const std::string& a,
                                          const std::string& b) {
        return '(' + a + ")<(" + b + ')';
      }  // end of OpLesser::getCxxFormula

      bool OpLesserOrEqual::apply(const double a, const double b) {
        return a <= b;
      }  // end of OpLesserOrEqual::apply

      std::string OpLesserOrEqual::getCxxFormula(const std::string& a,
                                                 const std::string& b) {
        return '(' + a + ")<=(" + b + ')';
      }  // end of OpLesserOrEqual::getCxxFormula

      bool OpAnd::apply(const bool a, const bool b) {
        return a && b;
      }  // end of OpAnd::apply

      std::string OpAnd::getCxxFormula(const std::string& a,
                                       const std::string& b) {
        return '(' + a + ")&&(" + b + ')';
      }  // end of OpAnd::getCxxFormula

      bool OpOr::apply(const bool a, const bool b) {
        return a || b;
      }  // end of OpAnd::apply

      std::string OpOr::getCxxFormula(const std::string& a,
                                      const std::string& b) {
        return '(' + a + ")||(" + b + ')';
      }  // end of OpOr::getCxxFormula

      LogicalExpr::~LogicalExpr() = default;

      NegLogicalExpression::NegLogicalExpression(
          const std::shared_ptr<LogicalExpr> a_)
          : a(a_) {}  // end of NegLogicalExpression::NegLogicalOperation

      bool NegLogicalExpression::getValue() const {
        return !this->a->getValue();
      }  // end of NegLogicalExpression::getValue() const

      std::string NegLogicalExpression::getCxxFormula(
          const std::vector<std::string>& m) const {
        return "!(" + this->a->getCxxFormula(m) + ")";
      }  // end of NegLogicalExpression::getCxxFormula

      void NegLogicalExpression::checkCyclicDependency(
          std::vector<std::string>& names) const {
        this->a->checkCyclicDependency(names);
      }  // end of NegLogicalExpression::checkCyclicDependency

      std::shared_ptr<LogicalExpr> NegLogicalExpression::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<LogicalExpr>(
            new NegLogicalExpression(this->a->resolveDependencies(v)));
      }  // end of NegLogicalExpression::resolveDependencies() const

      std::shared_ptr<LogicalExpr> NegLogicalExpression::clone(
          const std::vector<double>& v) const {
        return std::make_shared<NegLogicalExpression>(this->a->clone(v));
      }

      std::shared_ptr<LogicalExpr>
      NegLogicalExpression::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& p,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        auto na =
            this->a->createFunctionByChangingParametersIntoVariables(v, p, pos);
        return std::make_shared<NegLogicalExpression>(na);
      }

      void NegLogicalExpression::getParametersNames(
          std::set<std::string>& p) const {
        this->a->getParametersNames(p);
      }  // end of NegLogicalExpression::getParametersNames

      NegLogicalExpression::~NegLogicalExpression() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
