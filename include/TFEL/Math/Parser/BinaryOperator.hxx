/*!
 * \file   include/TFEL/Math/Parser/BinaryOperator.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_BINARYOPERATOR_HXX
#define LIB_TFEL_MATH_PARSER_BINARYOPERATOR_HXX

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct OpPlus {
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static double apply(const double, const double);
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static std::string getCxxFormula(const std::string&,
                                                           const std::string&);
  };  // end of struct OpPlus

  struct OpMinus {
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static double apply(const double, const double);
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static std::string getCxxFormula(const std::string&,
                                                           const std::string&);
  };  // end of struct OpMinus

  struct OpMult {
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static double apply(const double, const double);
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static std::string getCxxFormula(const std::string&,
                                                           const std::string&);
  };  // end of struct OpMult

  struct OpDiv {
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static double apply(const double, const double);
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static std::string getCxxFormula(const std::string&,
                                                           const std::string&);
  };  // end of struct OpDiv

  struct OpPower {
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static double apply(const double, const double);
    /*!
     * \param[in] a: lhs
     * \param[in] b: rhs
     */
    TFEL_VISIBILITY_LOCAL static std::string getCxxFormula(const std::string&,
                                                           const std::string&);
  };  // end of struct OpPower

  struct BinaryOperationBase {
    [[noreturn]] static void
    throwUnimplementedDifferentiateFunctionException();
  };  // end of struct BinaryOperationBase

  template <typename Op>
  struct TFEL_VISIBILITY_LOCAL BinaryOperation final
      : public Expr,
        protected BinaryOperationBase {
    BinaryOperation(const std::shared_ptr<Expr>,
                    const std::shared_ptr<Expr>) noexcept;
    //
    bool isConstant() const override;
    double getValue() const override final;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    std::string getCxxFormula(
        const std::vector<std::string>&) const override final;
    void checkCyclicDependency(std::vector<std::string>&) const override final;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override final;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override final;
    std::shared_ptr<Expr> clone(
        const std::vector<double>&) const override final;
    void getParametersNames(std::set<std::string>&) const override final;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override final;
    ~BinaryOperation() override;

   private:
    BinaryOperation& operator=(const BinaryOperation&) = delete;
    BinaryOperation& operator=(BinaryOperation&&) = delete;
    const std::shared_ptr<Expr> a;
    const std::shared_ptr<Expr> b;
  };  // end of struct BinaryOperation

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/BinaryOperator.ixx"

#endif /* LIB_TFEL_MATH_PARSER_BINARYOPERATOR_HXX */
