/*!
 * \file   CalculiXSymbolsGenerator.cxx
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
#include "MFront/CalculiXInterface.hxx"
#include "MFront/CalculiXSymbolsGenerator.hxx"

namespace mfront {

  CalculiXSymbolsGenerator::CalculiXSymbolsGenerator() = default;

  void CalculiXSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "CalculiXSymbolsGenerator::writexxBehaviourTypeSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!CalculiXInterface::hasFiniteStrainStrategy(mb)) {
        out << "1u;\n\n";
      } else {
        out << "2u;\n\n";
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "2u;\n\n";
    } else {
      throw_if(true, "unsupported behaviour type");
    }
  }  // end of CalculiXSymbolsGenerator::writexxBehaviourTypeSymbols

  void CalculiXSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b,
          "CalculiXSymbolsGenerator::writexxBehaviourKinematicSymbols: " + m);
    };
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = ";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (CalculiXInterface::hasFiniteStrainStrategy(mb)) {
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
  }  // end of CalculiXSymbolsGenerator::writexxBehaviourKinematicSymbols

  void CalculiXSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of CalculiXSymbolsGenerator::writeAdditionalSymbols

  bool CalculiXSymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of CalculiXSymbolsGenerator::handleStrainMeasure

  CalculiXSymbolsGenerator::~CalculiXSymbolsGenerator() = default;

}  // end of namespace mfront
