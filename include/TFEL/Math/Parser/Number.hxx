/*!
 * \file   include/TFEL/Math/Parser/Number.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_NUMBER_HXX
#define LIB_TFEL_MATH_PARSER_NUMBER_HXX

#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  //! \brief  Expression representing a number
  struct Number final : public Expr {
    //! \brief return an expression associated with zero
    static std::shared_ptr<Number> zero();
    //! \brief return an expression associated with one
    static std::shared_ptr<Number> one();
    /*!
     * \brief constructor
     * \param[in] s: string representation of the number
     * \param[in] v: value
     */
    Number(const std::string&, const double);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    //! \return the number value
    double getValue() const override;
    //! \brief destructor
    ~Number() override;

   private:
    Number& operator=(const Number&) = delete;
    Number& operator=(Number&&) = delete;
    //! \brief string representation
    const std::string str;
    //! \brief number value
    const double value;
  };  // end of struct Number

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_NUMBER_HXX */
