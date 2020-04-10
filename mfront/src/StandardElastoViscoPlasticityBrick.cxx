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
      tfel::raise(
          "StandardElastoViscoPlasticityBrick::"
          "StandardElastoViscoPlasticityBrick: " +
          m);
    };  // end of raise
    auto& iff = bbrick::InelasticFlowFactory::getFactory();
    auto& nmf = bbrick::PorosityNucleationModelFactory::getFactory();
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
      const auto& e = d.at("porosity_evolution");
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
        } else {
          raise("invalid entry '" + ped.first + "' in 'porosity_evolution'");
        }
      }
      if (this->porosity_growth_policy == UNDEFINEDPOROSITYGROWTHPOLICY) {
        this->porosity_growth_policy = STANDARDVISCOPLASTICPOROSITYGROWTHPOLICY;
      }
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
    }
  }  // end of StandardElastoViscoPlasticityBrick::completeVariableDeclaration

  void StandardElastoViscoPlasticityBrick::endTreatment() const {
    constexpr const auto uh =
        tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
    // value of the porosity at t+theta*dt
    if (this->isCoupledWithPorosityEvolution()) {
      const auto& f =
          bd.getBehaviourData(uh).getStateVariableDescriptionByExternalName(
              tfel::glossary::Glossary::Porosity);
      const auto f_ = f.name + "_";
      CodeBlock ib;
      ib.code += "const auto " + f_ + " = ";
      ib.code += "max(min(this->" + f.name + " + theta * (this->d" + f.name +
                 "), real(1)), real(0));\n ";
      bd.setCode(uh, BehaviourData::Integrator, ib,
                 BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
    }
    this->stress_potential->endTreatment(this->bd, this->dsl);
    const bool requiresAnalyticalJacobian =
        ((idsl.getSolver().usesJacobian()) &&
         (!idsl.getSolver().requiresNumericalJacobian()));
    if (requiresAnalyticalJacobian) {
      this->stress_potential->writeStressDerivatives(this->bd);
    }
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
  }  // end of StandardElastoViscoPlasticityBrick::endTreatment

  StandardElastoViscoPlasticityBrick::~StandardElastoViscoPlasticityBrick() =
      default;

}  // end of namespace mfront
