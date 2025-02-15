/*!
 * \file   AbaqusSymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   19/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
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
#include "MFront/AbaqusInterface.hxx"
#include "MFront/AbaqusSymbolsGenerator.hxx"

namespace mfront {

  AbaqusSymbolsGenerator::AbaqusSymbolsGenerator() = default;

  void AbaqusSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AbaqusSymbolsGenerator::writeAdditionalSymbols

  void AbaqusSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const FileDescription&,
      const std::string& name) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      const auto s =
          i.getFunctionNameBasis(name) + "_OrthotropyManagementPolicy";
      if (!mb.hasAttribute(AbaqusInterface::orthotropyManagementPolicy)) {
        exportUnsignedShortSymbol(out, s, 0u);
      } else {
        const auto omp = mb.getAttribute<std::string>(
            AbaqusInterface::orthotropyManagementPolicy);
        if (omp == "MFront") {
          exportUnsignedShortSymbol(out, s, 2u);
        } else if (omp == "Native") {
          exportUnsignedShortSymbol(out, s, 1u);
        } else {
          tfel::raise(
              "AbaqusSymbolsGenerator::writeSpecificSymbols: "
              "unsupported orthotropy management policy");
        }
      }
    }
  }  // end of AbaqusSymbolsGenerator::writeSpecificSymbols

  void AbaqusSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AbaqusSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) &&
          (AbaqusInterfaceBase::getFiniteStrainStrategy(mb) != "Native")) {
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
  }  // end of AbaqusSymbolsGenerator::writeBehaviourTypeSymbols

  void AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols: " + m);
    };
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
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
  }  // end of AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols

  bool AbaqusSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of AbaqusSymbolsGenerator::handleStrainMeasure

  AbaqusSymbolsGenerator::~AbaqusSymbolsGenerator() = default;

}  // end of namespace mfront
