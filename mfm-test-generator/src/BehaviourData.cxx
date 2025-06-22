/*!
 * \file   BehaviourData.cxx
 * \brief
 * \author Thomas Helfer
 * \date   07/10/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MTest/Behaviour.hxx"
#include "MFMTestGenerator/TestCaseParameters.hxx"
#include "MFMTestGenerator/BehaviourData.hxx"

namespace mfmtg {

  BehaviourData::BehaviourData(const TestCaseParameters& p,
                               const std::string& h)
      : library(get(p, "library")),
        function(get(p, "function")),
        hypothesis(h),
        // material properties are optional
        material_properties(getEvolutions(p, "material_properties")),
        // the external state variables must be defined (hence the false), since
        // the temperature must be
        external_state_variables(
            getEvolutions(p, "external_state_variables", false)) {
    // consistency checks
    const auto b = ::mtest::Behaviour::getBehaviour(
        "", this->library, this->function, ::mtest::Behaviour::Parameters{},
        tfel::material::ModellingHypothesis::fromString(this->hypothesis));
    const auto omps = b->getOptionalMaterialProperties();
    //
    auto check = [this, omps](const std::map<std::string, Evolution>& evs,
                              const std::vector<std::string>& names,
                              const char* const t) {
      for (const auto& n : names) {
        if (evs.count(n) == 0) {
          if (std::find(omps.begin(), omps.end(), n) == omps.end()) {
            const auto type = std::string{t};
            tfel::raise("BehaviourData::BehaviourData: " + type + " '" + n +
                        "' is not defined when declaring "
                        "behaviour '" +
                        this->function + "' in library '" + this->library +
                        "'");
          }
        }
      }
      for (const auto& ev : evs) {
        const auto& n = ev.first;
        if (std::find(names.begin(), names.end(), n) == names.end()) {
          const auto type = std::string{t};
          tfel::raise("BehaviourData::BehaviourData: no " + type + " named '" +
                      n + "expected.");
        }
      }
    };  // end of check
    check(this->material_properties, b->getMaterialPropertiesNames(),
          "material property");
    check(this->external_state_variables, b->getExternalStateVariablesNames(),
          "external state variable");
  }  // end of BehaviourData::BehaviourData

  BehaviourData::BehaviourData(BehaviourData&&) = default;

  BehaviourData::BehaviourData(const BehaviourData&) = default;

  BehaviourData::~BehaviourData() = default;

}  // end of namespace mfmtg
