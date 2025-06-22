/*!
 * \file   include/TFEL/Math/Parser/PowerFunction.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   25/04/2022
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_POWERFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_POWERFUNCTION_HXX

#include <memory>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"
#include "TFEL/Math/Parser/Function.hxx"

namespace tfel::math::parser {

  /*!
   * \brief build an expression which stands for the integer power of another
   * expression.
   */
  template <int N>
  struct TFEL_VISIBILITY_LOCAL PowerFunction final : public Function {
    /*!
     * \brief constructor
     * \param[in] e: expression
     */
    PowerFunction(const std::shared_ptr<Expr>) noexcept;
    double getValue() const override;
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    //! \brief destructor
    ~PowerFunction() override;

   private:
    PowerFunction& operator=(const PowerFunction&) = delete;
    PowerFunction& operator=(PowerFunction&&) = delete;
  };  // end of struct PowerFunction

  /*!
   * \brief build an expression which stands for the integer power of another
   * expression.
   */
  struct GeneralPowerFunction final : public Function {
    /*!
     * \brief constructor
     * \param[in] e: expression
     * \param[in] n: exponent
     */
    GeneralPowerFunction(const std::shared_ptr<Expr>, const int) noexcept;
    double getValue() const override;
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    std::shared_ptr<Expr> resolveDependencies(
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> differentiate(
        const std::vector<double>::size_type,
        const std::vector<double>&) const override;
    std::shared_ptr<Expr> clone(const std::vector<double>&) const override;
    std::shared_ptr<Expr> createFunctionByChangingParametersIntoVariables(
        const std::vector<double>&,
        const std::vector<std::string>&,
        const std::map<std::string, std::vector<double>::size_type>&)
        const override;
    //! \brief destructor
    ~GeneralPowerFunction() override;

   private:
    GeneralPowerFunction& operator=(const GeneralPowerFunction&) = delete;
    GeneralPowerFunction& operator=(GeneralPowerFunction&&) = delete;
    //! \brief exponent
    const int n;
  };  // end of struct GeneralPowerFunction

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/PowerFunction.ixx"

#endif /* LIB_TFEL_MATH_PARSER_POWERFUNCTION_HXX */
