/*!
 * \file   include/TFEL/Math/Parser/LogicalExpr.ixx
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

#ifndef LIB_TFEL_LOGICALEXPRIXX
#define LIB_TFEL_LOGICALEXPRIXX

#include <vector>
#include <string>
#include <memory>

namespace tfel {

  namespace math {

    namespace parser {

      template <typename Op>
      LogicalOperation<Op>::LogicalOperation(const std::shared_ptr<Expr> a_,
                                             const std::shared_ptr<Expr> b_)
          : a(a_), b(b_) {}  // end of LogicalOperation::LogicalOperation

      template <typename Op>
      bool LogicalOperation<Op>::getValue() const {
        return Op::apply(this->a->getValue(), this->b->getValue());
      }  // end of LogicalOperation::getValue() const

      template <typename Op>
      std::string LogicalOperation<Op>::getCxxFormula(
          const std::vector<std::string>& m) const {
        return Op::getCxxFormula(this->a->getCxxFormula(m),
                                 this->b->getCxxFormula(m));
      }  // end of LogicalOperation<Op>::getCxxFormula

      template <typename Op>
      void LogicalOperation<Op>::checkCyclicDependency(
          std::vector<std::string>& vars) const {
        std::vector<std::string> a_vars(vars);
        std::vector<std::string> b_vars(vars);
        this->a->checkCyclicDependency(a_vars);
        this->b->checkCyclicDependency(b_vars);
        mergeVariablesNames(vars, a_vars);
        mergeVariablesNames(vars, b_vars);
      }  // end of LogicalOperation::checkCyclicDependency

      template <typename Op>
      std::shared_ptr<LogicalExpr> LogicalOperation<Op>::resolveDependencies(
          const std::vector<double>& v) const {
        return std::make_shared<LogicalOperation<Op>>(
            this->a->resolveDependencies(v), this->b->resolveDependencies(v));
      }  // end of LogicalOperation<Op>::resolveDependencies() const

      template <typename Op>
      std::shared_ptr<LogicalExpr> LogicalOperation<Op>::clone(
          const std::vector<double>& v) const {
        return std::make_shared<LogicalOperation<Op>>(this->a->clone(v),
                                                      this->b->clone(v));
      }  // end of LogicalOperation<Op>::clone

      template <typename Op>
      std::shared_ptr<LogicalExpr>
      LogicalOperation<Op>::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& p,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        using std::shared_ptr;
        shared_ptr<Expr> na =
            this->a->createFunctionByChangingParametersIntoVariables(v, p, pos);
        shared_ptr<Expr> nb =
            this->b->createFunctionByChangingParametersIntoVariables(v, p, pos);
        return shared_ptr<LogicalExpr>(new LogicalOperation<Op>(na, nb));
      }  // end of
         // LogicalOperation<Op>::createFunctionByChangingParametersIntoVariables

      template <typename Op>
      void LogicalOperation<Op>::getParametersNames(
          std::set<std::string>& p) const {
        this->a->getParametersNames(p);
        this->b->getParametersNames(p);
      }  // end of LogicalOperation<Op>::getParametersNames

      template <typename Op>
      LogicalOperation<Op>::~LogicalOperation() {
      }  // end of LogicalOperation<Op>::~LogicalOperation()

      template <typename Op>
      LogicalBinaryOperation<Op>::LogicalBinaryOperation(
          const std::shared_ptr<LogicalExpr> a_,
          const std::shared_ptr<LogicalExpr> b_)
          : a(a_),
            b(b_) {}  // end of LogicalBinaryOperation::LogicalBinaryOperation

      template <typename Op>
      bool LogicalBinaryOperation<Op>::getValue() const {
        return Op::apply(this->a->getValue(), this->b->getValue());
      }  // end of LogicalBinaryOperation::getValue() const

      template <typename Op>
      std::string LogicalBinaryOperation<Op>::getCxxFormula(
          const std::vector<std::string>& m) const {
        return Op::getCxxFormula(this->a->getCxxFormula(m),
                                 this->b->getCxxFormula(m));
      }  // end of LogicalBinaryOperation::getCxxFormula() const

      template <typename Op>
      void LogicalBinaryOperation<Op>::checkCyclicDependency(
          std::vector<std::string>& vars) const {
        using namespace std;
        using std::vector;
        vector<string> a_vars(vars);
        vector<string> b_vars(vars);
        this->a->checkCyclicDependency(a_vars);
        this->b->checkCyclicDependency(b_vars);
        mergeVariablesNames(vars, a_vars);
        mergeVariablesNames(vars, b_vars);
      }  // end of LogicalBinaryOperation::checkCyclicDependency

      template <typename Op>
      void LogicalBinaryOperation<Op>::getParametersNames(
          std::set<std::string>& p) const {
        this->a->getParametersNames(p);
        this->b->getParametersNames(p);
      }  // end of LogicalBinaryOperation<Op>::getParametersNames

      template <typename Op>
      std::shared_ptr<LogicalExpr>
      LogicalBinaryOperation<Op>::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<LogicalExpr>(new LogicalBinaryOperation<Op>(
            this->a->resolveDependencies(v), this->b->resolveDependencies(v)));
      }  // end of LogicalBinaryOperation<Op>::resolveDependencies() const

      template <typename Op>
      std::shared_ptr<LogicalExpr> LogicalBinaryOperation<Op>::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<LogicalExpr>(new LogicalBinaryOperation<Op>(
            this->a->clone(v), this->b->clone(v)));
      }  // end of LogicalBinaryOperation<Op>::clone

      template <typename Op>
      std::shared_ptr<LogicalExpr> LogicalBinaryOperation<Op>::
          createFunctionByChangingParametersIntoVariables(
              const std::vector<double>& v,
              const std::vector<std::string>& p,
              const std::map<std::string, std::vector<double>::size_type>& pos)
              const {
        using std::shared_ptr;
        shared_ptr<LogicalExpr> na =
            this->a->createFunctionByChangingParametersIntoVariables(v, p, pos);
        shared_ptr<LogicalExpr> nb =
            this->b->createFunctionByChangingParametersIntoVariables(v, p, pos);
        return shared_ptr<LogicalExpr>(new LogicalBinaryOperation<Op>(na, nb));
      }  // end of
         // LogicalBinaryOperation<Op>::createFunctionByChangingParametersIntoVariables

      template <typename Op>
      LogicalBinaryOperation<Op>::~LogicalBinaryOperation() {
      }  // end of LogicalBinaryOperation<Op>::~LogicalBinaryOperation()

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_LOGICALEXPRIXX */
