/*!
 * \file   DianaFEASymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/01/2019
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
#include "MFront/DianaFEAInterface.hxx"
#include "MFront/DianaFEASymbolsGenerator.hxx"

namespace mfront {

  DianaFEASymbolsGenerator::DianaFEASymbolsGenerator() = default;

  void DianaFEASymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(
          b, "DianaFEASymbolsGenerator::writexxBehaviourTypeSymbols: " + m);
    };
    throw_if(mb.getBehaviourType() !=
                 BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR,
             "unsupported behaviour type");
    throw_if((mb.isStrainMeasureDefined()) &&
                 (mb.getStrainMeasure() != BehaviourDescription::LINEARISED),
             "unsupported strain measure");
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourType = ";
    out << "1u;\n\n";
  }  // end of DianaFEASymbolsGenerator::writexxBehaviourTypeSymbols

  void DianaFEASymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const StandardBehaviourInterface& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    out << "MFRONT_SHAREDOBJ unsigned short " << i.getFunctionNameBasis(name)
        << "_BehaviourKinematic = 1u;\n\n";
  }  // end of DianaFEASymbolsGenerator::writexxBehaviourKinematicSymbols

  void DianaFEASymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const StandardBehaviourInterface&,
      const BehaviourDescription&,
      const FileDescription&,
      const std::string&,
      const Hypothesis) const {
  }  // end of DianaFEASymbolsGenerator::writeAdditionalSymbols

  bool DianaFEASymbolsGenerator::handleStrainMeasure() const {
    return true;
  }  // end of DianaFEASymbolsGenerator::handleStrainMeasure

  DianaFEASymbolsGenerator::~DianaFEASymbolsGenerator() = default;

}  // end of namespace mfront
