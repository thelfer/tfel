/*!
 * \file   StandardElastoViscoPlasticityBrick.cxx
 * \brief
 * \author Thomas Helfer
 * \date   17/03/2018
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include "MFront/BehaviourBrick/InelasticFlow.hxx"
#include "MFront/BehaviourBrick/InelasticFlowFactory.hxx"
#include "MFront/AbstractBehaviourDSL.hxx"
#include "MFront/StandardElastoViscoPlasticityBrick.hxx"

namespace mfront {

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
    }; // end of raise
    auto& spf = bbrick::StressPotentialFactory::getFactory();
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
    }; // end of getDataStructure
    for (const auto& e : d) {
      if ((e.first == "elastic_potential") || (e.first == "stress_potential")) {
        const auto ds = getDataStructure(e.first, e.second);
        if (this->stress_potential != nullptr) {
          raise("the stress potential has already been defined");
        }
        this->stress_potential = spf.generate(ds.name);
        this->stress_potential->initialize(this->dsl, this->bd, ds.data);
      } else if (e.first == "inelastic_flow") {
        auto append_flow = [this, &iff, getDataStructure](const Data& ifd) {
          const auto ds = getDataStructure("inelatic_flow", ifd);
          auto iflow = iff.generate(ds.name);
          iflow->initialize(this->dsl, this->bd, ds.data);
          this->flows.push_back(iflow);
        };
        if (e.second.is<std::vector<Data>>()) {
          // multiple inelastic flows are defined
          const auto& ifs = e.second.get<std::vector<Data>>();
          for (const auto& iflow : ifs) {
            append_flow(iflow);
          }
        } else {
          append_flow(e.second);
        }
      } else {
        raise("unsupported entry '" + e.first + "'");
      }
    }
    if(this->stress_potential == nullptr){
      raise("no stress potential defined");
    }
  }  // end of StandardElastoViscoPlasticityBrick

  std::string StandardElastoViscoPlasticityBrick::getName() const {
    return "ElastoViscoPlasticity";
  }

  std::vector<StandardElastoViscoPlasticityBrick::Hypothesis>
  StandardElastoViscoPlasticityBrick::getSupportedModellingHypotheses() const {
    return this->stress_potential->getSupportedModellingHypotheses(this->dsl,
                                                                   this->bd);
  }

  void StandardElastoViscoPlasticityBrick::completeVariableDeclaration() const {
    this->stress_potential->completeVariableDeclaration(this->dsl, this->bd);
  }  // end of StandardElastoViscoPlasticityBrick::completeVariableDeclaration

  void StandardElastoViscoPlasticityBrick::endTreatment() const {
    this->stress_potential->endTreatment(this->dsl, this->bd);
  }  // end of StandardElastoViscoPlasticityBrick::endTreatment

  StandardElastoViscoPlasticityBrick::~StandardElastoViscoPlasticityBrick() =
      default;
}
