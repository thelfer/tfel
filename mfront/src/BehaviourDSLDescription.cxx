/*!
 * \file   BehaviourDSLDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date 29/08/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "MFront/BehaviourDSLDescription.hxx"

namespace mfront {

  BehaviourDSLDescription::BehaviourDSLDescription() = default;

  BehaviourDSLDescription::BehaviourDSLDescription(
      const BehaviourDSLDescription&) = default;

  BehaviourDSLDescription::BehaviourDSLDescription(
      BehaviourDSLDescription&&) noexcept = default;

  BehaviourDSLDescription& BehaviourDSLDescription::operator=(
      const BehaviourDSLDescription&) = default;

  BehaviourDSLDescription& BehaviourDSLDescription::operator=(
      BehaviourDSLDescription&&) noexcept = default;

  BehaviourDSLDescription::~BehaviourDSLDescription() = default;

  BehaviourDSLDescription getDefaultStrainBasedBehaviourDSLDescription() {
    auto d = BehaviourDSLDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::STANDARDSTRAINBASEDBEHAVIOUR;
    return d;
  }  // end of getDefaultStrainBasedBehaviourDSLDescription

  BehaviourDSLDescription getDefaultFiniteStrainBehaviourDSLDescription() {
    auto d = BehaviourDSLDescription{};
    d.behaviourType =
        tfel::material::MechanicalBehaviourBase::STANDARDFINITESTRAINBEHAVIOUR;
    return d;
  }  // end of getDefaultFiniteStrainBehaviourDSLDescription

  bool supportAllModellingHypothes(const BehaviourDSLDescription& d) {
    const auto& mh = d.supportedModellingHypotheses;
    for (const auto h :
         tfel::material::ModellingHypothesis::getModellingHypotheses()) {
      if (std::find(mh.begin(), mh.end(), h) == mh.end()) {
        return false;
      }
    }
    return true;
  }  // end of supportAllModellingHypothes

}  // end of namespace mfront
