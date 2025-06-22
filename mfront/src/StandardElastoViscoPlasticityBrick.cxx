/*!
 * \file   StandardElastoViscoPlasticityBrick.cxx
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
#include "TFEL/Utilities/Data.hxx"
#include "MFront/ImplicitDSLBase.hxx"
#include "MFront/NonLinearSystemSolver.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/BehaviourBrick/InelasticFlow.hxx"
#include "MFront/BehaviourBrick/InelasticFlowFactory.hxx"
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
      AbstractBehaviourDSL& dsl_,
      BehaviourDescription& mb_,
      const Parameters&,
      const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto raise = [](const std::string& m) {
      tfel::raise(
          "StandardElastoViscoPlasticityBrick::"
          "StandardElastoViscoPlasticityBrick: " +
          m);
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
    getStressPotential("elastic_potential");
    getStressPotential("stress_potential");
    if (this->stress_potential == nullptr) {
      raise("no stress potential defined");
    }
    for (const auto& e : d) {
      if ((e.first == "elastic_potential") || (e.first == "stress_potential")) {
        // already treated
      } else if (e.first == "inelastic_flow") {
        auto append_flow = [this, &iff, getDataStructure](const Data& ifd,
                                                          const size_t msize) {
          const auto ds = getDataStructure("inelatic_flow", ifd);
          auto iflow = iff.generate(ds.name);
          iflow->initialize(this->bd, this->dsl,
                            getId(this->flows.size(), msize), ds.data);
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
      } else {
        raise("unsupported entry '" + e.first + "'");
      }
    }
  }  // end of StandardElastoViscoPlasticityBrick

  std::string StandardElastoViscoPlasticityBrick::getName() const {
    return "ElastoViscoPlasticity";
  }

  std::vector<StandardElastoViscoPlasticityBrick::Hypothesis>
  StandardElastoViscoPlasticityBrick::getSupportedModellingHypotheses() const {
    return this->stress_potential->getSupportedModellingHypotheses(this->bd,
                                                                   this->dsl);
  }

  void StandardElastoViscoPlasticityBrick::completeVariableDeclaration() const {
    this->stress_potential->completeVariableDeclaration(this->bd, this->dsl);
    auto i = size_t{};
    for (const auto& f : this->flows) {
      f->completeVariableDeclaration(this->bd, this->dsl,
                                     getId(i, this->flows.size()));
      ++i;
    }
  }  // end of StandardElastoViscoPlasticityBrick::completeVariableDeclaration

  void StandardElastoViscoPlasticityBrick::endTreatment() const {
    const auto& idsl = dynamic_cast<const ImplicitDSLBase&>(this->dsl);
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
