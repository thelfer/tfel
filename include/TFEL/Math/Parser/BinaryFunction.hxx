/*!
 * \file   include/TFEL/Math/Parser/BinaryFunction.hxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02/10/2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_BINARYFUNCTION_HXX
#define LIB_TFEL_MATH_PARSER_BINARYFUNCTION_HXX

#include <memory>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel::math::parser {

  struct BinaryFunction : public Expr {
    ~BinaryFunction() override;
  };

  struct StandardBinaryFunctionBase {
    [[noreturn]] static void throwUnimplementedDifferentiateFunctionException();
    [[noreturn]] static void throwInvalidCallException(const int);
    /*!
     * \brief build the C++ formula resulting from the evolution
     * of a binary function.
     * \param[in] n: name
     * \param[in] e1: first  argument
     * \param[in] e2: second argument
     */
    static std::string getCxxFormula(const char* const,
                                     const std::string&,
                                     const std::string&);

  };  // end of struct StandardBinaryFunctionBase

  template <double (*f)(const double, const double)>
  struct TFEL_VISIBILITY_LOCAL StandardBinaryFunction final
      : public BinaryFunction,
        protected StandardBinaryFunctionBase {
    /*!
     * \param[in] n: name of the function
     * \param[in] e1: first argument
     * \param[in] e2: second argument
     */
    StandardBinaryFunction(const char* const,
                           const std::shared_ptr<Expr>,
                           const std::shared_ptr<Expr>) noexcept;
    //
    bool isConstant() const override;
    bool dependsOnVariable(const std::vector<double>::size_type) const override;
    double getValue() const override;
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
    ~StandardBinaryFunction() override;

   private:
    StandardBinaryFunction& operator=(const StandardBinaryFunction&) = delete;
    StandardBinaryFunction& operator=(StandardBinaryFunction&&) = delete;
    //! function name
    const char* const name;
    //! first argument
    const std::shared_ptr<Expr> expr1;
    //! second argument
    const std::shared_ptr<Expr> expr2;
  };  // end of struct StandardBinaryFunction

}  // end of namespace tfel::math::parser

#include "TFEL/Math/Parser/BinaryFunction.ixx"

#endif /* LIB_TFEL_MATH_PARSER_BINARYFUNCTION_HXX */
