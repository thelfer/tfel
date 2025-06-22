/*!
 * \file   AnsysSymbolsGenerator.cxx
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
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/AnsysInterface.hxx"
#include "MFront/AnsysSymbolsGenerator.hxx"

namespace mfront {

  AnsysSymbolsGenerator::AnsysSymbolsGenerator() = default;

  void AnsysSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AnsysSymbolsGenerator::writeAdditionalSymbols

  void AnsysSymbolsGenerator::writeSpecificSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&) const {}

  void AnsysSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AnsysSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((AnsysInterface::hasFiniteStrainStrategy(mb)) &&
          (AnsysInterface::getFiniteStrainStrategy(mb) != "Native")) {
        exportUnsignedShortSymbol(out, s, 2u);
      } else {
        exportUnsignedShortSymbol(out, s, 1u);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AnsysSymbolsGenerator::writeBehaviourTypeSymbols

  void AnsysSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AnsysSymbolsGenerator::writeBehaviourKinematicSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((AnsysInterface::hasFiniteStrainStrategy(mb)) &&
          (AnsysInterface::getFiniteStrainStrategy(mb) != "Native")) {
        exportUnsignedShortSymbol(out, s, 3u);
      } else {
        exportUnsignedShortSymbol(out, s, 1u);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AnsysSymbolsGenerator::writeBehaviourKinematicSymbols

  bool AnsysSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of AnsysSymbolsGenerator::handleStrainMeasure

  AnsysSymbolsGenerator::~AnsysSymbolsGenerator() = default;

}  // end of namespace mfront
