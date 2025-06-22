/*!
 * \file   mfront/src/StandardElastoViscoPlasticityBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/03/2018
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
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/InelasticFlow.hxx"
#include "MFront/BehaviourBrick/InelasticFlowFactory.hxx"
#include "MFront/BehaviourBrick/PorosityNucleationModel.hxx"
#include "MFront/BehaviourBrick/PorosityNucleationModelFactory.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"

namespace mfront {

  static std::string getId(const size_t i, const size_t m) {
    if (m == 1) {
      return "";
    }
    return std::to_string(i);
  }  // end of getId

  static void declarePorosityUpperBoundSafetyFactorParameter(
      BehaviourDescription& bd, const double v) {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto n =
        StandardElastoViscoPlasticityBrick::porosityUpperBoundSafetyFactor;
    VariableDescription p("real", n, 1u, 0u);
    p.description = "a safety factor for the porosity upper bound";
    bd.addParameter(uh, p, BehaviourData::UNREGISTRED);
    bd.setParameterDefaultValue(uh, n, v);
  }  // end of declarePorosityUpperBoundSafetyFactor

  static void
  declarePorosityUpperBoundSafetyFactorForFractureDetectionParameter(
      BehaviourDescription& bd, const double v) {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto n = StandardElastoViscoPlasticityBrick::
        porosityUpperBoundSafetyFactorForFractureDetection;
    VariableDescription p("real", n, 1u, 0u);
    p.description = "a safety factor for the porosity upper bound";
    bd.addParameter(uh, p, BehaviourData::UNREGISTRED);
    bd.setParameterDefaultValue(uh, n, v);
  }  // end of declarePorosityUpperBoundSafetyFactorForFractureDetection

  const char* const StandardElastoViscoPlasticityBrick::
      computeStandardSystemOfImplicitEquations =
          "compute_standard_system_of_implicit_equations";
  const char* const StandardElastoViscoPlasticityBrick::brokenVariable =
      "broken";
  const char* const StandardElastoViscoPlasticityBrick::
      currentEstimateOfThePorosityIncrement =
          "current_estimate_of_the_porosity_increment";
  const char* const
      StandardElastoViscoPlasticityBrick::nextEstimateOfThePorosityIncrement =
          "next_estimate_of_the_porosity_increment";
  const char* const StandardElastoViscoPlasticityBrick::fixedPointConverged =
      "fixed_point_converged";
  const char* const StandardElastoViscoPlasticityBrick::
      nextEstimateOfThePorosityAtTheEndOfTheTimeStep =
          "next_estimate_of_the_porosity_at_the_end_of_the_time_step";
  const char* const StandardElastoViscoPlasticityBrick::porosityUpperBound =
      "upper_bound_of_the_porosity";
  const char* const
      StandardElastoViscoPlasticityBrick::porosityUpperBoundSafetyFactor =
          "safety_factor_for_the_upper_bound_of_the_porosity";
  const char* const StandardElastoViscoPlasticityBrick::
      porosityUpperBoundSafetyFactorForFractureDetection =
          "safety_factor_for_the_upper_bound_of_the_porosity_for_fracture_"
          "detection";
  const char* const StandardElastoViscoPlasticityBrick::
      differenceBetweenSuccessiveEstimatesOfThePorosityIncrement =
          "difference_between_successive_estimates_of_the_porosity_increment";
  const char* const
      StandardElastoViscoPlasticityBrick::staggeredSchemeConvergenceCriterion =
          "staggered_scheme_porosity_criterion";
  const char* const
      StandardElastoViscoPlasticityBrick::staggeredSchemeIterationCounter =
          "staggered_scheme_iteration_counter";
  const char* const StandardElastoViscoPlasticityBrick::
      maximumNumberOfIterationsOfTheStaggeredScheme =
          "staggered_scheme_maximum_number_of_iterations";
  const char* const
      StandardElastoViscoPlasticityBrick::staggeredSchemeBissectionAlgorithm =
          "staggered_scheme_bissection_algorithm";
  const char* const StandardElastoViscoPlasticityBrick::
      staggeredSchemeAitkenAccelerationAlgorithm =
          "staggered_scheme_aitken_acceleration_algorithm";

  StandardElastoViscoPlasticityBrick::StandardElastoViscoPlasticityBrick(
      AbstractBehaviourDSL& dsl_, BehaviourDescription& mb_)
      : BehaviourBrickBase(dsl_, mb_) {}  // end of
  // StandardElastoViscoPlasticityBrick::StandardElastoViscoPlasticityBrick

  std::string StandardElastoViscoPlasticityBrick::getName() const {
    return "ElastoViscoPlasticity";
  }  // end of StandardElastoViscoPlasticityBrick::getName

  BehaviourBrickDescription StandardElastoViscoPlasticityBrick::getDescription()
      const {
    auto d = BehaviourBrickDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
    d.integrationScheme = IntegrationScheme::IMPLICITSCHEME;
    d.supportedModellingHypotheses =
        ModellingHypothesis::getModellingHypotheses();
    d.supportedBehaviourSymmetries = {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    return d;
  }  // end of StandardElastoViscoPlasticityBrick::getDescription

  std::vector<bbrick::OptionDescription>
  StandardElastoViscoPlasticityBrick::getOptions(const bool) const {
    auto opts = std::vector<bbrick::OptionDescription>{};
    opts.emplace_back("stress_potential",
                      "Decare the stress potential (required)",
                      bbrick::OptionDescription::STRING);
    opts.emplace_back("inelastic_flow", "Declare another inelastic flow",
                      bbrick::OptionDescription::STRING);
    opts.emplace_back(
        "porosity_evolution",
        "state if the porosity evolution must be taken into account",
        bbrick::OptionDescription::STRING);
    return opts;
  }  // end of StandardElastoViscoPlasticityBrick::getOptions

  void StandardElastoViscoPlasticityBrick::initialize(const Parameters&,
                                                      const DataMap& d) {
    auto raise = [](const std::string& m) {
      tfel::raise("StandardElastoViscoPlasticityBrick::initialize: " + m);
    };  // end of raise
    auto& iff = bbrick::InelasticFlowFactory::getFactory();
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
    auto getStressPotential = [&d, &getDataStructure, &raise,
                               this](const char* const n) {
      if (d.count(n) != 0) {
        const auto ds = getDataStructure(n, d.at(n));
        if (this->stress_potential != nullptr) {
          raise("the stress potential has already been defined");
        }
        auto& spf = bbrick::StressPotentialFactory::getFactory();
        this->stress_potential = spf.generate(ds.name);
        this->stress_potential->initialize(this->bd, this->dsl, ds.data);
      }
    };
    //
    this->checkOptionsNames(d);
    //
    getStressPotential("elastic_potential");
    getStressPotential("stress_potential");
    if (this->stress_potential == nullptr) {
      raise("no stress potential defined");
    }
    auto save_individual_porosity_increase = false;
    if (d.count("porosity_evolution") != 0) {
      save_individual_porosity_increase =
          this->treatPorosityEvolutionSection(d.at("porosity_evolution"));
    }
    for (const auto& e : d) {
      if ((e.first == "elastic_potential") || (e.first == "stress_potential")) {
        // already treated
      } else if (e.first == "inelastic_flow") {
        auto append_flow = [this, &iff, getDataStructure, raise,
                            &save_individual_porosity_increase](
                               const Data& ifd, const size_t msize) {
          const auto ds = getDataStructure("inelatic_flow", ifd);
          auto iflow = iff.generate(ds.name);
          auto data = ds.data;
          if (data.count("save_porosity_increase") == 0) {
            data["save_porosity_increase"] = save_individual_porosity_increase;
          }
          if (data.count("porosity_evolution_algorithm") != 0) {
            raise(
                "the `porosity_evolution_algorithm` entry is reserved and "
                "shall not be set by the user");
          }
          if (this->porosity_evolution_algorithm ==
              mfront::bbrick::PorosityEvolutionAlgorithm::
                  STANDARD_IMPLICIT_SCHEME) {
            data["porosity_evolution_algorithm"] =
                std::string("standard_implicit_scheme");
          } else if (this->porosity_evolution_algorithm ==
                     mfront::bbrick::PorosityEvolutionAlgorithm::
                         STAGGERED_SCHEME) {
            data["porosity_evolution_algorithm"] =
                std::string("staggered_scheme");
          } else {
            raise("internal error (unsupported porosity algorithm)");
          }
          iflow->initialize(this->bd, this->dsl,
                            getId(this->flows.size(), msize), data);
          this->flows.push_back(iflow);
        };
        if (e.second.is<std::vector<Data>>()) {
          // multiple inelastic flows are defined
          const auto& ifs = e.second.get<std::vector<Data>>();
          for (const auto& iflow : ifs) {
            append_flow(iflow, ifs.size());
          }
        } else {
          append_flow(e.second, 1u);
        }
      } else if (e.first == "porosity_evolution") {
      } else {
        raise("unsupported entry '" + e.first + "'");
      }
    }
    // say to every flows if the porosity is handled by the brick
    const auto pe = this->isCoupledWithPorosityEvolution();
    for (auto& f : this->flows) {
      f->setPorosityEvolutionHandled(pe);
    }
    //
    if (this->porosity_evolution_algorithm ==
        mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
      this->bd.appendToIncludes(
          "#include \"TFEL/Math/RootFinding/BissectionAlgorithmBase.hxx\"\n");
      mfront::bbrick::addLocalVariable(
          bd, "tfel::math::BissectionAlgorithmBase<real>",
          staggeredSchemeBissectionAlgorithm);
      if (this->staggered_scheme_parameters.acceleration_algorithm ==
          StaggeredSchemeParameters::AITKEN) {
        this->bd.appendToIncludes(
            "#include "
            "\"TFEL/Math/AccelerationAlgorithms/"
            "AitkenAccelerationAlgorithm.hxx\"\n");
        mfront::bbrick::addLocalVariable(
            bd, "tfel::math::AitkenAccelerationAlgorithm<real>",
            staggeredSchemeAitkenAccelerationAlgorithm);
      }
    }
  }  // end of StandardElastoViscoPlasticityBrick::initialize

  bool StandardElastoViscoPlasticityBrick::treatPorosityEvolutionSection(
      const Data& e) {
    auto save_individual_porosity_increase = false;
    auto& nmf = bbrick::PorosityNucleationModelFactory::getFactory();
    auto raise = [](const std::string& m) {
      tfel::raise(
          "StandardElastoViscoPlasticityBrick::"
          "treatPorosityEvolutionSection: " +
          m);
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
    if (!e.is<DataMap>()) {
      raise("invalid data type for entry 'porosity_evolution'");
    }
    const auto ed = e.get<DataMap>();
    if (ed.count("save_individual_porosity_increase") != 0) {
      const auto b = ed.at("save_individual_porosity_increase");
      if (!b.is<bool>()) {
        raise(
            "the 'save_individual_porosity_increase' option is not a boolean "
            "value");
      }
      save_individual_porosity_increase = b.get<bool>();
    }
    for (const auto& ped : ed) {
      if (ped.first == "save_individual_porosity_increase") {
        // already treated
      } else if (ped.first == "elastic_contribution") {
        const auto b = ed.at("elastic_contribution");
        if (!b.is<bool>()) {
          raise("the 'elastic_contribution' option is not a boolean value");
        }
        this->elastic_contribution = b.get<bool>();
      } else if (ped.first == "nucleation_model") {
        auto append_nucleation_model = [this, &nmf, getDataStructure, raise,
                                        &save_individual_porosity_increase](
                                           const Data& nmd,
                                           const size_t msize) {
          const auto ds = getDataStructure("nucleation_model", nmd);
          auto nm = nmf.generate(ds.name);
          auto data = ds.data;
          if (data.count("save_individual_porosity_increase") == 0) {
            data["save_individual_porosity_increase"] =
                save_individual_porosity_increase;
          }
          if (data.count("porosity_evolution_algorithm") != 0) {
            raise(
                "the `porosity_evolution_algorithm` entry is reserved and "
                "shall not be set by the user");
          }
          if (porosity_evolution_algorithm ==
              mfront::bbrick::PorosityEvolutionAlgorithm::
                  STANDARD_IMPLICIT_SCHEME) {
            data["porosity_evolution_algorithm"] =
                std::string("standard_implicit_scheme");
          } else if (porosity_evolution_algorithm ==
                     mfront::bbrick::PorosityEvolutionAlgorithm::
                         STAGGERED_SCHEME) {
            data["porosity_evolution_algorithm"] =
                std::string("staggered_scheme");
          } else {
            raise("internal error (unsupported porosity algorithm)");
          }
          nm->initialize(this->bd, this->dsl,
                         getId(this->nucleation_models.size(), msize), data);
          this->nucleation_models.push_back(nm);
        };
        if (ped.second.is<std::vector<Data>>()) {
          // multiple inelastic nucleation_models are defined
          const auto& nms = ped.second.get<std::vector<Data>>();
          for (const auto& nm : nms) {
            append_nucleation_model(nm, nms.size());
          }
        } else {
          append_nucleation_model(ped.second, 1u);
        }
      } else if (ped.first == "algorithm") {
        this->treatPorosityEvolutionAlgorithmSection(ped.second);
      } else if (ped.first == porosityUpperBoundSafetyFactor) {
        const auto value = tfel::utilities::convert<double>(ped.second);
        declarePorosityUpperBoundSafetyFactorParameter(bd, value);
      } else if (ped.first ==
                 porosityUpperBoundSafetyFactorForFractureDetection) {
        const auto value = tfel::utilities::convert<double>(ped.second);
        declarePorosityUpperBoundSafetyFactorForFractureDetectionParameter(
            bd, value);
      } else {
        raise("invalid entry '" + ped.first + "' in 'porosity_evolution'");
      }
    }  // end of the loop over the entry of the porosity_evolution options
    // Since a porosity_evolution section has been defined,
    // we consider that all the non purely deviatoric inelastic flow
    // contributes to the porosity evolution
    if (this->porosity_growth_policy == UNDEFINEDPOROSITYGROWTHPOLICY) {
      this->porosity_growth_policy = STANDARDVISCOPLASTICPOROSITYGROWTHPOLICY;
    }
    return save_individual_porosity_increase;
  }  // end of
     // StandardElastoViscoPlasticityBrick::treatPorosityEvolutionSection

  void
  StandardElastoViscoPlasticityBrick::treatPorosityEvolutionAlgorithmSection(
      const Data& d) {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "StandardElastoViscoPlasticityBrick::"
          "treatPorosityEvolutionAlgorithmSection: " +
          m);
    };  // end of raise
    auto get_algorithm = [&raise](const std::string& a) {
      if ((a == "standard implicit scheme") ||
          (a == "standard_implicit_scheme") ||
          (a == "StandardImplicitScheme")) {
        return mfront::bbrick::PorosityEvolutionAlgorithm::
            STANDARD_IMPLICIT_SCHEME;
      } else if (!((a == "staggered scheme") || (a == "staggered_scheme") ||
                   (a == "StaggeredScheme"))) {
        raise(
            "error while treating entry 'algorithm' in "
            "'porosity_evolution': unhandled algorithm '" +
            a +
            "'. Known algorithms are:\n"
            "- 'standard implicit scheme' (or equivalently "
            "'standard_implicit_scheme' or 'StandardImplicitScheme')\n"
            "- 'staggered scheme' (or equivalently 'staggered_scheme' or "
            "'StaggeredScheme'");
      }
      return mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME;
    };
    if (d.is<std::string>()) {
      this->porosity_evolution_algorithm = get_algorithm(d.get<std::string>());
    } else if (d.is<tfel::utilities::DataStructure>()) {
      const auto& a = d.get<tfel::utilities::DataStructure>();
      const auto& algorithm_parameters = a.data;
      this->porosity_evolution_algorithm = get_algorithm(a.name);
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::
              STANDARD_IMPLICIT_SCHEME) {
        if (!algorithm_parameters.empty()) {
          raise(
              "no algorithm parameter for the 'standard implicit "
              "scheme'");
        }
      } else if (this->porosity_evolution_algorithm ==
                 mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        this->treatStaggeredPorosityEvolutionAlgorithmParameters(
            algorithm_parameters);
      } else {
        raise("internal error (unsupported porosity evolution algorithm)");
      }
    } else {
      raise("invalid type for entry 'algorithm' in 'porosity_evolution'");
    }
  }  // end of
  // StandardElastoViscoPlasticityBrick::treatPorosityEvolutionAlgorithmSection

  void StandardElastoViscoPlasticityBrick::
      treatStaggeredPorosityEvolutionAlgorithmParameters(
          const DataMap& algorithm_parameters) {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "StandardElastoViscoPlasticityBrick::"
          "treatStaggeredPorosityEvolutionAlgorithmParameters: " +
          m);
    };  // end of raise
    for (const auto& p : algorithm_parameters) {
      if (p.first == "convergence_criterion") {
        if (!p.second.is<double>()) {
          raise("invalid type for the `convergence_criterion` parameter");
        }
        const auto& c = p.second.get<double>();
        if (c <= 0) {
          raise("invalid value of the `convergence_criterion` parameter");
        }
        this->staggered_scheme_parameters.convergence_criterion = c;
      } else if (p.first == "maximum_number_of_iterations") {
        if (!p.second.is<int>()) {
          raise(
              "invalid type for the `maximum_number_of_iterations` "
              "parameter");
        }
        const auto& i = p.second.get<int>();
        if (i < 1) {
          raise(
              "invalid value of the `maximum_number_of_iterations` "
              "parameter");
        }
        this->staggered_scheme_parameters.maximum_number_of_iterations =
            static_cast<unsigned int>(i);
      } else if (p.first == "acceleration_algorithm") {
        if (!p.second.is<std::string>()) {
          raise(
              "invalid type for the `acceleration_algorithm`"
              "parameter");
        }
        const auto& a = p.second.get<std::string>();
        if (a == "Aitken") {
          this->staggered_scheme_parameters.acceleration_algorithm =
              StaggeredSchemeParameters::AITKEN;
        } else if ((a == "Relaxation") || (a == "relaxation")) {
          this->staggered_scheme_parameters.acceleration_algorithm =
              StaggeredSchemeParameters::RELAXATION;
        } else {
          raise("unsupported acceleration algorithm '" + a +
                "' (expected 'Aitken' or 'Relaxation')");
        }
      } else {
        raise("invalid parameter '" + p.first + "'");
      }
    }
  }  // end of treatStaggeredPorosityEvolutionAlgorithmParameters

  bool StandardElastoViscoPlasticityBrick::isCoupledWithPorosityEvolution()
      const {
    for (const auto& f : this->flows) {
      if (f->isCoupledWithPorosityEvolution()) {
        return true;
      }
    }
    if (!this->nucleation_models.empty()) {
      return true;
    }
    // no flow coupled with porosity evolution. However, if
    // `porosity_growth_policy` is not equal to
    // `UNDEFINEDPOROSITYGROWTHPOLICY`,
    // the user has defined an `porosity_evolution` section, which means that
    // he wants that any non purely deviatoric inelastic flow contributes to
    // the
    // porosity growth
    return this->porosity_growth_policy != UNDEFINEDPOROSITYGROWTHPOLICY;
  }  // end if
     // StandardElastoViscoPlasticityBrick::isCoupledWithPorosityEvolution

  std::vector<StandardElastoViscoPlasticityBrick::Hypothesis>
  StandardElastoViscoPlasticityBrick::getSupportedModellingHypotheses() const {
    return this->stress_potential->getSupportedModellingHypotheses(this->bd,
                                                                   this->dsl);
  }

  std::map<std::string, std::shared_ptr<mfront::bbrick::InelasticFlow>>
  StandardElastoViscoPlasticityBrick::buildInelasticFlowsMap() const {
    auto i = size_t{};
    auto m = std::map<std::string, std::shared_ptr<bbrick::InelasticFlow>>{};
    for (const auto& f : this->flows) {
      m.insert({getId(i, this->flows.size()), f});
    }
    return m;
  }  // end of StandardElastoViscoPlasticityBrick::buildInelasticFlowsMap()

  void StandardElastoViscoPlasticityBrick::completeVariableDeclaration() const {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->isCoupledWithPorosityEvolution()) {
      mfront::bbrick::addStateVariableIfNotDefined(
          bd, "real", "f", tfel::glossary::Glossary::Porosity, 1u, true);
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      if (!f.hasPhysicalBounds()) {
        VariableBoundsDescription fbounds;
        fbounds.boundsType = VariableBoundsDescription::LOWERANDUPPER;
        fbounds.lowerBound = 0;
        fbounds.upperBound = 1;
        this->bd.setPhysicalBounds(uh, f.name, fbounds);
      }
      this->bd.reserveName(uh, f.name + "_");
      //
      mfront::bbrick::addAuxiliaryStateVariableIfNotDefined(
          bd, "real", brokenVariable, tfel::glossary::Glossary::Broken, 1u,
          true);
    }
    this->stress_potential->completeVariableDeclaration(this->bd, this->dsl);
    auto i = size_t{};
    for (const auto& f : this->flows) {
      f->completeVariableDeclaration(this->bd, this->dsl,
                                     getId(i, this->flows.size()));
      ++i;
    }
    i = size_t{};
    for (const auto& nm : this->nucleation_models) {
      nm->completeVariableDeclaration(this->bd, this->dsl,
                                      this->buildInelasticFlowsMap(),
                                      getId(i, this->nucleation_models.size()));
      ++i;
    }
    // automatic registration of the porosity for porous flows
    if (this->isCoupledWithPorosityEvolution()) {
      //
      if (!bd.hasParameter(uh, porosityUpperBoundSafetyFactor)) {
        declarePorosityUpperBoundSafetyFactorParameter(bd, 0.985);
      }
      //
      if (!bd.hasParameter(
              uh, porosityUpperBoundSafetyFactorForFractureDetection)) {
        declarePorosityUpperBoundSafetyFactorForFractureDetectionParameter(
            bd, 0.984);
      }
      mfront::bbrick::addLocalVariable(bd, "real", porosityUpperBound);
      //
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        mfront::bbrick::addLocalVariable(
            bd, "bool", computeStandardSystemOfImplicitEquations);
        mfront::bbrick::addLocalVariable(bd, "real",
                                         currentEstimateOfThePorosityIncrement);
        mfront::bbrick::addLocalVariable(bd, "ushort",
                                         staggeredSchemeIterationCounter);
        // convergence criterion of the staggered scheme
        VariableDescription eps("real", staggeredSchemeConvergenceCriterion, 1u,
                                0u);
        eps.description = "stopping criterion value of the staggered scheme";
        bd.addParameter(uh, eps, BehaviourData::UNREGISTRED);
        bd.setParameterDefaultValue(
            uh, staggeredSchemeConvergenceCriterion,
            this->staggered_scheme_parameters.convergence_criterion);
        // maximum number of iterations of the staggered scheme
        VariableDescription iterMax(
            "ushort", maximumNumberOfIterationsOfTheStaggeredScheme, 1u, 0u);
        iterMax.description =
            "maximum number of iterations of the staggered scheme allowed";
        bd.addParameter(uh, iterMax, BehaviourData::UNREGISTRED);
        bd.setParameterDefaultValue(
            uh, maximumNumberOfIterationsOfTheStaggeredScheme,
            static_cast<unsigned short>(this->staggered_scheme_parameters
                                            .maximum_number_of_iterations));
      }
    }
  }  // end of StandardElastoViscoPlasticityBrick::completeVariableDeclaration

  void StandardElastoViscoPlasticityBrick::endTreatment() const {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //
    if (this->isCoupledWithPorosityEvolution()) {
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        const auto& broken =
            bd.getBehaviourData(uh)
                .getAuxiliaryStateVariableDescriptionByExternalName(
                    tfel::glossary::Glossary::Broken);
        CodeBlock init;
        // initialize the porosity status
        init.code += "this->";
        init.code += StandardElastoViscoPlasticityBrick::
            computeStandardSystemOfImplicitEquations;
        init.code += " = 2 * (this->" + broken.name + ") > 1;\n";
        // initialize the porosity increment
        init.code += StandardElastoViscoPlasticityBrick::
            currentEstimateOfThePorosityIncrement;
        init.code += " = real{};\n";
        // initialize the iteration counter of the staggered scheme
        init.code += "this->";
        init.code +=
            StandardElastoViscoPlasticityBrick::staggeredSchemeIterationCounter;
        init.code += " = static_cast<unsigned short>(0);\n";
        if ((this->porosity_evolution_algorithm ==
             mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) &&
            (this->staggered_scheme_parameters.acceleration_algorithm ==
             StaggeredSchemeParameters::AITKEN)) {
          init.code += staggeredSchemeAitkenAccelerationAlgorithm;
          init.code += ".initialize(real(0));\n";
        }
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto f_ = f.name + "_";
      // value of the porosity at t+theta*dt
      CodeBlock ib;
      ib.code += "const auto " + f_ + " = ";
      ib.code += "std::max(std::min(this->" + f.name + " + (" +
                 this->bd.getClassName() + "::theta) * (this->d" + f.name +
                 "), (this->" + std::string(porosityUpperBoundSafetyFactor) +
                 ") * " + std::string(porosityUpperBound) + "), real(0));\n ";
      ib.code += "static_cast<void>(" + f_ + ");\n";
      if (this->elastic_contribution) {
        if (this->porosity_evolution_algorithm ==
            mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
          ib.code += "if(";
          ib.code += StandardElastoViscoPlasticityBrick::
              computeStandardSystemOfImplicitEquations;
          ib.code += "){\n";
          this->addElasticContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
              ib);
          ib.code += "}\n";
        } else if (this->porosity_evolution_algorithm ==
                   mfront::bbrick::PorosityEvolutionAlgorithm::
                       STANDARD_IMPLICIT_SCHEME) {
          this->addElasticContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
              ib);
        } else {
          tfel::raise(
              "StandardElastoViscoPlasticityBrick::endTreatment: internal "
              "error "
              "(unsupported porosity evolution algorithm)");
        }
      }
      bd.setCode(uh, BehaviourData::Integrator, ib,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // implicit equation associated with the porosity
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        ib.code = "if(!this->";
        ib.code += StandardElastoViscoPlasticityBrick::
            computeStandardSystemOfImplicitEquations;
        ib.code += "){\n";
        ib.code += "f" + f.name + " -= " +
                   StandardElastoViscoPlasticityBrick::
                       currentEstimateOfThePorosityIncrement +
                   ";\n";
        ib.code += "}\n";
        bd.setCode(uh, BehaviourData::Integrator, ib,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
      // modification of the implicit equation to impose the bounds
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::
              STANDARD_IMPLICIT_SCHEME) {
        const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
        const auto requiresAnalyticalJacobian =
            ((idsl.getSolver().usesJacobian()) &&
             (!idsl.getSolver().requiresNumericalJacobian()));
        ib.code = "if(this->" + f.name + " + this->d" + f.name +  //
                  " - f" + f.name + " < 0){\n";
        ib.code += "f" + f.name + " = " +  //
                   "this->d" + f.name + " - this->" + f.name + ";\n";
        if (requiresAnalyticalJacobian) {
          ib.code +=
              "for(unsigned short idx = 0; idx!= "  //
              "this->jacobian.getNbCols(); ++idx){\n";
          ib.code += "this->jacobian(" + f.name + "_offset, idx) = 0;\n";
          ib.code += "}\n";
          ib.code += "this->jacobian(" + f.name + "_offset, " + f.name +
                     "_offset) = 1;\n";
        }
        ib.code += "}\n";
        ib.code += "if(this->" + f.name + " + this->d" + f.name +  //
                   " - f" + f.name + " > " + porosityUpperBound + "){\n";
        ib.code += "f" + f.name + " = this->d" + f.name +  //
                   " - (" + porosityUpperBound + " - this->" + f.name + ");\n";
        if (requiresAnalyticalJacobian) {
          ib.code +=
              "for(unsigned short idx = 0; idx!= "  //
              "this->jacobian.getNbCols(); ++idx){\n";
          ib.code += "this->jacobian(" + f.name + "_offset, idx) = 0;\n";
          ib.code += "}\n";
          ib.code += "this->jacobian(" + f.name + "_offset, " + f.name +
                     "_offset) = 1;\n";
        }
        ib.code += "}\n";
        bd.setCode(uh, BehaviourData::Integrator, ib,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
      }
      // additional convergence checks
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        const auto& broken =
            bd.getBehaviourData(uh)
                .getAuxiliaryStateVariableDescriptionByExternalName(
                    tfel::glossary::Glossary::Broken);
        // additional convergence checks
        CodeBlock acc;
        std::ostringstream os;
        // We check if we did not built the implicit system only to have the
        // jacobian of the full implicit system after the convergence of the
        // staggered scheme. If this is the case, the converged flag is set
        // to true and the the resolution is stopped
        os << "if ((";
        os << computeStandardSystemOfImplicitEquations;
        os << ") || (2 * (this->" + broken.name + ") > 1)) {\n";
        os << "converged = true;\n";
        os << "return;\n";
        os << "}\n";
        acc.code = os.str();
        bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }
    this->stress_potential->endTreatment(this->bd, this->dsl);
    auto i = size_t{};
    for (const auto& f : this->flows) {
      f->endTreatment(this->bd, this->dsl, *(this->stress_potential),
                      getId(i, this->flows.size()));
      ++i;
    }
    i = size_t{};
    for (const auto& nm : this->nucleation_models) {
      nm->endTreatment(this->bd, this->dsl, *(this->stress_potential),
                       this->buildInelasticFlowsMap(),
                       getId(i, this->nucleation_models.size()));
      ++i;
    }
    // At this stage, one expects to be able to compute the upper porosity bound
    if (this->isCoupledWithPorosityEvolution()) {
      CodeBlock init;
      init.code += "this->";
      init.code += porosityUpperBound;
      init.code += " = real{1};\n";
      auto flow_id = size_t{};
      for (const auto& pf : this->flows) {
        const auto id = getId(flow_id, this->flows.size());
        init.code += pf->updatePorosityUpperBound(bd, id);
        ++flow_id;
      }
      //       init.code += "if(2 * (this->" + broken.name + ") > 1){\n";
      //       init.code += "this->" + f.name + " = (this->" +
      //                    std::string(porosityUpperBoundSafetyFactor) + ") *
      //                    (this->" +
      //                    std::string(porosityUpperBound) + ");\n";
      //       init.code += "}\n";
      //       if (this->porosity_evolution_algorithm ==
      //           mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME)
      //           {
      //         init.code += porosityIncrementLowerBound + " = -this->f;\n";
      //         init.code += porosityIncrementUpperBound + " = ";
      //         init.code += porosityUpperBound + "- (this->f);\n";
      //       }
      bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
    // at this stage, one assumes that the various components of the inelastic
    // flow (stress_potential, isotropic hardening rule) have added the
    // initialization of their material properties the
    // `BeforeInitializeLocalVariables`. We then ask the inelastic flows if
    // they
    // require an  activation state (in practice, it mean that an isotropic
    // hardening rule has been defined). If so, the initialization of the
    // activation states requires the the computation of an elastic prediction
    // of the stress. The brik asks the stress potential to compute it in a
    // variable called sigel and the inelastic flows shall use it to compute
    // their initial state. All thoses steps must be added to the
    // `BeforeInitializeLocalVariables` code block.
    const bool bep = [this] {
      for (const auto& pf : this->flows) {
        if (pf->requiresActivationState()) {
          return true;
        }
      }
      return false;
    }();
    if (bep) {
      // compute the elastic prediction
      this->stress_potential->computeElasticPrediction(bd);
      i = size_t{};
      for (const auto& pf : this->flows) {
        if (pf->requiresActivationState()) {
          pf->computeInitialActivationState(bd, *(this->stress_potential),
                                            getId(i, this->flows.size()));
        }
        ++i;
      }
    }
    if ((this->isCoupledWithPorosityEvolution()) &&
        (this->porosity_evolution_algorithm ==
         mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME)) {
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto& broken =
          bd.getBehaviourData(uh)
              .getAuxiliaryStateVariableDescriptionByExternalName(
                  tfel::glossary::Glossary::Broken);
      const auto df =
          differenceBetweenSuccessiveEstimatesOfThePorosityIncrement;
      const auto f_ets = nextEstimateOfThePorosityAtTheEndOfTheTimeStep;
      // additional convergence checks
      CodeBlock acc;
      // defining the `nextEstimateOfThePorosityIncrement` variable before the
      // user code, so that the user can update the porosity with its own
      // contribution
      acc.code = "auto " + std::string(fixedPointConverged) + " = true;\n";
      acc.code += "auto " + std::string(nextEstimateOfThePorosityIncrement) +
                  " = real{};\n";
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      acc.code.clear();
      std::ostringstream os;
      auto generate_debug_message = [&os, &f, &df](const bool b) {
        if (!getDebugMode()) {
          return;
        }
        if (b) {
          os << "std::cout "
             << "<< \"convergence of the staggered algorithm, iteration \"";
        } else {
          os << "std::cout "
             << "<< \"non convergence of the staggered algorithm:, iteration "
                "\"";
        }
        os << " << this->" << staggeredSchemeIterationCounter << " << '\\n'\n"
           << "<< \"- current estimate of the porosity at the end of the time "
           << "step: \""
           << " << ((this->" << f.name << ")+(this->"
           << currentEstimateOfThePorosityIncrement << ")) << '\\n'\n"
           << "<< \"- current estimate of the porosity increment: \" << "
              "(this->"
           << currentEstimateOfThePorosityIncrement << ") << '\\n'\n"
           << "<< \"- next estimate of the porosity increment: \" << "
           << nextEstimateOfThePorosityIncrement << " << '\\n'\n"
           << "<< \"- difference: \" << "
           << "std::abs(" << df << ") << '\\n';\n";
      };
      // now we check that the staggered scheme has convergenced
      os << "if (converged && (!";
      os << computeStandardSystemOfImplicitEquations;
      os << ")) {\n";
      if (this->elastic_contribution) {
        const auto f_ = "((this->" + f.name + ") + (" +
                        this->bd.getClassName() + "::theta) * (this->" +
                        std::string(StandardElastoViscoPlasticityBrick::
                                        currentEstimateOfThePorosityIncrement) +
                        "))";
        os << nextEstimateOfThePorosityIncrement
           << " += (1-" + f_ + ") * trace(this->deel);\n";
      }
      // 1. compute a new estimate of the porosity increment and get the maximum
      // allowed porosity (onset of fracture)
      auto flow_id = size_t{};
      for (const auto& pf : this->flows) {
        const auto id = getId(flow_id, this->flows.size());
        os << pf->updateNextEstimateOfThePorosityIncrement(bd, id);
        ++flow_id;
      }
      auto nucleation_id = size_t{};
      for (const auto& nm : this->nucleation_models) {
        const auto id = getId(nucleation_id, this->nucleation_models.size());
        os << nm->updateNextEstimateOfThePorosityIncrement(
            bd, this->buildInelasticFlowsMap(), id);
        ++nucleation_id;
      }
      auto apply_next_estimate_bounds = [&os, f_ets] {
        // next estimate of the porosity at the end of the time step
        os << "{\n"
           << "const auto " << f_ets << " = this->f + "
           << nextEstimateOfThePorosityIncrement << ";\n";
        // Treat the case when the newly computed porosity exceeds the
        // porosity upper bounds.
        //
        // The maximum allowed increment is the current value of the upper of
        // the porosity minus the value at the beginning of the time step. We
        // then use a dichotomic approach by choosing an increment which is an
        // average of the previous increment and this maximum allowed increment
        os << "if(" << f_ets << " > (this->" << porosityUpperBoundSafetyFactor
           << " ) * ( this->" << porosityUpperBound << ")){\n"
           << nextEstimateOfThePorosityIncrement << " = "
           << "(this->" << currentEstimateOfThePorosityIncrement << " + (this->"
           << porosityUpperBound
           << " - this->f))/2;\n"
           // Treat the case when the newly computed porosity is negative
           //
           // The maximum allowed increment is the opposite of the initial
           // porosity. We then use a dichotomic approach by choosing an
           // increment
           // which is an average of the previous increment and this maximum
           // allowed increment
           << "} else if(" << f_ets << " < 0){\n"
           << nextEstimateOfThePorosityIncrement << " = "
           << "(" << currentEstimateOfThePorosityIncrement
           << " - (this->f))/2;\n"
           << "}\n"
           << "}\n";
      };
      // 2. apply the bounds on the next estimate
      apply_next_estimate_bounds();
      // 3. check if the staggered algorithm has converged
      os << "const auto " << df << " = " << nextEstimateOfThePorosityIncrement
         << " - (this->" << currentEstimateOfThePorosityIncrement << ");\n";
      os << std::string(fixedPointConverged) << " =  ("
         << std::string(fixedPointConverged) << ") && "
         << "(std::abs(" << df << ") < this->"
         << staggeredSchemeConvergenceCriterion << ");\n";
      os << "if(" + std::string(fixedPointConverged) + "){\n";
      generate_debug_message(true);
      // try to dectect failure
      os << "if (this->f + this->" << currentEstimateOfThePorosityIncrement
         << " > (this->" << porosityUpperBoundSafetyFactorForFractureDetection
         << ") * (this->" << porosityUpperBound << ")){\n"
         << "this->" << broken.name << "= 1;\n"
         << "}\n"
         // makes one more iteration to compute the exact consistent tangent
         // operator if required
         << "converged = smt != CONSISTENTTANGENTOPERATOR;\n"
         // the staggered scheme has converged
         << computeStandardSystemOfImplicitEquations << " = true;\n"
         << "} else {\n";
      generate_debug_message(false);
      // the staggered scheme has not converged
      os << "if(this->" << staggeredSchemeIterationCounter << " > this->"
         << maximumNumberOfIterationsOfTheStaggeredScheme << "){\n"
         << "tfel::raise<DivergenceException>(\""
         << "maximum number of iterations of "
         << "the staggered algorithm reached\");\n"
         << "}\n";
      // update the current estimate
      os << staggeredSchemeBissectionAlgorithm << ".updateBounds(this->"
         << currentEstimateOfThePorosityIncrement << ","
         << "difference_between_successive_estimates_of_the_porosity_increment"
         << ");\n";
      if (this->staggered_scheme_parameters.acceleration_algorithm ==
          StaggeredSchemeParameters::RELAXATION) {
        os << staggeredSchemeBissectionAlgorithm << ".iterate("
           << nextEstimateOfThePorosityIncrement << ");\n";
        os << "this->" << currentEstimateOfThePorosityIncrement << " = "
           << "(this->" << currentEstimateOfThePorosityIncrement << " + "
           << nextEstimateOfThePorosityIncrement << ")/2;\n";
      } else if (this->staggered_scheme_parameters.acceleration_algorithm ==
                 StaggeredSchemeParameters::AITKEN) {
        os << staggeredSchemeAitkenAccelerationAlgorithm << ".accelerate("
           << nextEstimateOfThePorosityIncrement << ");\n";
        os << staggeredSchemeBissectionAlgorithm << ".iterate("
           << nextEstimateOfThePorosityIncrement << ");\n";
        apply_next_estimate_bounds();
        os << "this->" << currentEstimateOfThePorosityIncrement << " = "
           << nextEstimateOfThePorosityIncrement << ";\n";
      }
      // update the staggered iteration number
      os << "++(this->" << staggeredSchemeIterationCounter << ");\n"
         << "this->iter = 0;\n"
         << "converged = false;\n"
         << "}\n"
         << "} // end of if(converged&&...)\n";
      acc.code = os.str();
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
    //
    if ((this->isCoupledWithPorosityEvolution()) &&
        (this->porosity_evolution_algorithm ==
         mfront::bbrick::PorosityEvolutionAlgorithm::
             STANDARD_IMPLICIT_SCHEME)) {
      const auto& broken =
          bd.getBehaviourData(uh)
              .getAuxiliaryStateVariableDescriptionByExternalName(
                  tfel::glossary::Glossary::Broken);
      auto uasv = CodeBlock{};
      const auto fmax =
          "(this->" +
          std::string(porosityUpperBoundSafetyFactorForFractureDetection) +
          ") * " + std::string(porosityUpperBound);
      uasv.code += "if(this->f > " + fmax + "){\n";
      uasv.code += "this->" + broken.name + " = true;\n";
      uasv.code += "}\n";
      bd.setCode(uh, BehaviourData::UpdateAuxiliaryStateVariables, uasv,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
  }  // end of StandardElastoViscoPlasticityBrick::endTreatment

  void StandardElastoViscoPlasticityBrick::
      addElasticContributionToTheImplicitEquationAssociatedWithPorosityEvolution(
          CodeBlock& ib) const {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& f =
        bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
            tfel::glossary::Glossary::Porosity);
    const auto f_ = f.name + "_";
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    const auto requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    const auto& broken =
        bd.getBehaviourData(uh)
            .getAuxiliaryStateVariableDescriptionByExternalName(
                tfel::glossary::Glossary::Broken);
    ib.code += "if(2 * (this->" + broken.name + ") < 1){\n";
    ib.code += "f" + f.name + " -= (1 - " + f_ + ") * trace(this->deel);\n";
    if (requiresAnalyticalJacobian) {
      ib.code += "df" + f.name + "_ddeel -= " +  //
                 "(1 - " + f_ + ") * Stensor::Id();\n";
      ib.code += "df" + f.name + "_dd" + f.name + " += " +  //
                 "(" + this->bd.getClassName() +
                 "::theta) * trace(this->deel);\n";
    }
    ib.code += "}\n";
  }  // end of
  // addElasticContributionToTheImplicitEquationAssociatedWithPorosityEvolution

  StandardElastoViscoPlasticityBrick::~StandardElastoViscoPlasticityBrick() =
      default;

}  // end of namespace mfront
