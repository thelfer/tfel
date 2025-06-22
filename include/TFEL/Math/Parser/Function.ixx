/*!
 * \file   include/TFEL/Math/Parser/Function.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FUNCTIONIXX
#define LIB_TFEL_FUNCTIONIXX

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
#endif /* LIB_TFEL_FUNCTIONIXX */

namespace tfel {

  namespace math {

    namespace parser {

      template <StandardFunctionPtr f>
      StandardFunction<f>::StandardFunction(const char* const n,
                                            const std::shared_ptr<Expr> e)
          : name(n), expr(e) {}  // end of StandardFunction::StandardFunction

      template <StandardFunctionPtr f>
      StandardFunction<f>::~StandardFunction() = default;

      template <StandardFunctionPtr f>
      void StandardFunction<f>::getParametersNames(
          std::set<std::string>& p) const {
        this->expr->getParametersNames(p);
      }  // end of StandardFunction<f>::getParametersNames

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
          StandardFunctionBase::throwInvalidCallException(arg, e);
        }
        errno = old;
        return res;
      }  // end of StandardFunction::StandardFunction

      template <StandardFunctionPtr f>
      std::string StandardFunction<f>::getCxxFormula(
          const std::vector<std::string>& m) const {
        return StandardFunctionBase::getCxxFormula(
            this->name, this->expr->getCxxFormula(m));
      }  // end of StandardFunction<f>::getCxxFormula()

      template <StandardFunctionPtr f>
      void StandardFunction<f>::checkCyclicDependency(
          std::vector<std::string>& names) const {
        this->expr->checkCyclicDependency(names);
      }  // end of StandardFunction<f>::checkCyclicDependency

      template <StandardFunctionPtr f>
      std::shared_ptr<Expr> StandardFunction<f>::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new StandardFunction<f>(
            this->name, this->expr->resolveDependencies(v)));
      }  // end of StandardFunction<f>::resolveDependencies()

      template <StandardFunctionPtr f>
      std::shared_ptr<Expr> StandardFunction<f>::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(
            new StandardFunction<f>(this->name, this->expr->clone(v)));
      }  // end of StandardFunction<f>::clone

      template <StandardFunctionPtr f>
      std::shared_ptr<Expr>
      StandardFunction<f>::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& p,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        using std::shared_ptr;
        shared_ptr<Expr> nexpr =
            this->expr->createFunctionByChangingParametersIntoVariables(v, p,
                                                                        pos);
        return shared_ptr<Expr>(new StandardFunction<f>(this->name, nexpr));
      }  // end of
         // StandardFunction<f>::createFunctionByChangingParametersIntoVariables

      template <StandardFunctionPtr f>
      std::shared_ptr<Expr> differentiateFunction(
          const std::shared_ptr<Expr>,
          const std::vector<double>::size_type,
          const std::vector<double>&) {
        StandardFunctionBase::
            throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
        return {};
#endif
      }

      template <StandardFunctionPtr f>
      std::shared_ptr<Expr> StandardFunction<f>::differentiate(
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) const {
        return differentiateFunction<f>(this->expr, pos, v);
      }  // end of StandardFunction<f>::differentiate

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

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_FUNCTIONIXX */
