/*!
 * \file   include/TFEL/Math/Parser/Function.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   02 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_FUNCTION_HXX_
#define LIB_TFEL_FUNCTION_HXX_

#include <memory>
#include "TFEL/Config/TFELConfig.hxx"
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      struct Function : public Expr {
        virtual ~Function();
      };

      typedef double (*StandardFunctionPtr)(double);

      struct StandardFunctionBase {
        TFEL_NORETURN static void
        throwUnimplementedDifferentiateFunctionException(void);
        TFEL_NORETURN static void throwInvalidCallException(const double,
                                                            const int);
      };  // end of struct StandardFunctionBase

      template <StandardFunctionPtr f>
      struct TFEL_VISIBILITY_LOCAL StandardFunction final : public Function {
        StandardFunction(const std::shared_ptr<Expr>);
        virtual double getValue(void) const override;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual std::shared_ptr<Expr> resolveDependencies(
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> differentiate(
            const std::vector<double>::size_type,
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr> clone(
            const std::vector<double>&) const override;
        virtual std::shared_ptr<Expr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override;
        virtual void getParametersNames(std::set<std::string>&) const override;
        virtual ~StandardFunction();

       private:
        StandardFunction& operator=(const StandardFunction&) = delete;
        StandardFunction& operator=(StandardFunction&&) = delete;
        const std::shared_ptr<Expr> expr;
      };  // end of struct StandardFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Parser/Function.ixx"

#endif /* LIB_TFEL_FUNCTION_HXX_ */
