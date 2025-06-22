/*!
 * \file   src/Math/EvaluatorFunction.cxx
 * \brief
 * \author Helfer Thomas
 * \date   13 jan 2009
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <stdexcept>
#include "TFEL/Math/Parser/EvaluatorFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      EvaluatorFunctionBase::EvaluatorFunctionBase() = default;

      std::shared_ptr<Expr> EvaluatorFunctionBase::differentiate(
          const std::vector<double>::size_type,
          const std::vector<double>&) const {
        throw(
            std::runtime_error("EvaluatorFunctionBase::differentiate: "
                               "unimplemented feature"));
      }  // end of EvaluatorFunctionBase::differentiate

      EvaluatorFunctionBase::~EvaluatorFunctionBase() = default;

      EvaluatorFunction1VBase::EvaluatorFunction1VBase(
          const std::shared_ptr<Expr> e)
          : expr(e) {
      }  // end of EvaluatorFunction1VBase::EvaluatorFunction1VBase

      void EvaluatorFunction1VBase::checkCyclicDependency(
          std::vector<std::string>& names) const {
        this->expr->checkCyclicDependency(names);
      }  // end of EvaluatorFunction1VBase::checkCyclicDependency

      EvaluatorFunction1VBase::~EvaluatorFunction1VBase() = default;

      EvaluatorFunction2VBase::EvaluatorFunction2VBase(
          const std::shared_ptr<Expr> e1_, const std::shared_ptr<Expr> e2_)
          : e1(e1_),
            e2(e2_) {
      }  // end of EvaluatorFunction2VBase::EvaluatorFunction2VBase

      void EvaluatorFunction2VBase::checkCyclicDependency(
          std::vector<std::string>& names) const {
        using namespace std;
        vector<string> e1_vars;
        vector<string> e2_vars;
        this->e1->checkCyclicDependency(e1_vars);
        this->e2->checkCyclicDependency(e2_vars);
        mergeVariablesNames(names, e1_vars);
        mergeVariablesNames(names, e1_vars);
      }  // end of EvaluatorFunction2VBase::checkCyclicDependency

      EvaluatorFunction2VBase::~EvaluatorFunction2VBase() = default;

      EvaluatorFunction1P1V::EvaluatorFunction1P1V(
          const EvaluatorProxyFunctionPtr1P1V f_,
          const int n_,
          const std::shared_ptr<Expr> e)
          : EvaluatorFunction1VBase(e),
            f(f_),
            n(n_) {}  // end of EvaluatorFunction1P1V<f>::EvaluatorFunction1P1V

      double EvaluatorFunction1P1V::getValue() const {
        return (*(this->f))(this->n, this->expr->getValue());
      }  // end of EvaluatorFunction1P1V::getValue(void) const

      std::shared_ptr<Expr> EvaluatorFunction1P1V::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction1P1V(
            this->f, this->n, this->expr->resolveDependencies(v)));
      }  // end of EvaluatorFunction1P1V::resolveDependencies(void) const

      void EvaluatorFunction1P1V::getParametersNames(
          std::set<std::string>& p) const {
        this->expr->getParametersNames(p);
      }  // end of EvaluatorFunction1P1V::getParametersNames

      std::shared_ptr<Expr> EvaluatorFunction1P1V::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(
            new EvaluatorFunction1P1V(this->f, this->n, this->expr->clone(v)));
      }  // end of EvaluatorFunction1P1V::clone(const std::vector<double>&)
         // const

      std::shared_ptr<Expr>
      EvaluatorFunction1P1V::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& params,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        return std::shared_ptr<Expr>(new EvaluatorFunction1P1V(
            this->f, this->n,
            this->expr->createFunctionByChangingParametersIntoVariables(
                v, params, pos)));
      }  // end of
         // EvaluatorFunction1P1V::createFunctionByChangingParametersIntoVariables

      EvaluatorFunction1P1V::~EvaluatorFunction1P1V() = default;

      EvaluatorFunction2P1V::EvaluatorFunction2P1V(
          const EvaluatorProxyFunctionPtr2P1V f_,
          const int n_,
          const int m_,
          const std::shared_ptr<Expr> e)
          : EvaluatorFunction1VBase(e),
            f(f_),
            n(n_),
            m(m_) {}  // end of EvaluatorFunction2P1V::EvaluatorFunction2P1V

      double EvaluatorFunction2P1V::getValue() const {
        return (*(this->f))(this->n, this->m, this->expr->getValue());
      }  // end of EvaluatorFunction2P1V::getValue(void) const

      std::shared_ptr<Expr> EvaluatorFunction2P1V::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P1V(
            this->f, this->n, this->m, this->expr->resolveDependencies(v)));
      }  // end of EvaluatorFunction2P1V::resolveDependencies(void) const

      void EvaluatorFunction2P1V::getParametersNames(
          std::set<std::string>& p) const {
        this->expr->getParametersNames(p);
      }  // end of EvaluatorFunction2P1V::getParametersNames

      std::shared_ptr<Expr> EvaluatorFunction2P1V::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P1V(
            this->f, this->n, this->m, this->expr->clone(v)));
      }  // end of EvaluatorFunction2P1V::clone(const std::vector<double>&)
         // const

      std::shared_ptr<Expr>
      EvaluatorFunction2P1V::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& params,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P1V(
            this->f, this->n, this->m,
            this->expr->createFunctionByChangingParametersIntoVariables(
                v, params, pos)));
      }  // end of
         // EvaluatorFunction2P1V::createFunctionByChangingParametersIntoVariables

      EvaluatorFunction2P1V::~EvaluatorFunction2P1V() = default;

      EvaluatorFunction1P2V::EvaluatorFunction1P2V(
          const EvaluatorProxyFunctionPtr1P2V f_,
          const int n_,
          const std::shared_ptr<Expr> e1_,
          const std::shared_ptr<Expr> e2_)
          : EvaluatorFunction2VBase(e1_, e2_),
            f(f_),
            n(n_) {}  // end of EvaluatorFunction1P2V::EvaluatorFunction1P2V

      double EvaluatorFunction1P2V::getValue() const {
        return (*(this->f))(this->n, this->e1->getValue(),
                            this->e2->getValue());
      }  // end of EvaluatorFunction1P2V::getValue(void) const

      std::shared_ptr<Expr> EvaluatorFunction1P2V::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction1P2V(
            this->f, this->n, this->e1->resolveDependencies(v),
            this->e2->resolveDependencies(v)));
      }  // end of EvaluatorFunction1P2V::resolveDependencies(void) const

      void EvaluatorFunction1P2V::getParametersNames(
          std::set<std::string>& p) const {
        this->e1->getParametersNames(p);
        this->e2->getParametersNames(p);
      }  // end of EvaluatorFunction1P2V::getParametersNames

      std::shared_ptr<Expr> EvaluatorFunction1P2V::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction1P2V(
            this->f, this->n, this->e1->clone(v), this->e2->clone(v)));
      }  // end of EvaluatorFunction1P2V::clone(const std::vector<double>&)
         // const

      std::shared_ptr<Expr>
      EvaluatorFunction1P2V::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& params,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        return std::shared_ptr<Expr>(new EvaluatorFunction1P2V(
            this->f, this->n,
            this->e1->createFunctionByChangingParametersIntoVariables(v, params,
                                                                      pos),
            this->e2->createFunctionByChangingParametersIntoVariables(v, params,
                                                                      pos)));
      }  // end of
         // EvaluatorFunction1P2V::createFunctionByChangingParametersIntoVariables

      EvaluatorFunction1P2V::~EvaluatorFunction1P2V() = default;

      EvaluatorFunction2P2V::EvaluatorFunction2P2V(
          const EvaluatorProxyFunctionPtr2P2V f_,
          const int n_,
          const int m_,
          const std::shared_ptr<Expr> e1_,
          const std::shared_ptr<Expr> e2_)
          : EvaluatorFunction2VBase(e1_, e2_),
            f(f_),
            n(n_),
            m(m_) {}  // end of EvaluatorFunction2P2V::EvaluatorFunction2P2V

      double EvaluatorFunction2P2V::getValue() const {
        return (*(this->f))(this->n, this->m, this->e1->getValue(),
                            this->e2->getValue());
      }  // end of EvaluatorFunction2P2V::getValue(void) const

      std::shared_ptr<Expr> EvaluatorFunction2P2V::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P2V(
            this->f, this->n, this->m, this->e1->resolveDependencies(v),
            this->e2->resolveDependencies(v)));
      }  // end of EvaluatorFunction2P2V::resolveDependencies(void) const

      void EvaluatorFunction2P2V::getParametersNames(
          std::set<std::string>& p) const {
        this->e1->getParametersNames(p);
        this->e2->getParametersNames(p);
      }  // end of EvaluatorFunction2P2V::getParametersNames

      std::shared_ptr<Expr> EvaluatorFunction2P2V::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P2V(
            this->f, this->n, this->m, this->e1->clone(v), this->e2->clone(v)));
      }  // end of EvaluatorFunction2P2V::clone(const std::vector<double>&)
         // const

      std::shared_ptr<Expr>
      EvaluatorFunction2P2V::createFunctionByChangingParametersIntoVariables(
          const std::vector<double>& v,
          const std::vector<std::string>& params,
          const std::map<std::string, std::vector<double>::size_type>& pos)
          const {
        return std::shared_ptr<Expr>(new EvaluatorFunction2P2V(
            this->f, this->n, this->m,
            this->e1->createFunctionByChangingParametersIntoVariables(v, params,
                                                                      pos),
            this->e2->createFunctionByChangingParametersIntoVariables(v, params,
                                                                      pos)));
      }  // end of
         // EvaluatorFunction2P2V::createFunctionByChangingParametersIntoVariables

      EvaluatorFunction2P2V::~EvaluatorFunction2P2V() = default;

      double EvaluatorFunctionWrapper<0u>::apply(
          const EvaluatorFunctionWrapper<0u>::type f,
          const std::vector<std::shared_ptr<Expr>>&) {
        return (*f)();
      }  // end of EvaluatorFunctionWrapper<0u>::apply

      double EvaluatorFunctionWrapper<0u>::apply(
          const EvaluatorFunctionWrapper<0u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>&) {
        return (*f)(n);
      }  // end of EvaluatorFunctionWrapper<0u>::apply

      double EvaluatorFunctionWrapper<0u>::apply(
          const EvaluatorFunctionWrapper<0u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>&) {
        return (*f)(n, m);
      }  // end of EvaluatorFunctionWrapper<0u>::apply

      double EvaluatorFunctionWrapper<0u>::apply(
          const EvaluatorFunctionWrapper<0u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>&) {
        return (*f)(n);
      }  // end of EvaluatorFunctionWrapper<0u>::apply

      double EvaluatorFunctionWrapper<0u>::apply(
          const EvaluatorFunctionWrapper<0u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>&) {
        return (*f)(n, m);
      }  // end of EvaluatorFunctionWrapper<0u>::apply

      double EvaluatorFunctionWrapper<1u>::apply(
          const EvaluatorFunctionWrapper<1u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue());
      }  // end of EvaluatorFunctionWrapper<1u>::apply

      double EvaluatorFunctionWrapper<1u>::apply(
          const EvaluatorFunctionWrapper<1u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue());
      }  // end of EvaluatorFunctionWrapper<1u>::apply

      double EvaluatorFunctionWrapper<1u>::apply(
          const EvaluatorFunctionWrapper<1u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue());
      }  // end of EvaluatorFunctionWrapper<1u>::apply

      double EvaluatorFunctionWrapper<1u>::apply(
          const EvaluatorFunctionWrapper<1u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue());
      }  // end of EvaluatorFunctionWrapper<1u>::apply

      double EvaluatorFunctionWrapper<1u>::apply(
          const EvaluatorFunctionWrapper<1u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue());
      }  // end of EvaluatorFunctionWrapper<1u>::apply

      double EvaluatorFunctionWrapper<2u>::apply(
          const EvaluatorFunctionWrapper<2u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue());
      }  // end of EvaluatorFunctionWrapper<2u>::apply

      double EvaluatorFunctionWrapper<2u>::apply(
          const EvaluatorFunctionWrapper<2u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue());
      }  // end of EvaluatorFunctionWrapper<2u>::apply

      double EvaluatorFunctionWrapper<2u>::apply(
          const EvaluatorFunctionWrapper<2u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue());
      }  // end of EvaluatorFunctionWrapper<2u>::apply

      double EvaluatorFunctionWrapper<2u>::apply(
          const EvaluatorFunctionWrapper<2u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue());
      }  // end of EvaluatorFunctionWrapper<2u>::apply

      double EvaluatorFunctionWrapper<2u>::apply(
          const EvaluatorFunctionWrapper<2u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue());
      }  // end of EvaluatorFunctionWrapper<2u>::apply

      double EvaluatorFunctionWrapper<3u>::apply(
          const EvaluatorFunctionWrapper<3u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue());
      }  // end of EvaluatorFunctionWrapper<3u>::apply

      double EvaluatorFunctionWrapper<3u>::apply(
          const EvaluatorFunctionWrapper<3u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue());
      }  // end of EvaluatorFunctionWrapper<3u>::apply

      double EvaluatorFunctionWrapper<3u>::apply(
          const EvaluatorFunctionWrapper<3u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue());
      }  // end of EvaluatorFunctionWrapper<3u>::apply

      double EvaluatorFunctionWrapper<3u>::apply(
          const EvaluatorFunctionWrapper<3u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue());
      }  // end of EvaluatorFunctionWrapper<3u>::apply

      double EvaluatorFunctionWrapper<3u>::apply(
          const EvaluatorFunctionWrapper<3u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue());
      }  // end of EvaluatorFunctionWrapper<3u>::apply

      double EvaluatorFunctionWrapper<4u>::apply(
          const EvaluatorFunctionWrapper<4u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue());
      }  // end of EvaluatorFunctionWrapper<4u>::apply

      double EvaluatorFunctionWrapper<4u>::apply(
          const EvaluatorFunctionWrapper<4u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue());
      }  // end of EvaluatorFunctionWrapper<4u>::apply

      double EvaluatorFunctionWrapper<4u>::apply(
          const EvaluatorFunctionWrapper<4u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue());
      }  // end of EvaluatorFunctionWrapper<4u>::apply

      double EvaluatorFunctionWrapper<4u>::apply(
          const EvaluatorFunctionWrapper<4u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue());
      }  // end of EvaluatorFunctionWrapper<4u>::apply

      double EvaluatorFunctionWrapper<4u>::apply(
          const EvaluatorFunctionWrapper<4u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue());
      }  // end of EvaluatorFunctionWrapper<4u>::apply

      double EvaluatorFunctionWrapper<5u>::apply(
          const EvaluatorFunctionWrapper<5u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue());
      }  // end of EvaluatorFunctionWrapper<5u>::apply

      double EvaluatorFunctionWrapper<5u>::apply(
          const EvaluatorFunctionWrapper<5u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue());
      }  // end of EvaluatorFunctionWrapper<5u>::apply

      double EvaluatorFunctionWrapper<5u>::apply(
          const EvaluatorFunctionWrapper<5u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue());
      }  // end of EvaluatorFunctionWrapper<5u>::apply

      double EvaluatorFunctionWrapper<5u>::apply(
          const EvaluatorFunctionWrapper<5u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue());
      }  // end of EvaluatorFunctionWrapper<5u>::apply

      double EvaluatorFunctionWrapper<5u>::apply(
          const EvaluatorFunctionWrapper<5u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue());
      }  // end of EvaluatorFunctionWrapper<5u>::apply

      double EvaluatorFunctionWrapper<6u>::apply(
          const EvaluatorFunctionWrapper<6u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue());
      }  // end of EvaluatorFunctionWrapper<6u>::apply

      double EvaluatorFunctionWrapper<6u>::apply(
          const EvaluatorFunctionWrapper<6u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue());
      }  // end of EvaluatorFunctionWrapper<6u>::apply

      double EvaluatorFunctionWrapper<6u>::apply(
          const EvaluatorFunctionWrapper<6u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue());
      }  // end of EvaluatorFunctionWrapper<6u>::apply

      double EvaluatorFunctionWrapper<6u>::apply(
          const EvaluatorFunctionWrapper<6u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue());
      }  // end of EvaluatorFunctionWrapper<6u>::apply

      double EvaluatorFunctionWrapper<6u>::apply(
          const EvaluatorFunctionWrapper<6u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue());
      }  // end of EvaluatorFunctionWrapper<6u>::apply

      double EvaluatorFunctionWrapper<7u>::apply(
          const EvaluatorFunctionWrapper<7u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue());
      }  // end of EvaluatorFunctionWrapper<7u>::apply

      double EvaluatorFunctionWrapper<7u>::apply(
          const EvaluatorFunctionWrapper<7u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue());
      }  // end of EvaluatorFunctionWrapper<7u>::apply

      double EvaluatorFunctionWrapper<7u>::apply(
          const EvaluatorFunctionWrapper<7u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue());
      }  // end of EvaluatorFunctionWrapper<7u>::apply

      double EvaluatorFunctionWrapper<7u>::apply(
          const EvaluatorFunctionWrapper<7u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue());
      }  // end of EvaluatorFunctionWrapper<7u>::apply

      double EvaluatorFunctionWrapper<7u>::apply(
          const EvaluatorFunctionWrapper<7u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue());
      }  // end of EvaluatorFunctionWrapper<7u>::apply

      double EvaluatorFunctionWrapper<8u>::apply(
          const EvaluatorFunctionWrapper<8u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue());
      }  // end of EvaluatorFunctionWrapper<8u>::apply

      double EvaluatorFunctionWrapper<8u>::apply(
          const EvaluatorFunctionWrapper<8u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue());
      }  // end of EvaluatorFunctionWrapper<8u>::apply

      double EvaluatorFunctionWrapper<8u>::apply(
          const EvaluatorFunctionWrapper<8u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue());
      }  // end of EvaluatorFunctionWrapper<8u>::apply

      double EvaluatorFunctionWrapper<8u>::apply(
          const EvaluatorFunctionWrapper<8u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue());
      }  // end of EvaluatorFunctionWrapper<8u>::apply

      double EvaluatorFunctionWrapper<8u>::apply(
          const EvaluatorFunctionWrapper<8u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue());
      }  // end of EvaluatorFunctionWrapper<8u>::apply

      double EvaluatorFunctionWrapper<9u>::apply(
          const EvaluatorFunctionWrapper<9u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(
            expr[0]->getValue(), expr[1]->getValue(), expr[2]->getValue(),
            expr[3]->getValue(), expr[4]->getValue(), expr[5]->getValue(),
            expr[6]->getValue(), expr[7]->getValue(), expr[8]->getValue());
      }  // end of EvaluatorFunctionWrapper<9u>::apply

      double EvaluatorFunctionWrapper<9u>::apply(
          const EvaluatorFunctionWrapper<9u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(
            n, expr[0]->getValue(), expr[1]->getValue(), expr[2]->getValue(),
            expr[3]->getValue(), expr[4]->getValue(), expr[5]->getValue(),
            expr[6]->getValue(), expr[7]->getValue(), expr[8]->getValue());
      }  // end of EvaluatorFunctionWrapper<9u>::apply

      double EvaluatorFunctionWrapper<9u>::apply(
          const EvaluatorFunctionWrapper<9u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(
            n, m, expr[0]->getValue(), expr[1]->getValue(), expr[2]->getValue(),
            expr[3]->getValue(), expr[4]->getValue(), expr[5]->getValue(),
            expr[6]->getValue(), expr[7]->getValue(), expr[8]->getValue());
      }  // end of EvaluatorFunctionWrapper<9u>::apply

      double EvaluatorFunctionWrapper<9u>::apply(
          const EvaluatorFunctionWrapper<9u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(
            n, expr[0]->getValue(), expr[1]->getValue(), expr[2]->getValue(),
            expr[3]->getValue(), expr[4]->getValue(), expr[5]->getValue(),
            expr[6]->getValue(), expr[7]->getValue(), expr[8]->getValue());
      }  // end of EvaluatorFunctionWrapper<9u>::apply

      double EvaluatorFunctionWrapper<9u>::apply(
          const EvaluatorFunctionWrapper<9u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(
            n, m, expr[0]->getValue(), expr[1]->getValue(), expr[2]->getValue(),
            expr[3]->getValue(), expr[4]->getValue(), expr[5]->getValue(),
            expr[6]->getValue(), expr[7]->getValue(), expr[8]->getValue());
      }  // end of EvaluatorFunctionWrapper<9u>::apply

      double EvaluatorFunctionWrapper<10u>::apply(
          const EvaluatorFunctionWrapper<10u>::type f,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue(),
                    expr[8]->getValue(), expr[9]->getValue());
      }  // end of EvaluatorFunctionWrapper<10u>::apply

      double EvaluatorFunctionWrapper<10u>::apply(
          const EvaluatorFunctionWrapper<10u>::type1P f,
          const int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue(),
                    expr[8]->getValue(), expr[9]->getValue());
      }  // end of EvaluatorFunctionWrapper<10u>::apply

      double EvaluatorFunctionWrapper<10u>::apply(
          const EvaluatorFunctionWrapper<10u>::type2P f,
          const int n,
          const int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue(),
                    expr[8]->getValue(), expr[9]->getValue());
      }  // end of EvaluatorFunctionWrapper<10u>::apply

      double EvaluatorFunctionWrapper<10u>::apply(
          const EvaluatorFunctionWrapper<10u>::type1UP f,
          const unsigned int n,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue(),
                    expr[8]->getValue(), expr[9]->getValue());
      }  // end of EvaluatorFunctionWrapper<10u>::apply

      double EvaluatorFunctionWrapper<10u>::apply(
          const EvaluatorFunctionWrapper<10u>::type2UP f,
          const unsigned int n,
          const unsigned int m,
          const std::vector<std::shared_ptr<Expr>>& expr) {
        return (*f)(n, m, expr[0]->getValue(), expr[1]->getValue(),
                    expr[2]->getValue(), expr[3]->getValue(),
                    expr[4]->getValue(), expr[5]->getValue(),
                    expr[6]->getValue(), expr[7]->getValue(),
                    expr[8]->getValue(), expr[9]->getValue());
      }  // end of EvaluatorFunctionWrapper<10u>::apply

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
