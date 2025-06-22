/*!
 * \file   src/Math/Variable.cxx
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

#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/Variable.hxx"

namespace tfel::math::parser {

  Variable::Variable(const std::vector<double>& v_,
                     const std::vector<double>::size_type p_)
      : v(v_), pos(p_) {}  // end of Variable::Variable

  bool Variable::isConstant() const { return false; }  // end of isConstant

  bool Variable::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return pos == p;
  }  // end of dependsOnVariable

  double Variable::getValue() const {
    return this->v[this->pos];
  }  // end of Variable::getValue

  std::string Variable::getCxxFormula(const std::vector<std::string>& m) const {
    tfel::raise_if(this->pos >= m.size(),
                   "Variable::getCxxFormula: "
                   "invalid argument");
    return m[this->pos];
  }  // end of Variable::getCxxFormula

  void Variable::checkCyclicDependency(std::vector<std::string>&) const {
  }  // end of Variable::checkCyclicDependency

  void Variable::getParametersNames(std::set<std::string>&) const {
  }  // end of Variable::getParametersNames

  std::shared_ptr<Expr> Variable::differentiate(
      const std::vector<double>::size_type dpos,
      const std::vector<double>&) const {
    if (dpos != this->pos) {
      return Number::zero();
    }
    return Number::one();
  }  // end of Variable::differentiate

  std::shared_ptr<Expr> Variable::clone(const std::vector<double>& v_) const {
    return std::shared_ptr<Expr>(new Variable(v_, this->pos));
  }  // end of Variable::clone

  std::shared_ptr<Expr>
  Variable::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v_,
      const std::vector<std::string>&,
      const std::map<std::string, std::vector<double>::size_type>&) const {
    raise_if(v_.size() < this->pos,
             "Variable::createFunctionByChanging"
             "ParametersIntoVariables: "
             "invalid vector size");
    return std::shared_ptr<Expr>(new Variable(v_, this->pos));
  }  // end of Variable::createFunctionByChangingParametersIntoVariables

  std::shared_ptr<Expr> Variable::resolveDependencies(
      const std::vector<double>& nv) const {
    return std::shared_ptr<Expr>(new Variable(nv, this->pos));
  }  // end of Variable::resolveDependencies

  Variable::~Variable() = default;

}  // end of namespace tfel::math::parser
