/*!
 * \file   mfront/src/UserDefinedNonLinearSystemSolver.cxx
 * \brief
 * \author Thomas Helfer
 * \date   09/09/2021
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/UserDefinedNonLinearSystemSolver.hxx"

namespace mfront {

  UserDefinedNonLinearSystemSolver::UserDefinedNonLinearSystemSolver(
      const DataMap& d) {
    auto raise = [](const std::string& msg) {
      tfel::raise(
          "UserDefinedNonLinearSystemSolver::"
          "UserDefinedNonLinearSystemSolver: " +
          msg);
    };
    const auto authorised_keys = std::vector<std::string>{
        "class_name",
        "header_file",
        "use_jacobian",
        "use_jacobian_invert",
        "requires_numerical_jacobian",
        "allows_jacobian_initialisation",
        "allows_jacobian_invert_initialisation",
        "requires_jacobian_reinitialisation_to_identity_at_each_"
        "iterations"};
    for (auto& kv : d) {
      if (std::find(authorised_keys.begin(), authorised_keys.end(), kv.first) ==
          authorised_keys.end()) {
        auto msg =
            "unsupported option '" + kv.first + "'.\nAuthorized options are: ";
        for (const auto& k : authorised_keys) {
          msg += "- " + k + "\n";
        }
        raise(msg);
      }
    }
    auto extract = [&raise](auto& value, const auto& option,
                            const char* const n) {
      using value_type = std::remove_reference_t<decltype(value)>;
      if (!option.template is<value_type>()) {
        raise("invalid type for option '" + std::string{n} + "'");
      }
      value = option.template get<value_type>();
    };
    auto assign_if = [&d, &extract](bool& b, const char* const k) {
      const auto p = d.find(k);
      if (p != d.end()) {
        extract(b, p->second, k);
      }
    };
    // class name
    auto p = d.find("class_name");
    if (p == d.end()) {
      raise("no option  named 'class_name'");
    }
    extract(this->class_name, p->second, "class_name");
    //
    p = d.find("header_file");
    if (p != d.end()) {
      extract(this->header_file, p->second, "header_file");
    } else {
      this->header_file = "TFEL/Math/" + this->class_name + ".hxx";
    }
    // other options
    assign_if(this->use_jacobian, "use_jacobian");
    assign_if(this->use_jacobian_invert, "use_jacobian_invert");
    assign_if(this->requires_numerical_jacobian, "requires_numerical_jacobian");
    assign_if(this->allows_jacobian_initialisation,
              "allows_jacobian_initialisation");
    assign_if(this->allows_jacobian_invert_initialisation,
              "allows_jacobian_invert_initialisation");
    assign_if(
        this->requires_jacobian_reinitialisation_to_identity_at_each_iterations,
        "requires_jacobian_reinitialisation_to_identity_at_each_iterations");
  }  // end of UserDefinedNonLinearSystemSolver

  std::string UserDefinedNonLinearSystemSolver::getExternalAlgorithmClassName(
      const BehaviourDescription& bd, const Hypothesis h) const {
    return NonLinearSystemSolverBase::buildExternalAlgorithmClassName(
        bd, h, this->class_name);
  }  // end of getExternalAlgorithmClassName

  std::vector<std::string>
  UserDefinedNonLinearSystemSolver::getSpecificHeaders() const {
    return {this->header_file};
  }  // end of getSpecificHeaders

  bool UserDefinedNonLinearSystemSolver::usesJacobian() const {
    return this->use_jacobian;
  }  // end of usesJacobian

  bool UserDefinedNonLinearSystemSolver::usesJacobianInvert() const {
    return this->use_jacobian_invert;
  }  // end of usesJacobianInvert

  bool UserDefinedNonLinearSystemSolver::requiresNumericalJacobian() const {
    return this->requires_numerical_jacobian;
  }  // end of requiresNumericalJacobian

  bool UserDefinedNonLinearSystemSolver::allowsJacobianInitialisation() const {
    return this->allows_jacobian_initialisation;
  }  // end of allowsJacobianInitialisation

  bool UserDefinedNonLinearSystemSolver::allowsJacobianInvertInitialisation()
      const {
    return this->allows_jacobian_invert_initialisation;
  }  // end of allowsJacobianInvertInitialisation

  bool UserDefinedNonLinearSystemSolver::
      requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const {
    return this
        ->requires_jacobian_reinitialisation_to_identity_at_each_iterations;
  }  // end of requiresJacobianToBeReinitialisedToIdentityAtEachIterations

  std::pair<bool, UserDefinedNonLinearSystemSolver::tokens_iterator>
  UserDefinedNonLinearSystemSolver::treatSpecificKeywords(
      BehaviourDescription&,
      const std::string&,
      const tokens_iterator c,
      const tokens_iterator) {
    return {false, c};
  }  // end of treatSpecificKeywords

  void UserDefinedNonLinearSystemSolver::completeVariableDeclaration(
      BehaviourDescription&) const {}  // end of completeVariableDeclaration

  void UserDefinedNonLinearSystemSolver::writeSpecificInitializeMethodPart(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeSpecificInitializeMethodPart

  void UserDefinedNonLinearSystemSolver::writeSpecificMembers(
      std::ostream&, const BehaviourDescription&, const Hypothesis) const {
  }  // end of writeSpecificMembers

  UserDefinedNonLinearSystemSolver::~UserDefinedNonLinearSystemSolver() =
      default;

}  // end of namespace mfront
