/*!
 * \file   mfront/src/InelasticPotentialBase.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/InelasticPotentialBase.hxx"

namespace mfront {

  namespace bbrick {

    void InelasticPotentialBase::setStressCriterion(
        const std::shared_ptr<StressCriterion>& c) {
      tfel::raise_if(this->sc != nullptr,
                     "InelasticPotentialBase::setStressCriterion: "
                     "a stress criterion has already been defined");
      this->sc = c;
    }  // end of InelasticPotentialBase::setStressCriterion

    void InelasticPotentialBase::setIsotropicHardeningRule(
        const std::shared_ptr<IsotropicHardeningRule>& r) {
      tfel::raise_if(this->ihr != nullptr,
                     "InelasticPotentialBase::setIsotropicHardeningRule: "
                     "an isotropic hardening rule has already been defined");
      this->ihr = r;
    }  // end of InelasticPotentialBase::setIsotropicHardeningRule

    void InelasticPotentialBase::addKinematicHardeningRule(
        const std::shared_ptr<KinematicHardeningRule>& r) {
      this->khrs.push_back(r);
    }  // end of InelasticPotentialBase::addKinematicHardeningRule

    InelasticPotentialBase::~InelasticPotentialBase() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront