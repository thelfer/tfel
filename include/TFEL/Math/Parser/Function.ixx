/*!
 * \file   include/TFEL/Math/Parser/Function.ixx
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

#ifndef LIB_TFEL_MATH_PARSER_FUNCTION_IXX
#define LIB_TFEL_MATH_PARSER_FUNCTION_IXX

#include <string>
#include <cerrno>
#include <cstring>
#include <cmath>

#ifndef __SUNPRO_CC
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(X) \
  template <>                                                                \
  std::shared_ptr<Expr> differentiateFunction<std::X>(                       \
      const std::shared_ptr<Expr>, const std::vector<double>::size_type,     \
      const std::vector<double>&)
#else /* __SUNPRO_CC */
#define TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(X) \
  template <>                                                                \
  std::shared_ptr<Expr> differentiateFunction<X>(                            \
      const std::shared_ptr<Expr>, const std::vector<double>::size_type,     \
      const std::vector<double>&)
#endif /* __SUNPRO_CC */

namespace tfel::math::parser {

  template <StandardFunctionPtr f>
  StandardFunction<f>::StandardFunction(const char* const n,
                                        const std::shared_ptr<Expr> e) noexcept
      : Function(e), name(n) {}  // end of StandardFunction

  template <StandardFunctionPtr f>
  double StandardFunction<f>::getValue() const {
    using namespace std;
    const auto arg = this->expr->getValue();
    int old = errno;
    errno = 0;
    const auto res = f(arg);
    if (errno != 0) {
      int e = errno;
      errno = old;
      FunctionBase::throwInvalidCallException(arg, e);
    }
    errno = old;
    return res;
  }  // end of getValue

  template <StandardFunctionPtr f>
  std::string StandardFunction<f>::getCxxFormula(
      const std::vector<std::string>& m) const {
    return FunctionBase::getCxxFormula(this->name,
                                       this->expr->getCxxFormula(m));
  }  // end of getCxxFormula

  template <StandardFunctionPtr f>
  std::shared_ptr<Expr> StandardFunction<f>::resolveDependencies(
      const std::vector<double>& v) const {
    return std::make_shared<StandardFunction<f>>(
        this->name, this->expr->resolveDependencies(v));
  }  // end of resolveDependencies

  template <StandardFunctionPtr f>
  std::shared_ptr<Expr> StandardFunction<f>::clone(
      const std::vector<double>& v) const {
    return std::make_shared<StandardFunction<f>>(this->name,
                                                 this->expr->clone(v));
  }  // end of clone

  template <StandardFunctionPtr f>
  std::shared_ptr<Expr>
  StandardFunction<f>::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& p,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    auto nexpr =
        this->expr->createFunctionByChangingParametersIntoVariables(v, p, pos);
    return std::make_shared<StandardFunction<f>>(this->name, nexpr);
  }  // end of createFunctionByChangingParametersIntoVariables

  template <StandardFunctionPtr f>
  std::shared_ptr<Expr> differentiateFunction(
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&) {
    FunctionBase::throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
    return {};
#endif
  }

  template <StandardFunctionPtr f>
  std::shared_ptr<Expr> StandardFunction<f>::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    return differentiateFunction<f>(this->expr, pos, v);
  }  // end of differentiate

  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(exp);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sin);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(cos);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(tan);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sqrt);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(log);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(log10);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(asin);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(acos);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(atan);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(sinh);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(cosh);
  TFEL_MATH_DIFFERENTIATEFUNCTION_PARTIALSPECIALISATION_DECLARATION(tanh);

  template <StandardFunctionPtr f>
  StandardFunction<f>::~StandardFunction() = default;

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_FUNCTION_IXX */
