/*!
 * \file   src/Math/ExternalCastemFunction.cxx
 * \brief
 * \author Helfer Thomas
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <sstream>
#include <stdexcept>
#include "TFEL/Math/Parser/ExternalCastemFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      ExternalCastemFunction::ExternalCastemFunction(
          const tfel::system::CastemFunctionPtr f_, const unsigned short n)
          : variables(n), f(f_) {}  // end of ExternalCastemFunction

      double ExternalCastemFunction::getValue() const {
        return (*f)(&(this->variables[0]));
      }  // end of getValue(void) const

      void ExternalCastemFunction::setVariableValue(
          const std::vector<double>::size_type pos, const double value) {
        using namespace std;
        if (pos >= this->variables.size()) {
          ostringstream msg;
          msg << "ExternalCFunctionBase::setVariableValue : "
              << "invalid index " << pos << " (function has only "
              << this->variables.size() << " variables).";
          throw(runtime_error(msg.str()));
        }
        this->variables[pos] = value;
      }  // end of ExternalCastemFunction::setVariableValue

      std::vector<double>::size_type
      ExternalCastemFunction::getNumberOfVariables() const {
        return this->variables.size();
      }  // end of ExternalCastemFunction::getNumberOfVariables

      void ExternalCastemFunction::checkCyclicDependency(
          const std::string&) const {
      }  // end of ExternalCastemFunction::checkCyclicDependency

      void ExternalCastemFunction::checkCyclicDependency(
          std::vector<std::string>&) const {
      }  // end of ExternalCastemFunction::checkCyclicDependency

      void ExternalCastemFunction::getParametersNames(
          std::set<std::string>&) const {
      }  // endo of ExternalCastemFunction::getParametersNames

      std::shared_ptr<ExternalFunction>
      ExternalCastemFunction::createFunctionByChangingParametersIntoVariables(
          const std::vector<std::string>&) const {
        throw(std::runtime_error(
            "ExternalCastemFunction::"
            "createFunctionByChangingParametersIntoVariables : "
            "invalid call"));
      }  // end of
         // ExternalCastemFunction::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<ExternalFunction>
      ExternalCastemFunction::createFunctionByChangingParametersIntoVariables(
          std::vector<std::string>& v,
          const std::vector<double>&,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&) const {
        v.clear();
        return this->resolveDependencies();
      }  // end of
         // ExternalCastemFunction::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<ExternalFunction>
      ExternalCastemFunction::resolveDependencies() const {
        return std::shared_ptr<ExternalFunction>(new ExternalCastemFunction(
            this->f, static_cast<unsigned short>(this->variables.size())));
      }  // end of ExternalCastemFunction::resolveDependencies(void) const

      std::shared_ptr<ExternalFunction> ExternalCastemFunction::differentiate(
          const std::vector<double>::size_type) const {
        using namespace std;
        string msg("ExternalCastemFunction::differentiate : ");
        msg += "can't differentiate external function";
        throw(runtime_error(msg));
      }  // end of ExternalCastemFunction::differentiate

      std::shared_ptr<ExternalFunction> ExternalCastemFunction::differentiate(
          const std::string&) const {
        using namespace std;
        string msg("ExternalCastemFunction::differentiate : ");
        msg += "can't differentiate external function";
        throw(runtime_error(msg));
      }  // end of ExternalCastemFunction::differentiate

      ExternalCastemFunction::~ExternalCastemFunction() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
