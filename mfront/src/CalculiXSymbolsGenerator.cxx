/*!
 * \file   CalculiXSymbolsGenerator.cxx
 * \brief
 * \author Thomas Helfer
 * \date 19/07/2018
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
#include "MFront/CalculiXInterface.hxx"
#include "MFront/CalculiXSymbolsGenerator.hxx"

namespace mfront {

  CalculiXSymbolsGenerator::CalculiXSymbolsGenerator() = default;

  void CalculiXSymbolsGenerator::writeBehaviourTypeSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + "_BehaviourType";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (!CalculiXInterface::hasFiniteStrainStrategy(mb)) {
        exportUnsignedShortSymbol(out, s, 1u);
      } else {
        exportUnsignedShortSymbol(out, s, 2u);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 2u);
    } else {
      tfel::raise(
          "CalculiXSymbolsGenerator::writexxBehaviourTypeSymbols: "
          "unsupported behaviour type");
    }
  }  // end of CalculiXSymbolsGenerator::writexxBehaviourTypeSymbols

  void CalculiXSymbolsGenerator::writeBehaviourKinematicSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name) const {
    const auto s = i.getFunctionNameBasis(name) + +"_BehaviourKinematic";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      if (CalculiXInterface::hasFiniteStrainStrategy(mb)) {
        exportUnsignedShortSymbol(out, s, 3u);
      } else {
        exportUnsignedShortSymbol(out, s, 1u);
      }
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      exportUnsignedShortSymbol(out, s, 3u);
    } else {
      tfel::raise(
          "CalculiXSymbolsGenerator::writeBehaviourKinematicSymbols: "
          "unsupported behaviour type");
    }
  }  // end of CalculiXSymbolsGenerator::writexxBehaviourKinematicSymbols

  void CalculiXSymbolsGenerator::writeStateVariablesSymbols(
      std::ostream& out,
      const BehaviourInterfaceBase& i,
      const BehaviourDescription& mb,
      const std::string& name,
      const Hypothesis h) const {
    if ((mb.getBehaviourType() !=
         BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
        (!mb.hasAttribute(CalculiXInterface::finiteStrainStrategy)) ||
        (mb.getAttribute<std::string>(
             CalculiXInterface::finiteStrainStrategy) !=
         "MieheApelLambrechtLogarithmicStrainII")) {
      SymbolsGenerator::writeStateVariablesSymbols(out, i, mb, name, h);
      return;
    }
    // the MieheApelLambrechtLogarithmicStrainII finite strain strategy
    // stores the logarithmic strain and its dual stress after the state
    // variables of the behaviour, as in the generated input file example
    auto isvs = mb.getBehaviourData(h).getPersistentVariables();
    isvs.push_back(
        VariableDescription{"StrainStensor", "logarithmicStrain", 1, 0u});
    isvs.push_back(VariableDescription{"StressStensor",
                                       "dualOfTheLogarithmicStrain", 1, 0u});
    out << "MFRONT_EXPORT_SYMBOL(unsigned short, "
        << this->getSymbolName(i, name, h) << "_nInternalStateVariables, "
        << static_cast<unsigned short>(isvs.getNumberOfVariables()) << "u);\n";
    this->writeExternalNames(out, i, name, h, mb.getExternalNames(h, isvs),
                             "InternalStateVariables");
    this->writeVariablesTypesSymbol(out, i, name, h, isvs,
                                    "InternalStateVariables");
  }  // end of CalculiXSymbolsGenerator::writeStateVariablesSymbols

  void CalculiXSymbolsGenerator::writeAdditionalSymbols(
      std::ostream&,
      const BehaviourInterfaceBase&,
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
