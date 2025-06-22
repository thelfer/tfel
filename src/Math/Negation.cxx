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
      : expr(e) {}  // end of Negation::Negation

  std::string Negation::getCxxFormula(const std::vector<std::string>& m) const {
    return "-(" + this->expr->getCxxFormula(m) + ')';
  }  // end of Negation::getCxxFormula

  double Negation::getValue() const {
    return -(this->expr->getValue());
  }  // end of Negation::getValue()

  void Negation::checkCyclicDependency(std::vector<std::string>& names) const {
    this->expr->checkCyclicDependency(names);
  }  // end of Negation::checkCyclicDependency

  std::shared_ptr<Expr> Negation::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& variable) const {
    using std::shared_ptr;
    shared_ptr<Expr> e = this->expr->differentiate(pos, variable);
    return shared_ptr<Expr>(new Negation(e));
  }  // end of Negation::differentiate

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
  }  // end of Negation::createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> Negation::resolveDependencies(
      const std::vector<double>& v) const {
    using std::shared_ptr;
    return shared_ptr<Expr>(new Negation(this->expr->resolveDependencies(v)));
  }  // end of Negation::resolveDependencies

  void Negation::getParametersNames(std::set<std::string>& p) const {
    this->expr->getParametersNames(p);
  }  // end of Negation::getParametersNames

  Negation::~Negation() = default;

}  // end of namespace tfel::math::parser
