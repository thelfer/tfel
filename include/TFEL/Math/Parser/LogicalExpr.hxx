/*!
 * \file   include/TFEL/Math/Parser/LogicalExpr.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_LOGICALEXPR_HXX_
#define LIB_TFEL_LOGICALEXPR_HXX_

#include <set>
#include <vector>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct OpEqual {
        static bool TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpEqual

      struct OpGreater {
        static bool TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpGreater

      struct OpGreaterOrEqual {
        static bool TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpGreaterOrEqual

      struct OpLesser {
        static bool TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpLess

      struct OpLesserOrEqual {
        static bool TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpLessOrEqual

      struct OpAnd {
        static bool TFEL_VISIBILITY_LOCAL apply(const bool, const bool);
      };  // end of struct OpAnd

      struct OpOr {
        static bool apply(const bool, const bool);
      };  // end of struct OpOr

      struct LogicalExpr {
        virtual bool getValue(void) const = 0;
        virtual void checkCyclicDependency(std::vector<std::string>&) const = 0;
        virtual std::shared_ptr<LogicalExpr> resolveDependencies(
            const std::vector<double>&) const = 0;
        virtual std::shared_ptr<LogicalExpr> clone(
            const std::vector<double>&) const = 0;
        virtual void getParametersNames(std::set<std::string>&) const = 0;
        virtual std::shared_ptr<LogicalExpr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const = 0;
        virtual ~LogicalExpr();
      };  // end of struct LogicalExpr

      //! a simple alias
      using LogicalExprPtr = std::shared_ptr<LogicalExpr>;

      template <typename Op>
      struct TFEL_VISIBILITY_LOCAL LogicalOperation final : public LogicalExpr {
        LogicalOperation(const ExprPtr, const ExprPtr);
        virtual bool getValue(void) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual LogicalExprPtr resolveDependencies(
            const std::vector<double>&) const override;
        virtual LogicalExprPtr clone(const std::vector<double>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual LogicalExprPtr createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual ~LogicalOperation();

       private:
        LogicalOperation& operator=(const LogicalOperation&) = delete;
        LogicalOperation& operator=(LogicalOperation&&) = delete;
        const ExprPtr a;
        const ExprPtr b;
      };  // end of struct LogicalOperation

      template <typename Op>
      struct TFEL_VISIBILITY_LOCAL LogicalBinaryOperation final
          : public LogicalExpr {
        LogicalBinaryOperation(LogicalExprPtr, LogicalExprPtr);
        virtual bool getValue(void) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual LogicalExprPtr resolveDependencies(
            const std::vector<double>&) const override;
        virtual LogicalExprPtr clone(const std::vector<double>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual LogicalExprPtr createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual ~LogicalBinaryOperation();

       private:
        LogicalBinaryOperation& operator=(const LogicalBinaryOperation&) =
            delete;
        LogicalBinaryOperation& operator=(LogicalBinaryOperation&&) = delete;
        LogicalExprPtr a;
        LogicalExprPtr b;
      };  // end of struct LogicalBinaryOperation

      struct TFEL_VISIBILITY_LOCAL NegLogicalExpression final
          : public LogicalExpr {
        NegLogicalExpression(LogicalExprPtr);
        virtual bool getValue(void) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual LogicalExprPtr resolveDependencies(
            const std::vector<double>&) const override;
        virtual LogicalExprPtr clone(const std::vector<double>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual LogicalExprPtr createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        ~NegLogicalExpression();

       private:
        NegLogicalExpression& operator=(const NegLogicalExpression&) = delete;
        NegLogicalExpression& operator=(NegLogicalExpression&&) = delete;
        LogicalExprPtr a;
      };  // end of struct LogicalBinaryOperation

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Parser/LogicalExpr.ixx"

#endif /* LIB_TFEL_LOGICALEXPR_HXX_ */
