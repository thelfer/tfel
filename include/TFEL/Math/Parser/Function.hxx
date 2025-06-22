/*!
 * \file   include/TFEL/Math/Parser/Function.hxx
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

#ifndef LIB_TFEL_MATH_PARSER_FUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_FUNCTION_HXX

#include <memory>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct Function : public Expr {
    ~Function() override;
  };

  typedef double (*StandardFunctionPtr)(double);

  struct StandardFunctionBase {
    [[noreturn]] static void throwUnimplementedDifferentiateFunctionException();
    [[noreturn]] static void throwInvalidCallException(const double, const int);
    /*!
     * \brief build the C++ formula resulting from the evolution
     * of a unary function.
     * \param[in] n: name
     * \param[in] e: argument
     */
    static std::string getCxxFormula(const char*, const std::string&);
  };  // end of struct StandardFunctionBase

  template <StandardFunctionPtr f>
  struct TFEL_VISIBILITY_LOCAL StandardFunction final : public Function {
    /*!
     * \param[in] n: name of the function
     * \param[in] e: expression
     */
    StandardFunction(const char* const, const std::shared_ptr<Expr>) noexcept;
    /*!
     *\return the value resulting for the evaluation of the
     * function and its argument
     */
    double getValue() const override;
    /*!
     * \return a string representation of the evaluator suitable to
     * be integrated in a C++ code.
     * \param[in] m: a map used to change the names of the variables
     */
    std::string getCxxFormula(const std::vector<std::string>&) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
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
    void getParametersNames(std::set<std::string>&) const override;
    ~StandardFunction() override;

   private:
    StandardFunction& operator=(const StandardFunction&) = delete;
    StandardFunction& operator=(StandardFunction&&) = delete;
    //! name
    const char* const name;
    //! argument
    const std::shared_ptr<Expr> expr;
  };  // end of struct StandardFunction

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/Function.ixx"

#endif /* LIB_TFEL_MATH_PARSER_FUNCTION_HXX */
