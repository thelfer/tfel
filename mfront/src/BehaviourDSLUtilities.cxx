/*!
 * \file   mfront/src/BehaviourDSLUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date 23/04/2025
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of theses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <cmath>
#include <vector>
#include <optional>

#include "TFEL/Raise.hxx"
#include "TFEL/Math/General/IEEE754.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
#include "MFront/MaterialPropertyDescription.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/BehaviourDSLUtilities.hxx"

namespace mfront {

  BehaviourDescription::MaterialProperty
  getBehaviourDescriptionMaterialProperty(AbstractBehaviourDSL& dsl,
                                          const std::string_view n,
                                          const tfel::utilities::Data& d) {
    if (d.is<double>()) {
      BehaviourDescription::ConstantMaterialProperty cmp;
      cmp.value = d.get<double>();
      return cmp;
    } else if (d.is<int>()) {
      BehaviourDescription::ConstantMaterialProperty cmp;
      cmp.value = static_cast<double>(d.get<int>());
      return cmp;
    }
    if (!d.is<std::string>()) {
      tfel::raise(
          "getBehaviourDescriptionMaterialProperty: "
          "unsupported data type for material property '" +
          std::string(n) + "'");
    }
    const auto mp = d.get<std::string>();
    if (tfel::utilities::ends_with(mp, ".mfront")) {
      // file name
      BehaviourDescription::ExternalMFrontMaterialProperty emp;
      emp.mpd = dsl.handleMaterialPropertyDescription(mp);
      return emp;
    }
    BehaviourDescription::AnalyticMaterialProperty amp;
    amp.f = mp;
    return amp;
  }  // end of getBehaviourDescriptionMaterialProperty_impl

  void addThermalExpansionCoefficientsIfDefined(
      AbstractBehaviourDSL& dsl,
      BehaviourDescription& bd,
      const tfel::utilities::DataMap& d,
      const bool allow_reference_temperature) {
    constexpr auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto Tref1 = "thermal_expansion_reference_temperature";
    const auto Tref2 = "reference_temperature";
    auto check = [&d](const char* const n) {
      if (d.count(n) == 0) {
        tfel::raise("addThermalExpansionCoefficientsIfDefined: entry '" +
                    std::string(n) + "' is not defined");
      }
    };
    auto check_not = [&d](const char* const n) {
      if (d.count(n) != 0) {
        tfel::raise("addThermalExpansionCoefficientsIfDefined: entry '" +
                    std::string(n) + "' shall not be defined");
      }
    };
    auto get_mp = [&dsl, &d, check](const char* const n) {
      check(n);
      return getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
    };
    auto addTi = [&bd, &d]() {
      const auto n = "initial_geometry_reference_temperature";
      const auto v = [&d, &n] {
        if (d.count(n) != 0) {
          const auto Ti = d.at(n);
          if (Ti.is<int>()) {
            return static_cast<double>(Ti.get<int>());
          }
          return Ti.get<double>();
        } else {
          return 293.15;
        }
      }();
      VariableDescription Ti("temperature", n, 1u, 0u);
      Ti.description =
          "value of the temperature when the initial geometry was measured";
      bd.addParameter(uh, Ti, BehaviourData::ALREADYREGISTRED);
      bd.setParameterDefaultValue(uh, n, v);
      bd.setEntryName(uh, n, "ReferenceTemperatureForInitialGeometry");
    };  // end of addTi
    using MaterialProperties =
        std::vector<BehaviourDescription::MaterialProperty>;
    auto addTref = [&bd, &d, &Tref1, &Tref2](const MaterialProperties& mps) {
      auto ov = [&d, &Tref1, &Tref2]() -> std::optional<double> {
        if (d.count(Tref1) != 0) {
          const auto Tref = d.at(Tref1);
          if (Tref.is<int>()) {
            return static_cast<double>(Tref.get<int>());
          }
          return Tref.get<double>();
        } else if (d.count(Tref2) != 0) {
          const auto Tref = d.at(Tref2);
          if (Tref.is<int>()) {
            return static_cast<double>(Tref.get<int>());
          }
          return Tref.get<double>();
        }
        return {};
      }();
      // reference temperature declared by external material property files
      for (const auto& mp : mps) {
        using ExternalMFrontMaterialProperty =
            BehaviourDescription::ExternalMFrontMaterialProperty;
        if (!mp.is<ExternalMFrontMaterialProperty>()) {
          continue;
        }
        const auto& mpd = *(mp.get<ExternalMFrontMaterialProperty>().mpd);
        if (!mpd.staticVars.contains("ReferenceTemperature")) {
          continue;
        }
        const auto Tref = static_cast<double>(
            mpd.staticVars.get("ReferenceTemperature").value);
        if (!ov.has_value()) {
          ov = Tref;
        } else if (tfel::math::ieee754::fpclassify(*ov - Tref) != FP_ZERO) {
          tfel::raise(
              "addThermalExpansionCoefficientsIfDefined: "
              "inconsistent reference temperature");
        }
      }
      const auto v = ov.has_value() ? *ov : 293.15;
      VariableDescription Tref("temperature", Tref1, 1u, 0u);
      Tref.description =
          "reference value for the the thermal expansion coefficient";
      bd.addParameter(uh, Tref, BehaviourData::ALREADYREGISTRED);
      bd.setParameterDefaultValue(uh, Tref1, v);
      bd.setEntryName(uh, Tref1, "ThermalExpansionReferenceTemperature");
    };  // end of addTref
    if (d.count(Tref1) != 0) {
      check_not(Tref2);
    }
    if (d.count(Tref2) != 0) {
      if (!allow_reference_temperature) {
        tfel::raise("addThermalExpansionCoefficientsIfDefined: '" +
                    std::string(Tref2) +
                    "' is not allowed in this context, use '" +
                    std::string(Tref1) + "' instead");
      }
      check_not(Tref1);
    }
    if (d.count("thermal_expansion") != 0) {
      check_not("thermal_expansion1");
      check_not("thermal_expansion2");
      check_not("thermal_expansion3");
      const auto alpha = get_mp("thermal_expansion");
      addTi();
      addTref({alpha});
      bd.setThermalExpansionCoefficient(alpha);
      if (get_if(d, "save_thermal_expansion", false)) {
        VariableDescription a("strain", "mfront_thermal_expansion", 1u, 0u);
        a.setEntryName("ComputedThermalExpansion");
        a.description = "mean linear thermal expansion";
        bd.addAuxiliaryStateVariable(uh, a);
        bd.setAttribute(BehaviourDescription::saveThermalExpansion, true,
                        false);
      }
    } else if ((d.count("thermal_expansion1") != 0) ||
               (d.count("thermal_expansion2") != 0) ||
               (d.count("thermal_expansion3") != 0)) {
      if (bd.getSymmetryType() != mfront::ORTHOTROPIC) {
        tfel::raise(
            "addThermalExpansionCoefficientsIfDefined: "
            "the mechanical behaviour must be orthotropic "
            "to give more than one thermal expansion coefficient.");
      }
      const auto alpha1 = get_mp("thermal_expansion1");
      const auto alpha2 = get_mp("thermal_expansion2");
      const auto alpha3 = get_mp("thermal_expansion3");
      addTi();
      addTref({alpha1, alpha2, alpha3});
      bd.setThermalExpansionCoefficients(alpha1, alpha2, alpha3);
      if (get_if(d, "save_thermal_expansion", false)) {
        VariableDescription a("strain", "mfront_thermal_expansion", 3u, 0u);
        a.setEntryName("ComputedThermalExpansion");
        a.description = "mean linear thermal expansion";
        bd.addAuxiliaryStateVariable(uh, a);
        bd.setAttribute(BehaviourDescription::saveThermalExpansion, true,
                        false);
      }
    } else {
      check_not("initial_geometry_reference_temperature");
      check_not("thermal_expansion_reference_temperature");
      check_not("reference_temperature");
      check_not("save_thermal_expansion");
    }
  }  // end of addThermalExpansionCoefficientsIfDefined

}  // end of namespace mfront
