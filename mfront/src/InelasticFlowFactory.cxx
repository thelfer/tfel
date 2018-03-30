/*!
 * \file   mfront/src/InelasticFlowFactory.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/NortonInelasticFlow.hxx"
#include "MFront/BehaviourBrick/InelasticFlowFactory.hxx"

namespace mfront {

  namespace bbrick {

    InelasticFlowFactory& InelasticFlowFactory::getFactory() {
      static InelasticFlowFactory i;
      return i;
    }  // end of InelasticFlowFactory::getFactory

    void InelasticFlowFactory::addGenerator(const std::string& n,
                                            const Generator& g) {
      if (!this->generators.insert({n, g}).second) {
        tfel::raise(
            "InelasticFlowFactory::addGenerator: "
            "generator '" +
            n + "' already registred");
      }
    }  // end of InelasticFlowFactory::addGenerator

    std::shared_ptr<InelasticFlow> InelasticFlowFactory::generate(
        const std::string& n) const {
      const auto p = this->generators.find(n);
      if (p == this->generators.end()) {
        tfel::raise(
            "InelasticFlowFactory::generate: "
            "no generator named '" +
            n + "'");
      }
      return p->second();
    }  // end of InelasticFlowFactory::generate

    InelasticFlowFactory::InelasticFlowFactory() {
      this->addGenerator(
          "Norton", []() { return std::make_shared<NortonInelasticFlow>(); });
    }  // end of InelasticFlowFactory::InelasticFlowFactory

    InelasticFlowFactory::~InelasticFlowFactory() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
