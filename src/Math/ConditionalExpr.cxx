/*!
 * \file   src/Math/ConditionalExpr.cxx
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

#include "TFEL/Math/Parser/ConditionalExpr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      ConditionalExpr::ConditionalExpr(const std::shared_ptr<LogicalExpr> c_,
                                       const std::shared_ptr<Expr> a_,
                                       const std::shared_ptr<Expr> b_)
          : c(c_), a(a_), b(b_) {}  // end of ConditionalExpr::ConditionalExpr

      double ConditionalExpr::getValue() const {
        if (this->c->getValue()) {
          return this->a->getValue();
        }
        return this->b->getValue();
      }  // end of ConditionalExpr::getValue() const

      void ConditionalExpr::checkCyclicDependency(
          std::vector<std::string>& vars) const {
        std::vector<std::string> a_vars(vars);
        std::vector<std::string> b_vars(vars);
        std::vector<std::string> c_vars(vars);
        this->c->checkCyclicDependency(c_vars);
        this->a->checkCyclicDependency(a_vars);
        this->b->checkCyclicDependency(b_vars);
        mergeVariablesNames(vars, a_vars);
        mergeVariablesNames(vars, b_vars);
        mergeVariablesNames(vars, c_vars);
      }  // end of ConditionalExpr::checkCyclicDependency(const
         // std::vector<std::string>& vars) const

      std::shared_ptr<Expr> ConditionalExpr::resolveDependencies(
          const std::vector<double>& v) const {
        return std::make_shared<ConditionalExpr>(
            this->c->resolveDependencies(v), this->a->resolveDependencies(v),
            this->b->resolveDependencies(v));
      }  // end of ConditionalExpr::resolveDependencies

      void ConditionalExpr::getParametersNames(std::set<std::string>& p) const {
        this->c->getParametersNames(p);
        this->a->getParametersNames(p);
        this->b->getParametersNames(p);
      }  // end of ConditionalExpr::getParametersNames

      std::shared_ptr<Expr> ConditionalExpr::differentiate(
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) const {
        return std::make_shared<ConditionalExpr>(
            this->c->clone(v), this->a->differentiate(pos, v),
            this->b->differentiate(pos, v));
      }  // end of ConditionalExpr::differentiate

      std::shared_ptr<Expr> ConditionalExpr::clone(
          const std::vector<double>& v) const {
        return std::make_shared<ConditionalExpr>(
            this->c->clone(v), this->a->clone(v), this->b->clone(v));
      }  // end of ConditionalExpr::clone

      std::shared_ptr<Expr>
      ConditionalExpr::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& p,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        auto nc =
            this->c->createFunctionByChangingParametersIntoVariables(v, p, pos);
        auto na =
            this->a->createFunctionByChangingParametersIntoVariables(v, p, pos);
        auto nb =
            this->b->createFunctionByChangingParametersIntoVariables(v, p, pos);
        return std::make_shared<ConditionalExpr>(nc, na, nb);
      }  // end of
         // ConditionalExpr::createFunctionByChangingParametersIntoVariables

      ConditionalExpr::~ConditionalExpr() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
