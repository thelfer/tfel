/*!
 * \file   src/Math/DifferentiatedFunctionExpr.cxx
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

#include <sstream>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/Parser/Number.hxx"
#include "TFEL/Math/Parser/Variable.hxx"
#include "TFEL/Math/Parser/BinaryOperator.hxx"
#include "TFEL/Math/Parser/ExternalFunctionExpr2.hxx"
#include "TFEL/Math/Parser/DifferentiatedFunctionExpr.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      DifferentiatedFunctionExpr::DifferentiatedFunctionExpr(
          std::shared_ptr<ExternalFunction> ff,
          std::vector<std::shared_ptr<Expr>>& fargs,
          const std::vector<std::vector<double>::size_type>& fpvar)
          : f(std::move(ff)), args(fargs), pvar(fpvar) {
        raise_if(f->getNumberOfVariables() != args.size(),
                 "DifferentiatedFunctionExpr::DifferentiatedFunctionExpr: "
                 "invalid number of arguments for function "
                 "(" +
                     std::to_string(this->args.size()) + " given, " +
                     std::to_string(f->getNumberOfVariables()) + " required)");
      }  // end of DifferentiatedFunctionExpr::DifferentiatedFunctionExpr

      std::shared_ptr<ExternalFunction>
      DifferentiatedFunctionExpr::getDerivative() const {
        auto df = this->f;
        for (const auto& p : this->pvar) {
          df = df->differentiate(p);
        }
        return df;
      }  // end of DifferentiatedFunctionExpr::getDerivative

      double DifferentiatedFunctionExpr::getValue() const {
        using namespace std;
        using namespace tfel::math::parser;
        vector<shared_ptr<Expr>>::const_iterator p;
        vector<shared_ptr<Expr>>::size_type i;
        auto df = this->getDerivative();
        for (p = this->args.begin(), i = 0u; p != this->args.end(); ++p, ++i) {
          const double val = (*p)->getValue();
          df->setVariableValue(i, val);
        }
        return df->getValue();
      }  // end of DifferentiatedFunctionExpr::getValue

      void DifferentiatedFunctionExpr::checkCyclicDependency(
          std::vector<std::string>& names) const {
        std::vector<std::string> v(names);
        this->f->checkCyclicDependency(names);
        for (const auto& a : this->args) {
          std::vector<std::string> n(v);
          a->checkCyclicDependency(n);
          mergeVariablesNames(names, n);
        }
      }  // end of DifferentiatedFunctionExpr::checkCyclicDependency

      std::shared_ptr<Expr> DifferentiatedFunctionExpr::differentiate(
          const std::vector<double>::size_type pos,
          const std::vector<double>& v) const {
        using namespace std;
        vector<shared_ptr<Expr>> nargs(this->args.size());
        auto p = this->args.begin();
        vector<shared_ptr<Expr>>::const_iterator p3;
        vector<shared_ptr<Expr>>::iterator p4;
        unsigned short i = 0;
        if (args.empty()) {
          return std::make_shared<Number>(0.);
        }
        for (p3 = this->args.begin(), p4 = nargs.begin();
             p3 != this->args.end(); ++p3, ++p4) {
          *p4 = (*p3)->clone(v);
        }
        auto ndf = this->getDerivative();
        auto df_ = std::make_shared<ExternalFunctionExpr2>(
            ndf->differentiate(i), nargs);
        std::shared_ptr<Expr> df = std::make_shared<BinaryOperation<OpMult>>(
            df_, (*p)->differentiate(pos, v));
        ++p;
        ++i;
        while (p != this->args.end()) {
          df_ = std::make_shared<ExternalFunctionExpr2>(ndf->differentiate(i),
                                                        nargs);
          std::shared_ptr<Expr> df2 = std::make_shared<BinaryOperation<OpMult>>(
              df_, (*p)->differentiate(pos, v));
          df = std::make_shared<BinaryOperation<OpPlus>>(df, df2);
          ++p;
          ++i;
        }
        return df;
      }  // end of DifferentiatedFunctionExpr::differentiate

      std::shared_ptr<Expr> DifferentiatedFunctionExpr::clone(
          const std::vector<double>& v) const {
        using namespace std;
        vector<shared_ptr<Expr>> nargs(this->args.size());
        vector<shared_ptr<Expr>>::const_iterator p;
        vector<shared_ptr<Expr>>::iterator p2;
        for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
             ++p, ++p2) {
          *p2 = (*p)->clone(v);
        }
        return shared_ptr<Expr>(
            new DifferentiatedFunctionExpr(this->f, nargs, this->pvar));
      }  // end of DifferentiatedFunctionExpr::clone

      std::shared_ptr<Expr> DifferentiatedFunctionExpr::
          createFunctionByChangingParametersIntoVariables(
              const std::vector<double>& v,
              const std::vector<std::string>& params,
              const std::map<std::string, std::vector<double>::size_type>& pos)
              const {
        using namespace std;
        vector<shared_ptr<Expr>> nargs;
        vector<string> vnames;
        shared_ptr<ExternalFunction> nf;
        vector<shared_ptr<Expr>>::const_iterator p;
        vector<shared_ptr<Expr>>::iterator p2;
        map<string, vector<double>::size_type>::const_iterator p3;
        vector<string>::size_type i;
        nf = this->f->createFunctionByChangingParametersIntoVariables(
            vnames, v, params, pos);
        raise_if(nf->getNumberOfVariables() <= this->args.size(),
                 "ExternalFunctionExpr::getValue: "
                 "internal error (function has less variable after "
                 "'createFunctionByChangingParametersIntoVariables' "
                 "than before");
        nargs.resize(nf->getNumberOfVariables());
        for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
             ++p, ++p2) {
          *p2 = (*p)->createFunctionByChangingParametersIntoVariables(v, params,
                                                                      pos);
        }
        for (i = 0; i != vnames.size(); ++i) {
          p3 = pos.find(vnames[i]);
          raise_if(p3 == pos.end(),
                   "ExternalFunctionExpr::"
                   "createFunctionByChangingParametersIntoVariables: "
                   "internal error (no position found for "
                   "parameter '" +
                       vnames[i] + "')");
          nargs[args.size() + i] =
              shared_ptr<Expr>(new Variable(v, p3->second));
        }
        return shared_ptr<Expr>(
            new DifferentiatedFunctionExpr(nf, nargs, this->pvar));
      }  // end of
         // DifferentiatedFunctionExpr::createFunctionByChangingParametersIntoVariables

      void DifferentiatedFunctionExpr::getParametersNames(
          std::set<std::string>& p) const {
        this->f->getParametersNames(p);
        for (const auto& a : this->args) {
          a->getParametersNames(p);
        }
      }  // end of DifferentiatedFunctionExpr::getParametersNames

      std::shared_ptr<Expr> DifferentiatedFunctionExpr::resolveDependencies(
          const std::vector<double>& v) const {
        using namespace std;
        vector<shared_ptr<Expr>> nargs(this->args.size());
        vector<shared_ptr<Expr>>::const_iterator p;
        vector<shared_ptr<Expr>>::iterator p2;
        for (p = this->args.begin(), p2 = nargs.begin(); p != this->args.end();
             ++p, ++p2) {
          *p2 = (*p)->resolveDependencies(v);
        }
        return std::make_shared<ExternalFunctionExpr2>(this->getDerivative(),
                                                       nargs);
      }  // end of DifferentiatedFunctionExpr::resolveDependencies

      DifferentiatedFunctionExpr::~DifferentiatedFunctionExpr() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
