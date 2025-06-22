/*!
 * \file   include/TFEL/Math/Parser/Negation.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_NEGATION_HXX
#define LIB_TFEL_MATH_PARSER_NEGATION_HXX

#include <memory>
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct Negation final : public Expr {
    Negation(const std::shared_ptr<Expr>);
    double getValue() const override;
    /*!
     * \return a string representation of the evaluator suitable to
     * be integrated in a C++ code.
     * \param[in] m: a map used to change the names of the variables
     */
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    void getParametersNames(std::set<std::string>&) const override;
    ~Negation() override;

   private:
    Negation& operator=(const Negation&) = delete;
    Negation& operator=(Negation&&) = delete;
    const std::shared_ptr<Expr> expr;
  };

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_NEGATION_HXX */
