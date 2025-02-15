/*!
 * \file   include/TFEL/Math/Parser/BinaryOperator.ixx
 * \brief
 *
 * \author Thomas Helfer
 * \date   04/11/2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_TFEL_MATH_PARSER_BINARYOPERATORIXX
#define LIB_TFEL_MATH_PARSER_BINARYOPERATORIXX

namespace tfel::math::parser {

  template <typename Op>
  BinaryOperation<Op>::BinaryOperation(const std::shared_ptr<Expr> a_,
                                       const std::shared_ptr<Expr> b_) noexcept
      : a(a_), b(b_) {}  // end of BinaryOperation

  template <typename Op>
  bool BinaryOperation<Op>::isConstant() const {
    return (this->a->isConstant()) && (this->b->isConstant());
  }  // end of isConstant

  template <typename Op>
  bool BinaryOperation<Op>::dependsOnVariable(
      const std::vector<double>::size_type p) const {
    return this->a->dependsOnVariable(p) || this->b->dependsOnVariable(p);
  }  // end of dependsOnVariable

  template <typename Op>
  double BinaryOperation<Op>::getValue() const {
    return Op::apply(this->a->getValue(), this->b->getValue());
  }  // end of getValue

  template <typename Op>
  std::string BinaryOperation<Op>::getCxxFormula(
      const std::vector<std::string>& m) const {
    return Op::getCxxFormula(this->a->getCxxFormula(m),
                             this->b->getCxxFormula(m));
  }  // end of getCxxFormula

  template <typename Op>
  void BinaryOperation<Op>::checkCyclicDependency(
      std::vector<std::string>& names) const {
    using namespace std;
    using std::vector;
    vector<string> a_names(names);
    vector<string> b_names(names);
    this->a->checkCyclicDependency(a_names);
    this->b->checkCyclicDependency(b_names);
    mergeVariablesNames(names, a_names);
    mergeVariablesNames(names, b_names);
  }  // end of checkCyclicDependency

  template <typename Op>
  void BinaryOperation<Op>::getParametersNames(std::set<std::string>& p) const {
    this->a->getParametersNames(p);
    this->b->getParametersNames(p);
  }  // end of getParametersNames

  template <typename Op>
  std::shared_ptr<Expr> BinaryOperation<Op>::resolveDependencies(
      const std::vector<double>& v) const {
    return std::shared_ptr<Expr>(new BinaryOperation<Op>(
        this->a->resolveDependencies(v), this->b->resolveDependencies(v)));
  }  // end of resolveDependencies

  template <typename Op>
  std::shared_ptr<Expr> BinaryOperation<Op>::clone(
      const std::vector<double>& v) const {
    return std::shared_ptr<Expr>(
        new BinaryOperation<Op>(this->a->clone(v), this->b->clone(v)));
  }  // end of clone

  template <typename Op>
  std::shared_ptr<Expr>
  BinaryOperation<Op>::createFunctionByChangingParametersIntoVariables(
      const std::vector<double>& v,
      const std::vector<std::string>& params,
      const std::map<std::string, std::vector<double>::size_type>& pos) const {
    using std::shared_ptr;
    shared_ptr<Expr> na(
        this->a->createFunctionByChangingParametersIntoVariables(v, params,
                                                                 pos));
    shared_ptr<Expr> nb(
        this->b->createFunctionByChangingParametersIntoVariables(v, params,
                                                                 pos));
    return shared_ptr<Expr>(new BinaryOperation<Op>(na, nb));
  }  // end of createFunctionByChangingParametersIntoVariables

  template <typename BinaryOperation>
  std::shared_ptr<Expr> differentiateBinaryOperation(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&) {
    BinaryOperationBase::throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
    return {};
#endif
  }  // end of differentiateBinaryOperation

  template <typename Op>
  BinaryOperation<Op>::~BinaryOperation() {}  // end of ~BinaryOperation()

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpPlus>(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&);

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpMinus>(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&);

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpMult>(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&);

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpDiv>(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&);

  template <>
  std::shared_ptr<Expr> differentiateBinaryOperation<OpPower>(
      const std::shared_ptr<Expr>,
      const std::shared_ptr<Expr>,
      const std::vector<double>::size_type,
      const std::vector<double>&);

  template <typename Op>
  std::shared_ptr<Expr> BinaryOperation<Op>::differentiate(
      const std::vector<double>::size_type pos,
      const std::vector<double>& v) const {
    return differentiateBinaryOperation<Op>(this->a, this->b, pos, v);
  }  // end of differentiate

}  // end of namespace tfel::math::parser

#endif /* LIB_TFEL_MATH_PARSER_BINARYOPERATORIXX */
