/*!
 * \file   StandardElastoViscoPlasticityBrick.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   17/03/2018
 */

#include"TFEL/Raise.hxx"
#include"MFront/BehaviourBrick/StressPotentialFactory.hxx"
#include"MFront/AbstractBehaviourDSL.hxx"
#include"MFront/StandardElastoViscoPlasticityBrick.hxx"

namespace mfront{

  StandardElastoViscoPlasticityBrick::StandardElastoViscoPlasticityBrick(
      AbstractBehaviourDSL& dsl_,
      BehaviourDescription& mb_,
      const Parameters&,
      const DataMap& d)
      : BehaviourBrickBase(dsl_, mb_) {
    auto& spf = bbrick::StressPotentialFactory::getFactory();
    auto getDataStructure = [](const std::string& n, const Data& ds) {
      if (ds.is<std::string>()) {
        tfel::utilities::DataStructure nds;
        nds.name = ds.get<std::string>();
        return nds;
      }
      if (!ds.is<tfel::utilities::DataStructure>()) {
        tfel::raise(
            "StandardElastoViscoPlasticityBrick::"
            "StandardElastoViscoPlasticityBrick: "
            "invalid data type for entry '" +
            n + "'");
      }
      return ds.get<tfel::utilities::DataStructure>();
    };
    for (const auto& e : d) {
      if ((e.first == "elastic_potential") || (e.first == "stress_potential")) {
        const auto ds = getDataStructure(e.first,e.second);
        this->stress_potential =
            spf.generate(ds.name, this->dsl, this->bd, ds.data);
      }
    }
  } // end of StandardElastoViscoPlasticityBrick

  std::string StandardElastoViscoPlasticityBrick::getName() const {
    return "ElastoViscoPlasticity";
  }

  std::vector<StandardElastoViscoPlasticityBrick::Hypothesis>
  StandardElastoViscoPlasticityBrick::getSupportedModellingHypotheses() const {
    auto dmh = this->dsl.getDefaultModellingHypotheses();
    std::vector<Hypothesis> mh(dmh.begin(), dmh.end());
    mh.push_back(ModellingHypothesis::PLANESTRESS);
    mh.push_back(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS);
    return mh;
  }

  void StandardElastoViscoPlasticityBrick::completeVariableDeclaration() const {}

  void StandardElastoViscoPlasticityBrick::endTreatment() const {}

  StandardElastoViscoPlasticityBrick::~StandardElastoViscoPlasticityBrick() = default;
  
}
