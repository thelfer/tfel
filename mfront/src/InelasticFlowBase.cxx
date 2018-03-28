/*!
 * \file   mfront/src/InelasticFlowBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/InelasticFlowBase.hxx"

namespace mfront {

  namespace bbrick {

    void InelasticFlowBase::setStressCriterion(
        const std::shared_ptr<StressCriterion>& c) {
      tfel::raise_if(this->sc != nullptr,
                     "InelasticFlowBase::setStressCriterion: "
                     "a stress criterion has already been defined");
      this->sc = c;
    }  // end of InelasticFlowBase::setStressCriterion

    void InelasticFlowBase::setIsotropicHardeningRule(
        const std::shared_ptr<IsotropicHardeningRule>& r) {
      tfel::raise_if(this->ihr != nullptr,
                     "InelasticFlowBase::setIsotropicHardeningRule: "
                     "an isotropic hardening rule has already been defined");
      this->ihr = r;
    }  // end of InelasticFlowBase::setIsotropicHardeningRule

    void InelasticFlowBase::addKinematicHardeningRule(
        const std::shared_ptr<KinematicHardeningRule>& r) {
      this->khrs.push_back(r);
    }  // end of InelasticFlowBase::addKinematicHardeningRule

    InelasticFlowBase::~InelasticFlowBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront