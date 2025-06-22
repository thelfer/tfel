/*!
 * \file   mfront/src/__StressCriterionName__StressCriterion.cxx
 * \brief
 * \author __Author__
 * \date   __Date__
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "TFEL/Raise.hxx"

#ifdef MFRONT_ADITIONNAL_LIBRARY
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#endif /* MFRONT_ADITIONNAL_LIBRARY */

#include "MFront/BehaviourBrick/__StressCriterionName__StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    __StressCriterionName__StressCriterion::
        __StressCriterionName__StressCriterion()
        : StandardPorousStressCriterionBase("__StressCriterionName__") {
    }  // end of
       // __StressCriterionName__StressCriterion::__StressCriterionName__StressCriterion

    std::vector<mfront::BehaviourSymmetryType>
    __StressCriterionName__StressCriterion::getSupportedBehaviourSymmetries()
        const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of
       // __StressCriterionName__StressCriterion::getSupportedBehaviourSymmetries()

    std::vector<OptionDescription>
    __StressCriterionName__StressCriterion::getOptions() const {
      auto opts = StressCriterionBase::getOptions();
      return opts;
    }  // end of __StressCriterionName__StressCriterion::getOptions()

    StressCriterion::PorosityEffectOnFlowRule
    __StressCriterionName__StressCriterion::
        getPorosityEffectOnEquivalentPlasticStrain() const {
      return StressCriterion::
          STANDARD_POROSITY_CORRECTION_ON_EQUIVALENT_PLASTIC_STRAIN;
    }  // end of
       // __StressCriterionName__StressCriterion::getPorosityEffectOnEquivalentPlasticStrain

    std::string
    __StressCriterionName__StressCriterion::updatePorosityUpperBound(
        const BehaviourDescription &, const std::string &, const Role r) const {
      if (!((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION))) {
        tfel::raise(
            "StressCriterionBase::updatePorosityUpperBound: invalid call for "
            "this stress criterion. This method is not valid for flow "
            "criteria (i.e. when the stress criterion is used to dertermine "
            "the flow direction in non associated plasticity).");
      }
      return std::string{};
    }  // end of
       // __StressCriterionName__StressCriterion::updatePorosityUpperBound

    __StressCriterionName__StressCriterion::
        ~__StressCriterionName__StressCriterion() = default;

#ifdef MFRONT_ADITIONNAL_LIBRARY

    struct __StressCriterionName__StressCriterionProxy {
      __StressCriterionName__StressCriterionProxy() {
        auto &f = StressCriterionFactory::getFactory();
        f.addGenerator("__StressCriterionName__", [] {
          return std::make_shared<__StressCriterionName__StressCriterion>();
        });
      }  // end of __StressCriterionName__StressCriterionProxy
    };   // end of struct __StressCriterionName__StressCriterionProxy

    static __StressCriterionName__StressCriterionProxy proxy;

#endif /* MFRONT_ADITIONNAL_LIBRARY */

  }  // end of namespace bbrick

}  // end of namespace mfront
