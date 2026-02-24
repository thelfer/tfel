/*!
 * \file   mfront/src/BehaviourDSLUtilities.cxx
 * \brief
 * \author Thomas Helfer
 * \date   23/04/2025
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/Material/ModellingHypothesis.hxx"
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
          return d.at(n).get<double>();
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
    auto addTref = [&bd, &d, &Tref1, &Tref2]() {
      const auto v = [&d, &Tref1, &Tref2] {
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
        } else {
          return 293.15;
        }
      }();
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
      addTi();
      addTref();
      bd.setThermalExpansionCoefficient(get_mp("thermal_expansion"));
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
      addTi();
      addTref();
      bd.setThermalExpansionCoefficients(get_mp("thermal_expansion1"),
                                         get_mp("thermal_expansion2"),
                                         get_mp("thermal_expansion3"));
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
      check_not("save_thermal_expansion");
    }
  }  // end of addThermalExpansionCoefficientsIfDefined

}  // end of namespace mfront
