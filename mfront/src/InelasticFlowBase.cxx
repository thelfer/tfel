/*!
 * \file   mfront/src/InelasticFlowBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Glossary/Glossary.hxx"
#include "TFEL/Glossary/GlossaryEntry.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRule.hxx"
#include "MFront/BehaviourBrick/IsotropicHardeningRuleFactory.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRule.hxx"
#include "MFront/BehaviourBrick/KinematicHardeningRuleFactory.hxx"
#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront::bbrick {

  std::vector<OptionDescription> InelasticFlowBase::getOptions() const {
    std::vector<OptionDescription> opts;
    opts.emplace_back(
        "criterion",
        "stress criterion to be used (Mises, Hill, Hosford, Barlat)",
        OptionDescription::DATASTRUCTURE);
    opts.emplace_back("flow_criterion",
                      "stress criterion used to build the plastic potential "
                      "(Mises, Hill, Hosford, Barlat)",
                      OptionDescription::DATASTRUCTURE);
    opts.emplace_back("isotropic_hardening",
                      "choice of an isotropic hardening rule",
                      OptionDescription::DATASTRUCTURES);
    opts.emplace_back("kinematic_hardening", "description of an hardening rule",
                      OptionDescription::DATASTRUCTURES);
    opts.emplace_back(
        "save_porosity_increase",
        "if appropriate, save the porosity increase induced "
        "by this inelastic flow in a dedicated auxiliary state variable",
        OptionDescription::BOOLEAN);
    opts.emplace_back(
        "equivalent_strain_external_name",
        "external name of the equivalent (plastic or viscoplastic) strain.",
        OptionDescription::STRING);
    opts.emplace_back(
        "save_stress_criterion",
        "flag stating if the stress criterion in the middle of the time step "
        "shall be saved in a dedicated auxiliary state variable",
        OptionDescription::BOOLEAN);
    opts.emplace_back("stress_criterion_external_name",
                      "external name of the auxiliary state variable in which "
                      "the stress criterion shall be saved if the "
                      "save_stress_criterion option is `true.",
                      OptionDescription::STRING);
    opts.emplace_back("save_yield_surface_radius",
                      "flag stating if the yield surface radius in the middle "
                      "of the time step "
                      "shall be saved in a dedicated auxiliary state variable",
                      OptionDescription::BOOLEAN);
    opts.emplace_back("yield_surface_radius_external_name",
                      "external name of the auxiliary state variable in which "
                      "the yield surface radius shall be saved if the "
                      "save_yield_surface_radius option is `true.",
                      OptionDescription::STRING);
    opts.emplace_back(
        "porosity_effect_on_equivalent_plastic_strain",
        "specify the effect of the porosity of the flow rule. "
        "Valid strings are 'StandardPorosityEffect' (or equivalently "
        "'standard_porosity_effect') or 'None' (or equivalently 'none' "
        "or 'false')'",
        OptionDescription::STRING);
    opts.emplace_back("porosity_evolution_algorithm",
                      "reserved for internal use", OptionDescription::STRING);
    opts.emplace_back("cosine_threshold",
                      "minimum value of the cosine of the angle between two "
                      "successive estimates of the flow direction. If the "
                      "computed angle is lower than this threshold, the "
                      "Newton step is rejected. This parameter must be in "
                      "the range [-1:1].",
                      OptionDescription::REAL);
    opts.emplace_back(
        "cosine_check_minimum_iteration_factor",
        "a factor $\\alpha$ which gives the threshold below which the "
        "check on the cosine of the angle between two successive estimates "
        "of the flow direction is performed, i.e. the test is performed if "
        "the iteration counter is greater than $\\alpha \\cdot i_{\\max{}}$ "
        "where $i_{\\max{}}$ is the maximum number of iterations. This "
        "parameter must be in the range [0:1].",
        OptionDescription::REAL);
    opts.emplace_back(
        "cosine_check_maximum_iteration_factor",
        "a factor $\\alpha$ which gives the threshold upper which the "
        "check on the cosine of the angle between two successive estimates "
        "of the flow direction is performed, i.e. the test is performed if "
        "the iteration counter is lower than $\\alpha \\cdot i_{\\max{}}$ "
        "where $i_{\\max{}}$ is the maximum number of iterations. This "
        "parameter must be in the range [0:1].",
        OptionDescription::REAL);
    return opts;
  }  // end of getOptions()

  void InelasticFlowBase::checkOptions(const DataMap& d) const {
    mfront::bbrick::check(d, this->getOptions());
  }  // end of checkOptions

  void InelasticFlowBase::initialize(BehaviourDescription& bd,
                                     AbstractBehaviourDSL& dsl,
                                     const std::string& id,
                                     const DataMap& d) {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "InelasticFlowBase::initialize\n";
    }
    auto raise = [](const std::string& m) {
      tfel::raise("InelasticFlowBase::initialize: " + m);
    };  // end of raise
    auto getDataStructure = [&raise](const std::string& n, const Data& ds) {
      if (ds.is<std::string>()) {
        tfel::utilities::DataStructure nds;
        nds.name = ds.get<std::string>();
        return nds;
      }
      if (!ds.is<tfel::utilities::DataStructure>()) {
        raise("invalid data type for entry '" + n + "'");
      }
      return ds.get<tfel::utilities::DataStructure>();
    };  // end of getDataStructure
    // checking options
    if (getVerboseMode() >= VERBOSE_DEBUG) {
      getLogStream() << "checking options\n";
    }
    this->checkOptions(d);
    //
    this->stress_criterion_external_name = "EquivalentStress" + id;
    this->yield_surface_radius_external_name = "YieldSurfaceRadius" + id;
    // parsing options
    for (const auto& e : d) {
      if (e.first == "criterion") {
        if (this->sc != nullptr) {
          raise("criterion has already been defined");
        }
        const auto ds = getDataStructure(e.first, e.second);
        auto& cf = StressCriterionFactory::getFactory();
        this->sc = cf.generate(ds.name);
        if (d.count("flow_criterion") != 0) {
          this->sc->initialize(bd, dsl, id, ds.data,
                               StressCriterion::STRESSCRITERION);
        } else {
          this->sc->initialize(bd, dsl, id, ds.data,
                               StressCriterion::STRESSANDFLOWCRITERION);
        }
      } else if (e.first == "flow_criterion") {
        if (this->fc != nullptr) {
          raise("criterion has already been defined");
        }
        const auto ds = getDataStructure(e.first, e.second);
        auto& cf = StressCriterionFactory::getFactory();
        this->fc = cf.generate(ds.name);
        this->fc->initialize(bd, dsl, id, ds.data,
                             StressCriterion::FLOWCRITERION);
      } else if (e.first == "isotropic_hardening") {
        auto add_isotropic_hardening_rule =
            [this, &bd, &dsl, &id](const tfel::utilities::DataStructure& ds) {
              if (getVerboseMode() >= VERBOSE_DEBUG) {
                getLogStream()
                    << "adding isotropic hardening rule '" + ds.name + "'"
                    << std::endl;
              }
              auto& rf = IsotropicHardeningRuleFactory::getFactory();
              const auto ihr = rf.generate(ds.name);
              ihr->initialize(bd, dsl, id, std::to_string(this->ihrs.size()),
                              ds.data);
              this->ihrs.push_back(ihr);
            };
        if (e.second.is<std::vector<Data>>()) {
          for (const auto& ird : e.second.get<std::vector<Data>>()) {
            add_isotropic_hardening_rule(getDataStructure(e.first, ird));
          }
        } else {
          const auto ds = getDataStructure(e.first, e.second);
          if (getVerboseMode() >= VERBOSE_DEBUG) {
            getLogStream() << "adding isotropic hardening rule '" + ds.name +
                                  "'"
                           << std::endl;
          }
          auto& rf = IsotropicHardeningRuleFactory::getFactory();
          const auto ihr = rf.generate(ds.name);
          ihr->initialize(bd, dsl, id, "", ds.data);
          this->ihrs.push_back(ihr);
        }
      } else if (e.first == "kinematic_hardening") {
        auto add_kinematic_hardening_rule =
            [this, &bd, &dsl, &id](const tfel::utilities::DataStructure& ds) {
              if (getVerboseMode() >= VERBOSE_DEBUG) {
                getLogStream()
                    << "adding kinematic hardening rule '" + ds.name + "'"
                    << std::endl;
              }
              auto& kf = KinematicHardeningRuleFactory::getFactory();
              const auto k = kf.generate(ds.name);
              k->initialize(bd, dsl, id, std::to_string(this->khrs.size()),
                            ds.data);
              this->khrs.push_back(k);
            };
        if (e.second.is<std::vector<Data>>()) {
          for (const auto& krd : e.second.get<std::vector<Data>>()) {
            add_kinematic_hardening_rule(getDataStructure(e.first, krd));
          }
        } else {
          add_kinematic_hardening_rule(getDataStructure(e.first, e.second));
        }
      } else if (e.first == "equivalent_strain_external_name") {
        if (!e.second.is<std::string>()) {
          raise("'equivalent_strain_external_name' is not a string");
        }
        this->equivalent_strain_external_name = e.second.get<std::string>();
      } else if (e.first == "stress_criterion_external_name") {
        if (!e.second.is<std::string>()) {
          raise("'stress_criterion_external_name' is not a string");
        }
        this->stress_criterion_external_name = e.second.get<std::string>();
      } else if (e.first == "save_stress_criterion") {
        if (!e.second.is<bool>()) {
          raise("'save_stress_criterion' is not a boolean");
        }
        this->save_stress_criterion = e.second.get<bool>();
      } else if (e.first == "yield_surface_radius_external_name") {
        if (!e.second.is<std::string>()) {
          raise("'yield_surface_radius_external_name' is not a string");
        }
        this->yield_surface_radius_external_name = e.second.get<std::string>();
      } else if (e.first == "save_yield_surface_radius") {
        if (!e.second.is<bool>()) {
          raise("'save_yield_surface_radius' is not a boolean");
        }
        this->save_yield_surface_radius = e.second.get<bool>();
      } else if (e.first == "save_porosity_increase") {
        if (!e.second.is<bool>()) {
          raise("'save_porosity_increase' is not a boolean");
        }
        this->save_porosity_increase = e.second.get<bool>();
      } else if (e.first == "porosity_evolution_algorithm") {
        if (!e.second.is<std::string>()) {
          raise("'porosity_evolution_algorithm' is not a boolean");
        }
        const auto& a = e.second.get<std::string>();
        if (a == "standard_implicit_scheme") {
          this->porosity_evolution_algorithm =
              PorosityEvolutionAlgorithm::STANDARD_IMPLICIT_SCHEME;
        } else if (a == "staggered_scheme") {
          this->porosity_evolution_algorithm =
              PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
        } else {
          raise("internal error: unsupported porosity evolution algorithm");
        }
      } else if (e.first == "porosity_effect_on_equivalent_plastic_strain") {
        if (!e.second.is<std::string>()) {
          raise(
              "'porosity_effect_on_equivalent_plastic_strain' is not a "
              "string");
        }
        const auto& pe = e.second.get<std::string>();
        if ((pe == "StandardPorosityEffect") ||
            (pe == "standard_porosity_effect")) {
          this->porosity_effect_on_equivalent_plastic_strain =
              STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
        } else if ((pe == "None") || (pe == "none") || (pe == "false")) {
          this->porosity_effect_on_equivalent_plastic_strain =
              NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
        } else if (pe != "Undefined") {
          raise(
              "invalid value of "
              "'porosity_effect_on_equivalent_plastic_strain'. Expected "
              "'StandardPorosityEffect' (or equivalently "
              "'standard_porosity_effect') or 'None' (or equivalently 'none' "
              "or 'false'), but got '" +
              pe + "'");
        }
      } else if (e.first == "cosine_threshold") {
        this->cosine_threshold = tfel::utilities::convert<double>(e.second);
        if ((this->cosine_threshold < -1) || (this->cosine_threshold > 1)) {
          raise(
              "invalid value for the option `cosine_threshold` (value must "
              "be in range [-1:1]");
        }
      } else if (e.first == "cosine_check_minimum_iteration_factor") {
        if (d.count("cosine_threshold") == 0) {
          raise(
              "option `cosine_check_minimum_iteration_factor` is only "
              "meaningful if the option `cosine_threshold` is also "
              "provided");
        }
        this->cosine_check_minimum_iteration_factor =
            tfel::utilities::convert<double>(e.second);
        if ((this->cosine_check_minimum_iteration_factor < -1) ||
            (this->cosine_check_minimum_iteration_factor > 1)) {
          raise(
              "invalid value for the option "
              "`cosine_check_minimum_iteration_factor` (value must be in "
              "range [-1:1]");
        }
        if (this->cosine_check_maximum_iteration_factor <
            this->cosine_check_minimum_iteration_factor) {
          raise(
              "the value of the option "
              "`cosine_check_maximum_iteration_factor` must be greater than "
              "the vaue of `cosine_check_minmum_iteration_factor`");
        }
      } else if (e.first == "cosine_check_maximum_iteration_factor") {
        if (d.count("cosine_threshold") == 0) {
          raise(
              "option `cosine_check_maximum_iteration_factor` is only "
              "meaningful if the option `cosine_threshold` is also "
              "provided");
        }
        this->cosine_check_maximum_iteration_factor =
            tfel::utilities::convert<double>(e.second);
        if ((this->cosine_check_maximum_iteration_factor < -1) ||
            (this->cosine_check_maximum_iteration_factor > 1)) {
          raise(
              "invalid value for the option "
              "`cosine_check_maximum_iteration_factor` (value must be in "
              "range [-1:1]");
        }
        if (this->cosine_check_maximum_iteration_factor <
            this->cosine_check_minimum_iteration_factor) {
          raise(
              "the value of the option "
              "`cosine_check_maximum_iteration_factor` must be greater than "
              "the vaue of `cosine_check_minmum_iteration_factor`");
        }
      }  // other options must be treated in child classes
    }
    if (this->cosine_threshold < 1.5) {
      addLocalVariable(bd, "Stensor", "np" + id);
      CodeBlock init;
      init.code = "this->np" + id + " = Stensor(real(0));\n";
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
    if (this->sc == nullptr) {
      raise("criterion has not been defined");
    }
    if (!this->ihrs.empty()) {
      addLocalVariable(bd, "bool", "bpl" + id);
      if (this->ihrs.size() != 1) {
        const auto Rel = "Rel" + id;
        const auto R = "R" + id;
        const auto dR = "d" + R + "_dp" + id;
        bd.reserveName(uh, Rel);
        bd.reserveName(uh, R);
        bd.reserveName(uh, dR);
      }
    }
    if (this->isCoupledWithPorosityEvolution()) {
      if (this->fc != nullptr) {
        if (this->fc->isNormalDeviatoric()) {
          raise(
              "InelasticFlowBase::initialize: "
              "the flow is coupled with the porosity evolution, but "
              "the flow direction is deviatoric");
        }
      }
      if (this->sc->isNormalDeviatoric()) {
        raise(
            "InelasticFlowBase::initialize: "
            "the flow is coupled with the porosity evolution by "
            "the flow direction is deviatoric");
      }
      if (!this->khrs.empty()) {
        raise(
            "InelasticFlowBase::initialize: "
            "kinematic hardening rules are not supported "
            "when coupled with a porosity evolution");
      }
    }
    if (this->contributesToPorosityGrowth()) {
      addLocalVariable(bd, "real", "trace_n" + id);
      if (this->save_porosity_increase) {
        addLocalVariable(bd, "real", "dfg" + id);
        VariableDescription fg("real", "fg" + id, 1u, 0u);
        const auto& g =
            tfel::glossary::Glossary::PorosityIncreaseDueToInelasticFlow;
        if (id.empty()) {
          fg.setGlossaryName(g);
        } else {
          fg.setEntryName(g.getKey() + id);
        }
        bd.addAuxiliaryStateVariable(uh, fg);
      }
    }
    if (this->save_stress_criterion) {
      const auto n = "mfront_" + this->stress_criterion_external_name;
      VariableDescription stress_criterion("stress", n, 1u, 0u);
      const auto& g = tfel::glossary::Glossary::getGlossary();
      if (g.contains(this->stress_criterion_external_name)) {
        stress_criterion.setGlossaryName(this->stress_criterion_external_name);
      } else {
        stress_criterion.setEntryName(this->stress_criterion_external_name);
      }
      bd.addAuxiliaryStateVariable(uh, stress_criterion);
    }
    if (this->save_yield_surface_radius) {
      if (this->ihrs.empty()) {
        raise(
            "InelasticFlowBase::initialize: "
            "`save_yield_surface_radius` is set to true, but no isotropic "
            "hardening rule is defined");
      }
      const auto n = "mfront_" + this->yield_surface_radius_external_name;
      VariableDescription yield_surface_radius("stress", n, 1u, 0u);
      const auto& g = tfel::glossary::Glossary::getGlossary();
      if (g.contains(this->yield_surface_radius_external_name)) {
        yield_surface_radius.setGlossaryName(
            this->yield_surface_radius_external_name);
      } else {
        yield_surface_radius.setEntryName(
            this->yield_surface_radius_external_name);
      }
      bd.addAuxiliaryStateVariable(uh, yield_surface_radius);
    }
  }  // end of initialize

  void InelasticFlowBase::setPorosityEvolutionHandled(const bool b) {
    if ((!b) && (this->isCoupledWithPorosityEvolution())) {
      tfel::raise(
          "InelasticFlowBase::setPorosityEvolutionHandled: "
          "internal error (consistent situation)");
    }
    this->porosity_evolution_explicitely_handled = b;
  }  // end of setPorosityEvolutionHandled

  bool InelasticFlowBase::contributesToPorosityGrowth() const {
    if ((this->porosity_evolution_explicitely_handled) ||
        (this->isCoupledWithPorosityEvolution())) {
      if (this->sc == nullptr) {
        tfel::raise(
            "InelasticFlowBase::contributesToPorosityGrowth: "
            "unitialised object");
      }
      const bool bn = [this] {
        if (this->fc != nullptr) {
          return !this->fc->isNormalDeviatoric();
        }
        return !this->sc->isNormalDeviatoric();
      }();
      return bn;
    }
    return false;
  }  // end of contributesToPorosityGrowth

  void InelasticFlowBase::completeVariableDeclaration(
      BehaviourDescription&,
      const AbstractBehaviourDSL&,
      const std::string&) const {}  // end of completeVariableDeclaration

  bool InelasticFlowBase::requiresActivationState() const {
    return !this->ihrs.empty();
  }  // end of requiresActivationState

  void InelasticFlowBase::computeInitialActivationState(
      BehaviourDescription& bd,
      const StressPotential& sp,
      const std::string& id) const {
    if (!this->ihrs.empty()) {
      CodeBlock i;
      if (this->khrs.empty()) {
        i.code += "const auto& sel" + id + " = sigel;\n";
      } else {
        auto kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          i.code += khr->computeKinematicHardeningsInitialValues(
              id, std::to_string(kid));
          ++kid;
        }
        // effective stress
        i.code += "const auto sel" + id + " = eval(sigel";
        kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          for (const auto& X :
               khr->getKinematicHardeningsVariables(id, std::to_string(kid))) {
            i.code += "-" + X;
            ++kid;
          }
        }
        i.code += ");\n";
      }
      i.code += this->sc->computeElasticPrediction(id, bd, sp);
      i.code += computeElasticLimitInitialValue(bd, this->ihrs, id);
      i.code += "this->bpl" + id + " = seqel" + id + " > Rel" + id + ";\n";
      if (this->save_stress_criterion) {
        i.code += "this->mfront_" + stress_criterion_external_name +
                  " = seqel" + id + ";\n";
      }
      if (this->save_yield_surface_radius) {
        i.code += "this->mfront_" + yield_surface_radius_external_name +
                  " = Rel" + id + ";\n";
      }
      bd.setCode(ModellingHypothesis::UNDEFINEDHYPOTHESIS,
                 BehaviourData::BeforeInitializeLocalVariables, i,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of computeInitialActivationState

  std::string InelasticFlowBase::computeEffectiveStress(
      const std::string& id) const {
    if (this->khrs.empty()) {
      return "const auto& s" + id + " = this->sig;\n";
    }
    auto c = std::string{};
    auto kid = decltype(khrs.size()){};
    for (const auto& khr : khrs) {
      c += khr->computeKinematicHardenings(id, std::to_string(kid));
      ++kid;
    }
    // effective stress
    c += "const auto s" + id + " = eval(this->sig";
    kid = decltype(khrs.size()){};
    for (const auto& khr : khrs) {
      for (const auto& X :
           khr->getKinematicHardeningsVariables(id, std::to_string(kid))) {
        c += "-" + X + "_";
      }
      ++kid;
    }
    c += ");\n";
    return c;
  }  // end of computeEffectiveStress

  void InelasticFlowBase::endTreatment(BehaviourDescription& bd,
                                       const AbstractBehaviourDSL& dsl,
                                       const StressPotential& sp,
                                       const std::string& id) const {
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    if (this->fc != nullptr) {
      this->sc->endTreatment(bd, dsl, id, StressCriterion::STRESSCRITERION);
      this->fc->endTreatment(bd, dsl, id, StressCriterion::FLOWCRITERION);
    } else {
      this->sc->endTreatment(bd, dsl, id,
                             StressCriterion::STRESSANDFLOWCRITERION);
    }
    const bool is_deviatoric = (this->fc != nullptr)
                                   ? this->fc->isNormalDeviatoric()
                                   : this->sc->isNormalDeviatoric();
    auto iid = decltype(ihrs.size()){};
    if (this->ihrs.size() == 1) {
      ihrs[0]->endTreatment(bd, dsl, id, "");
    } else {
      for (const auto& ihr : this->ihrs) {
        ihr->endTreatment(bd, dsl, id, std::to_string(iid));
        ++iid;
      }
    }
    auto kid = decltype(khrs.size()){};
    for (const auto& khr : this->khrs) {
      khr->endTreatment(bd, dsl, id, std::to_string(kid));
      ++kid;
    }
    const auto requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    // implicit equation associated with the elastic strain
    CodeBlock ib;
    if (!this->ihrs.empty()) {
      ib.code += "if(this->bpl" + id + "){\n";
    }
    if (idsl.getSolver().usesJacobian()) {
      ib.code += "if(!perturbatedSystemEvaluation){\n";
    }
    //       ib.code += "this->dp" + id + " = this->dp" + id + ",
    //       strain(0));\n";
    if (idsl.getSolver().usesJacobian()) {
      ib.code += "}\n";
    }
    ib.code += this->computeEffectiveStress(id);
    if (requiresAnalyticalJacobian) {
      if (this->fc == nullptr) {
        ib.code += this->sc->computeNormalDerivative(
            id, bd, sp, StressCriterion::STRESSANDFLOWCRITERION);
      } else {
        ib.code += this->sc->computeNormal(id, bd, sp,
                                           StressCriterion::STRESSCRITERION);
        ib.code += this->fc->computeNormalDerivative(
            id, bd, sp, StressCriterion::FLOWCRITERION);
      }
    } else {
      if (this->fc == nullptr) {
        ib.code += this->sc->computeNormal(
            id, bd, sp, StressCriterion::STRESSANDFLOWCRITERION);
      } else {
        ib.code += this->sc->computeCriterion(id, bd, sp);
        ib.code +=
            this->fc->computeNormal(id, bd, sp, StressCriterion::FLOWCRITERION);
      }
    }
    if (this->save_stress_criterion) {
      ib.code += "this->mfront_" + stress_criterion_external_name + " = seq" +
                 id + ";\n";
    }
    // check on the flow direction
    if (this->cosine_threshold < 1.5) {
      const auto imin =
          std::to_string(this->cosine_check_minimum_iteration_factor) +
          " * this->iterMax";
      const auto imax =
          std::to_string(this->cosine_check_maximum_iteration_factor) +
          " * this->iterMax";
      const auto seps = sp.getEquivalentStressLowerBound(bd);
      if (this->fc == nullptr) {
        ib.code += "if((seq" + id + ">" + seps + ")&&";
      } else {
        ib.code += "if((seqf" + id + ">" + seps + ")&&";
      }
      ib.code += "(this->iter > " + imin + ")&&";
      ib.code += "(this->iter < " + imax + ")){\n";
      ib.code += "if (std::abs(n" + id + " | this->np" + id + ") < ";
      ib.code += "std::sqrt((n" + id + ") | (n" + id + ")) * ";
      ib.code += "std::sqrt((this->np" + id + ") | (this->np" + id + ")) * " +
                 std::to_string(this->cosine_threshold) + ") {\n";
      ib.code += "return false;\n";
      ib.code += "}\n";
      ib.code += "}\n";
      ib.code += "this->np" + id + " = n" + id + ";\n";
    }
    if (this->isCoupledWithPorosityEvolution()) {
      ib.code += "this->trace_n" + id + " = trace(n" + id + ");\n";
    }
    // elasticity
    if (this->getPorosityEffectOnEquivalentPlasticStrain() ==
        STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN) {
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto f_ = f.name + "_";
      ib.code +=
          "feel += (1 - " + f_ + ") * this->dp" + id + "* n" + id + ";\n";
      if (requiresAnalyticalJacobian) {
        // jacobian terms
        ib.code += "dfeel_ddp" + id + " = (1 - " + f_ + ") * n" + id + ";\n";
        ib.code += sp.generateImplicitEquationDerivatives(
            bd, "StrainStensor", "eel",
            "(1 - " + f_ + ") * (this->dp" + id + ") * dn" + id + "_ds" + id,
            is_deviatoric);
        kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          ib.code += khr->generateImplicitEquationDerivatives(
              "eel",
              "- (1 - " + f_ + ") * (this->dp" + id + ") * dn" + id + "_ds" +
                  id,
              id, std::to_string(kid));
          ++kid;
        }
        ib.code += "dfeel_dd" + f.name + " += ";
        ib.code += "- theta * this->dp" + id + "* n" + id;
        ib.code += "+ theta * (1 - " + f_ + ") * this->dp" + id + "* dn" + id +
                   "_d" + f.name + ";\n";
      }
    } else {
      ib.code += "feel += this->dp" + id + "* n" + id + ";\n";
      if (requiresAnalyticalJacobian) {
        // jacobian terms
        ib.code += "dfeel_ddp" + id + " = n" + id + ";\n";
        ib.code += sp.generateImplicitEquationDerivatives(
            bd, "StrainStensor", "eel",
            "(this->dp" + id + ") * dn" + id + "_ds" + id, is_deviatoric);
        kid = decltype(khrs.size()){};
        for (const auto& khr : khrs) {
          ib.code += khr->generateImplicitEquationDerivatives(
              "eel", "-(this->dp" + id + ") * dn" + id + "_ds" + id, id,
              std::to_string(kid));
          ++kid;
        }
        if (this->isCoupledWithPorosityEvolution()) {
          const auto& f =
              bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                  tfel::glossary::Glossary::Porosity);
          ib.code += "dfeel_dd" + f.name + " += ";
          ib.code +=
              "theta * this->dp" + id + " * dn" + id + "_d" + f.name + ";\n";
        }
      }
    }
    // inelastic flow
    ib.code += this->buildFlowImplicitEquations(bd, sp, id,
                                                requiresAnalyticalJacobian);
    // hardening rules
    kid = decltype(khrs.size()){};
    for (const auto& khr : khrs) {
      if (this->fc != nullptr) {
        ib.code += khr->buildBackStrainImplicitEquations(
            bd, sp, *(this->fc), this->khrs, id, std::to_string(kid),
            requiresAnalyticalJacobian);
      } else {
        ib.code += khr->buildBackStrainImplicitEquations(
            bd, sp, *(this->sc), this->khrs, id, std::to_string(kid),
            requiresAnalyticalJacobian);
      }
      ++kid;
    }
    // porosity evolution
    if (this->contributesToPorosityGrowth()) {
      if (this->porosity_evolution_algorithm ==
          PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        ib.code += "if(";
        ib.code += StandardElastoViscoPlasticityBrick::
            computeStandardSystemOfImplicitEquations;
        ib.code += "){\n";
        this->addFlowContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
            ib, bd, dsl, sp, id);
        ib.code += "}\n";
      } else if (this->porosity_evolution_algorithm ==
                 PorosityEvolutionAlgorithm::STANDARD_IMPLICIT_SCHEME) {
        this->addFlowContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
            ib, bd, dsl, sp, id);
      } else {
        tfel::raise(
            "InelasticFlowBase::endTreatment: internal error "
            "(unsupported porosity evolution algorithm)");
      }
    }
    if (!this->ihrs.empty()) {
      ib.code += "} // end if(this->bpl" + id + ")\n";
    }
    bd.setCode(uh, BehaviourData::Integrator, ib, BehaviourData::CREATEORAPPEND,
               BehaviourData::AT_BEGINNING);
    // additional checks
    if (!this->ihrs.empty()) {
      CodeBlock acc;
      acc.code += "if (converged) {\n";
      acc.code += "// checking status consistency\n";
      acc.code += "if (this->bpl" + id + ") {\n";
      acc.code += "if (this->dp" + id + " < -2*this->epsilon) {\n";
      acc.code += "// desactivating this system\n";
      acc.code += "converged = this->bpl" + id + " = false;\n";
      acc.code += "}\n";
      acc.code += "} else {\n";
      if (this->isCoupledWithPorosityEvolution()) {
        const auto& f =
            bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Porosity);
        acc.code += "const auto " + f.name + "_ = ";
        acc.code += "this->" + f.name + " + (" + bd.getClassName() +
                    "::theta) * (this->d" + f.name + ");\n";
      }
      acc.code += this->computeEffectiveStress(id);
      acc.code += this->sc->computeCriterion(id, bd, sp);
      acc.code += computeElasticLimit(bd, this->ihrs, id);
      acc.code += "if(seq" + id + " > R" + id + ") {\n";
      acc.code += "converged = false;\n";
      acc.code += "this->bpl" + id + " = true;\n";
      acc.code += "}\n";
      acc.code += "}\n";
      acc.code += "} // end of if(converged)\n";
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
    // update auxiliary state variables
    if ((this->contributesToPorosityGrowth()) &&
        (this->save_porosity_increase)) {
      CodeBlock uav;
      uav.code += "this->fg" + id + " += this->dfg" + id + ";\n";
      bd.setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, uav,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of endTreatment

  std::string InelasticFlowBase::updateNextEstimateOfThePorosityIncrement(
      const BehaviourDescription& bd, const std::string& id) const {
    if (!this->contributesToPorosityGrowth()) {
      return "";
    }
    const auto df = [this, &bd, &id] {
      constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto theta = bd.getClassName() + "::theta";
      const auto f_ = "((this->" + f.name + ") + (" + theta + ") * (this->" +
                      std::string(StandardElastoViscoPlasticityBrick::
                                      currentEstimateOfThePorosityIncrement) +
                      "))";
      if (this->getPorosityEffectOnEquivalentPlasticStrain() ==
          STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN) {
        return "power<2>(1 - " + f_ + ") * " +  //
               "(this->dp" + id + ") * (this->trace_n" + id + ")";
      }
      return "(1 - " + f_ + ") * " +  //
             "(this->dp" + id + ") * (this->trace_n" + id + ")";
    }();
    auto c = std::string{};
    if (!this->ihrs.empty()) {
      c = "if (this->bpl" + id + "){\n";
    }
    if (this->save_porosity_increase) {
      c += "this->dfg" + id + " = " + df + ";\n";
      c += StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      c += " += this->dfg" + id + ";\n";
    } else {
      c += StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      c += " += " + df + ";\n";
    }
    if (this->save_porosity_increase) {
      c += "} else {\n";
      c += "this->dfg" + id + " = real{};\n";
    }
    if (!this->ihrs.empty()) {
      c += "}\n";
    }
    return c;
  }  // end of updateNextEstimateOfThePorosityIncrement

  void InelasticFlowBase::
      addFlowContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
          CodeBlock& ib,
          const BehaviourDescription& bd,
          const AbstractBehaviourDSL& dsl,
          const StressPotential& sp,
          const std::string& id) const {
    if (!this->contributesToPorosityGrowth()) {
      return;
    }
    constexpr auto uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(dsl);
    const auto requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    const auto& broken =
        bd.getBehaviourData(uh)
            .getAuxiliaryStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Broken);
    const auto f_ = f.name + "_";
    ib.code += "if(2 * (this->" + broken.name + ") > 1){\n";
    if (this->save_porosity_increase) {
      ib.code += "this->dfg" + id + " = real{};\n";
      ib.code += "f" + f.name + " -= this->dfg" + id + ";\n";
    } else {
      ib.code += "f" + f.name + " -= real{};\n";
    }
    ib.code += "} else {\n";
    if (this->getPorosityEffectOnEquivalentPlasticStrain() ==
        STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN) {
      if (this->save_porosity_increase) {
        ib.code += "this->dfg" + id + " = power<2>(1 - " + f_ +
                   ") * (this->dp" + id + ") * (this->trace_n" + id + ");\n";
        ib.code += "f" + f.name + " -= this->dfg" + id + ";\n";
      } else {
        ib.code += "f" + f.name;
        ib.code += " -= power<2>(1 - " + f_ + ") * (this->dp" + id +
                   ") * (this->trace_n" + id + ");\n";
      }
    } else {
      if (this->save_porosity_increase) {
        ib.code += "this->dfg" + id + " = (1 - " + f_ + ") * (this->dp" + id +
                   ") * (this->trace_n" + id + ");\n";
        ib.code += "f" + f.name + " -= this->dfg" + id + ";\n";
      } else {
        ib.code += "f" + f.name;
        ib.code += " -= (1 - " + f_ + ") * (this->dp" + id +
                   ") * (this->trace_n" + id + ");\n";
      }
    }
    if (requiresAnalyticalJacobian) {
      if (this->getPorosityEffectOnEquivalentPlasticStrain() ==
          STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN) {
        ib.code += "df" + f.name + "_dd" + f.name;
        ib.code += " += 2 * theta * (1 - " + f_ + ") * (this->dp" + id +
                   ") * (this->trace_n" + id + ")";
        ib.code += "  -     theta * power<2>(1 - " + f_ + ")* (this->dp" + id +
                   ") * (Stensor::Id() | dn_d" + f.name + ");\n";
        // derivatives with respect to stress
        for (const auto& dsig : sp.getStressDerivatives(bd)) {
          const auto& dsig_ddv = std::get<0>(dsig);
          const auto& v = std::get<1>(dsig);
          const auto& t = std::get<2>(dsig);
          if ((t != SupportedTypes::SCALAR) && (t != SupportedTypes::STENSOR)) {
            tfel::raise(
                "InelasticFlowBase::endTreatment: "
                "stress dependency on variable '" +
                v + "' is not unsupported ");
          }
          ib.code += "df" + f.name + "_dd" + v + " -= ";
          ib.code += "power<2>(1 - " + f_ + ") * (this->dp" + id + ") * ";
          ib.code += "(Stensor::Id() | (dn" + id + "_ds" + id + " * (" +
                     dsig_ddv + ")));\n";
        }
        auto kid = decltype(khrs.size()){};
        for (const auto& khr : this->khrs) {
          const auto an = khr->getBackStrainVariable(id, std::to_string(kid));
          const auto dX = khr->getBackStressDerivative(id, std::to_string(kid));
          ib.code += "df" + f.name + "_dd" + an + " += ";
          ib.code += "power<2>(1 - " + f_ + ") * (this->dp" + id + ") * ";
          ib.code += "(Stensor::Id() | " + dX + ");\n";
          ++kid;
        }
        ib.code += "df" + f.name + "_ddp" + id;
        ib.code +=
            " = - power<2>(1 - " + f_ + ") * (this->trace_n" + id + ");\n";
      } else {
        ib.code += "df" + f.name + "_dd" + f.name;
        ib.code +=
            " += theta *(this->dp" + id + ") * (this->trace_n" + id + ")";
        ib.code += "  - theta * (1 - " + f_ + ")* (this->dp" + id +
                   ") * (Stensor::Id() | dn_d" + f.name + ");\n";
        // derivatives with respect to stress
        for (const auto& dsig : sp.getStressDerivatives(bd)) {
          const auto& dsig_ddv = std::get<0>(dsig);
          const auto& v = std::get<1>(dsig);
          const auto& t = std::get<2>(dsig);
          if ((t != SupportedTypes::SCALAR) && (t != SupportedTypes::STENSOR)) {
            tfel::raise(
                "InelasticFlowBase::endTreatment: "
                "stress dependency on variable '" +
                v + "' is not unsupported ");
          }
          ib.code += "df" + f.name + "_dd" + v + " -= ";
          ib.code += "(1 - " + f_ + ") * (this->dp" + id + ") * ";
          ib.code += "(Stensor::Id() | (dn" + id + "_ds" + id + " * (" +
                     dsig_ddv + ")));\n";
        }
        auto kid = decltype(khrs.size()){};
        for (const auto& khr : this->khrs) {
          const auto an = khr->getBackStrainVariable(id, std::to_string(kid));
          const auto dX = khr->getBackStressDerivative(id, std::to_string(kid));
          ib.code += "df" + f.name + "_dd" + an + " += ";
          ib.code += "(1 - " + f_ + ") * (this->dp" + id + ") * ";
          ib.code += "(Stensor::Id() | " + dX + ");\n";
          ++kid;
        }
        ib.code += "df" + f.name + "_ddp" + id;
        ib.code += " = - (1 - " + f_ + ") * (this->trace_n" + id + ");\n";
      }
    }
    ib.code += "}\n";
  }  // end of
     // InelasticFlowBase::addFlowContributionToTheImplicitEquationAssociatedWithPorosityEvolution

  bool InelasticFlowBase::isCoupledWithPorosityEvolution() const {
    if (this->sc == nullptr) {
      tfel::raise(
          "InelasticFlowBase::isCoupledWithPorosityEvolution: "
          "uninitialized flow");
    }
    if (this->sc->isCoupledWithPorosityEvolution()) {
      return true;
    }
    if (this->fc != nullptr) {
      return this->fc->isCoupledWithPorosityEvolution();
    }
    return false;
  }  // end of isCoupledWithPorosityEvolution

  InelasticFlowBase::PorosityEffectOnFlowRule
  InelasticFlowBase::getPorosityEffectOnEquivalentPlasticStrain() const {
    if (this->porosity_effect_on_equivalent_plastic_strain ==
        UNDEFINED_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN) {
      if (this->sc == nullptr) {
        tfel::raise(
            "InelasticFlowBase::getPorosityEffectOnEquivalentPlasticStrain:"
            "uninitialised flow");
      }
      const auto scpe = this->sc->getPorosityEffectOnEquivalentPlasticStrain();
      if ((scpe !=
           StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN) &&
          (scpe !=
           StressCriterion::
               STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN)) {
        tfel::raise(
            "InelasticFlowBase::getPorosityEffectOnEquivalentPlasticStrain:"
            "unsupported porosity effect defined by the stress criterion");
      }
      if (this->fc != nullptr) {
        const auto fcpe =
            this->fc->getPorosityEffectOnEquivalentPlasticStrain();
        if ((fcpe != StressCriterion::
                         NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN) &&
            (fcpe !=
             StressCriterion::
                 STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN)) {
          tfel::raise(
              "InelasticFlowBase::getPorosityEffectOnEquivalentPlasticStrain:"
              "unsupported porosity effect defined by the flow criterion");
        }
        if ((scpe ==
             StressCriterion::
                 STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN) ||
            (fcpe ==
             StressCriterion::
                 STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN)) {
          return STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
        }
        return NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
      } else {
        if (scpe ==
            StressCriterion::NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN) {
          return NO_POROSITY_EFFECT_ON_EQUIVALENT_PLASTIC_STRAIN;
        }
        return STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
      }
    }
    return this->porosity_effect_on_equivalent_plastic_strain;
  }  // end of getPorosityEffectOnEquivalentPlasticStrain

  std::string InelasticFlowBase::updatePorosityUpperBound(
      const BehaviourDescription& bd, const std::string& id) const {
    if (this->sc == nullptr) {
      tfel::raise(
          "InelasticFlowBase::updatePorosityUpperBound: "
          "uninitialized flow");
    }
    return this->sc->updatePorosityUpperBound(bd, id,
                                              StressCriterion::STRESSCRITERION);
  }  // end of updatePorosityUpperBound

  InelasticFlowBase::~InelasticFlowBase() = default;

}  // end of namespace mfront::bbrick
