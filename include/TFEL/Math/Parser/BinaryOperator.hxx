/*!
 * \file   include/TFEL/Math/Parser/BinaryOperator.hxx
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

#ifndef LIB_TFEL_BINARYOPERATOR_HXX_
#define LIB_TFEL_BINARYOPERATOR_HXX_

#include "TFEL/Config/TFELConfig.hxx"
#include <memory>
#include "TFEL/Math/Parser/Expr.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      struct OpPlus {
        static double TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpPlus

      struct OpMinus {
        static double TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpMinus

      struct OpMult {
        static double TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpMult

      struct OpDiv {
        static double TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpDiv

      struct OpPower {
        static double TFEL_VISIBILITY_LOCAL apply(const double, const double);
      };  // end of struct OpPower

      struct BinaryOperationBase {
        TFEL_NORETURN static void TFEL_VISIBILITY_LOCAL
        throwUnimplementedDifferentiateFunctionException();
      };  // end of struct BinaryOperationBase

      template <typename Op>
      struct TFEL_VISIBILITY_LOCAL BinaryOperation final
          : public Expr,
            protected BinaryOperationBase {
        BinaryOperation(const std::shared_ptr<Expr>,
                        const std::shared_ptr<Expr>);
        virtual double getValue(void) const override final;
        virtual void checkCyclicDependency(
            std::vector<std::string>&) const override final;
        virtual std::shared_ptr<Expr> resolveDependencies(
            const std::vector<double>&) const override final;
        virtual std::shared_ptr<Expr> differentiate(
            const std::vector<double>::size_type,
            const std::vector<double>&) const override final;
        virtual std::shared_ptr<Expr> clone(
            const std::vector<double>&) const override final;
        virtual void getParametersNames(
            std::set<std::string>&) const override final;
        virtual std::shared_ptr<Expr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const override final;
        virtual ~BinaryOperation();

       private:
        BinaryOperation& operator=(const BinaryOperation&) = delete;
        BinaryOperation& operator=(BinaryOperation&&) = delete;
        const std::shared_ptr<Expr> a;
        const std::shared_ptr<Expr> b;
      };  // end of struct BinaryOperation

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#include "TFEL/Math/Parser/BinaryOperator.ixx"

#endif /* LIB_TFEL_BINARYOPERATOR_HXX_ */
