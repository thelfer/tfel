/*!
 * \file   mfront/src/MichelAndSuquet1992HollowSphereStressCriterion.cxx
 * \brief
 * \author Thomas Helfer, Jérémy Hure, Mohamed Shokeir
 * \date   20/07/2020
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "TFEL/Raise.hxx"

#ifdef MFRONT_ADITIONNAL_LIBRARY
#include "MFront/BehaviourBrick/StressCriterionFactory.hxx"
#endif /* MFRONT_ADITIONNAL_LIBRARY */

#include "MFront/BehaviourBrick/MichelAndSuquet1992HollowSphereStressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    MichelAndSuquet1992HollowSphereStressCriterion::MichelAndSuquet1992HollowSphereStressCriterion()
        : StandardPorousStressCriterionBase("MichelAndSuquet1992HollowSphere") {}  // end of
    // MichelAndSuquet1992HollowSphereStressCriterion::MichelAndSuquet1992HollowSphereStressCriterion

    std::vector<mfront::BehaviourSymmetryType>
    MichelAndSuquet1992HollowSphereStressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of
    // MichelAndSuquet1992HollowSphereStressCriterion::getSupportedBehaviourSymmetries()

    std::vector<OptionDescription> MichelAndSuquet1992HollowSphereStressCriterion::getOptions()
        const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("n", "Norton exponent of the matrix",
                        OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of MichelAndSuquet1992HollowSphereStressCriterion::getOptions()

    StressCriterion::PorosityEffectOnFlowRule
    MichelAndSuquet1992HollowSphereStressCriterion::getPorosityEffectOnFlowRule() const {
      return StressCriterion::NO_POROSITY_EFFECT_ON_FLOW_RULE;
    }  // end of MichelAndSuquet1992HollowSphereStressCriterion::getPorosityEffectOnFlowRule()

    MichelAndSuquet1992HollowSphereStressCriterion::
        ~MichelAndSuquet1992HollowSphereStressCriterion() = default;

#ifdef MFRONT_ADITIONNAL_LIBRARY

    struct MichelAndSuquet1992HollowSphereStressCriterionProxy {
      MichelAndSuquet1992HollowSphereStressCriterionProxy() {
        auto &f = StressCriterionFactory::getFactory();
        f.addGenerator("MichelAndSuquet1992HollowSphere", [] {
          return std::make_shared<MichelAndSuquet1992HollowSphereStressCriterion>();
        });
      }  // end of MichelAndSuquet1992HollowSphereStressCriterionProxy
    };   // end of struct MichelAndSuquet1992HollowSphereStressCriterionProxy

    static MichelAndSuquet1992HollowSphereStressCriterionProxy proxy;

#endif /* MFRONT_ADITIONNAL_LIBRARY */

  }  // end of namespace bbrick

}  // end of namespace mfront
