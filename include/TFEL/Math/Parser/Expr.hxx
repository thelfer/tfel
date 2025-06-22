/*!
 * \file   include/TFEL/Math/Parser/Expr.hxx
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

#ifndef LIB_TFEL_EXPR_HXX_
#define LIB_TFEL_EXPR_HXX_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <memory>

namespace tfel {
  namespace math {

    namespace parser {

      struct Expr {
        virtual double getValue(void) const = 0;
        virtual void checkCyclicDependency(std::vector<std::string>&) const = 0;
        virtual std::shared_ptr<Expr> resolveDependencies(
            const std::vector<double>&) const = 0;
        virtual std::shared_ptr<Expr> clone(
            const std::vector<double>&) const = 0;
        virtual std::shared_ptr<Expr> differentiate(
            const std::vector<double>::size_type,
            const std::vector<double>&) const = 0;
        virtual void getParametersNames(std::set<std::string>&) const = 0;
        virtual std::shared_ptr<Expr>
        createFunctionByChangingParametersIntoVariables(
            const std::vector<double>&,
            const std::vector<std::string>&,
            const std::map<std::string, std::vector<double>::size_type>&)
            const = 0;
        virtual ~Expr();
      };  // end of struct Expr

      //! a simple alias
      using ExprPtr = std::shared_ptr<Expr>;

      //! a simple helper function for checkCyclicDependency
      void mergeVariablesNames(std::vector<std::string>&,
                               const std::vector<std::string>&);

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_EXPR_HXX_ */
