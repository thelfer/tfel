/*!
 * \file   include/TFEL/Math/Parser/LogicalExpr.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date 13/01/2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_LOGICALEXPR_HXX
#define LIB_TFEL_MATH_PARSER_LOGICALEXPR_HXX

#include <set>
#include <vector>
#include <string>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct OpEqual {
    [[nodiscard]] static bool apply(const double, const double);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpEqual

  struct OpGreater {
    [[nodiscard]] static bool apply(const double, const double);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpGreater

  struct OpGreaterOrEqual {
    [[nodiscard]] static bool apply(const double, const double);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpGreaterOrEqual

  struct OpLesser {
    [[nodiscard]] static bool apply(const double, const double);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpLess

  struct OpLesserOrEqual {
    [[nodiscard]] static bool apply(const double, const double);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpLessOrEqual

  struct OpAnd {
    [[nodiscard]] static bool apply(const bool, const bool);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpAnd

  struct OpOr {
    [[nodiscard]] static bool apply(const bool, const bool);
    /*!
     * \brief return a string suitable for integration in a C++
     * code.
     * \param[in] a: left hand side
     * \param[in] b: right hand side
     */
    [[nodiscard]] static std::string getCxxFormula(const std::string&,
                                                   const std::string&);
  };  // end of struct OpOr

  /*!
   * \brief structure representing a logical expression
   */
  struct LogicalExpr {
    //! \return the result of the evaluation of the logical expression
    [[nodiscard]] virtual bool getValue() const = 0;
    //! \brief return if the expression is constant
    [[nodiscard]] virtual bool isConstant() const = 0;
    /*!
     * \return if the expression depends of the variable associated with the
     * given position
     * \param[in] p: position
     */
    [[nodiscard]] virtual bool dependsOnVariable(
        const std::vector<double>::size_type) const = 0;
    /*!
     * \return a string representation of the evaluator suitable to
     * be integrated in a C++ code.
     * \param[in] m: a map used to change the names of the variables
     */
    [[nodiscard]] virtual std::string getCxxFormula(
        const std::vector<std::string>&) const = 0;
    virtual void checkCyclicDependency(std::vector<std::string>&) const = 0;
    [[nodiscard]] virtual std::shared_ptr<LogicalExpr> resolveDependencies(
        const std::vector<double>&) const = 0;
    [[nodiscard]] virtual std::shared_ptr<LogicalExpr> clone(
        const std::vector<double>&) const = 0;
    virtual void getParametersNames(std::set<std::string>&) const = 0;
    [[nodiscard]] virtual std::shared_ptr<LogicalExpr>
    createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&) const = 0;
    virtual ~LogicalExpr();
  };  // end of struct LogicalExpr

  //! a simple alias
  using LogicalExprPtr = std::shared_ptr<LogicalExpr>;

  template <typename Op>
  struct TFEL_VISIBILITY_LOCAL LogicalOperation final : public LogicalExpr {
    LogicalOperation(const ExprPtr, const ExprPtr) noexcept;
    //
    LogicalOperation& operator=(const LogicalOperation&) = delete;
    LogicalOperation& operator=(LogicalOperation&&) = delete;
    //
    [[nodiscard]] bool isConstant() const override;
    [[nodiscard]] bool dependsOnVariable(
        const std::vector<double>::size_type) const override;
    [[nodiscard]] bool getValue() const override;
    [[nodiscard]] std::string getCxxFormula(
        const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr resolveDependencies(
        const std::vector<double>&) const override;
    [[nodiscard]] LogicalExprPtr clone(
        const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr
    createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    ~LogicalOperation() override;

   private:
    const ExprPtr a;
    const ExprPtr b;
  };  // end of struct LogicalOperation

  template <typename Op>
  struct TFEL_VISIBILITY_LOCAL LogicalBinaryOperation final
      : public LogicalExpr {
    LogicalBinaryOperation(LogicalExprPtr, LogicalExprPtr) noexcept;
    //
    LogicalBinaryOperation& operator=(const LogicalBinaryOperation&) = delete;
    LogicalBinaryOperation& operator=(LogicalBinaryOperation&&) = delete;
    //
    [[nodiscard]] bool isConstant() const override;
    [[nodiscard]] bool dependsOnVariable(
        const std::vector<double>::size_type) const override;
    [[nodiscard]] bool getValue() const override;
    [[nodiscard]] std::string getCxxFormula(
        const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr resolveDependencies(
        const std::vector<double>&) const override;
    [[nodiscard]] LogicalExprPtr clone(
        const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr
    createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    ~LogicalBinaryOperation() override;

   private:
    LogicalExprPtr a;
    LogicalExprPtr b;
  };  // end of struct LogicalBinaryOperation

  struct TFEL_VISIBILITY_LOCAL NegLogicalExpression final : public LogicalExpr {
    explicit NegLogicalExpression(LogicalExprPtr) noexcept;
    //
    NegLogicalExpression& operator=(const NegLogicalExpression&) = delete;
    NegLogicalExpression& operator=(NegLogicalExpression&&) = delete;
    //
    [[nodiscard]] bool isConstant() const override;
    [[nodiscard]] bool dependsOnVariable(
        const std::vector<double>::size_type) const override;
    [[nodiscard]] bool getValue() const override;
    [[nodiscard]] std::string getCxxFormula(
        const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr resolveDependencies(
        const std::vector<double>&) const override;
    [[nodiscard]] LogicalExprPtr clone(
        const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    [[nodiscard]] LogicalExprPtr
    createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    ~NegLogicalExpression() override;

   private:
    LogicalExprPtr a;
  };  // end of struct LogicalBinaryOperation

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/LogicalExpr.ixx"

#endif /* LIB_TFEL_MATH_PARSER_LOGICALEXPR_HXX */
