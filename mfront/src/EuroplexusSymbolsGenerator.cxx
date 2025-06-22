/*!
 * \file   EuroplexusSymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/07/2018
 * \copyright Copyright (C) 2006-2025 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence with
 * linking exception or the CECILL-A licence. A copy of thoses licences are
 * delivered with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <ostream>
#include "TFEL/Raise.hxx"
#include "MFront/DSLUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
#include "MFront/EuroplexusInterface.hxx"
#include "MFront/EuroplexusSymbolsGenerator.hxx"

namespace mfront {

  EuroplexusSymbolsGenerator::EuroplexusSymbolsGenerator() = default;

  void EuroplexusSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(!EuroplexusInterface::hasFiniteStrainStrategy(mb),
                     "EuroplexusSymbolsGenerator::writeBehaviourTypeSymbols: "
                     "behaviours written in the small strain framework "
                     "must be embedded in a strain strategy");
      exportUnsignedShortSymbol(out, s, 2u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else {
      tfel::raise(
          "EuroplexusSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported behaviour type");
    }
  }  // end of EuroplexusSymbolsGenerator::writeBehaviourTypeSymbols

  void EuroplexusSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(
          !EuroplexusInterface::hasFiniteStrainStrategy(mb),
          "EuroplexusSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "behaviours written in the small strain framework "
          "must be embedded in a strain strategy");
      exportUnsignedShortSymbol(out, s, 3u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else {
      tfel::raise(
          "EuroplexusSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "unsupported behaviour type");
    }
  }  // end of EuroplexusSymbolsGenerator::writeBehaviourKinematicSymbols

  void EuroplexusSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of EuroplexusSymbolsGenerator::writeAdditionalSymbols

  bool EuroplexusSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of EuroplexusSymbolsGenerator::handleStrainMeasure

  EuroplexusSymbolsGenerator::~EuroplexusSymbolsGenerator() = default;

}  // end of namespace mfront
