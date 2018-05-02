/*!
 * \file   mfront/src/HookeStressPotentialBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 * <!-- Local IspellDict: english -->
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/LocalDataStructure.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/HookeStressPotentialBase.hxx"

namespace mfront {

  namespace bbrick {

    HookeStressPotentialBase::HookeStressPotentialBase() = default;

    void HookeStressPotentialBase::initialize(BehaviourDescription& bd,
                                          AbstractBehaviourDSL& dsl,
                                          const DataMap& d) {
      constexpr const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(b, "HookeStressPotentialBase::HookeStressPotentialBase: " + m);
      };
      auto check = [&d](const char* const n) {
        if (d.count(n) == 0) {
          tfel::raise("HookeStressPotentialBase::HookeStressPotentialBase: entry '" +
                      std::string(n) + "' is not defined");
        }
      };
      auto check_not = [&d](const char* const n) {
        if (d.count(n) != 0) {
          tfel::raise("HookeStressPotentialBase::HookeStressPotentialBase: entry '" +
                      std::string(n) + "' shall not be defined");
        }
      };
      auto get_mp = [&dsl, &bd, &d, check](const char* const n) {
        check(n);
        return getBehaviourDescriptionMaterialProperty(dsl, n, d.at(n));
      };
      auto addTi = [&bd, &d,&uh]() {
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
      auto addTref = [&bd, &d, &uh]() {
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
      };  // end of addTref
      // options
      auto update = [throw_if, &d](bool& b, const char* n) {
        if (d.count(n) != 0) {
          const auto& v = d.at(n);
          throw_if(!v.is<bool>(),
                   "invalid type for option '" + std::string(n) + "'");
          b = v.get<bool>();
        }
      };
      // checking options
      mfront::bbrick::check(d, this->getOptions());
      // reserve some specific variables
      bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS, "sebdata");
      update(this->pss, "plane_stress_support");
      update(this->gto, "generic_tangent_operator");
      update(this->gpo, "generic_tangent_operator");
      if (this->pss) {
        bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                              "etozz");
        bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                              "detozz");
        bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                              "sigzz");
        bd.registerMemberName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                              "dsigzz");
        bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                       "prediction_stress");
        bd.reserveName(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                       "prediction_strain");
      }
      // basic checks
      throw_if(
          bd.getBehaviourType() !=
              BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
          "this behaviour brick is only usable for strain based behaviours");
      throw_if(
          bd.getIntegrationScheme() != BehaviourDescription::IMPLICITSCHEME,
          "this behaviour brick is only usable in implicit schemes");
      if ((d.count("young_modulus") != 0) || (d.count("poisson_ratio") != 0)) {
        check_not("young_modulus1");
        check_not("young_modulus2");
        check_not("young_modulus3");
        check_not("poisson_ratio12");
        check_not("poisson_ratio23");
        check_not("poisson_ratio13");
        check_not("young_modulus12");
        check_not("young_modulus23");
        check_not("young_modulus13");
        std::vector<BehaviourDescription::MaterialProperty> emps = {
            get_mp("young_modulus"), get_mp("poisson_ratio")};
        bd.setElasticMaterialProperties(emps);
      } else if ((d.count("young_modulus1") != 0) ||
                 (d.count("young_modulus2") != 0) ||
                 (d.count("young_modulus3") != 0) ||
                 (d.count("poisson_ratio12") != 0) ||
                 (d.count("poisson_ratio23") != 0) ||
                 (d.count("poisson_ratio13") != 0) ||
                 (d.count("shear_modulus12") != 0) ||
                 (d.count("shear_modulus23") != 0) ||
                 (d.count("shear_modulus13") != 0)) {
        std::vector<BehaviourDescription::MaterialProperty> emps = {
            get_mp("young_modulus1"),  get_mp("young_modulus2"),
            get_mp("young_modulus3"),  get_mp("poisson_ratio12"),
            get_mp("poisson_ratio23"), get_mp("poisson_ratio13"),
            get_mp("shear_modulus12"), get_mp("shear_modulus23"),
            get_mp("shear_modulus13")};
        bd.setElasticMaterialProperties(emps);
        bd.setAttribute(BehaviourDescription::computesStiffnessTensor, true,
                        false);
        bd.setAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,
                        true, false);
      }
      if (d.count("thermal_expansion") != 0) {
        check_not("thermal_expansion1");
        check_not("thermal_expansion2");
        check_not("thermal_expansion3");
        addTi();
        addTref();
        bd.setThermalExpansionCoefficient(get_mp("thermal_expansion"));
      } else if ((d.count("thermal_expansion1") != 0) ||
                 (d.count("thermal_expansion2") != 0) ||
                 (d.count("thermal_expansion3") != 0)) {
        addTi();
        addTref();
        bd.setThermalExpansionCoefficients(get_mp("thermal_expansion1"),
                                           get_mp("thermal_expansion2"),
                                           get_mp("thermal_expansion3"));
      }
      // relative stress criterion
      const auto seps_n =
          "relative_value_for_the_equivalent_stress_lower_bound";
      const auto seps_v = [&d,seps_n] {
        if (d.count(seps_n)) {
          const auto seps_d = d.at(seps_n);
          if (seps_d.is<int>()) {
            return static_cast<double>(seps_d.get<int>());
          }
          return seps_d.get<double>();
        }
        return 1.e-12;
      }();
      VariableDescription seps("real",seps_n, 1u, 0u);
      seps.description =
          "Relative value used to define a lower bound "
          "for the equilavent stress. For isotropic parameters, "
          "this lower bound will be equal to this value multiplied "
          "by the Young modulus. For orthotropic materials, this lower "
          "bound will be this value multiplied by the first component "
          "of the stiffness tensor.";
      bd.addParameter(uh, seps, BehaviourData::UNREGISTRED);
      bd.setParameterDefaultValue(uh, seps_n, seps_v);
      bd.setEntryName(
          uh, seps_n, "RelativeValueForTheEquivalentStressLowerBoundDefinition");
    }  // end of HookeStressPotentialBase::HookeStressPotentialBase

    std::string HookeStressPotentialBase::getName() const {
      return "Hooke";
    }  // end of HookeStressPotentialBase

    std::vector<OptionDescription> HookeStressPotentialBase::getOptions() const {
      using tfel::glossary::Glossary;
      std::vector<OptionDescription> opts;
      opts.emplace_back(
          "young_modulus", Glossary::YoungModulus,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"poisson_ratio"},
          std::vector<std::string>{
              "young_modulus1", "young_modulus2", "young_modulus3",
              "poisson_ratio12", "poisson_ratio23", "poisson_ratio13",
              "shear_modulus12", "shear_modulus23", "shear_modulus13"});
      opts.emplace_back(
          "young_modulus1", Glossary::YoungModulus1,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus2", "young_modulus3",
                                   "poisson_ratio12", "poisson_ratio23",
                                   "poisson_ratio13", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "young_modulus2", Glossary::YoungModulus2,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus3",
                                   "poisson_ratio12", "poisson_ratio23",
                                   "poisson_ratio13", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "young_modulus3", Glossary::YoungModulus3,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "poisson_ratio12", "poisson_ratio23",
                                   "poisson_ratio13", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "poisson_ratio", Glossary::PoissonRatio,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus"},
          std::vector<std::string>{
              "young_modulus1", "young_modulus2", "young_modulus3",
              "poisson_ratio12", "poisson_ratio23", "poisson_ratio13",
              "shear_modulus12", "shear_modulus23", "shear_modulus13"});
      opts.emplace_back(
          "poisson_ratio12", Glossary::PoissonRatio12,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio23",
                                   "poisson_ratio13", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "poisson_ratio23", Glossary::PoissonRatio23,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio12",
                                   "poisson_ratio13", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "poisson_ratio13", Glossary::PoissonRatio13,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio12",
                                   "poisson_ratio23", "shear_modulus12",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "shear_modulus12", Glossary::ShearModulus12,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio12",
                                   "poisson_ratio23", "poisson_ratio13",
                                   "shear_modulus23", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "shear_modulus23", Glossary::ShearModulus23,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio12",
                                   "poisson_ratio23", "poisson_ratio13",
                                   "shear_modulus12", "shear_modulus13"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "shear_modulus13", Glossary::ShearModulus13,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"young_modulus1", "young_modulus2",
                                   "young_modulus3", "poisson_ratio12",
                                   "poisson_ratio23", "poisson_ratio13",
                                   "shear_modulus12", "shear_modulus23"},
          std::vector<std::string>{"young_modulus", "poisson_ratio"});
      opts.emplace_back(
          "thermal_expansion", Glossary::ThermalExpansion,
          OptionDescription::MATERIALPROPERTY, std::vector<std::string>{},
          std::vector<std::string>{"thermal_expansion1", "thermal_expansion2",
                                   "thermal_expansion3"});
      opts.emplace_back(
          "thermal_expansion1", Glossary::ThermalExpansion1,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"thermal_expansion2", "thermal_expansion3"},
          std::vector<std::string>{"thermal_expansion"});
      opts.emplace_back(
          "thermal_expansion2", Glossary::ThermalExpansion2,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"thermal_expansion1", "thermal_expansion3"},
          std::vector<std::string>{"thermal_expansion"});
      opts.emplace_back( 
          "thermal_expansion3", Glossary::ThermalExpansion3,
          OptionDescription::MATERIALPROPERTY,
          std::vector<std::string>{"thermal_expansion1", "thermal_expansion2"},
          std::vector<std::string>{"thermal_expansion"});
      opts.emplace_back("thermal_expansion_reference_temperature",
                        "reference temperature for the thermal expansion",
                        OptionDescription::REAL);
      opts.emplace_back(
          "relative_value_for_the_equivalent_stress_lower_bound",
          "Relative value used to define a lower bound "
          "for the equilavent stress. For isotropic parameters, "
          "this lower bound will be equal to this value multiplied "
          "by the Young modulus. For orthotropic materials, this lower "
          "bound will be this value multiplied by the first component "
          "of the stiffness tensor.",
          OptionDescription::REAL);
      opts.emplace_back("plane_stress_support", "", OptionDescription::BOOLEAN);
      opts.emplace_back("generic_tangent_operator", "",
                        OptionDescription::BOOLEAN);
      opts.emplace_back("generic_prediction_operator", "",
                        OptionDescription::BOOLEAN);
      return opts;
    }  // end of HookeStressPotentialBase::getOptions()

    void HookeStressPotentialBase::completeVariableDeclaration(
        BehaviourDescription& bd, const AbstractBehaviourDSL&) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b, "HookeStressPotentialBase::completeVariableDeclaration: " + m);
      };
      using tfel::glossary::Glossary;
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "HookeStressPotentialBase::completeVariableDeclaration: begin\n";
      }
      LocalDataStructure d;
      d.name = "sebdata";
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      // deformation strain
      const auto b = bd.checkVariableExistence("eel");
      if (b.first) {
        throw_if(
            !b.second,
            "'eel' is not declared for all specialisation of the behaviour");
        bd.checkVariableExistence("eel", "IntegrationVariable");
        bd.checkVariableGlossaryName("eel", Glossary::ElasticStrain);
      } else {
        VariableDescription eel("StrainStensor", "eel", 1u, 0u);
        eel.description = "elastic strain";
        bd.addStateVariable(uh, eel);
        bd.setGlossaryName(uh, "eel", Glossary::ElasticStrain);
      }
      // treating material properties and stress computation
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        this->declareComputeStressWhenStiffnessTensorIsDefined(bd);
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          this->treatIsotropicBehaviour(bd, d);
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          this->treatOrthotropicBehaviour(bd);
        } else {
          throw_if(true, "unsupported elastic symmetry type");
        }
      }
      if (this->pss) {
        const auto agps =
            ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
        const auto ps = ModellingHypothesis::PLANESTRESS;
        if (bmh.count(agps) != 0) {
          VariableDescription etozz("strain", "etozz", 1u, 0u);
          etozz.description = "axial strain";
          bd.addStateVariable(agps, etozz, BehaviourData::ALREADYREGISTRED);
          bd.setGlossaryName(agps, "etozz",
                             tfel::glossary::Glossary::AxialStrain);
          VariableDescription sigzz("strain", "sigzz", 1u, 0u);
          etozz.description = "axial stress";
          bd.addExternalStateVariable(agps, sigzz,
                                      BehaviourData::ALREADYREGISTRED);
          bd.setGlossaryName(agps, "sigzz",
                             tfel::glossary::Glossary::AxialStress);
          d.addVariable(agps, {"stress", "szz"});
        }
        if (bmh.count(ps) != 0) {
          VariableDescription etozz("strain", "etozz", 1u, 0u);
          etozz.description = "axial strain";
          bd.addStateVariable(ps, etozz, BehaviourData::ALREADYREGISTRED);
          bd.setGlossaryName(ps, "etozz",
                             tfel::glossary::Glossary::AxialStrain);
          d.addVariable(ps, {"stress", "szz"});
        }
      }
      if (this->gpo) {
        const bool agps =
            bmh.count(
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
        const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
        if (agps || ps) {
          if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
            if (!bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              bd.setAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor, true,
                  false);
            }
            throw_if(
                !bd.getAttribute<bool>(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor),
                "genertic tangent operator support for "
                "plane stress hypotheses requires the use of an unaltered "
                "stiffness tensor");
          }
        }
      }
      bd.addLocalDataStructure(d, BehaviourData::ALREADYREGISTRED);
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "HookeStressPotentialBase::completeVariableDeclaration: end\n";
      }
    }

    void HookeStressPotentialBase::endTreatment(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotentialBase::endTreatment: begin\n";
      }
      // implicit equation associated with the elastic strain
      CodeBlock integrator;
      integrator.code = "feel -= this->deto;\n";
      bd.setCode(uh, BehaviourData::Integrator, integrator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // modelling hypotheses supported by the behaviour
      // plane stress support
      if (this->pss) {
        const bool agps =
            bmh.count(
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
        const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
        if (ps) {
          this->addPlaneStressSupport(bd, dsl);
        }
        if (agps) {
          this->addAxisymmetricalGeneralisedPlaneStressSupport(bd, dsl);
        }
      }
      // declaring the computeElasticPrediction member
      this->declareComputeElasticPredictionMethod(bd);
      // prediction operator
      if (this->gpo) {
        this->addGenericPredictionOperatorSupport(bd);
      }
      // tangent operator
      if (this->gto) {
        this->addGenericTangentOperatorSupport(bd, dsl);
      }
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotentialBase::endTreatment: end\n";
      }
    }  // end of HookeStressPotentialBase::endTreatment

    void HookeStressPotentialBase::declareComputeElasticPredictionMethod(
        BehaviourDescription& bd) const {
      for (const auto h : bd.getDistinctModellingHypotheses()) {
        std::string m =
            "//! \\brief return an elastic prediction of the stresses\n"
            "StressStensor computeElasticPrediction() const{\n";
        if (h == ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) {
          if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
              (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
            if ((bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
                (bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
              m += "throw(std::runtime_error(\"computeElasticPrediction: "
                   "unsupported case\"));\n";
            } else {
              if (bd.hasAttribute(
                      BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
                  m += "StressStensor prediction_stress;\n";
                  m += "StrainStensor prediction_strain = "
                       "this->eel+(this->theta)*this->deto;\n";
                  m += "prediction_stress(1) = "
                       "this->sigzz+(this->theta)*(this->dsigzz);\n";
                if(this->handleIsotropicDamage()){
                  // damage effect
                  const std::string de = "(1-this->d)";
                  m += "prediction_stress(0) = " + de + "*(" +
                       "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(0,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz)));\n";
                  m += "prediction_stress(2) = " + de + "*(" +
                       "(this->D(2,0)-((this->D(2,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(2,2)-((this->D(2,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(2,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz)));\n";
                } else {
                  m += "prediction_stress(0) = "
                       "(this->D(0,0)-((this->D(0,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,2)-((this->D(0,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(0,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz));"
                       "\n";
                  m += "prediction_stress(2) = "
                       "(this->D(2,0)-((this->D(2,1))*(this->D(1,0)))/"
                       "(this->D(1,1)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(2,2)-((this->D(2,1))*(this->D(1,2)))/"
                       "(this->D(1,1)))*prediction_strain(2)+\n";
                  m += "                       "
                       "(this->D(2,1))/"
                       "(this->D(1,1))*(this->sigzz+(this->theta)*(this->"
                       "dsigzz));"
                       "\n";
                }
                m += "return prediction_stress;\n";
              } else {
                m += "throw(std::runtime_error(\"computeElasticPrediction: "
                     "unsupported case\"));\n";
              }
            }
          } else {
            if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
              auto b = bd.getAttribute(
                  "HookeStressPotentialBase::UseLocalLameCoeficients", false);
              const std::string lambda =
                  b ? "this->sebdata.lambda" : "this->lambda";
              const std::string mu = b ? "this->sebdata.mu" : "this->mu";
              m += "StressStensor prediction_stress;\n";
              m += "StrainStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
                if(this->handleIsotropicDamage()){
                  // damage effect
                  const std::string de = "(1-this->d)";
                  m += "prediction_stress(0) = " + de + "*(2*(" + mu + ")*((" +
                       lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(2))+"
                       "prediction_"
                       "strain(0))+\n";
                  m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(this->sigzz+(this->theta)*(this->dsigzz)));\n";
                  m += "prediction_stress(2) = " + de + "*(2*(" + mu + ")*((" +
                       lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(2))+"
                       "prediction_"
                       "strain(2))+\n";
                  m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(this->sigzz+(this->theta)*(this->dsigzz)));\n";
                } else {
                  m += "prediction_stress(0) = 2*(" + mu + ")*((" + lambda +
                       ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(2))+"
                       "prediction_"
                       "strain(0))+\n";
                  m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(this->sigzz+(this->theta)*(this->dsigzz));\n";
                  m += "prediction_stress(2) = 2*(" + mu + ")*((" + lambda +
                       ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(2))+"
                       "prediction_"
                       "strain(2))+\n";
                  m += "(" + lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(this->sigzz+(this->theta)*(this->dsigzz));\n";
                }
                m += "prediction_stress(1) = "
                     "this->sigzz+(this->theta)*(this->dsigzz);\n";
                m += "return prediction_stress;\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotentialBase::"
                             "declareComputeElasticPredictionMethod: "
                             "the stiffness tensor must be defined for "
                             "orthotropic behaviours");
            }
          }
        } else if (h == ModellingHypothesis::PLANESTRESS) {
          if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
              (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
            if ((bd.getElasticSymmetryType() != mfront::ISOTROPIC) &&
                (bd.getElasticSymmetryType() != mfront::ORTHOTROPIC)) {
              m += "throw(std::runtime_error(\"computeElasticPrediction: "
                   "unsupported material symmetry (neither isotropic "
                   "or orthotropic)\"));\n";
            } else {
              if (bd.hasAttribute(
                      BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
                m += "StressStensor prediction_stress;\n";
                m += "StrainStensor prediction_strain = "
                     "this->eel+(this->theta)*this->deto;\n";
                if(this->handleIsotropicDamage()){
                  // damage effect
                  const std::string de = "(1-this->d)";
                  m += "prediction_stress(0) = " + de +
                       "*("
                       "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                       "(this->D(2,2)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                       "(this->D(2,2)))*prediction_strain(1));\n";
                  m += "prediction_stress(1) = " + de +
                       "*("
                       "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                       "(this->D(2,2)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                       "(this->D(2,2)))*prediction_strain(1));\n";
                  m += "prediction_stress(3) = " + de +
                       "*(this->D(3,3))*prediction_strain(3);\n";
                } else {
                  m += "prediction_stress(0) = "
                       "(this->D(0,0)-((this->D(0,2))*(this->D(2,0)))/"
                       "(this->D(2,2)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(0,1)-((this->D(0,2))*(this->D(2,1)))/"
                       "(this->D(2,2)))*prediction_strain(1);\n";
                  m += "prediction_stress(1) = "
                       "(this->D(1,0)-((this->D(1,2))*(this->D(2,0)))/"
                       "(this->D(2,2)))*prediction_strain(0)+\n";
                  m += "                       "
                       "(this->D(1,1)-((this->D(1,2))*(this->D(2,1)))/"
                       "(this->D(2,2)))*prediction_strain(1);\n";
                  m += "prediction_stress(3) = "  //
                       "(this->D(3,3))*prediction_strain(3);\n";
                }
                m += "prediction_stress(2) = stress(0);\n";
                m += "return prediction_stress;\n";
              } else {
                m += "return (this->D)*(this->eel+(this->theta)*this->deto);";
              }
            }
          } else {
            if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
              auto b = bd.getAttribute(
                  "HookeStressPotentialBase::UseLocalLameCoeficients", false);
              const std::string lambda =
                  b ? "this->sebdata.lambda" : "this->lambda";
              const std::string mu = b ? "this->sebdata.mu" : "this->mu";
              m += "StressStensor prediction_stress;\n";
              m += "StressStensor prediction_strain = "
                   "this->eel+(this->theta)*this->deto;\n";
              if (this->handleIsotropicDamage()) {
                  // damage effect
                  const std::string de = "(1-this->d)";
                  m += "prediction_stress(0) = " + de + "*2*(" + mu + ")*((" +
                       lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(1))+"
                       "prediction_"
                       "strain(0));\n";
                  m += "prediction_stress(1) = " + de + "*2*(" + mu + ")*((" +
                       lambda + ")/(" + lambda + "+2*(" + mu +
                       "))*(prediction_strain(0)+prediction_strain(1))+"
                       "prediction_"
                       "strain(1));\n";
                  m += "prediction_stress(3) = " + de + "*2*(" + mu +
                       ")*prediction_strain(3);\n";
              } else {
                m +=
                    "prediction_stress(0) = 2*(" + mu + ")*((" + lambda +
                    ")/(" + lambda + "+2*(" + mu +
                    "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                    "strain(0));\n";
                m +=
                    "prediction_stress(1) = 2*(" + mu + ")*((" + lambda +
                    ")/(" + lambda + "+2*(" + mu +
                    "))*(prediction_strain(0)+prediction_strain(1))+prediction_"
                    "strain(1));\n";
                m += "prediction_stress(3) = 2*(" + mu +
                     ")*prediction_strain(3);\n";
              }
              m += "prediction_stress(2) = stress(0);\n";
              m += "return prediction_stress;\n";
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotentialBase::"
                             "declareComputeElasticPredictionMethod: "
                             "the stiffness tensor must be defined for "
                             "orthotropic behaviours");
            }
          }
        } else {
          if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                               false)) ||
              (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                               false))) {
            if (this->handleIsotropicDamage()) {
              // damage effect
              const std::string de = "(1-this->d)";
              m += "return " + de +
                   "*(this->D)*(this->eel+(this->theta)*this->deto);\n";
            } else {
              m += "return (this->D)*(this->eel+(this->theta)*this->deto);\n";
            }
          } else {
            if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
              auto b = bd.getAttribute(
                  "HookeStressPotentialBase::UseLocalLameCoeficients", false);
              const std::string lambda =
                  b ? "this->sebdata.lambda" : "this->lambda";
              const std::string mu = b ? "this->sebdata.mu" : "this->mu";
              if (this->handleIsotropicDamage()) {
                // damage effect
                const std::string de = "(1-this->d)";
                m += "return " + de + "*(" + lambda +
                     "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id("
                     ")+"
                     "2*(" +
                     mu + ")*(this->eel+(this->theta)*(this->deto)));\n";
              } else {
                m += "return " + lambda +
                     "*trace(this->eel+(this->theta)*(this->deto))*Stensor::Id("
                     ")+"
                     "2*(" +
                     mu + ")*(this->eel+(this->theta)*(this->deto));\n";
              }
            } else {
              tfel::raise_if(!bd.getAttribute<bool>(
                                 BehaviourDescription::requiresStiffnessTensor),
                             "HookeStressPotentialBase::"
                             "declareComputeElasticPredictionMethod: "
                             "the stiffness tensor must be defined for "
                             "orthotropic behaviours");
            }
          }
        }
        m += "}\n";
        bd.appendToMembers(h, m, false);
      }
    }  // end of HookeStressPotentialBase::declareComputeElasticPredictionMethod

    void HookeStressPotentialBase::declareComputeStressWhenStiffnessTensorIsDefined(
        BehaviourDescription& bd) const {
      const auto h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      CodeBlock smts, sets;
      const std::string D_tdt =
          bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                false)
              ? "this->D_tdt"
              : "this->D";
      if (this->handleIsotropicDamage()) {
        // damage effect
        const std::string d_mts = "(1-this->d+(this->theta)*(this->dd))";
        const std::string d_ets = "(1-this->d)";
        smts.code = "this->sig = " + d_mts +
                    "*(this->D)*(this->eel+(this->theta)*(this->deel))"
                    ";\n";
        sets.code = "this->sig = " + d_ets + "*(" + D_tdt + ")*(this->eel);\n";
      } else {
        smts.code =
            "this->sig = (this->D)*(this->eel+(this->theta)*(this->deel))"
            ";\n";
        sets.code = "this->sig = (" + D_tdt + ")*(this->eel);\n";
      }
      bd.setCode(h, BehaviourData::ComputeStress, smts, BehaviourData::CREATE,
                 BehaviourData::AT_BEGINNING, false);
      bd.setCode(h, BehaviourData::ComputeFinalStress, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    }  // end of
       // HookeStressPotentialBase::declareComputeStressWhenStiffnessTensorIsDefined

    void HookeStressPotentialBase::treatIsotropicBehaviour(
        BehaviourDescription& bd, LocalDataStructure& d) const {
      using tfel::glossary::Glossary;
      const auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream()
            << "HookeStressPotentialBase::treatIsotropicBehaviour: begin\n";
      }
      CodeBlock smts;
      CodeBlock sets;
      if (bd.areElasticMaterialPropertiesDefined()) {
        if (this->handleIsotropicDamage()) {
          // damage effect
          const std::string d_mts = "(1-this->d+(this->theta)*(this->dd))";
          const std::string d_ets = "(1-this->d)";
          smts.code =
              "this->sig = " + d_mts +
              "*(this->lambda*trace(this->eel+(this->theta)*(this->deel))"  //
              "*Stensor::Id()+"                                             //
              "2*(this->mu)*(this->eel+(this->theta)*(this->deel)));\n";
          sets.code = "this->sig = " + d_ets +
                      "*(this->lambda_tdt*trace(this->eel)*Stensor::Id()+"  //
                      "2*(this->mu_tdt)*this->eel);\n";
        } else {
          smts.code =
              "this->sig=this->lambda*trace(this->eel+(this->theta)*(this->"
              "deel))*Stensor::Id()+"  //
              "2*(this->mu)*(this->eel+(this->theta)*(this->deel));\n";
          sets.code =
              "this->sig=this->lambda_tdt*trace(this->eel)*Stensor::Id()+"  //
              "2*(this->mu_tdt)*this->eel;\n";
        }
      } else {
        bd.setAttribute("HookeStressPotentialBase::UseLocalLameCoeficients", true,
                        false);
        addMaterialPropertyIfNotDefined(bd, "stress", "young",
                                        Glossary::YoungModulus);
        addMaterialPropertyIfNotDefined(bd, "real", "nu",
                                        Glossary::PoissonRatio);
        d.addVariable(uh, {"stress", "lambda"});
        d.addVariable(uh, {"stress", "mu"});
        // local variable initialisation
        CodeBlock init;
        init.code =
            "// initialisation Lame's coefficient\n"
            "this->sebdata.lambda = "  //
            "tfel::material::computeLambda(this->young,this->nu);\n"
            "this->sebdata.mu = "  //
            "tfel::material::computeMu(this->young,this->nu);\n";
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING,
                   true);
        // Hooke law
        if (this->handleIsotropicDamage()) {
          // damage effect
          const std::string d_mts = "(1-this->d+(this->theta)*(this->dd))";
          const std::string d_ets = "(1-this->d)";
          smts.code =
              "this->sig = " + d_mts +
              " * ("                                                        //
              "(this->sebdata.lambda)*"                                     //
              "trace(this->eel+(this->theta)*(this->deel))*Stensor::Id()+"  //
              "2*(this->sebdata.mu)*(this->eel+(this->theta)*(this->deel)));\n";
          sets.code =
              "this->sig = " + d_ets +
              " * ("                                                    //
              "(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+"  //
              "2*(this->sebdata.mu)*this->eel);\n";
        } else {
          smts.code =
              "this->sig = "                                                //
              "(this->sebdata.lambda)*"                                     //
              "trace(this->eel+(this->theta)*(this->deel))*Stensor::Id()+"  //
              "2*(this->sebdata.mu)*(this->eel+(this->theta)*(this->deel));\n";
          sets.code =
              "this->sig = "                                            //
              "(this->sebdata.lambda)*trace(this->eel)*Stensor::Id()+"  //
              "2*(this->sebdata.mu)*this->eel;\n";
        }
      }
      bd.setCode(uh, BehaviourData::ComputeStress, smts, BehaviourData::CREATE,
                 BehaviourData::AT_BEGINNING, false);
      bd.setCode(uh, BehaviourData::ComputeFinalStress, sets,
                 BehaviourData::CREATE, BehaviourData::AT_BEGINNING, false);
    }  // end of HookeStressPotentialBase::treatIsotropicBehaviour

    void HookeStressPotentialBase::treatOrthotropicBehaviour(
        BehaviourDescription& bd) const {
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotentialBase::treatOrthotropic: begin\n";
      }
      if (!bd.hasAttribute(BehaviourDescription::requiresStiffnessTensor)) {
        bd.setAttribute(BehaviourDescription::requiresStiffnessTensor, true,
                        false);
      }
      tfel::raise_if(
          !bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor),
          "HookeStressPotentialBase::treatOrthotropicBehaviour: "
          "the stiffness tensor must be defined for "
          "orthotropic behaviours");
      if (getVerboseMode() >= VERBOSE_DEBUG) {
        getLogStream() << "HookeStressPotentialBase::treatOrthotropic: end\n";
      }
    }  // end of HookeStressPotentialBase::treatOrthotropicBehaviour

    void HookeStressPotentialBase::addAxisymmetricalGeneralisedPlaneStressSupport(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      const auto agps =
          ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS;
      CodeBlock integrator;
      // The brick contains a reference to an abstract behaviour dsl.
      // We need to know if we have to define the jacobian terms. So we
      // downcast it to the ImplicitDSLBase class have access to the
      // solver. See the getSolver call below.
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                                 false)) ||
          (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                 false))) {
        const std::string D =
            bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                  false)
                ? "D_tdt"
                : "D";
        integrator.code +=
            "// the generalised plane stress equation is satisfied at the end "
            "of "
            "the time step\n"
            "this->sebdata.szz = (this->" +
            D +
            "(1,1))*(this->eel(1)+this->deel(1))+"
            "(this->" +
            D +
            "(1,0))*(this->eel(0)+this->deel(0))+"
            "(this->" +
            D +
            "(1,2))*(this->eel(2)+this->deel(2));\n"
            "fetozz   = (this->sebdata.szz-this->sigzz-this->dsigzz)/(this->" +
            D +
            "(1,1));\n"
            "// modification of the partition of strain\n"
            "feel(1) -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(1) = -1;\n"
              "dfetozz_ddetozz  = real(0);\n"
              "dfetozz_ddeel(1) = 1;\n"
              "dfetozz_ddeel(0) = (this->" +
              D + "(1,0))/(this->" + D +
              "(1,1));\n"
              "dfetozz_ddeel(2) = (this->" +
              D + "(1,2))/(this->" + D + "(1,1));\n";
        }
      } else {
        if (bd.areElasticMaterialPropertiesDefined()) {
          integrator.code +=
              "// the generalised plane stress equation is satisfied at the "
              "end "
              "of the time step\n"
              "this->sebdata.szz = "
              "(this->lambda_tdt+2*(this->mu_tdt))*(this->eel(1)+this->deel(1))"
              "+ (this->lambda_tdt)*(this->eel(0)+this->deel(0)+"
              "this->eel(2)+this->deel(2));\n"
              "fetozz   = "
              "(this->sebdata.szz-this->sigzz-this->dsigzz)/this->young_tdt;\n"
              "// modification of the partition of strain\n"
              "feel(1) -= this->detozz;\n";
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            integrator.code +=
                "// jacobian\n"
                "dfeel_ddetozz(1) = -1;\n"
                "dfetozz_ddetozz  = real(0);\n"
                "dfetozz_ddeel(1) = "
                "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
                "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
                "dfetozz_ddeel(2) = this->lambda_tdt/this->young_tdt;\n";
          }
        } else {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu";
          integrator.code +=
              "// the generalised plane stress equation is satisfied at the "
              "end "
              "of the time step\n"
              "this->sebdata.szz =   (" +
              lambda + "+2*(" + mu +
              "))*(this->eel(1)+this->deel(1))"
              "                   + (" +
              lambda +
              ")*(this->eel(0)+this->deel(0)+this->eel(2)+this->deel(2));\n"
              "fetozz   = "
              "(this->sebdata.szz-this->sigzz-this->dsigzz)/this->young;\n"
              "// modification of the partition of strain\n"
              "feel(1) -= this->detozz;\n";
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            integrator.code +=
                "// jacobian\n"
                "dfeel_ddetozz(1) = -1;\n"
                "dfetozz_ddetozz  = real(0);\n"
                "dfetozz_ddeel(1) = (" +
                lambda + "+2*(" + mu +
                "))/this->young;\n"
                "dfetozz_ddeel(0) = " +
                lambda +
                "/this->young;\n"
                "dfetozz_ddeel(2) = " +
                lambda + "/this->young;\n";
          }
        }
      }
      bd.setCode(agps, BehaviourData::Integrator, integrator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }

    void HookeStressPotentialBase::addPlaneStressSupport(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      const auto ps = ModellingHypothesis::PLANESTRESS;
      CodeBlock integrator;
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                                 false)) ||
          (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                 false))) {
        const std::string D =
            bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                  false)
                ? "D_tdt"
                : "D";
        integrator.code +=
            "// the plane stress equation is satisfied at the end of the time "
            "step\n"
            "fetozz   = this->eel(2)+this->deel(2)+"
            "           ((this->" +
            D + "(2,0))/(this->" + D +
            "(2,2)))*(this->eel(0)+this->deel(0))+"
            "           ((this->" +
            D + "(2,1))/(this->" + D +
            "(2,2)))*(this->eel(1)+this->deel(1));\n"
            "// modification of the partition of strain\n"
            "feel(2)          -= this->detozz;\n";
        if ((idsl.getSolver().usesJacobian()) &&
            (!idsl.getSolver().requiresNumericalJacobian())) {
          integrator.code +=
              "// jacobian\n"
              "dfeel_ddetozz(2)  = -1;\n"
              "dfetozz_ddetozz   = real(0);\n"
              "dfetozz_ddeel(2)  = 1;\n"
              "dfetozz_ddeel(0)  = (this->" +
              D + "(1,0))/(this->" + D +
              "(1,1));\n"
              "dfetozz_ddeel(1)  = (this->" +
              D + "(2,0))/(this->" + D + "(1,1));\n";
        }
      } else {
        if (bd.areElasticMaterialPropertiesDefined()) {
          integrator.code +=
              "// the plane stress equation is satisfied at the end of the "
              "time "
              "step\n"
              "this->sebdata.szz = "
              "(this->lambda_tdt+2*(this->mu_tdt))*(this->eel(2)+this->deel(2))"
              "+"
              "                   "
              "(this->lambda_tdt)*(this->eel(0)+this->deel(0)+this->eel(1)+"
              "this->"
              "deel(1));\n"
              "fetozz   = this->sebdata.szz/(this->young_tdt);\n"
              "// modification of the partition of strain\n"
              "feel(2) -= detozz;\n";
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            integrator.code +=
                "// jacobian\n"
                "dfeel_ddetozz(2) = -1;\n"
                "dfetozz_ddetozz  = real(0);\n"
                "dfetozz_ddeel(2) = "
                "(this->lambda_tdt+2*(this->mu_tdt))/this->young_tdt;\n"
                "dfetozz_ddeel(0) = this->lambda_tdt/this->young_tdt;\n"
                "dfetozz_ddeel(1) = this->lambda_tdt/this->young_tdt;\n";
          }
        } else {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu";
          integrator.code +=
              "// the plane stress equation is satisfied at the end of the "
              "time "
              "step\n"
              "this->sebdata.szz = (" +
              lambda + "+2*(" + mu +
              "))*(this->eel(2)+this->deel(2))+"
              "                   (" +
              lambda +
              ")*(this->eel(0)+this->deel(0)+this->eel(1)+this->deel(1));\n"
              "fetozz   = this->sebdata.szz/(this->young);\n"
              "// modification of the partition of strain\n"
              "feel(2) -= detozz;\n";
          if ((idsl.getSolver().usesJacobian()) &&
              (!idsl.getSolver().requiresNumericalJacobian())) {
            integrator.code +=
                "// jacobian\n"
                "dfeel_ddetozz(2) = -1;\n"
                "dfetozz_ddetozz  = real(0);\n"
                "dfetozz_ddeel(2) = (" +
                lambda + "+2*(" + mu +
                "))/this->young;\n"
                "dfetozz_ddeel(0) = " +
                lambda +
                "/this->young;\n"
                "dfetozz_ddeel(1) = " +
                lambda + "/this->young;\n";
          }
        }
      }
      bd.setCode(ps, BehaviourData::Integrator, integrator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }

    void HookeStressPotentialBase::addGenericTangentOperatorSupport(
        BehaviourDescription& bd, const AbstractBehaviourDSL& dsl) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b, "HookeStressPotentialBase::addGenericTangentOperatorSupport: " + m);
      };
      const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
      bd.checkVariablePosition("eel", "IntegrationVariable", 0u);
      CodeBlock tangentOperator;
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        const std::string D =
            bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)
                ? "this->D"
                : "this->D_tdt";
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = " +
            D + ";\n";
        if (idsl.getSolver().usesJacobian()) {
          tangentOperator.code +=
              "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
              "  Stensor4 Je;\n"
              "  getPartialJacobianInvert(Je);\n"
              "  this->Dt = (" +
              D + ")*Je;\n";
        }
        tangentOperator.code +=
            "} else {\n"
            "  return false;\n"
            "}";
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda_tdt";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
          tangentOperator.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu + ");\n";
          if (idsl.getSolver().usesJacobian()) {
            tangentOperator.code +=
                "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
                "  StiffnessTensor Hooke;\n"
                "  Stensor4 Je;\n"
                "  computeElasticStiffness<N,Type>::exe(Hooke," +
                lambda + "," + mu +
                ");\n"
                "  getPartialJacobianInvert(Je);\n"
                "  Dt = Hooke*Je;\n";
          }
          tangentOperator.code +=
              "} else {\n"
              "  return false;\n"
              "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          tangentOperator.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  this->Dt = this->D_tdt;\n";
          if (idsl.getSolver().usesJacobian()) {
            tangentOperator.code +=
                "} else if (smt==CONSISTENTTANGENTOPERATOR){\n"
                "  Stensor4 Je;\n"
                "  getPartialJacobianInvert(Je);\n"
                "  this->Dt = (this->D_tdt)*Je;\n";
          }
          tangentOperator.code +=
              "} else {\n"
              "  return false;\n"
              "}";
        } else {
          throw_if(true, "unsupported elastic symmetry type");
        }
      }
      bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                      BehaviourData::hasConsistentTangentOperator, true, true);
      bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                 BehaviourData::ComputeTangentOperator, tangentOperator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }

    void HookeStressPotentialBase::addGenericPredictionOperatorSupport(
        BehaviourDescription& bd) const {
      auto throw_if = [](const bool b, const std::string& m) {
        tfel::raise_if(
            b,
            "HookeStressPotentialBase::addGenericPredictionOperatorSupport: " + m);
      };
      CodeBlock tangentOperator;
      // modelling hypotheses supported by the behaviour
      const auto bmh = bd.getModellingHypotheses();
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        const bool agps =
            bmh.count(
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS) != 0;
        const bool ps = bmh.count(ModellingHypothesis::PLANESTRESS) != 0;
        if (agps || ps) {
          if (bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                              false)) {
            if (!bd.hasAttribute(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor)) {
              bd.setAttribute(
                  BehaviourDescription::requiresUnAlteredStiffnessTensor, true,
                  false);
            }
            throw_if(
                !bd.getAttribute<bool>(
                    BehaviourDescription::requiresUnAlteredStiffnessTensor),
                "genertic tangent operator support for "
                "plane stress hypotheses requires the use of an unaltered "
                "stiffness tensor");
          }
        }
        const std::string D =
            bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                            false)
                ? "this->D"
                : "this->D_tdt";
        tangentOperator.code =
            "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
            "  this->Dt = " +
            D +
            ";\n"
            "} else {\n"
            "  return FAILURE;\n"
            "}";
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda_tdt";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu_tdt";
          tangentOperator.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  computeAlteredElasticStiffness<hypothesis,Type>::exe(Dt," +
              lambda + "," + mu +
              ");\n"
              "} else {\n"
              "  return FAILURE;\n"
              "}";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          throw_if(!bd.getAttribute<bool>(
                       BehaviourDescription::computesStiffnessTensor, false),
                   "orthotropic behaviour shall require the stiffness tensor");
          tangentOperator.code =
              "if((smt==ELASTIC)||(smt==SECANTOPERATOR)){\n"
              "  this->Dt = this->D_tdt;\n"
              "} else {\n"
              "  return FAILURE;\n"
              "}";
        } else {
          throw_if(true, "unsupported elastic symmetry type");
        }
      }
      bd.setAttribute(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                      BehaviourData::hasPredictionOperator, true, true);
      bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                 BehaviourData::ComputePredictionOperator, tangentOperator,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }

    std::vector<HookeStressPotentialBase::Hypothesis>
    HookeStressPotentialBase::getSupportedModellingHypotheses(
        const BehaviourDescription&, const AbstractBehaviourDSL& dsl) const {
      auto dmh = dsl.getDefaultModellingHypotheses();
      std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
      if (this->pss) {
        if (dsl.isModellingHypothesisSupported(
                ModellingHypothesis::PLANESTRESS)) {
          mh.push_back(ModellingHypothesis::PLANESTRESS);
        }
        if (dsl.isModellingHypothesisSupported(
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)) {
          mh.push_back(
              ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
        }
      }
      return mh;
    }  // end of HookeStressPotentialBase::getSupportedModellingHypothesis

    std::string HookeStressPotentialBase::computeDerivatives(
        const BehaviourDescription& bd,
        const std::string& v,
        const std::string& dfv_ds) const {
      auto c = std::string{};
      c = "df" + v + "_ddeel += ";
      if ((bd.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                           false)) ||
          (bd.getAttribute(BehaviourDescription::computesStiffnessTensor,
                           false))) {
        c += "(this->theta) * (" + dfv_ds + ") * (this->D);\n";
      } else {
        if (bd.getElasticSymmetryType() == mfront::ISOTROPIC) {
          auto b = bd.getAttribute(
              "HookeStressPotentialBase::UseLocalLameCoeficients", false);
          const std::string lambda =
              b ? "this->sebdata.lambda" : "this->lambda";
          const std::string mu = b ? "this->sebdata.mu" : "this->mu";
          c += "(this->theta) * (" + dfv_ds + ") * (2 * (" + mu + ") * Stensor4::Id()+(" +
               lambda + ") * Stensor4::IxI());\n";
        } else if (bd.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
          if (!bd.getAttribute<bool>(
                  BehaviourDescription::computesStiffnessTensor, false)) {
            tfel::raise(
                "HookeStressPotentialBase::writeStressDerivatives: "
                "orthotropic behaviour shall require the stiffness tensor");
          }
          c += "(this->theta)*(" + dfv_ds + ") * (this->D);\n";
        } else {
          tfel::raise(
              "HookeStressPotentialBase::writeStressDerivatives: "
              "unsupported elastic symmetry type");
        }
      }
      return c;
    }  // end of HookeStressPotentialBase::computeDerivatives

    void HookeStressPotentialBase::writeStressDerivatives(
        BehaviourDescription&) const {
    }  // end of HookeStressPotentialBase::writeStressDerivatives

    void HookeStressPotentialBase::computeElasticPrediction(
        BehaviourDescription& bd) const {
      CodeBlock i;
      i.code = "const auto sigel = computeElasticPrediction();\n";
      bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                 BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }  // end of HookeStressPotentialBase::computeElasticPrediction

    std::string HookeStressPotentialBase::getStressNormalisationFactor(
        const BehaviourDescription& bd) const {
      if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                                 false)) ||
          (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                 false))) {
        return "this->D(0,0)";
      }
      return "this->young";
    } // end of HookeStressPotentialBase::getStressNormalisationFactor

    std::string HookeStressPotentialBase::getEquivalentStressLowerBound(
        const BehaviourDescription& bd) const {
      if ((bd.getAttribute<bool>(BehaviourDescription::requiresStiffnessTensor,
                                 false)) ||
          (bd.getAttribute<bool>(BehaviourDescription::computesStiffnessTensor,
                                 false))) {
        return "(this->relative_value_for_the_equivalent_stress_lower_bound)*"
               "(this->D(0,0))";
      }
      return "(this->relative_value_for_the_equivalent_stress_lower_bound)*"
             "(this->young)";
    }  // end of getEquivalentStressLowerBound

    HookeStressPotentialBase::~HookeStressPotentialBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
