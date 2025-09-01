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

  /*!
   * \brief base class with some helpful methods
   */
  struct FunctionBase {
    [[noreturn]] static void throwUnimplementedDifferentiateFunctionException();
    [[noreturn]] static void throwInvalidCallException(const double, const int);
    /*!
     * \brief build the C++ formula resulting from the evolution
     * of a unary function.
     * \param[in] n: name
     * \param[in] e: argument
     */
    static std::string getCxxFormula(const char*, const std::string&);
  };  // end of struct FunctionBase

  /*!
   * \brief base class for unary functions
   */
  struct Function : public Expr {
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    void checkCyclicDependency(std::vector<std::string>&) const override;
    void getParametersNames(std::set<std::string>&) const override;
    //! \brief destructor
    ~Function() override;

   protected:
    /*!
     * \brief constructor
     * \param[in] e: expression representing the argument of the function
     */
    Function(const std::shared_ptr<Expr>) noexcept;
    //! \brief argument
    const std::shared_ptr<Expr> expr;
  };

  //! \brief a simple alias
  typedef double (*StandardFunctionPtr)(double);

  /*!
   * \brief implementation of an unary function from a standard C-function
   */
  template <StandardFunctionPtr f>
  struct TFEL_VISIBILITY_LOCAL StandardFunction final : public Function {
    /*!
     * \param[in] n: name of the function
     * \param[in] e: expression
     */
    StandardFunction(const char* const, const std::shared_ptr<Expr>) noexcept;
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
    ~StandardFunction() override;

   private:
    StandardFunction& operator=(const StandardFunction&) = delete;
    StandardFunction& operator=(StandardFunction&&) = delete;
    //! \brief name
    const char* const name;
  };  // end of struct StandardFunction

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/Function.ixx"

#endif /* LIB_TFEL_MATH_PARSER_FUNCTION_HXX */
