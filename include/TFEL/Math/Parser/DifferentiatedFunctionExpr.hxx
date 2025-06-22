/*!
 * \file   include/TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef TFEL_LIB_MATH_PARSER_DIFFERENTIATEDFUNCTIONEXPR_H_
#define TFEL_LIB_MATH_PARSER_DIFFERENTIATEDFUNCTIONEXPR_H_

#include <string>
#include <vector>
#include <memory>

#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/ExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      struct DifferentiatedFunctionExpr final : public Expr {
        DifferentiatedFunctionExpr(
            std::shared_ptr<ExternalFunction>,
            std::vector<std::shared_ptr<Expr>>&,
            const std::vector<std::vector<double>::size_type>&);
        virtual double getValue(void) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual std::shared_ptr<Expr> differentiate(
            const std::vector<double>::size_type,
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> resolveDependencies(
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> clone(
            const std::vector<double>&) const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual std::shared_ptr<Expr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual ~DifferentiatedFunctionExpr();

       private:
        std::shared_ptr<ExternalFunction> TFEL_VISIBILITY_LOCAL
        getDerivative(void) const;
        mutable std::shared_ptr<ExternalFunction> f;
        std::vector<std::shared_ptr<Expr>> args;
        std::vector<std::vector<double>::size_type> pvar;
      };  // end of struct DifferentiatedFunctionExpr

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* TFEL_LIB_MATH_PARSER_DIFFERENTIATEDFUNCTIONEXPR_H_ */
