/*!
 * \file   include/TFEL/Math/Parser/BinaryFunction.ixx
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

#ifndef LIB_TFEL_BINARYFUNCTION_IXX_
#define LIB_TFEL_BINARYFUNCTION_IXX_

#include <string>
#include <cstring>
#include <cerrno>

namespace tfel {
  namespace math {

    namespace parser {

      template <double (*f)(const double, const double)>
      StandardBinaryFunction<f>::StandardBinaryFunction(
          const std::shared_ptr<Expr> e1, const std::shared_ptr<Expr> e2)
          : expr1(e1),
            expr2(e2) {
      }  // end of StandardBinaryFunction::StandardBinaryFunction

      template <double (*f)(const double, const double)>
      StandardBinaryFunction<f>::~StandardBinaryFunction() {
      }  // end of StandardBinaryFunction::StandardBinaryFunction

      template <double (*f)(const double, const double)>
      double StandardBinaryFunction<f>::getValue(void) const {
        using namespace std;
        double res;
        int old = errno;
        errno = 0;
        res = f(this->expr1->getValue(), this->expr2->getValue());
        if (errno != 0) {
          int e = errno;
          errno = old;
          StandardBinaryFunctionBase::throwInvalidCallException(e);
        }
        return res;
      }  // end of StandardBinaryFunction::StandardBinaryFunction

      template <double (*f)(const double, const double)>
      void StandardBinaryFunction<f>::checkCyclicDependency(
          std::vector<std::string>& names) const {
        using namespace std;
        using std::vector;
        vector<string> a_names(names);
        vector<string> b_names(names);
        this->expr1->checkCyclicDependency(a_names);
        this->expr2->checkCyclicDependency(b_names);
        mergeVariablesNames(names, a_names);
        mergeVariablesNames(names, b_names);
      }  // end of StandardBinaryFunction<f>::checkCyclicDependency

      template <double (*f)(const double, const double)>
      std::shared_ptr<Expr> StandardBinaryFunction<f>::resolveDependencies(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(
            new StandardBinaryFunction<f>(this->expr1->resolveDependencies(v),
                                          this->expr2->resolveDependencies(v)));
      }  // end of StandardBinaryFunction<f>::resolveDependencies

      template <double (*f)(const double, const double)>
      void StandardBinaryFunction<f>::getParametersNames(
          std::set<std::string>& p) const {
        this->expr1->getParametersNames(p);
        this->expr2->getParametersNames(p);
      }  // end of StandardBinaryFunction<f>::getParametersNames

      template <double (*f)(const double, const double)>
      std::shared_ptr<Expr> StandardBinaryFunction<f>::differentiate(
          const std::vector<double>::size_type,
          const std::vector<double>&) const {
        StandardBinaryFunctionBase::
            throwUnimplementedDifferentiateFunctionException();
#ifndef _MSC_VER
        return {};
#endif
      }  // end of StandardBinaryFunction<f>::differentiate

      template <double (*f)(const double, const double)>
      std::shared_ptr<Expr> StandardBinaryFunction<f>::clone(
          const std::vector<double>& v) const {
        return std::shared_ptr<Expr>(new StandardBinaryFunction<f>(
            this->expr1->clone(v), this->expr2->clone(v)));
      }  // end of StandardBinaryFunction<f>::clone

      template <double (*f)(const double, const double)>
      std::shared_ptr<Expr> StandardBinaryFunction<f>::
          createFunctionByChangingParametersIntoVariables(
              const std::vector<double>& v,
              const std::vector<std::string>& params,
              const std::map<std::string, std::vector<double>::size_type>& pos)
              const {
        return std::shared_ptr<Expr>(new StandardBinaryFunction<f>(
            this->expr1->createFunctionByChangingParametersIntoVariables(
                v, params, pos),
            this->expr2->createFunctionByChangingParametersIntoVariables(
                v, params, pos)));
      }

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel

#endif /* LIB_TFEL_BINARYFUNCTION_IXX_ */
