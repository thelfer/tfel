/*!
 * \file   AbaqusExplicitSymbolsGenerator.cxx
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
#include "MFront/CodeGeneratorUtilities.hxx"
#include "MFront/BehaviourDescription.hxx"
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/AbaqusExplicitInterface.hxx"
#include "MFront/AbaqusExplicitSymbolsGenerator.hxx"

namespace mfront {

  AbaqusExplicitSymbolsGenerator::AbaqusExplicitSymbolsGenerator() = default;

  void AbaqusExplicitSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {}  // end of writeAdditionalSymbols

  void AbaqusExplicitSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const FileDescription&,
      const std::string& name) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      const auto s =
          i.getFunctionNameBasis(name) + "_OrthotropyManagementPolicy";
      if (!mb.hasAttribute(
              AbaqusExplicitInterface::orthotropyManagementPolicy)) {
        exportUnsignedShortSymbol(out, s, 0u);
      } else {
        const auto omp = mb.getAttribute<std::string>(
            AbaqusExplicitInterface::orthotropyManagementPolicy);
        if (omp == "MFront") {
          exportUnsignedShortSymbol(out, s, 2u);
        } else if (omp == "Native") {
          exportUnsignedShortSymbol(out, s, 1u);
        } else {
          tfel::raise(
              "AbaqusExplicitSymbolsGenerator::writeSpecificSymbols: "
              "unsupported orthotropy management policy");
        }
      }
    }
  }  // end of writeSpecificSymbols

  void AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(
          !AbaqusInterfaceBase::hasFiniteStrainStrategy(mb),
          "AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols: "
          "behaviours written in the small strain framework "
          "must be embedded in a strain strategy");
      exportUnsignedShortSymbol(out, s, 2u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else {
      tfel::raise(
          "AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported behaviour type");
    }
  }  // end of writeBehaviourTypeSymbols

  void AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(
          !AbaqusInterfaceBase::hasFiniteStrainStrategy(mb),
          "AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "behaviours written in the small strain framework "
          "must be embedded in a strain strategy");
      exportUnsignedShortSymbol(out, s, 3u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else {
      tfel::raise(
          "AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "unsupported behaviour type");
    }
  }  // end of writeBehaviourKinematicSymbols

  bool AbaqusExplicitSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of handleStrainMeasure

  AbaqusExplicitSymbolsGenerator::~AbaqusExplicitSymbolsGenerator() = default;

}  // end of namespace mfront
