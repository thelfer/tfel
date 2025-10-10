/*!
 * \file   src/Math/Number.cxx
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

#include "TFEL/Math/Parser/Number.hxx"

namespace tfel::math::parser {

  std::shared_ptr<Number> Number::zero() {
    return std::make_shared<Number>("0", 0);
  }  // end of zero

  std::shared_ptr<Number> Number::one() {
    return std::make_shared<Number>("1", 1);
  }  // end of one

  Number::Number(const std::string& s, const double v)
      : str(s), value(v) {}  // end of Number

  bool Number::isConstant() const { return true; }  // end of isConstant

  bool Number::dependsOnVariable(const std::vector<double>::size_type) const {
    return false;
  }  // end of dependsOnVariable

  std::string Number::getCxxFormula(const std::vector<std::string>&) const {
    return this->str;
  }  // end of getCxxFormula

  double Number::getValue() const { return this->value; }  // end of getValue

  void Number::getParametersNames(std::set<std::string>&) const {
  }  // end of getParametersNames

  void Number::checkCyclicDependency(std::vector<std::string>&) const {
  }  // end of checkCyclicDependency

  std::shared_ptr<Expr> Number::differentiate(
      const std::vector<double>::size_type, const std::vector<double>&) const {
    return Number::zero();
  }  // end of differentiate

  std::shared_ptr<Expr> Number::clone(const std::vector<double>&) const {
    return std::make_shared<Number>(this->str, this->value);
  }  // end of clone

  std::shared_ptr<Expr> Number::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>&,
      const std::map<std::string, std::vector<double>::size_type>&) const {
    return this->clone(v);
  }  // end of createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> Number::resolveDependencies(
      const std::vector<double>& v) const {
    return this->clone(v);
  }  // end of resolveDependencies

  Number::~Number() = default;

}  // end of namespace tfel::math::parser
