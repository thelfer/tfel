/*!
 * \file   LSDYNASymbolsGenerator.cxx
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
#include "MFront/BehaviourInterfaceBase.hxx"
#include "MFront/LSDYNASymbolsGenerator.hxx"

namespace mfront {

  LSDYNASymbolsGenerator::LSDYNASymbolsGenerator() = default;

  void LSDYNASymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of LSDYNASymbolsGenerator::writeAdditionalSymbols

  void LSDYNASymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase&,
      const BehaviourDescription& mb,
      const std::string& name) const {
    //     out << "MFRONT_SHAREDOBJ unsigned short " <<
    //     this->getFunctionName(name)
    //         << "_BehaviourType = ";
    //     if (mb.getBehaviourType() ==
    //         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
    //       tfel::raise_if(!hasFiniteStrainStrategy(mb),
    //                      "LSDYNASymbolsGenerator::writeBehaviourTypeSymbols:
    //                      "
    //                      "behaviours written in the small strain framework
    //                      "
    //                      "must be embedded in a strain strategy");
    //       out << "2u;\n\n";
    //     } else if (mb.getBehaviourType() ==
    //                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
    //       out << "2u;\n\n";
    //     } else {
    //       tfel::raise(
    //           "LSDYNASymbolsGenerator::writeBehaviourTypeSymbols: "
    //           "unsupported behaviour type");
    //     }
  }  // end of LSDYNASymbolsGenerator::writeBehaviourTypeSymbols

  void LSDYNASymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase&,
      const BehaviourDescription& mb,
      const std::string& name) const {
    //     out << "MFRONT_SHAREDOBJ unsigned short " <<
    //     this->getFunctionName(name)
    //         << "_BehaviourKinematic = ";
    //     if (mb.getBehaviourType() ==
    //         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
    //       tfel::raise_if(!hasFiniteStrainStrategy(mb),
    //                      "LSDYNASymbolsGenerator::writeBehaviourKinematicSymbols:
    //                      "
    //                      "behaviours written in the small strain framework "
    //                      "must be embedded in a strain strategy");
    //       out << "3u;\n\n";
    //     } else if (mb.getBehaviourType() ==
    //                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
    //       out << "3u;\n\n";
    //     } else {
    //       tfel::raise(
    //           "LSDYNASymbolsGenerator::writeBehaviourKinematicSymbols:
    //           "
    //           "unsupported behaviour type");
    //     }
  }  // end of LSDYNASymbolsGenerator::writeBehaviourKinematicSymbols

  bool LSDYNASymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of LSDYNASymbolsGenerator::handleStrainMeasure

  LSDYNASymbolsGenerator::~LSDYNASymbolsGenerator() = default;

}  // end of namespace mfront
