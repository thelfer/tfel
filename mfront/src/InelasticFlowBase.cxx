/*!
 * \file   mfront/src/InelasticFlowBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "TFEL/Utilities/Data.hxx"
#include "MFront/BehaviourBrick/StressCriterion.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    void InelasticFlowBase::initialize(BehaviourDescription& bd,
                                       AbstractBehaviourDSL& dsl,
                                       const std::string&,
                                       const DataMap& d) {
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
      mfront::bbrick::check(d, this->getOptions());
      // parsing options
      for (const auto& e : d) {
        if (e.first == "criterion") {
          const auto ds = getDataStructure(e.first, e.second);
          if (this->sc != nullptr) {
            raise("criterion has already been defined");
          }
          auto& cf = StressCriterionFactory::getFactory();
          this->sc = cf.generate(ds.name);
          this->sc->initialize(bd, dsl, ds.data);
        }
      }
      if (this->sc == nullptr) {
        raise("criterion has not been defined");
      }
    }  // end of InelasticFlowBase::initialize

    std::vector<OptionDescription> InelasticFlowBase::getOptions() const {
      std::vector<OptionDescription> opts;
      opts.emplace_back(
          "criterion",
          "stress criterion to be used (Mises, Hill, Hosford, Barlat)",
          OptionDescription::DATASTRUCTURE);
      return opts;
    }  // end of InelasticFlowBase::getOptions()

    //     void InelasticFlowBase::setIsotropicHardeningRule(
    //         const std::shared_ptr<IsotropicHardeningRule>& r) {
    //       tfel::raise_if(this->ihr != nullptr,
    //                      "InelasticFlowBase::setIsotropicHardeningRule: "
    //                      "an isotropic hardening rule has already been
    //                      defined");
    //       this->ihr = r;
    //     }  // end of InelasticFlowBase::setIsotropicHardeningRule
    //
    //     void InelasticFlowBase::addKinematicHardeningRule(
    //         const std::shared_ptr<KinematicHardeningRule>& r) {
    //       this->khrs.push_back(r);
    //     }  // end of InelasticFlowBase::addKinematicHardeningRule

    InelasticFlowBase::~InelasticFlowBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront