/*!
 * \file   mfront/src/StressPotentialBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   27/02/2022
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotentialBase.hxx"

namespace mfront::bbrick {

  void StressPotentialBase::initialize(BehaviourDescription& bd,
                                       AbstractBehaviourDSL& dsl,
                                       const DataMap& d) {
    StressPotentialBase::handleThermalExpansionOptions(bd, dsl, d);
    StressPotentialBase::
        declareRelativeValueForTheEquivalentStressLowerBoundDefinitionParameter(
            bd, d);
  }  // end of initialize

  std::vector<OptionDescription> StressPotentialBase::getGeneralOptions() {
    return {{"relative_value_for_the_equivalent_stress_lower_bound",
             "Relative value used to define a lower bound "
             "for the equivalent stress. For isotropic parameters, "
             "this lower bound will be equal to this value multiplied "
             "by the Young modulus. For orthotropic materials, this lower "
             "bound will be this value multiplied by the first component "
             "of the stiffness tensor.",
             OptionDescription::REAL}};
  }  // end of getGeneralOptions

  std::vector<OptionDescription>
  StressPotentialBase::getThermalExpansionReferenceTemperatureOption() {
    return {{"thermal_expansion_reference_temperature",
             "reference temperature for the thermal expansion",
             OptionDescription::REAL}};
  }  // end of getThermalExpansionReferenceTemperatureOption

  std::vector<OptionDescription>
  StressPotentialBase::getIsotropicThermalExpansionOptions() {
    auto opts = std::vector<OptionDescription>{};
    opts.emplace_back(
        "thermal_expansion", tfel::glossary::Glossary::ThermalExpansion,
        OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
        std::vector<std::string>{"thermal_expansion1", "thermal_expansion2",
                                 "thermal_expansion3"});
    return opts;
  } // end of getIsotropicThermalExpansionOptions

  std::vector<OptionDescription>
  StressPotentialBase::getOrthotropicThermalExpansionOptions() {
    auto opts = std::vector<OptionDescription>{};
    opts.emplace_back(
        "thermal_expansion1", tfel::glossary::Glossary::ThermalExpansion1,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"thermal_expansion2", "thermal_expansion3"},
        std::vector<std::string>{"thermal_expansion"});
    opts.emplace_back(
        "thermal_expansion2", tfel::glossary::Glossary::ThermalExpansion2,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"thermal_expansion1", "thermal_expansion3"},
        std::vector<std::string>{"thermal_expansion"});
    opts.emplace_back(
        "thermal_expansion3", tfel::glossary::Glossary::ThermalExpansion3,
        OptionDescription::MATERIALPROPERTY,
        std::vector<std::string>{"thermal_expansion1", "thermal_expansion2"},
        std::vector<std::string>{"thermal_expansion"});
    return opts;
  } // end of getOrthotropicThermalExpansionOptions

  std::string
  StressPotentialBase::findIfParameterOrMaterialPropertyIsUniformelyDefined(
      const BehaviourDescription& bd,
      const std::string& g,
      const std::string& t) {
    const auto r = [&bd, &g]() -> std::pair<VariableDescription, bool> {
      for (const auto h : bd.getDistinctModellingHypotheses()) {
        const auto& d = bd.getBehaviourData(h);
        if (d.isGlossaryNameUsed(g)) {
          const auto& v = d.getVariableDescriptionByExternalName(g);
          if (d.isMaterialPropertyName(v.name)) {
            return {v, true};
          }
          if (!d.isParameterName(v.name)) {
            tfel::raise(
                "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
                "Variable '" +
                displayName(v) + "' with the glossary name '" + g +
                "' is neither a material property nor a parameter.");
          }
          return {v, false};
        }
      }
      return {VariableDescription{}, false};
    }();
    if (r.first.name.empty()) {
      return "";
    }
    // uniformity check
    for (const auto h : bd.getDistinctModellingHypotheses()) {
      const auto& d = bd.getBehaviourData(h);
      if (r.second) {
        if (!d.isMaterialPropertyName(r.first.name)) {
          tfel::raise(
              "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
              "Variable '" +
              displayName(r.first) +
              "' is not defined as a material property for all " +
              "modelling hypothesis.");
        }
      } else {
        if (!d.isParameterName(r.first.name)) {
          tfel::raise(
              "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
              "Variable '" +
              displayName(r.first) +
              "' is not defined as a parameter for all " +
              "modelling hypothesis.");
        }
      }
      const auto& v = d.getVariableDescriptionByExternalName(g);
      if ((!v.hasGlossaryName()) || (v.getExternalName() != g)) {
        tfel::raise(
            "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
            "Variable '" +
            displayName(v) + "' is not associated with the glossary name '" +
            g + "' for all modelling hypotheses.");
      }
      if (v.type != t) {
        if (SupportedTypes::getTypeFlag(v.type) !=
            SupportedTypes::getTypeFlag(t)) {
          tfel::raise(
              "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
              "Variable '" +
              displayName(v) + "' is not of type '" + t +
              "' as expected (is a '" + v.type + "').");
        } else {
          auto& log = getLogStream();
          log << "findIfParameterOrMaterialPropertyIsUniformelyDefined: "
              << "inconsistent type for variable '" << displayName(v) << "' ('"
              << v.type << "' vs '" << t << "')\n";
        }
      }
    }
    return r.first.name;
  }  // end of findIfParameterOrMaterialPropertyIsUniformelyDefined

  void StressPotentialBase::addInitialGeometryReferenceTemperatureParameter(
      BehaviourDescription& bd, const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
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
  }  // end of addInitialGeometryReferenceTemperatureParameter

  void StressPotentialBase::addThermalExpansionReferenceTemperatureParameter(
      BehaviourDescription& bd, const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto n = "thermal_expansion_reference_temperature";
    const auto v = [&d, &n] {
      if (d.count(n) != 0) {
        const auto Tref = d.at(n);
        if (Tref.is<int>()) {
          return static_cast<double>(Tref.get<int>());
        }
        return Tref.get<double>();
      } else {
        return 293.15;
      }
    }();
    VariableDescription Tref("temperature", n, 1u, 0u);
    Tref.description =
        "reference value for the the thermal expansion coefficient";
    bd.addParameter(uh, Tref, BehaviourData::ALREADYREGISTRED);
    bd.setParameterDefaultValue(uh, n, v);
    bd.setEntryName(uh, n, "ThermalExpansionReferenceTemperature");
  }  // end of addThermalExpansionReferenceTemperatureParameter

  BehaviourDescription::MaterialProperty
  StressPotentialBase::getMaterialProperty(AbstractBehaviourDSL& dsl,
                                           const DataMap& d,
                                           const std::string& n) {
    if (d.count(n) == 0) {
      tfel::raise(
          "StressPotentialBase::handleThermalExpansionOptions: no option '" +
          std::string(n) + "' defined");
    }
    return getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
  }  // end of getMaterialProperty

  void StressPotentialBase::handleThermalExpansionOptions(
      BehaviourDescription& bd, AbstractBehaviourDSL& dsl, const DataMap& d) {
    auto check_not = [&d](const char* const n) {
      if (d.count(n) != 0) {
        tfel::raise(
            "StressPotentialBase::handleThermalExpansionOptions: option '" +
            std::string(n) + "' shall not be defined");
      }
    };
    if (d.count("thermal_expansion") != 0) {
      check_not("thermal_expansion1");
      check_not("thermal_expansion2");
      check_not("thermal_expansion3");
      addInitialGeometryReferenceTemperatureParameter(bd, d);
      addThermalExpansionReferenceTemperatureParameter(bd, d);
      bd.setThermalExpansionCoefficient(
          getMaterialProperty(dsl, d, "thermal_expansion"));
    } else if ((d.count("thermal_expansion1") != 0) ||
               (d.count("thermal_expansion2") != 0) ||
               (d.count("thermal_expansion3") != 0)) {
      addInitialGeometryReferenceTemperatureParameter(bd, d);
      addThermalExpansionReferenceTemperatureParameter(bd, d);
      bd.setThermalExpansionCoefficients(
          getMaterialProperty(dsl, d, "thermal_expansion1"),
          getMaterialProperty(dsl, d, "thermal_expansion2"),
          getMaterialProperty(dsl, d, "thermal_expansion3"));
    }
  }  // end of handleThermalExpansionOptions

  void StressPotentialBase::
      declareRelativeValueForTheEquivalentStressLowerBoundDefinitionParameter(
          BehaviourDescription& bd, const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto seps_n = "relative_value_for_the_equivalent_stress_lower_bound";
    const auto seps_v = [&d, seps_n]() -> double {
      if (d.count(seps_n) != 0) {
        const auto seps_d = d.at(seps_n);
        if (seps_d.is<int>()) {
          return static_cast<double>(seps_d.get<int>());
        }
        return seps_d.get<double>();
      }
      return 1.e-12;
    }();
    VariableDescription seps("real", seps_n, 1u, 0u);
    seps.description =
        "Relative value used to define a lower bound "
        "for the equivalent stress. For isotropic parameters, "
        "this lower bound will be equal to this value multiplied "
        "by the Young modulus. For orthotropic materials, this lower "
        "bound will be this value multiplied by the first component "
        "of the stiffness tensor.";
    bd.addParameter(uh, seps, BehaviourData::UNREGISTRED);
    bd.setParameterDefaultValue(uh, seps_n, seps_v);
    bd.setEntryName(uh, seps_n,
                    "RelativeValueForTheEquivalentStressLowerBoundDefinition");
  }  // end of
     // declareRelativeValueForTheEquivalentStressLowerBoundDefinitionParameter

  void StressPotentialBase::declareElasticStrainIfRequired(
      BehaviourDescription& bd) {
    using namespace tfel::glossary;
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto b = bd.checkVariableExistence("eel");
    if (b.first) {
      if(!b.second){
        tfel::raise(
            "StressPotentialBase::declareElasticStrainIfRequired: "
            "'eel' is not declared for all specialisation of the behaviour");
        bd.checkVariableExistence("eel", "IntegrationVariable");
        bd.checkVariableGlossaryName("eel", Glossary::ElasticStrain);
      }
    } else {
      VariableDescription eel("StrainStensor", "εᵉˡ", "eel", 1u, 0u);
      eel.description = "elastic strain";
      bd.addStateVariable(uh, eel);
      bd.setGlossaryName(uh, "eel", Glossary::ElasticStrain);
    }
  }  // end of declaredElasticStrainIfRequired

  StressPotentialBase::~StressPotentialBase() = default;

}  // end of namespace mfront::bbrick
