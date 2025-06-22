/*!
 * \file   include/TFEL/Math/Parser/Variable.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_VARIABLE_HXX
#define LIB_TFEL_MATH_PARSER_VARIABLE_HXX

#include <vector>
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct Variable final : public Expr {
    /*!
     * \brief constructor
     * \param[in] v_: buffer containing the values of all the variables
     * \param[in] p_: position of the variable in the buffer
     */
    Variable(const std::vector<double>&, const std::vector<double>::size_type);
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
    std::string getCxxFormula(const std::vector<std::string>&) const override;

    void checkCyclicDependency(std::vector<std::string>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    //! \brief destructor
    ~Variable() override;

   private:
    Variable& operator=(const Variable&) = delete;
    Variable& operator=(Variable&&) = delete;
    const std::vector<double>& v;
    const std::vector<double>::size_type pos;
  };  // end of struct Variable

}  // end of namespace  tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_VARIABLE_HXX */
