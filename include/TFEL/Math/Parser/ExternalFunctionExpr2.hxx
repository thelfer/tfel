/*!
 * \file   include/TFEL/Math/Parser/ExternalFunctionExpr2.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONEXPR2_HXX
#define LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONEXPR2_HXX

#include <string>
#include <vector>
#include <memory>
#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel::math::parser {

  struct ExternalFunctionExpr2 final : public Expr {
    ExternalFunctionExpr2(std::shared_ptr<ExternalFunction>,
                          std::vector<std::shared_ptr<Expr>>&);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    ~ExternalFunctionExpr2() override;

   private:
    mutable std::shared_ptr<ExternalFunction> f;
    std::vector<std::shared_ptr<Expr>> args;
  };  // end of struct ExternalFunctionExpr2

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_EXTERNALFUNCTIONEXPR2_HXX */
