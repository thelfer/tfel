/*!
 * \file   src/Math/ExternalCyranoFunction.cxx
 * \brief
 * \author Thomas Helfer
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
#include "TFEL/Math/Parser/ExternalCyranoFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      ExternalCyranoFunction::ExternalCyranoFunction(
          const CyranoMaterialPropertyPtr f_, const unsigned short n)
          : variables(n), f(f_) {}  // end of ExternalCyranoFunction

      double ExternalCyranoFunction::getValue() const {
        const auto* const v =
            this->variables.size() == 0 ? nullptr : this->variables.data();
        auto s = CyranoOutputStatus{};
        const auto r =
            (*f)(&s, v, static_cast<CyranoIntegerType>(this->variables.size()),
                 CYRANO_NONE_POLICY);
        if (s.status != 0) {
          tfel::raise("ExternalCyranoFunction::getValue: " +
                      std::string(s.msg));
        }
        return r;
      }  // end of getValue() const

      void ExternalCyranoFunction::setVariableValue(
          const std::vector<double>::size_type pos, const double value) {
        raise_if(pos >= this->variables.size(),
                 "ExternalCFunctionBase::setVariableValue: "
                 "invalid index " +
                     std::to_string(pos) + " (function has only " +
                     std::to_string(this->variables.size()) + " variables).");
        this->variables[pos] = value;
      }  // end of ExternalCyranoFunction::setVariableValue

      std::vector<double>::size_type
      ExternalCyranoFunction::getNumberOfVariables() const {
        return this->variables.size();
      }  // end of ExternalCyranoFunction::getNumberOfVariables

      void ExternalCyranoFunction::checkCyclicDependency(
          const std::string&) const {
      }  // end of ExternalCyranoFunction::checkCyclicDependency

      void ExternalCyranoFunction::checkCyclicDependency(
          std::vector<std::string>&) const {
      }  // end of ExternalCyranoFunction::checkCyclicDependency

      void ExternalCyranoFunction::getParametersNames(
          std::set<std::string>&) const {
      }  // endo of ExternalCyranoFunction::getParametersNames

      std::shared_ptr<ExternalFunction>
      ExternalCyranoFunction::createFunctionByChangingParametersIntoVariables(
          const std::vector<std::string>&) const {
        raise(
            "ExternalCyranoFunction::"
            "createFunctionByChangingParametersIntoVariables : "
            "invalid call");
      }  // end of
         // ExternalCyranoFunction::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<ExternalFunction>
      ExternalCyranoFunction::createFunctionByChangingParametersIntoVariables(
          std::vector<std::string>& v,
          const std::vector<double>&,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&) const {
        v.clear();
        return this->resolveDependencies();
      }  // end of
         // ExternalCyranoFunction::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<ExternalFunction>
      ExternalCyranoFunction::resolveDependencies() const {
        return std::shared_ptr<ExternalFunction>(new ExternalCyranoFunction(
            this->f, static_cast<unsigned short>(this->variables.size())));
      }  // end of ExternalCyranoFunction::resolveDependencies() const

      std::shared_ptr<ExternalFunction> ExternalCyranoFunction::differentiate(
          const std::vector<double>::size_type) const {
        raise(
            "ExternalCyranoFunction::differentiate: "
            "can't differentiate external function");
      }  // end of ExternalCyranoFunction::differentiate

      std::shared_ptr<ExternalFunction> ExternalCyranoFunction::differentiate(
          const std::string&) const {
        raise(
            "ExternalCyranoFunction::differentiate: "
            "can't differentiate external function");
      }  // end of ExternalCyranoFunction::differentiate

      ExternalCyranoFunction::~ExternalCyranoFunction() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
