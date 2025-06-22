/*!
 * \file   include/TFEL/Math/Parser/BinaryFunction.hxx
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

#ifndef LIB_TFEL_BINARYFUNCTION_HXX_
#define LIB_TFEL_BINARYFUNCTION_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      struct BinaryFunction : public Expr {
        virtual ~BinaryFunction();
      };

      struct StandardBinaryFunctionBase {
        TFEL_NORETURN static void
        throwUnimplementedDifferentiateFunctionException(void);
        TFEL_NORETURN static void throwInvalidCallException(const int);
      };  // end of struct StandardBinaryFunctionBase

      template <double (*f)(const double, const double)>
      struct TFEL_VISIBILITY_LOCAL StandardBinaryFunction final
          : public BinaryFunction,
            protected StandardBinaryFunctionBase {
        StandardBinaryFunction(const std::shared_ptr<Expr>,
                               const std::shared_ptr<Expr>);
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override;
        virtual double getValue(void) const override;
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
        virtual ~StandardBinaryFunction();

       private:
        StandardBinaryFunction& operator=(const StandardBinaryFunction&) =
            delete;
        StandardBinaryFunction& operator=(StandardBinaryFunction&&) = delete;
        const std::shared_ptr<Expr> expr1;
        const std::shared_ptr<Expr> expr2;
      };  // end of struct StandardBinaryFunction

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Parser/BinaryFunction.ixx"

#endif /* LIB_TFEL_BINARYFUNCTION_HXX_ */
