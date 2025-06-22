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
#include "MFront/BehaviourDescription.hxx"
#include "MFront/StandardBehaviourInterface.hxx"
#include "MFront/AbaqusExplicitInterface.hxx"
#include "MFront/AbaqusExplicitSymbolsGenerator.hxx"

namespace mfront {

  AbaqusExplicitSymbolsGenerator::AbaqusExplicitSymbolsGenerator() = default;

  void AbaqusExplicitSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AbaqusExplicitSymbolsGenerator::writeAdditionalSymbols

  void AbaqusExplicitSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const FileDescription&,
      const std::string& name) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (!mb.hasAttribute(
              AbaqusExplicitInterface::orthotropyManagementPolicy)) {
        out << "MFRONT_SHAREDOBJ unsigned short "
            << i.getFunctionNameBasis(name)
            << "_OrthotropyManagementPolicy = 0u;\n\n";
      } else {
        const auto omp = mb.getAttribute<std::string>(
            AbaqusExplicitInterface::orthotropyManagementPolicy);
        if (omp == "MFront") {
          out << "MFRONT_SHAREDOBJ unsigned short "
              << i.getFunctionNameBasis(name)
              << "_OrthotropyManagementPolicy = 2u;\n\n";
        } else if (omp == "Native") {
          out << "MFRONT_SHAREDOBJ unsigned short "
              << i.getFunctionNameBasis(name)
              << "_OrthotropyManagementPolicy = 1u;\n\n";
        } else {
          tfel::raise(
              "AbaqusExplicitSymbolsGenerator::writeSpecificSymbols: "
              "unsupported orthotropy management policy");
        }
      }
    }
  }  // end of AbaqusExplicitSymbolsGenerator::writeSpecificSymbols

  void AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(
          !AbaqusInterfaceBase::hasFiniteStrainStrategy(mb),
          "AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols: "
          "behaviours written in the small strain framework "
          "must be embedded in a strain strategy");
      out << "2u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "2u;\n\n";
    } else {
      tfel::raise(
          "AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols: "
          "unsupported behaviour type");
    }
  }  // end of AbaqusExplicitSymbolsGenerator::writeBehaviourTypeSymbols

  void AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise_if(
          !AbaqusInterfaceBase::hasFiniteStrainStrategy(mb),
          "AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "behaviours written in the small strain framework "
          "must be embedded in a strain strategy");
      out << "3u;\n\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "3u;\n\n";
    } else {
      tfel::raise(
          "AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "unsupported behaviour type");
    }
  }  // end of AbaqusExplicitSymbolsGenerator::writeBehaviourKinematicSymbols

  bool AbaqusExplicitSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of AbaqusExplicitSymbolsGenerator::handleStrainMeasure

  AbaqusExplicitSymbolsGenerator::~AbaqusExplicitSymbolsGenerator() = default;

}  // end of namespace mfront
