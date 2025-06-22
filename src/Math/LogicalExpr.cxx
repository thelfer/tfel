/*!
 * \file   src/Math/LogicalExpr.cxx
 * \brief
 * \author Helfer Thomas
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

#include "TFEL/Math/Parser/LogicalExpr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      bool OpEqual::apply(const double a, const double b) {
        return std::abs(a - b) < 10 * std::numeric_limits<double>::min();
      }  // end of OpEqual::apply

      bool OpGreater::apply(const double a, const double b) {
        return a > b;
      }  // end of OpGreater::apply

      bool OpGreaterOrEqual::apply(const double a, const double b) {
        return a >= b;
      }  // end of OpGreaterOrEqual::apply

      bool OpLesser::apply(const double a, const double b) {
        return a < b;
      }  // end of OpLesser::apply

      bool OpLesserOrEqual::apply(const double a, const double b) {
        return a <= b;
      }  // end of OpLesserOrEqual::apply

      bool OpAnd::apply(const bool a, const bool b) {
        return a && b;
      }  // end of OpAnd::apply

      bool OpOr::apply(const bool a, const bool b) {
        return a || b;
      }  // end of OpAnd::apply

      LogicalExpr::~LogicalExpr() = default;

      NegLogicalExpression::NegLogicalExpression(
          const std::shared_ptr<LogicalExpr> a_)
          : a(a_) {}  // end of NegLogicalExpression::NegLogicalOperation

      bool NegLogicalExpression::getValue() const {
        return !this->a->getValue();
      }  // end of NegLogicalExpression::getValue(void) const

      void NegLogicalExpression::checkCyclicDependency(
          std::vector<std::string>& names) const {
        this->a->checkCyclicDependency(names);
      }  // end of NegLogicalExpression::checkCyclicDependency

      std::shared_ptr<LogicalExpr> NegLogicalExpression::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<LogicalExpr>(
            new NegLogicalExpression(this->a->resolveDependencies(v)));
      }  // end of NegLogicalExpression::resolveDependencies(void) const

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
