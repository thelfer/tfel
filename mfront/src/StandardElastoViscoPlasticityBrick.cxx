/*!
 * \file   StandardElastoViscoPlasticityBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/03/2018
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

  const char* const StandardElastoViscoPlasticityBrick::
      computeStandardSystemOfImplicitEquations =
          "compute_standard_system_of_implicit_equations";
  const char* const StandardElastoViscoPlasticityBrick::
      currentEstimateOfThePorosityIncrement =
          "current_estimate_of_the_porosity_increment";
  const char* const
      StandardElastoViscoPlasticityBrick::nextEstimateOfThePorosityIncrement =
          "next_estimate_of_the_porosity_increment";
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
        raise("'save_individual_porosity_increase' is not a boolean value");
      }
      save_individual_porosity_increase = b.get<bool>();
    }
    for (const auto& ped : ed) {
      if (ped.first == "save_individual_porosity_increase") {
        // already treated
      } else if (ped.first == "nucleation_model") {
        auto append_nucleation_model = [this, &nmf, getDataStructure, raise,
                                        &save_individual_porosity_increase](
            const Data& nmd, const size_t msize) {
          const auto ds = getDataStructure("nucleation_model", nmd);
          auto nm = nmf.generate(ds.name);
          auto data = ds.data;
          if (data.count("save_individual_porosity_increase") == 0) {
            data["save_individual_porosity_increase"] =
                save_individual_porosity_increase;
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
  }  // end of StandardElastoViscoPlasticityBrick::treatPorosityEvolutionSection

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
          const std::map<std::string, Data>& algorithm_parameters) {
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
        this->staggered_algorithm_parameters.convergence_criterion = c;
      } else if (p.first == "maximum_number_of_iterations") {
        if (!p.second.is<int>()) {
          raise(
              "invalid type for the `maximum_number_of_iterations` parameter");
        }
        const auto& i = p.second.get<int>();
        if (i < 1) {
          raise(
              "invalid value of the `maximum_number_of_iterations` parameter");
        }
        this->staggered_algorithm_parameters.maximum_number_of_iterations =
            static_cast<unsigned int>(i);
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
    // `porosity_growth_policy` is not equal to `UNDEFINEDPOROSITYGROWTHPOLICY`,
    // the user has defined an `porosity_evolution` section, which means that
    // he wants that any non purely deviatoric inelastic flow contributes to the
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
      mfront::bbrick::addStateVariableIfNotDefined(
          bd, "real", "f", tfel::glossary::Glossary::Porosity, 1u, true);
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      this->bd.reserveName(uh, f.name + "_");
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        mfront::bbrick::addLocalVariable(
            bd, "bool", StandardElastoViscoPlasticityBrick::
                            computeStandardSystemOfImplicitEquations);
        mfront::bbrick::addLocalVariable(
            bd, "real", StandardElastoViscoPlasticityBrick::
                            currentEstimateOfThePorosityIncrement);
        mfront::bbrick::addLocalVariable(bd, "ushort",
                                         StandardElastoViscoPlasticityBrick::
                                             staggeredSchemeIterationCounter);
        // convergence criterion of the staggered scheme
        VariableDescription eps("real", StandardElastoViscoPlasticityBrick::
                                            staggeredSchemeConvergenceCriterion,
                                1u, 0u);
        eps.description = "stopping criterion value of the staggered scheme";
        bd.addParameter(uh, eps, BehaviourData::UNREGISTRED);
        bd.setParameterDefaultValue(
            uh, StandardElastoViscoPlasticityBrick::
                    staggeredSchemeConvergenceCriterion,
            this->staggered_algorithm_parameters.convergence_criterion);
        // maximum number of iterations of the staggered scheme
        VariableDescription iterMax(
            "ushort", StandardElastoViscoPlasticityBrick::
                          maximumNumberOfIterationsOfTheStaggeredScheme,
            1u, 0u);
        iterMax.description =
            "maximum number of iterations of the staggered scheme allowed";
        bd.addParameter(uh, iterMax, BehaviourData::UNREGISTRED);
        bd.setParameterDefaultValue(
            uh, StandardElastoViscoPlasticityBrick::
                    maximumNumberOfIterationsOfTheStaggeredScheme,
            static_cast<unsigned short>(this->staggered_algorithm_parameters
                                            .maximum_number_of_iterations));
      }
    }
  }  // end of StandardElastoViscoPlasticityBrick::completeVariableDeclaration

  void StandardElastoViscoPlasticityBrick::endTreatment() const {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    if (this->isCoupledWithPorosityEvolution()) {
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto f_ = f.name + "_";
      if (this->porosity_evolution_algorithm ==
          mfront::bbrick::PorosityEvolutionAlgorithm::STAGGERED_SCHEME) {
        CodeBlock init;
        // initialize the porosity status
        init.code += "this->";
        init.code += StandardElastoViscoPlasticityBrick::
            computeStandardSystemOfImplicitEquations;
        init.code += " = false;\n";
        // initialize the porosity increment
        init.code += StandardElastoViscoPlasticityBrick::
            currentEstimateOfThePorosityIncrement;
        init.code += " = real{};\n";
        // initialize the iteration counter of the staggered scheme
        init.code +=
            StandardElastoViscoPlasticityBrick::staggeredSchemeIterationCounter;
        init.code += " = static_cast<unsigned short>(0);\n";
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, init,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
      // value of the porosity at t+theta*dt
      CodeBlock ib;
      ib.code += "const auto " + f_ + " = ";
      ib.code += "std::max(std::min(this->" + f.name + " + theta * (this->d" +
                 f.name + "), real(1)), real(0));\n ";
      bd.setCode(uh, BehaviourData::Integrator, ib,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      ib.code = "if(!";
      ib.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      ib.code += "){\n";
      ib.code = "f" + f.name + " -= " +
                StandardElastoViscoPlasticityBrick::
                    currentEstimateOfThePorosityIncrement +
                ";\n";
      ib.code += "}\n";
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
      const auto df = std::string(
          StandardElastoViscoPlasticityBrick::
              differenceBetweenSuccessiveEstimatesOfThePorosityIncrement);
      // additional convergence checks
      CodeBlock acc;
      auto generate_debug_message = [&acc, &f, &df](const bool b) {
        if (!getDebugMode()) {
          return;
        }
        if (b) {
          acc.code +=
              "std::cout "  //
              "<< \"convergence of the staggered algorithm:\\n\"";
        } else {
          acc.code +=
              "std::cout "  //
              "<< \"non convergence of the staggered algorithm:\\n\"";
        }
        acc.code +=
            "<< \"- current estimate of the porosity at the end of the time "
            "step: \"";
        acc.code += " << ((this->" + f.name + ")+(this->";
        acc.code += StandardElastoViscoPlasticityBrick::
            currentEstimateOfThePorosityIncrement;
        acc.code += ")) << '\\n'";
        acc.code +=
            "<< \"- current estimate of the porosity increment: \" << "
            "(this->";
        acc.code += StandardElastoViscoPlasticityBrick::
            currentEstimateOfThePorosityIncrement;
        acc.code += ") << '\\n'";
        acc.code += "<< \"- next estimate of the porosity increment: \" << ";
        acc.code += StandardElastoViscoPlasticityBrick::
            nextEstimateOfThePorosityIncrement;
        acc.code += " << '\\n'";
        acc.code += "<< \"- difference: \" << ";
        acc.code += "std::abs(" + df + ") << '\\n';\n";
      };
      // We check if we did not built the implicit system only to have the
      // jacobian of the full implicit system after the convergence of the
      // staggered scheme. If this is the case, the converged flag is set
      // to true and the the resolution is stopped
      acc.code += "if (";
      acc.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      acc.code += ") {\n";
      acc.code += "converged = true;\n";
      acc.code += "return;\n";
      acc.code += "}\n";
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      // now we check that the staggered scheme has convergenced
      acc.code = "if (converged && (!";
      acc.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      acc.code += ")) {\n";
      // 1. compute a new estimate of the porosity increment
      acc.code += "auto ";
      acc.code += StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      acc.code += " = real{};\n";
      auto flow_id = size_t{};
      for (const auto& pf : this->flows) {
        acc.code += pf->updateNextEstimateOfThePorosityIncrement(
            bd, getId(flow_id, this->flows.size()));
        ++flow_id;
      }
      //       auto nucleation_id = size_t{};
      //       for (const auto& nm : this->nucleation_models) {
      //         nm->updateNextEstimateOfThePorosityIncrement(
      //             getId(nucleation_id, this->nucleation_models.size()));
      //         ++nucleation_id;
      //       }
      // 2. get the maximum allowed porosity
      // 3. check if the staggered algorithm has converged
      acc.code += "const auto " + df + " = ";
      acc.code += StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      acc.code += " - (this->";
      acc.code += StandardElastoViscoPlasticityBrick::
          currentEstimateOfThePorosityIncrement;
      acc.code += ");\n";
      acc.code += "if(std::abs(" + df + ") < this->";
      acc.code += StandardElastoViscoPlasticityBrick::
          staggeredSchemeConvergenceCriterion;
      acc.code += "){\n";
      generate_debug_message(true);
      // makes one more iteration to compute the exact consistent tangent
      // operator if required
      acc.code += "converged = smt != CONSISTENTTANGENTOPERATOR;\n";
      // the staggered scheme has converged
      acc.code += StandardElastoViscoPlasticityBrick::
          computeStandardSystemOfImplicitEquations;
      acc.code += " = true;\n";
      acc.code += "} else {\n";
      generate_debug_message(false);
      // the staggered scheme has not converged
      acc.code += "if(";
      acc.code +=
          StandardElastoViscoPlasticityBrick::staggeredSchemeIterationCounter;
      acc.code += " > this->";
      acc.code += StandardElastoViscoPlasticityBrick::
          maximumNumberOfIterationsOfTheStaggeredScheme;
      acc.code += "){";
      acc.code += "tfel::raise<DivergenceException>(\"";
      acc.code += "maximum number of iterations of ";
      acc.code += "the staggered algorithm reached\");\n";
      acc.code += "}\n";
      // update the current estimate
      acc.code += "this->";
      acc.code += StandardElastoViscoPlasticityBrick::
          currentEstimateOfThePorosityIncrement;
      acc.code += " = ";
      acc.code += StandardElastoViscoPlasticityBrick::
          nextEstimateOfThePorosityIncrement;
      acc.code += ";\n";
      // update the staggered iteration number
      acc.code += "++";
      acc.code +=
          StandardElastoViscoPlasticityBrick::staggeredSchemeIterationCounter;
      acc.code += ";\n";
      acc.code += "this->iter = 0;\n";
      acc.code += "converged = false;\n";
      acc.code += "}\n";
      acc.code += "} // end of if(converged&&...)\n";
      bd.setCode(uh, BehaviourData::AdditionalConvergenceChecks, acc,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_END);
    }
  }  // end of StandardElastoViscoPlasticityBrick::endTreatment

  StandardElastoViscoPlasticityBrick::~StandardElastoViscoPlasticityBrick() =
      default;

}  // end of namespace mfront
