/*!
 * \file   include/TFEL/Math/Parser/Expr.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_EXPR_HXX
#define LIB_TFEL_MATH_PARSER_EXPR_HXX

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

namespace tfel::math::parser {

  /*!
   * \brief base class resulting from the analysis of a formula.
   */
  struct Expr {
    /*!
     * \return if the expression depends of the variable associated with the
     * given position
     * \param[in] p: position
     */
    virtual bool dependsOnVariable(
        const std::vector<double>::size_type) const = 0;
    //! \brief return if the expression is constant
    virtual bool isConstant() const = 0;
    //! \return the result of the evaluation of the expression
    virtual double getValue() const = 0;
    //! \brief check if the expression does not lead to a cyclic dependency
    virtual void checkCyclicDependency(std::vector<std::string>&) const = 0;
    virtual std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const = 0;
    virtual std::shared_ptr<Expr> clone(const std::vector<double>&) const = 0;
    virtual std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const = 0;
    virtual void getParametersNames(std::set<std::string>&) const = 0;
    /*!
     * \return a string representation of the evaluator suitable to
     * be integrated in a C++ code.
     * \param[in] m: a map used to change the names of the variables
     */
    virtual std::string getCxxFormula(
        const std::vector<std::string>&) const = 0;

    virtual std::shared_ptr<Expr>
    createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&) const = 0;
    virtual ~Expr();
  };  // end of struct Expr

  //! a simple alias
  using ExprPtr = std::shared_ptr<Expr>;

  //! a simple helper function for checkCyclicDependency
  void mergeVariablesNames(std::vector<std::string>&,
                           const std::vector<std::string>&);

  /*!
   * \brief small utility function computing d1 * d2 if d2 is not equal to 1
   * \param[in] d1: left term
   * \param[in] d2: right term
   */
  std::shared_ptr<Expr> applyChainRule(const std::shared_ptr<Expr>,
                                       const std::shared_ptr<Expr>);

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_EXPR_HXX */
