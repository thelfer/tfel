/*!
 * \file  src/Math/ConstantExternalFunction.cxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mars 2013
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
#include "TFEL/Math/Parser/ConstantExternalFunction.hxx"

namespace tfel {

  namespace math {

    namespace parser {

      ConstantExternalFunction::ConstantExternalFunction(const double v)
          : value(v) {
      }  // end of ConstantExternalFunction::ConstantExternalFunction

      double ConstantExternalFunction::getValue() const {
        return this->value;
      }  // end of ConstantExternalFunction::getValue

      void ConstantExternalFunction::setVariableValue(
          const std::vector<double>::size_type pos, const double) {
        raise(
            "ConstantExternalFunction::setVariableValue: "
            "invalid index " +
            std::to_string(pos) + " (function has no  variable).");
      }

      std::vector<double>::size_type
      ConstantExternalFunction::getNumberOfVariables() const {
        return 0u;
      }

      void ConstantExternalFunction::checkCyclicDependency(
          const std::string&) const {}

      void ConstantExternalFunction::checkCyclicDependency(
          std::vector<std::string>&) const {}

      std::shared_ptr<ExternalFunction> ConstantExternalFunction::differentiate(
          const std::vector<double>::size_type) const {
        return std::make_shared<ConstantExternalFunction>(0.);
      }  // end of ConstantExternalFunction:: differentiate

      std::shared_ptr<ExternalFunction> ConstantExternalFunction::differentiate(
          const std::string&) const {
        return std::make_shared<ConstantExternalFunction>(0.);
      }  // end of ConstantExternalFunction:: differentiate

      std::shared_ptr<ExternalFunction>
      ConstantExternalFunction::resolveDependencies() const {
        return std::make_shared<ConstantExternalFunction>(this->value);
      }

      std::shared_ptr<ExternalFunction>
      ConstantExternalFunction::createFunctionByChangingParametersIntoVariables(
          const std::vector<std::string>&) const {
        raise(
            "ConstantExternalFunction::"
            "createFunctionByChangingParametersIntoVariables : "
            "invalid call");
      }  // end of
         // ConstantExternalFunction::createFunctionByChangingParametersIntoVariables

      std::shared_ptr<ExternalFunction>
      ConstantExternalFunction::createFunctionByChangingParametersIntoVariables(
          std::vector<std::string>&,
          const std::vector<double>&,
          const std::vector<std::string>&,
          const std::map<std::string, std::vector<double>::size_type>&) const {
        return this->resolveDependencies();
      }  // end of
         // ConstantExternalFunction::createFunctionByChangingParametersIntoVariables

      void ConstantExternalFunction::getParametersNames(
          std::set<std::string>&) const {
      }  // endo of ConstantExternalFunction::getParametersNames

      ConstantExternalFunction::~ConstantExternalFunction() = default;

    }  // end of namespace parser

  }  // end of namespace math

}  // end of namespace tfel
