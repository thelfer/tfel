/*!
 * \file   GenericBehaviourSymbolsGenerator.cxx
 * \brief
 * \author th202608
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
#include "MFront/GenericBehaviourSymbolsGenerator.hxx"

namespace mfront {

  GenericBehaviourSymbolsGenerator::GenericBehaviourSymbolsGenerator() =
      default;

  void GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& os,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    os << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
       << "_BehaviourType = ";
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          os << "1u";
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          os << "2u";
        } else if (fs == BehaviourDescription::HENCKY) {
          os << "2u";
        } else {
          tfel::raise(
              "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
              "unsupported strain measure");
          os << "0u";
        }
      } else {
        os << "1u";
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "2u";
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      os << "3u";
    } else {
      tfel::raise(
          "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported strain measure");
    }
    os << ";\n\n";
  }  // end of GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols

  void GenericBehaviourSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& os,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& bd,
      const std::string& name) const {
    os << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
       << "_BehaviourKinematic = ";
    if (bd.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (bd.isStrainMeasureDefined()) {
        const auto fs = bd.getStrainMeasure();
        if (fs == BehaviourDescription::LINEARISED) {
          os << "1u";
        } else if (fs == BehaviourDescription::GREENLAGRANGE) {
          os << "3u";
        } else if (fs == BehaviourDescription::HENCKY) {
          os << "3u";
        } else {
          tfel::raise(
              "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
              "unsupported strain measure");
        }
      } else {
        os << "1u";
      }
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      os << "3u";
    } else if (bd.getBehaviourType() ==
               BehaviourDescription::COHESIVEZONEMODEL) {
      os << "2u";
    } else {
      tfel::raise(
          "GenericBehaviourSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported strain measure");
    }
    os << ";\n\n";
  }  // end of
     // GenericBehaviourSymbolsGenerator::writeBehaviourKinematicSymbols

  void GenericBehaviourSymbolsGenerator::writeAdditionalSymbols(
      std::ostream& os,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      Hypothesis) const {
    os << '\n';
  }  // end of GenericBehaviourSymbolsGenerator::writeAdditionalSymbols

  bool GenericBehaviourSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of GenericBehaviourSymbolsGenerator::handleStrainMeasure

  GenericBehaviourSymbolsGenerator::~GenericBehaviourSymbolsGenerator() =
      default;

}  // end of namespace mfront
