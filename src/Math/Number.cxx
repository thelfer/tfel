/*!
 * \file   src/Math/Number.cxx
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

#include "TFEL/Math/Parser/Number.hxx"

namespace tfel {
  namespace math {

    namespace parser {

      Number::Number(const double v) : value(v) {}  // end of Number::Number

      double Number::getValue() const {
        return this->value;
      }  // end of Number::getValue

      void Number::getParametersNames(std::set<std::string>&) const {
      }  // end of Number::getParametersNames

      void Number::checkCyclicDependency(std::vector<std::string>&) const {
      }  // end of Number::checkCyclicDependency

      std::shared_ptr<Expr> Number::differentiate(
          const std::vector<double>::size_type,
          const std::vector<double>&) const {
        return std::shared_ptr<Expr>(new Number(0.));
      }  // end of Number::differentiate

      std::shared_ptr<Expr> Number::clone(const std::vector<double>&) const {
        return std::shared_ptr<Expr>(new Number(this->value));
      }  // end of Number::clone

      std::shared_ptr<Expr>
      Number::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&) const {
        return this->clone(v);
      }  // end of Number::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<Expr> Number::resolveDependencies(
          const std::vector<double>& v) const {
        return this->clone(v);
      }  // end of Number::resolveDependencies

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
