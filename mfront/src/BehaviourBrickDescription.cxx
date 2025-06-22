/*!
 * \file   BehaviourBrickDescription.cxx
 * \brief
 * \author Thomas Helfer
 * \date   29/08/2019
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <algorithm>
#include "TFEL/Raise.hxx"
#include "MFront/BehaviourDSLDescription.hxx"
#include "MFront/BehaviourBrickDescription.hxx"

namespace mfront {

  BehaviourBrickDescription::BehaviourBrickDescription() = default;

  BehaviourBrickDescription::BehaviourBrickDescription(
      const BehaviourBrickDescription&) = default;

  BehaviourBrickDescription::BehaviourBrickDescription(
      BehaviourBrickDescription&&) = default;

  BehaviourBrickDescription& BehaviourBrickDescription::operator=(
      const BehaviourBrickDescription&) = default;

  BehaviourBrickDescription& BehaviourBrickDescription::operator=(
      BehaviourBrickDescription&&) = default;

  BehaviourBrickDescription::~BehaviourBrickDescription() = default;

  static void checkCompatibilityAndComplete(
      BehaviourDSLDescription& d,
      const BehaviourBrickDescription& bbd,
      const bool b) {
    tfel::raise_if(
        d.behaviourType != bbd.behaviourType,
        "complete: the behaviour DSL description and the brick description do "
        "not describe the same behaviour type");
    tfel::raise_if(
        d.integrationScheme != bbd.integrationScheme,
        "complete: the behaviour DSL description and the brick description do "
        "not support the same integration scheme");
    auto mhs = d.supportedModellingHypotheses;
    mhs.erase(std::remove_if(
                  mhs.begin(), mhs.end(),
                  [&bbd](const BehaviourDSLDescription::Hypothesis h) {
                    return std::find(bbd.supportedModellingHypotheses.cbegin(),
                                     bbd.supportedModellingHypotheses.cend(),
                                     h) ==
                           bbd.supportedModellingHypotheses.cend();
                  }),
              mhs.end());
    tfel::raise_if(
        mhs.empty(),
        "complete: the behaviour DSL description and the brick description "
        "do not have modelling hypothesis in common");
    auto bss = d.supportedBehaviourSymmetries;
    bss.erase(std::remove_if(
                  bss.begin(), bss.end(),
                  [&bbd](const BehaviourSymmetryType s) {
                    return std::find(bbd.supportedBehaviourSymmetries.cbegin(),
                                     bbd.supportedBehaviourSymmetries.cend(),
                                     s) ==
                           bbd.supportedBehaviourSymmetries.cend();
                  }),
              bss.end());
    tfel::raise_if(
        mhs.empty(),
        "complete: the behaviour DSL description and the brick description "
        "do not have a behaviour symmetry in common");
    tfel::raise_if((!d.allowCrystalStructureDefinition) &&
                       (bbd.requireCrystalStructureDefinition),
                   "complete: the behaviour DSL description does not allow the "
                   "definition of a crystal structure, by the definition of a "
                   "crystal structure is required by the behaviour brick");
    tfel::raise_if((!d.allowStiffnessTensorDefinition) &&
                       (bbd.requireStiffnessTensorDefinition),
                   "complete: the behaviour DSL description does not allow the "
                   "definition of a stiffness tensor, by the definition of a "
                   "stiffness tensor is required by the behaviour brick");
    auto cbs = d.typicalCodeBlocks;
    const auto& mcbs = bbd.managedCodeBlocks;
    cbs.erase(std::remove_if(cbs.begin(), cbs.end(),
                             [&mcbs](const std::string& v) {
                               return std::find(mcbs.begin(), mcbs.end(), v) !=
                                      mcbs.end();
                             }),
              cbs.end());
    // everything is ok
    if (b) {
      if (bbd.requireCrystalStructureDefinition) {
        d.requireCrystalStructureDefinition = true;
      }
      if (bbd.requireStiffnessTensorDefinition) {
        d.allowElasticPropertiesDefinition = false;
        d.requireStiffnessTensorDefinition = true;
      }
      std::swap(d.supportedModellingHypotheses, mhs);
      std::swap(d.supportedBehaviourSymmetries, bss);
      std::swap(d.typicalCodeBlocks, cbs);
    }
  }  // end of checkCompatibilityAndComplete

  bool areCompatible(BehaviourDSLDescription& d,
                     const BehaviourBrickDescription& bbd) {
    try {
      checkCompatibilityAndComplete(d, bbd, false);
    } catch (std::runtime_error&) {
      return false;
    }
    return true;
  }  // end of checkCompatibility

  void complete(BehaviourDSLDescription& d,
                const BehaviourBrickDescription& bbd) {
    checkCompatibilityAndComplete(d, bbd, true);
  }  // end of complete

}  // end of namespace mfront
