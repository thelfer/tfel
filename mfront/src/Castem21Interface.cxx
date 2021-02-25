/*!
 * \file   mfront/src/Castem21Interface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   24/02/2021
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include "TFEL/Raise.hxx"
#include "TFEL/System/System.hxx"
#include "MFront/Castem21Interface.hxx"

namespace mfront {

  std::string Castem21Interface::getName() {
    return "castem21";
  }  // end of getName

  Castem21Interface::Castem21Interface() {
  }  // end of Castem21Interface::Castem21Interface

  std::vector<BehaviourMaterialProperty>
  Castem21Interface::getDefaultMaterialPropertiesList(
      const BehaviourDescription& mb, const Hypothesis h) const {
    if (!((mb.getBehaviourType() ==
           BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
          (mb.getBehaviourType() ==
           BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) ||
          (mb.getBehaviourType() == BehaviourDescription::COHESIVEZONEMODEL))) {
      tfel::raise(
          "CastemInterface::getDefaultMaterialPropertiesList: "
          "unsupported behaviour type");
    }
    auto mprops = CastemInterface::getDefaultMaterialPropertiesList(mb, h);
    appendToMaterialPropertiesList(
        mprops, "temperature",
        "ReferenceTemperatureForTheThermalExpansionCoefficients", "TALP",
        false);
    appendToMaterialPropertiesList(mprops, "temperature",
                                   "ReferenceTemperatureForTheThermalExpansion",
                                   "TREF", false);
    return mprops;
  }  // end of Castem21Interface::getDefaultMaterialPropertiesList

  std::string Castem21Interface::getMaterialPropertiesOffsetForBehaviourTraits(
      const BehaviourDescription& md) const {
    return CastemInterface::getMaterialPropertiesOffsetForBehaviourTraits(md) +
           " + 2u";
  }  // end of getMaterialPropertiesOffsetForBehaviourTraits

  Castem21Interface::~Castem21Interface() = default;

}  // end of namespace mfront
