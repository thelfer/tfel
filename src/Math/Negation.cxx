/*!
 * \file   src/Math/Negation.cxx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04 oct 2007
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Math/Parser/Negation.hxx"

namespace tfel::math::parser {

  Negation::Negation(const std::shared_ptr<Expr> e)
      : expr(e) {}  // end of Negation

  bool Negation::isConstant() const {
    return this->expr->isConstant();
  }  // end of isConstant

  bool Negation::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return this->expr->dependsOnVariable(p);
  }  // end of dependsOnVariable

  std::string Negation::getCxxFormula(const std::vector<std::string>& m) const {
    return "-(" + this->expr->getCxxFormula(m) + ')';
  }  // end of getCxxFormula

  double Negation::getValue() const {
    return -(this->expr->getValue());
  }  // end of getValue()

  void Negation::checkCyclicDependency(std::vector<std::string>& names) const {
    this->expr->checkCyclicDependency(names);
  }  // end of checkCyclicDependency

  std::shared_ptr<Expr> Negation::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& variable) const {
    using std::shared_ptr;
    shared_ptr<Expr> e = this->expr->differentiate(pos, variable);
    return shared_ptr<Expr>(new Negation(e));
  }  // end of differentiate

  std::shared_ptr<Expr> Negation::clone(const std::vector<double>& v) const {
    return std::shared_ptr<Expr>(new Negation(this->expr->clone(v)));
  }

  std::shared_ptr<Expr>
  Negation::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& params,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    using std::shared_ptr;
    shared_ptr<Expr> nexpr =
        this->expr->createFunctionByChangingParametersIntoVariables(v, params,
                                                                    pos);
    return shared_ptr<Expr>(new Negation(nexpr));
  }  // end of createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> Negation::resolveDependencies(
      const std::vector<double>& v) const {
    using std::shared_ptr;
    return shared_ptr<Expr>(new Negation(this->expr->resolveDependencies(v)));
  }  // end of resolveDependencies

  void Negation::getParametersNames(std::set<std::string>& p) const {
    this->expr->getParametersNames(p);
  }  // end of getParametersNames

  Negation::~Negation() = default;

}  // end of namespace tfel::math::parser
