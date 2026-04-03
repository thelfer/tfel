/*!
 * \file   AsterSymbolsGenerator.cxx
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
#include "MFront/AsterInterface.hxx"
#include "MFront/AsterSymbolsGenerator.hxx"

namespace mfront {

  AsterSymbolsGenerator::AsterSymbolsGenerator() = default;

  void AsterSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AsterSymbolsGenerator::writeAdditionalSymbols

  void AsterSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AsterSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 1u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AsterSymbolsGenerator::writeBehaviourTypeSymbols

  void AsterSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AsterSymbolsGenerator::writeBehaviourKinematicSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (mb.isStrainMeasureDefined()) {
        const auto fs = mb.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          exportUnsignedShortSymbol(out, s, 1u);
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          exportUnsignedShortSymbol(out, s, 5u);
        } else if (fs == BehaviourDescription::HENCKY) {
          exportUnsignedShortSymbol(out, s, 6u);
        } else {
          exportUnsignedShortSymbol(out, s, 0u);
        }
      } else {
        exportUnsignedShortSymbol(out, s, 0u);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AsterSymbolsGenerator::writeBehaviourKinematicSymbols

  void AsterSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& bd,
      const FileDescription&,
      const std::string& name) const {
    const auto v = [&bd]() -> unsigned short {
      if (bd.getAttribute<bool>(AsterInterface::saveTangentOperator, false)) {
        return 1u;
      }
      return 0u;
    }();
    exportUnsignedShortSymbol(
        out, i.getFunctionNameBasis(name) + "_savesTangentOperator", v);
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      const auto fs = [&bd]() -> unsigned short {
        if (!bd.hasAttribute(AsterInterface::finiteStrainFormulation)) {
          return 1u;
        } else {
          const auto& afsf = bd.getAttribute<std::string>(
              AsterInterface::finiteStrainFormulation);
          if (afsf == AsterInterface::simo_miehe) {
            return 1u;
          } else if (afsf == AsterInterface::grot_gdep) {
            return 2u;
          }
          tfel::raise(
              "AsterSymbolsGenerator::writeSpecificSymbols: "
              "internal error: unsupported finite strain formulation");
        }
      }();
      exportUnsignedShortSymbol(
          out, i.getFunctionNameBasis(name) + "_FiniteStrainFormulation", fs);
    }
  }  // end of AsterSymbolsGenerator::writeSpecificSymbols

  bool AsterSymbolsGenerator::handleStrainMeasure() const {
    return false;
  }  // end of AsterSymbolsGenerator::handleStrainMeasure

  AsterSymbolsGenerator::~AsterSymbolsGenerator() = default;

}  // end of namespace mfront
