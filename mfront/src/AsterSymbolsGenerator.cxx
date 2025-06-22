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
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
#include "MFront/AsterInterface.hxx"
#include "MFront/AsterSymbolsGenerator.hxx"

namespace mfront {

  AsterSymbolsGenerator::AsterSymbolsGenerator() = default;

  void AsterSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AsterSymbolsGenerator::writeAdditionalSymbols

  void AsterSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AsterSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "1u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "2u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "3u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AsterSymbolsGenerator::writeBehaviourTypeSymbols

  void AsterSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AsterSymbolsGenerator::writeBehaviourKinematicSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (mb.isStrainMeasureDefined()) {
        const auto fs = mb.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          out << "1u;\n\n";
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          out << "5u;\n\n";
        } else if (fs == BehaviourDescription::HENCKY) {
          out << "6u;\n\n";
        } else {
          out << "0u;\n\n";
        }
      } else {
        out << "0u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "3u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      out << "2u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AsterSymbolsGenerator::writeBehaviourKinematicSymbols

  void AsterSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const FileDescription&,
      const std::string& name) const {
    auto throw_if = [](const bool c, const std::string& m) {
      tfel::raise_if(c, "AsterSymbolsGenerator::writeSpecificSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_savesTangentOperator = ";
    if (bd.getAttribute<bool>(AsterInterface::saveTangentOperator, false)) {
      out << "1";
    } else {
      out << "0";
    }
    out << ";\n";
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
          << "_FiniteStrainFormulation = ";
      if (!bd.hasAttribute(AsterInterface::finiteStrainFormulation)) {
        out << "1u;\n";
      } else {
        const auto& afsf = bd.getAttribute<std::string>(
            AsterInterface::finiteStrainFormulation);
        if (afsf == AsterInterface::simo_miehe) {
          out << "1u;\n";
        } else if (afsf == AsterInterface::grot_gdep) {
          out << "2u;\n";
        } else {
          throw_if(true,
                   "internal error: unsupported finite strain formulation");
        }
      }
    }
  }  // end of AsterSymbolsGenerator::writeSpecificSymbols

  bool AsterSymbolsGenerator::handleStrainMeasure() const {
    return false;
  }  // end of AsterSymbolsGenerator::handleStrainMeasure

  AsterSymbolsGenerator::~AsterSymbolsGenerator() = default;

}  // end of namespace mfront
