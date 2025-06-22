/*!
 * \file   AbaqusSymbolsGenerator.cxx
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
#include "MFront/AbaqusInterface.hxx"
#include "MFront/AbaqusSymbolsGenerator.hxx"

namespace mfront {

  AbaqusSymbolsGenerator::AbaqusSymbolsGenerator() = default;

  void AbaqusSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of AbaqusSymbolsGenerator::writeAdditionalSymbols

  void AbaqusSymbolsGenerator::writeSpecificSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const FileDescription&,
      const std::string& name) const {
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      if (!mb.hasAttribute(AbaqusInterface::orthotropyManagementPolicy)) {
        out << "MFRONT_SHAREDOBJ unsigned short "
            << i.getFunctionNameBasis(name)
            << "_OrthotropyManagementPolicy = 0u;\n\n";
      } else {
        const auto omp = mb.getAttribute<std::string>(
            AbaqusInterface::orthotropyManagementPolicy);
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
              "AbaqusSymbolsGenerator::writeSpecificSymbols: "
              "unsupported orthotropy management policy");
        }
      }
    }
  }  // end of AbaqusSymbolsGenerator::writeSpecificSymbols

  void AbaqusSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b,
                     "AbaqusSymbolsGenerator::writeBehaviourTypeSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if ((AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) &&
          (AbaqusInterfaceBase::getFiniteStrainStrategy(mb) != "Native")) {
        out << "2u;\n\n";
      } else {
        out << "1u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "2u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AbaqusSymbolsGenerator::writeBehaviourTypeSymbols

  void AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (AbaqusInterfaceBase::hasFiniteStrainStrategy(mb)) {
        out << "3u;\n\n";
      } else {
        out << "1u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "3u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of AbaqusSymbolsGenerator::writeBehaviourKinematicSymbols

  bool AbaqusSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of AbaqusSymbolsGenerator::handleStrainMeasure

  AbaqusSymbolsGenerator::~AbaqusSymbolsGenerator() = default;

}  // end of namespace mfront
